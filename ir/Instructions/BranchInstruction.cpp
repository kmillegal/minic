///
/// @file BranchInstruction.cpp
/// @brief 条件分支指令，根据条件跳转到不同的目标
///
/// @author HuangKm
/// @version 1.0
/// @date 2025-5-13
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025-5-13 <td>1.0     <td>HuangKm  <td>新建
/// </table>
///

#include "VoidType.h"
#include "Value.h"
#include "LabelInstruction.h"
#include "Use.h"         // 确保包含 Use.h
#include "Instruction.h" // 确保包含 Instruction.h

#include "BranchInstruction.h"

BranchInstruction::BranchInstruction(Function * _func,
                                     Value * _condition,
                                     LabelInstruction * _true_target,
                                     LabelInstruction * _false_target)
    : Instruction(_func, IRInstOperator::IRINST_OP_BRANCH, VoidType::getType()),
      condition(_condition), // 这个 Value 是被 BranchInstruction 使用的
      true_target(_true_target), false_target(_false_target)
{
    if (condition) {
        // 1. 创建一个 Use 对象。
        //    'this' (BranchInstruction) 是 user。
        //    'condition' (Value*) 是被使用的 value。
        //    假设 Use 构造函数是 Use(Value* val, Instruction* user)
        Use * conditionUse = new Use(condition, this);
        // 或者如果构造函数是 Use(Instruction* user, Value* val):
        // Use* conditionUse = new Use(this, condition);

        // 2. 将这个 Use 对象注册到被使用的 Value (condition) 上。
        condition->addUse(conditionUse);

        // 3. (如果 Instruction 基类也需要记录它的 uses/operands)
        //    假设 Instruction 有一个 addOperand(Use* use) 或类似的方法
        //    如果 Instruction::addUse(Use*) 是你之前遇到的问题，那么这里就是修复的地方：
        Instruction::addUse(conditionUse); // 调用 Instruction 基类的 addUse
                                           // 或者如果基类有更具体的 addOperand(Use* use)
                                           // Instruction::addOperand(conditionUse);

        // 如果 Instruction 基类没有 addUse(Use*) 或 addOperand(Use*)，
        // 而是直接管理一个 Value* 的列表（不通过 Use 对象），那会更简单，
        // 但通常 IR 会有明确的 Use 对象来丰富关系。
        // 如果 Instruction 基类期望直接是 Value*，那么之前的问题可能误导了我们。
        // 但错误信息 "Cannot initialize a parameter of type 'Use *' with an lvalue of type 'Value *'"
        // 强烈暗示 Instruction 的某个方法期望 Use*。
    }
}

// ... toString 和 getter 方法保持不变 ...
void BranchInstruction::toString(std::string & str)
{
    std::string cond_str = (condition) ? condition->getIRName() : "<null_cond>";
    std::string true_label_str = (true_target) ? true_target->getIRName() : "<null_true_label>";
    std::string false_label_str = (false_target) ? false_target->getIRName() : "<null_false_label>";

    str = "bc " + cond_str + ", label " + true_label_str + ", label " + false_label_str;
}

Value * BranchInstruction::getCondition() const
{
    return condition;
}

LabelInstruction * BranchInstruction::getTrueTarget() const
{
    return true_target;
}

LabelInstruction * BranchInstruction::getFalseTarget() const
{
    return false_target;
}