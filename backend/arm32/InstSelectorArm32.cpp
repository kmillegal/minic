///
/// @file InstSelectorArm32.cpp
/// @brief 指令选择器-ARM32的实现
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-11-21
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-11-21 <td>1.0     <td>zenglj  <td>新做
/// </table>
///
#include <cstdio>

#include "BinaryInstruction.h"
#include "Common.h"
#include "ILocArm32.h"
#include "InstSelectorArm32.h"
#include "Instruction.h"
#include "IntegerType.h"
#include "PlatformArm32.h"

#include "PointerType.h"
#include "RegVariable.h"
#include "Function.h"

#include "LabelInstruction.h"
#include "GotoInstruction.h"
#include "FuncCallInstruction.h"
#include "MoveInstruction.h"
#include "BranchInstruction.h"
#include "Type.h"

/// @brief 构造函数
/// @param _irCode 指令
/// @param _iloc ILoc
/// @param _func 函数
InstSelectorArm32::InstSelectorArm32(vector<Instruction *> & _irCode,
                                     ILocArm32 & _iloc,
                                     Function * _func,
                                     SimpleRegisterAllocator & allocator)
    : ir(_irCode), iloc(_iloc), func(_func), simpleRegisterAllocator(allocator)
{
    translator_handlers[IRInstOperator::IRINST_OP_ENTRY] = &InstSelectorArm32::translate_entry;
    translator_handlers[IRInstOperator::IRINST_OP_EXIT] = &InstSelectorArm32::translate_exit;

    translator_handlers[IRInstOperator::IRINST_OP_LABEL] = &InstSelectorArm32::translate_label;
    translator_handlers[IRInstOperator::IRINST_OP_GOTO] = &InstSelectorArm32::translate_goto;
    translator_handlers[IRInstOperator::IRINST_OP_BRANCH] = &InstSelectorArm32::translate_branch;

    translator_handlers[IRInstOperator::IRINST_OP_ASSIGN] = &InstSelectorArm32::translate_assign;

    translator_handlers[IRInstOperator::IRINST_OP_ADD_I] = &InstSelectorArm32::translate_add_int32;
    translator_handlers[IRInstOperator::IRINST_OP_SUB_I] = &InstSelectorArm32::translate_sub_int32;
    translator_handlers[IRInstOperator::IRINST_OP_MUL_I] = &InstSelectorArm32::translate_mul_int32;
    translator_handlers[IRInstOperator::IRINST_OP_DIV_I] = &InstSelectorArm32::translate_div_int32;
    translator_handlers[IRInstOperator::IRINST_OP_MOD_I] = &InstSelectorArm32::translate_mod_int32;
    translator_handlers[IRInstOperator::IRINST_OP_NEG_I] = &InstSelectorArm32::translate_neg_int32;
    translator_handlers[IRInstOperator::IRINST_OP_CMP_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_EQ_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_NE_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_LT_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_LE_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_GT_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_GE_I] = &InstSelectorArm32::translate_cmp_int32;
    translator_handlers[IRInstOperator::IRINST_OP_LOAD] = &InstSelectorArm32::translate_load_int32;
    translator_handlers[IRInstOperator::IRINST_OP_STORE] = &InstSelectorArm32::translate_store_int32;


    translator_handlers[IRInstOperator::IRINST_OP_FUNC_CALL] = &InstSelectorArm32::translate_call;
    translator_handlers[IRInstOperator::IRINST_OP_ARG] = &InstSelectorArm32::translate_arg;
}

///
/// @brief 析构函数
///
InstSelectorArm32::~InstSelectorArm32()
{}

/// @brief 指令选择执行
void InstSelectorArm32::run()
{
    for (auto inst: ir) {
        // 逐个指令进行翻译
        if (!inst->isDead()) {
            translate(inst);

            // !!! 新增逻辑 !!!
            // 如果当前翻译的是 EXIT 指令，则当前函数的翻译完成，退出循环
            if (inst->getOp() == IRInstOperator::IRINST_OP_EXIT) {
                // Optionally add a comment to clearly mark function end in assembly
                iloc.comment("--- End of Function ---");
                break; // 退出循环，停止翻译当前函数后续的IR指令
            }
        }
    }
}

/// @brief 指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate(Instruction * inst)
{
    // 操作符
    IRInstOperator op = inst->getOp();

    map<IRInstOperator, translate_handler>::const_iterator pIter;
    pIter = translator_handlers.find(op);
    if (pIter == translator_handlers.end()) {
        // 没有找到，则说明当前不支持
        printf("Translate: Operator(%d) not support", (int) op);
        return;
    }

    // 开启时输出IR指令作为注释
    if (showLinearIR) {
        outputIRInstruction(inst);
    }

    (this->*(pIter->second))(inst);
}

///
/// @brief 输出IR指令
///
void InstSelectorArm32::outputIRInstruction(Instruction * inst)
{
    std::string irStr;
    inst->toString(irStr);
    if (!irStr.empty()) {
        iloc.comment(irStr);
    }
}

/// @brief NOP翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_nop(Instruction * inst)
{
    (void) inst;
    iloc.nop();
}

/// @brief Label指令指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_label(Instruction * inst)
{
    Instanceof(labelInst, LabelInstruction *, inst);

    iloc.label(labelInst->getName());
}

/// @brief goto指令指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_goto(Instruction * inst)
{
    Instanceof(gotoInst, GotoInstruction *, inst);

    // 无条件跳转
    iloc.jump(gotoInst->getTarget()->getName());
}

/// @brief branch指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_branch(Instruction * inst)
{
    Instanceof(branchInst, BranchInstruction *, inst);

    LabelInstruction * true_target = branchInst->getTrueTarget();
    LabelInstruction * false_target = branchInst->getFalseTarget();
    Value * condition_value = branchInst->getCondition(); // e.g., %t5 in your IR

    Instruction * source_inst = dynamic_cast<Instruction *>(condition_value);
    std::string cond_code_for_branch;
    // 条件跳转
    // 获取分支条件 Value
	// 1. 如果条件值是一个比较指令，则直接使用其操作码
    if (source_inst && (source_inst->getOp() == IRInstOperator::IRINST_OP_EQ_I ||
                        source_inst->getOp() == IRInstOperator::IRINST_OP_NE_I ||
                        source_inst->getOp() == IRInstOperator::IRINST_OP_LT_I ||
                        source_inst->getOp() == IRInstOperator::IRINST_OP_LE_I ||
                        source_inst->getOp() == IRInstOperator::IRINST_OP_GT_I ||
                        source_inst->getOp() == IRInstOperator::IRINST_OP_GE_I)) {
        // 条件值直接来自一个比较指令，我们可以直接使用其操作码
        cond_code_for_branch = get_arm_condition_code(source_inst->getOp());
    } else {
        // 条件值不是直接的比较结果
        // 我们需要将其与0比较，看它是否非零
        // 1. 加载 condition_value 到一个寄存器
        int32_t cond_val_reg_no = -1;
        std::string cond_val_reg_str;

        if (condition_value->getRegId() == -1) {
            cond_val_reg_no = simpleRegisterAllocator.Allocate(condition_value);
            if (cond_val_reg_no == -1) {
                minic_log(LOG_ERROR,
                          "Branch: Failed to allocate register for condition value %s",
                          condition_value->getName().c_str());
                iloc.inst("b", false_target->getName()); // Fallback or error
                return;
            }
            iloc.load_var(cond_val_reg_no, condition_value);
            cond_val_reg_str = PlatformArm32::regName[cond_val_reg_no];
        } else {
            cond_val_reg_no = condition_value->getRegId();
            cond_val_reg_str = PlatformArm32::regName[cond_val_reg_no];
        }

        // 2. 与0比较
        iloc.inst("cmp", cond_val_reg_str, "#0");

        // 3. 如果 condition_value 是临时加载的，释放其寄存器
        if (condition_value->getRegId() == -1 && cond_val_reg_no != -1) {
            simpleRegisterAllocator.free(condition_value);
        }

        // 4. 分支条件现在是 "Not Equal"
        cond_code_for_branch = "ne";
    }

    iloc.inst("b" + cond_code_for_branch, true_target->getName());
    if (false_target) { // 确保 false_target 存在
        iloc.inst("b", false_target->getName());
    }
}

/// @brief 函数入口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_entry(Instruction * inst)
{
    // 查看保护的寄存器
    auto & protectedRegNo = func->getProtectedReg();
    auto & protectedRegStr = func->getProtectedRegStr();

    bool first = true;
    for (auto regno: protectedRegNo) {
        if (first) {
            protectedRegStr = PlatformArm32::regName[regno];
            first = false;
        } else {
            protectedRegStr += "," + PlatformArm32::regName[regno];
        }
    }

    if (!protectedRegStr.empty()) {
        iloc.inst("push", "{" + protectedRegStr + "}");
    }

    // 为fun分配栈帧，含局部变量、函数调用值传递的空间等
    iloc.allocStack(func, ARM32_TMP_REG_NO);
}

/// @brief 函数出口指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_exit(Instruction * inst)
{
    if (inst->getOperandsNum()) {
        // 存在返回值
        Value * retVal = inst->getOperand(0);

        // 赋值给寄存器R0
        iloc.load_var(0, retVal);
    }

    // 恢复栈空间
    iloc.inst("mov", "sp", "fp");

    // 保护寄存器的恢复
    auto & protectedRegStr = func->getProtectedRegStr();
    if (!protectedRegStr.empty()) {
        iloc.inst("pop", "{" + protectedRegStr + "}");
    }

    iloc.inst("bx", "lr");
}

/// @brief 赋值指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_assign(Instruction * inst)
{
    Value * result = inst->getOperand(0);
    Value * arg1 = inst->getOperand(1);

    int32_t arg1_regId = arg1->getRegId();
    int32_t result_regId = result->getRegId();

    if (arg1_regId != -1) {
        // 寄存器 => 内存
        // 寄存器 => 寄存器

        // r8 -> rs 可能用到r9
        iloc.store_var(arg1_regId, result, ARM32_TMP_REG_NO);
    } else if (result_regId != -1) {
        // 内存变量 => 寄存器
        if (arg1->getType()->isArrayType()) {
            // 如果源操作数(src)的类型是数组，
            // 那么我们就调用 lea_var 来加载它的【地址】。
            iloc.lea_var(result_regId, arg1);
        }else{
            iloc.load_var(result_regId, arg1);
        }
    } else {
        // 内存变量 => 内存变量

        int32_t temp_regno = simpleRegisterAllocator.Allocate();

        // arg1 -> r8
        iloc.load_var(temp_regno, arg1);

        // r8 -> rs 可能用到r9
        iloc.store_var(temp_regno, result, ARM32_TMP_REG_NO);

        simpleRegisterAllocator.free(temp_regno);
    }
}

// 用于翻译单目求负指令 (例如: result = -operand)
void InstSelectorArm32::translate_neg_int32(Instruction * inst)
{
    // 单目求负指令: result = -operand
    Value * result = inst;
    Value * operand = inst->getOperand(0);

    int32_t operand_reg_no = operand->getRegId();
    int32_t result_reg_no = inst->getRegId();
    int32_t load_operand_reg_no;
    int32_t load_result_reg_no;

    if (operand_reg_no == -1) {

        load_operand_reg_no = simpleRegisterAllocator.Allocate(operand);


        iloc.load_var(load_operand_reg_no, operand);
    } else {

        load_operand_reg_no = operand_reg_no;
    }

    if (result_reg_no == -1) {

        load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    } else {

        load_result_reg_no = result_reg_no;
    }


    iloc.inst("rsb",
              PlatformArm32::regName[load_result_reg_no],
              PlatformArm32::regName[load_operand_reg_no],
              "#0");

    if (result_reg_no == -1) {

        iloc.store_var(load_result_reg_no, result, ARM32_TMP_REG_NO);
    }

    simpleRegisterAllocator.free(operand); // 释放操作数使用的寄存器
    simpleRegisterAllocator.free(result);  // 释放结果使用的寄存器
}
// 辅助函数，将 IR 的比较操作符转换为 ARM 条件码
std::string InstSelectorArm32::get_arm_condition_code(IRInstOperator ir_op)
{
    switch (ir_op) {
        case IRInstOperator::IRINST_OP_EQ_I:
            return "eq"; // Equal
        case IRInstOperator::IRINST_OP_NE_I:
            return "ne"; // Not Equal
        case IRInstOperator::IRINST_OP_LT_I:
            return "lt"; // Less Than (signed)
        case IRInstOperator::IRINST_OP_LE_I:
            return "le"; // Less Than or Equal (signed)
        case IRInstOperator::IRINST_OP_GT_I:
            return "gt"; // Greater Than (signed)
        case IRInstOperator::IRINST_OP_GE_I:
            return "ge"; // Greater Than or Equal (signed)

        default:
            minic_log(LOG_ERROR, "Unsupported IR compare operator: %d", static_cast<int>(ir_op));
            return "al"; // Always - fallback, should not happen
    }
}
std::string InstSelectorArm32::get_opposite_arm_condition_code(IRInstOperator ir_op, InstSelectorArm32 * selector)
{
	switch (ir_op) {
		case IRInstOperator::IRINST_OP_EQ_I:
			return "ne"; // Not Equal
		case IRInstOperator::IRINST_OP_NE_I:
			return "eq"; // Equal
		case IRInstOperator::IRINST_OP_LT_I:
			return "ge"; // Greater Than or Equal (signed)
		case IRInstOperator::IRINST_OP_LE_I:
			return "gt"; // Greater Than (signed)
		case IRInstOperator::IRINST_OP_GT_I:
			return "le"; // Less Than or Equal (signed)
		case IRInstOperator::IRINST_OP_GE_I:
			return "lt"; // Less Than (signed)

		default:
			minic_log(LOG_ERROR, "Unsupported IR compare operator: %d", static_cast<int>(ir_op));
			return "al"; // Always - fallback, should not happen
	}
}

    /// @brief 整数比较指令翻译成ARM32汇编
    /// @param inst IR指令
    void InstSelectorArm32::translate_cmp_int32(Instruction * inst)
{

    if (inst->getOperandsNum() < 2) {
        minic_log(LOG_ERROR,
                  "CMP IR instruction expects 2 operands. Found %d for inst %s",
                  inst->getOperandsNum(),
                  inst->getName().c_str());
        return;
    }

    Value * lhs = inst->getOperand(0);
    Value * rhs_value = inst->getOperand(1);
    IRInstOperator compare_op = inst->getOp(); // 获取比较操作类型

    // 1. 执行比较 (CMP 指令)
    int32_t lhs_reg_no = -1;
    std::string lhs_reg_str;

    if (lhs->getRegId() == -1) {
        lhs_reg_no = simpleRegisterAllocator.Allocate(lhs);
        if (lhs_reg_no == -1) {
            return;
        }
        iloc.load_var(lhs_reg_no, lhs);
        lhs_reg_str = PlatformArm32::regName[lhs_reg_no];
    } else {
        lhs_reg_no = lhs->getRegId();
        lhs_reg_str = PlatformArm32::regName[lhs_reg_no];
    }

    ConstInt * rhs_ci = dynamic_cast<ConstInt *>(rhs_value);
    if (rhs_ci) {
        int32_t const_val = rhs_ci->getVal();
        if (PlatformArm32::constExpr(const_val)) {
            iloc.inst("cmp", lhs_reg_str, "#" + std::to_string(const_val));
        } else {
            goto load_rhs_to_reg_for_cmp_materialize;
        }
    } else {
    load_rhs_to_reg_for_cmp_materialize:
        int32_t rhs_reg_no = -1;
        std::string rhs_reg_str;
        if (rhs_value->getRegId() == -1) {
            rhs_reg_no = simpleRegisterAllocator.Allocate(rhs_value);
            if (rhs_reg_no == -1) { /* ... error handling ... */
                if (lhs->getRegId() == -1 && lhs_reg_no != -1)
                    simpleRegisterAllocator.free(lhs);
                return;
            }
            iloc.load_var(rhs_reg_no, rhs_value);
            rhs_reg_str = PlatformArm32::regName[rhs_reg_no];
        } else {
            rhs_reg_no = rhs_value->getRegId();
            rhs_reg_str = PlatformArm32::regName[rhs_reg_no];
        }
        iloc.inst("cmp", lhs_reg_str, rhs_reg_str);
        if (rhs_value->getRegId() == -1 && rhs_reg_no != -1) {
            simpleRegisterAllocator.free(rhs_value);
        }
    }

    // 临时加载的 lhs 可以释放了
    if (lhs->getRegId() == -1 && lhs_reg_no != -1) {
        simpleRegisterAllocator.free(lhs);
    }

    // 2. 物化比较结果 (0 或 1) 到 inst (结果 Value) 对应的寄存器
    int32_t result_target_reg_no;
    bool result_reg_is_temporary = false;

    if (inst->getRegId() == -1) {
        // 结果 Value 没有预分配寄存器 (通常意味着它是个临时变量，需要存到栈上)
        result_target_reg_no = simpleRegisterAllocator.Allocate(inst); // 分配一个临时寄存器
        if (result_target_reg_no == -1) {
            minic_log(LOG_ERROR, "Failed to allocate register for CMP result Value: %s", inst->getName().c_str());
            return;
        }
        result_reg_is_temporary = true;
    } else {
        // 结果 Value 已经分配了寄存器
        result_target_reg_no = inst->getRegId();
    }

    std::string result_target_reg_str = PlatformArm32::regName[result_target_reg_no];
    std::string true_cond_code = get_arm_condition_code(compare_op);

    if (true_cond_code != "al") { // 确保不是无效的比较操作符导致 "al"
        iloc.inst("mov" + true_cond_code, result_target_reg_str, "#1");
        // 为了确保寄存器有明确的0或1，需要设置相反条件为0
        // 或者先统一设置为0，再条件设置为1
        std::string false_cond_code = get_opposite_arm_condition_code(compare_op, this);
        if (false_cond_code != "al") {
            iloc.inst("mov" + false_cond_code, result_target_reg_str, "#0");
        } else {                                           // 如果无法获取false_cond，则用先设0再条件设1
            iloc.inst("mov", result_target_reg_str, "#0"); // 先设为0 (false)
            iloc.inst("mov" + true_cond_code, result_target_reg_str, "#1"); // 条件为真时设为1
        }
    } else {

        iloc.inst("mov", result_target_reg_str, "#0"); // Default to 0 on error
        minic_log(LOG_ERROR, "CMP result materialization with 'al' condition for op: %d", static_cast<int>(compare_op));
    }

    // 3. 如果结果寄存器是临时的 (即 inst 原本没有 regId)，则将结果存回 inst 的内存位置
    if (result_reg_is_temporary) {
        iloc.store_var(result_target_reg_no, inst, ARM32_TMP_REG_NO);
        simpleRegisterAllocator.free(inst);                           // 释放为 inst 临时分配的寄存器
    }
}

/// @brief load指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_load_int32(Instruction * inst)
{
    // 操作数: op0 = dest (目标Value), op1 = addr_val (持有地址的Value)
    Value * result = inst;
    Value * dest = inst->getOperand(0);
    Value * addr_val = inst->getOperand(1);

    // --- 步骤 1: 为目标 dest 分配一个寄存器 ---
    // LOAD指令的结果必须放入一个寄存器。
    // 寄存器分配器会记录下来：dest 这个 Value 现在活在这个寄存器里。
    int32_t dest_reg_no = simpleRegisterAllocator.Allocate(dest);
    if (dest_reg_no == -1) {
        minic_log(LOG_ERROR,
                  "Translate LOAD: Failed to allocate register for destination %s.",
                  dest->getName().c_str());
        return;
    }

    // --- 步骤 2: 将地址加载到一个临时寄存器 (r_addr) ---
    int32_t addr_reg_no = simpleRegisterAllocator.Allocate();
    if (addr_reg_no == -1) {
        minic_log(LOG_ERROR, "Translate LOAD: Failed to allocate register for address value.");
        simpleRegisterAllocator.free(dest); // 清理已分配的资源
        return;
    }
    iloc.load_var(addr_reg_no, addr_val);

    // --- 步骤 3: 执行核心的 LDR 指令 ---
    // 生成 LDR r_dest, [r_addr]
    iloc.inst("ldr", PlatformArm32::regName[dest_reg_no], "[" + PlatformArm32::regName[addr_reg_no] + "]");

    iloc.store_var(dest_reg_no, result, ARM32_TMP_REG_NO); // 将结果存储到 dest 的内存位置

    // --- 步骤 4: 清理 ---
    // 地址寄存器使命已完成，立即释放。
    simpleRegisterAllocator.free(addr_reg_no);
    simpleRegisterAllocator.free(dest_reg_no); // 释放 dest 寄存器
}

/// @brief store指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_store_int32(Instruction * inst)
{
    // 根据 MoveInstruction 的构造逻辑，STORE 操作的操作数是：
    // op0 = addr_val (目标地址), op1 = src_val (要存储的源值)
    Value * addr_val = inst->getOperand(0);
    Value * src_val = inst->getOperand(1);

    // --- 步骤 1: 将地址加载到寄存器 (r_addr) ---
    // 我们需要一个临时寄存器来存放目标地址。
    int32_t addr_reg_no = simpleRegisterAllocator.Allocate();
    if (addr_reg_no == -1) {
        minic_log(LOG_ERROR, "Translate STORE: Failed to allocate register for address value.");
        return;
    }
    // 使用 iloc.load_var 将地址值加载到我们新分配的寄存器中。
    iloc.load_var(addr_reg_no, addr_val);

    // --- 步骤 2: 将源值加载到寄存器 (r_src) ---
    // 我们需要一个寄存器来存放要写入内存的数据。
    int32_t src_reg_no = src_val->getRegId();
    bool src_is_temporary = false; // 标记我们是否为 src 临时分配了寄存器

    if (src_reg_no == -1) {
        // 如果源值不在寄存器中 (例如，它是一个常量或内存变量)，
        // 我们需要为它临时分配一个寄存器并加载它的值。
        src_reg_no = simpleRegisterAllocator.Allocate();
        if (src_reg_no == -1) {
            minic_log(LOG_ERROR,
                      "Translate STORE: Failed to allocate register for source value %s.",
                      src_val->getName().c_str());
            simpleRegisterAllocator.free(addr_reg_no); // 清理已分配的资源
            return;
        }
        iloc.load_var(src_reg_no, src_val);
        src_is_temporary = true;
    }
    // 如果 src_val 本身就在寄存器里，我们直接使用它的 regId 即可。

    // --- 步骤 3: 执行核心的 STR 指令 ---
    // 生成 STR r_src, [r_addr]
    iloc.inst("str", PlatformArm32::regName[src_reg_no], "[" + PlatformArm32::regName[addr_reg_no] + "]");

    // --- 步骤 4: 清理 ---
    // 释放所有为本次操作临时分配的寄存器。
    simpleRegisterAllocator.free(addr_reg_no);

    if (src_is_temporary) {
        simpleRegisterAllocator.free(src_reg_no);
    }
}

/// @brief 二元操作指令翻译成ARM32汇编
/// @param inst IR指令
/// @param operator_name 操作码
/// @param rs_reg_no 结果寄存器号
/// @param op1_reg_no 源操作数1寄存器号
/// @param op2_reg_no 源操作数2寄存器号
void InstSelectorArm32::translate_two_operator(Instruction * inst, string operator_name)
{
    Value * result = inst;
    Value * arg1 = inst->getOperand(0);
    Value * arg2 = inst->getOperand(1);

    int32_t arg1_reg_no = arg1->getRegId();
    int32_t arg2_reg_no = arg2->getRegId();
    int32_t result_reg_no = inst->getRegId();
    int32_t load_result_reg_no, load_arg1_reg_no, load_arg2_reg_no;

    // 看arg1是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg1_reg_no == -1) {

        // 分配一个寄存器r8
        load_arg1_reg_no = simpleRegisterAllocator.Allocate(arg1);

        if (arg1->getType()->isArrayType()) {
            ArrayType * array_type = static_cast<ArrayType *>(arg1->getType());
            if (array_type->getNumElements() == 0) {
                // 数组形参，加载值
                iloc.load_var(load_arg1_reg_no, arg1);
            } else {
                // 如果是数组加载地址
                iloc.lea_var(load_arg1_reg_no, arg1);
            }

        } else {
            // 否则加载值
            iloc.load_var(load_arg1_reg_no, arg1);
        }
    } else {
        load_arg1_reg_no = arg1_reg_no;
    }

    // 看arg2是否是寄存器，若是则寄存器寻址，否则要load变量到寄存器中
    if (arg2_reg_no == -1) {

        // 分配一个寄存器r9
        load_arg2_reg_no = simpleRegisterAllocator.Allocate(arg2);
		if (arg2->getType()->isArrayType()) {
            // 如果是数组
            ArrayType * array_type = static_cast<ArrayType *>(arg1->getType());
            if( array_type->getNumElements() == 0) {
				// 数组形参，加载值
				iloc.load_var(load_arg2_reg_no, arg2);
			} else {
				// 如果是数组加载地址
                iloc.lea_var(load_arg2_reg_no, arg2);
			}
		} else {
			// 否则加载值
            iloc.load_var(load_arg2_reg_no, arg2);
        }

    } else {
        load_arg2_reg_no = arg2_reg_no;
    }

    // 看结果变量是否是寄存器，若不是则需要分配一个新的寄存器来保存运算的结果
    if (result_reg_no == -1) {
        // 分配一个寄存器r10，用于暂存结果
        load_result_reg_no = simpleRegisterAllocator.Allocate(result);
    } else {
        load_result_reg_no = result_reg_no;
    }

    // r8 + r9 -> r10
    iloc.inst(operator_name,
              PlatformArm32::regName[load_result_reg_no],
              PlatformArm32::regName[load_arg1_reg_no],
              PlatformArm32::regName[load_arg2_reg_no]);

    // 结果不是寄存器，则需要把rs_reg_name保存到结果变量中
    if (result_reg_no == -1) {

        // 这里使用预留的临时寄存器，因为立即数可能过大，必须借助寄存器才可操作。

        // r10 -> result
        iloc.store_var(load_result_reg_no, result, ARM32_TMP_REG_NO);
    }

    // 释放寄存器
    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
}

/// @brief 整数加法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_add_int32(Instruction * inst)
{
    translate_two_operator(inst, "add");
}

/// @brief 整数减法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_sub_int32(Instruction * inst)
{
    translate_two_operator(inst, "sub");
}

/// @brief 整数乘法指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_mul_int32(Instruction * inst)
{
	translate_two_operator(inst, "mul");
}

/// @brief 整数除法指令翻译成ARM32汇编 (假设支持 sdiv)
/// @param inst IR指令
void InstSelectorArm32::translate_div_int32(Instruction * inst)
{
    translate_two_operator(inst, "sdiv");
}

/// @brief 整数取模指令翻译成ARM32汇编 (a % b = a - (a / b) * b)
/// @param inst IR指令
void InstSelectorArm32::translate_mod_int32(Instruction * inst)
{
    // Modulo: result = arg1 % arg2

    Value * result = inst;
    Value * arg1 = inst->getOperand(0); // Dividend (a)
    Value * arg2 = inst->getOperand(1); // Divisor (b)


    int32_t r_arg1 = simpleRegisterAllocator.Allocate(arg1);
    int32_t r_arg2 = simpleRegisterAllocator.Allocate(arg2);
    int32_t r_result = simpleRegisterAllocator.Allocate(result);

    bool arg1_needs_load = (arg1->getRegId() == -1);
    bool arg2_needs_load = (arg2->getRegId() == -1);
    bool res_needs_store = (result->getRegId() == -1);

    if (arg1_needs_load) {
        iloc.load_var(r_arg1, arg1);
    }
    if (arg2_needs_load) {
        iloc.load_var(r_arg2, arg2);
    }


    int32_t r_div_tmp = simpleRegisterAllocator.Allocate(); // Temp for a / b
    int32_t r_mul_tmp = simpleRegisterAllocator.Allocate(); // Temp for (a / b) * b


    iloc.inst("sdiv",
              PlatformArm32::regName[r_div_tmp],
              PlatformArm32::regName[r_arg1],
              PlatformArm32::regName[r_arg2]);


    iloc.inst("mul",
              PlatformArm32::regName[r_mul_tmp],
              PlatformArm32::regName[r_div_tmp], // (a / b)
              PlatformArm32::regName[r_arg2]);   // b

    simpleRegisterAllocator.free(r_div_tmp);

    iloc.inst("sub",
              PlatformArm32::regName[r_result],
              PlatformArm32::regName[r_arg1],
              PlatformArm32::regName[r_mul_tmp]); // (a / b) * b

    simpleRegisterAllocator.free(r_mul_tmp);

    if (res_needs_store) {
        iloc.store_var(r_result, result, ARM32_TMP_REG_NO);
    }

    simpleRegisterAllocator.free(arg1);
    simpleRegisterAllocator.free(arg2);
    simpleRegisterAllocator.free(result);
}

/// @brief 函数调用指令翻译成ARM32汇编
/// @param inst IR指令
void InstSelectorArm32::translate_call(Instruction * inst)
{
    FuncCallInstruction * callInst = dynamic_cast<FuncCallInstruction *>(inst);

    int32_t operandNum = callInst->getOperandsNum();

    if (operandNum != realArgCount) {

        // 两者不一致 也可能没有ARG指令，正常
        if (realArgCount != 0) {

            minic_log(LOG_ERROR, "ARG指令的个数与调用函数个数不一致");
        }
    }

    if (operandNum) {

        // 强制占用这几个寄存器参数传递的寄存器
        simpleRegisterAllocator.Allocate(0);
        simpleRegisterAllocator.Allocate(1);
        simpleRegisterAllocator.Allocate(2);
        simpleRegisterAllocator.Allocate(3);

        // 前四个的后面参数采用栈传递
        int esp = 0;
        for (int32_t k = 4; k < operandNum; k++) {

            auto arg = callInst->getOperand(k);

            // 新建一个内存变量，用于栈传值到形参变量中
            MemVariable * newVal = func->newMemVariable((Type *) PointerType::get(arg->getType()));
            newVal->setMemoryAddr(ARM32_SP_REG_NO, esp);
            esp += 4;

            Instruction * assignInst = new MoveInstruction(func, newVal, arg);

            // 翻译赋值指令
            translate_assign(assignInst);

            delete assignInst;
        }

        for (int32_t k = 0; k < operandNum && k < 4; k++) {

            auto arg = callInst->getOperand(k);

            // 检查实参的类型是否是临时变量。
            // 如果是临时变量，该变量可更改为寄存器变量即可，或者设置寄存器号
            // 如果不是，则必须开辟一个寄存器变量，然后赋值即可
            Value * targetReg = PlatformArm32::intRegVal[k];

            // 判断 arg 是需要传值还是传地址

			Type * argType = arg->getType();
            if (argType->isArrayType() || argType->isPointerType()) {
                // TODO

                if (arg && arg->getMemoryAddr()) // 确保它是一个已分配地址的栈变量
                {
                    // ======================= 这是最终的实现 =======================

                    // 1. 从 targetReg 对象中获取目标寄存器的【编号】
                    //    这需要您的 Register 类（或其基类）有一个类似 getRegNo() 的方法
                    int dest_reg_no = targetReg->getRegId();

                    // 4. 用提取出的三个整数，调用您的 leaStack 函数
                    iloc.lea_var(dest_reg_no, arg);

                    // =================================================================
                }
            } else {
                Instruction * assignInst = new MoveInstruction(func, PlatformArm32::intRegVal[k], arg);

                // 翻译赋值指令
                translate_assign(assignInst);
                delete assignInst;
            }
        }
    }

    iloc.call_fun(callInst->getName());

    if (operandNum) {
        simpleRegisterAllocator.free(0);
        simpleRegisterAllocator.free(1);
        simpleRegisterAllocator.free(2);
        simpleRegisterAllocator.free(3);
    }

    // 赋值指令
    if (callInst->hasResultValue()) {

        // 新建一个赋值操作
        Instruction * assignInst = new MoveInstruction(func, callInst, PlatformArm32::intRegVal[0]);

        // 翻译赋值指令
        translate_assign(assignInst);

        delete assignInst;
    }

    // 函数调用后清零，使得下次可正常统计
    realArgCount = 0;
}

///
/// @brief 实参指令翻译成ARM32汇编
/// @param inst
///
void InstSelectorArm32::translate_arg(Instruction * inst)
{
    // 翻译之前必须确保源操作数要么是寄存器，要么是内存，否则出错。
    Value * src = inst->getOperand(0);

    // 当前统计的ARG指令个数
    int32_t regId = src->getRegId();

    if (realArgCount < 4) {
        // 前四个参数
        if (regId != -1) {
            if (regId != realArgCount) {
                // 肯定寄存器分配有误
                minic_log(LOG_ERROR, "第%d个ARG指令对象寄存器分配有误: %d", argCount + 1, regId);
            }
        } else {
            minic_log(LOG_ERROR, "第%d个ARG指令对象不是寄存器", argCount + 1);
        }
    } else {
        // 必须是内存分配，若不是则出错
        int32_t baseRegId;
        bool result = src->getMemoryAddr(&baseRegId);
        if ((!result) || (baseRegId != ARM32_SP_REG_NO)) {

            minic_log(LOG_ERROR, "第%d个ARG指令对象不是SP寄存器寻址", argCount + 1);
        }
    }

    realArgCount++;
}
