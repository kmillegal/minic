///
/// @file IRGenerator.cpp
/// @brief AST遍历产生线性IR的源文件
/// @author zenglj (zenglj@live.com)
/// @version 1.1
/// @date 2024-11-23
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// <tr><td>2024-11-23 <td>1.1     <td>zenglj  <td>表达式版增强
/// </table>
///
#include <cstdint>
#include <cstdio>
#include <unordered_map>
#include <vector>
#include <iostream>

#include "AST.h"
#include "ArrayType.h"
#include "ConstInt.h"
#include "IntegerType.h"
#include "LocalVariable.h"
#include "PointerType.h"
#include "Common.h"
#include "Function.h"
#include "IRCode.h"
#include "IRGenerator.h"
#include "Instruction.h"
#include "MemVariable.h"
#include "Module.h"
#include "EntryInstruction.h"
#include "LabelInstruction.h"
#include "ExitInstruction.h"
#include "FuncCallInstruction.h"
#include "BinaryInstruction.h"
#include "MoveInstruction.h"
#include "GotoInstruction.h"
#include "BranchInstruction.h"

/// @brief 构造函数
/// @param _root AST的根
/// @param _module 符号表
IRGenerator::IRGenerator(ast_node * _root, Module * _module) : root(_root), module(_module)
{
    /* 叶子节点 */
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_LITERAL_UINT] = &IRGenerator::ir_leaf_node_uint;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_VAR_ID] = &IRGenerator::ir_leaf_node_var_id;
    ast2ir_handlers[ast_operator_type::AST_OP_LEAF_TYPE] = &IRGenerator::ir_leaf_node_type;

    /* 表达式运算， 加减乘除取余 */
    ast2ir_handlers[ast_operator_type::AST_OP_SUB] = &IRGenerator::ir_sub;
    ast2ir_handlers[ast_operator_type::AST_OP_ADD] = &IRGenerator::ir_add;
    ast2ir_handlers[ast_operator_type::AST_OP_MINUS] = &IRGenerator::ir_minus;
    ast2ir_handlers[ast_operator_type::AST_OP_MUL] = &IRGenerator::ir_mul;
    ast2ir_handlers[ast_operator_type::AST_OP_DIV] = &IRGenerator::ir_div;
    ast2ir_handlers[ast_operator_type::AST_OP_MOD] = &IRGenerator::ir_mod;
    ast2ir_handlers[ast_operator_type::AST_OP_EQ] = &IRGenerator::ir_eq;
    ast2ir_handlers[ast_operator_type::AST_OP_NE] = &IRGenerator::ir_ne;
    ast2ir_handlers[ast_operator_type::AST_OP_LE] = &IRGenerator::ir_le;
    ast2ir_handlers[ast_operator_type::AST_OP_LT] = &IRGenerator::ir_lt;
    ast2ir_handlers[ast_operator_type::AST_OP_GE] = &IRGenerator::ir_ge;
    ast2ir_handlers[ast_operator_type::AST_OP_GT] = &IRGenerator::ir_gt;
    /* 逻辑运算 */
    ast2ir_handlers[ast_operator_type::AST_OP_AND] = &IRGenerator::ir_and;
    ast2ir_handlers[ast_operator_type::AST_OP_OR] = &IRGenerator::ir_or;
    ast2ir_handlers[ast_operator_type::AST_OP_NOT] = &IRGenerator::ir_not;

    /* 语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_ASSIGN] = &IRGenerator::ir_assign;
    ast2ir_handlers[ast_operator_type::AST_OP_RETURN] = &IRGenerator::ir_return;
    ast2ir_handlers[ast_operator_type::AST_OP_IF] = &IRGenerator::ir_if;
    ast2ir_handlers[ast_operator_type::AST_OP_WHILE] = &IRGenerator::ir_while;
    ast2ir_handlers[ast_operator_type::AST_OP_BREAK] = &IRGenerator::ir_break;
    ast2ir_handlers[ast_operator_type::AST_OP_CONTINUE] = &IRGenerator::ir_continue;

    /* 数组 */
    ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_DECL] = &IRGenerator::ir_array_declare;
    ast2ir_handlers[ast_operator_type::AST_OP_ARRAY_ACCESS] = &IRGenerator::ir_array_access;

    /* 函数调用 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_CALL] = &IRGenerator::ir_function_call;

    /* 函数定义 */
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_DEF] = &IRGenerator::ir_function_define;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAMS] = &IRGenerator::ir_function_formal_params;
    ast2ir_handlers[ast_operator_type::AST_OP_FUNC_FORMAL_PARAM] = &IRGenerator::ir_function_formal_param;

    /* 变量定义语句 */
    ast2ir_handlers[ast_operator_type::AST_OP_DECL_STMT] = &IRGenerator::ir_declare_statment;
    ast2ir_handlers[ast_operator_type::AST_OP_VAR_DECL] = &IRGenerator::ir_variable_declare;

    /* 语句块 */
    ast2ir_handlers[ast_operator_type::AST_OP_BLOCK] = &IRGenerator::ir_block;

    /* 编译单元 */
    ast2ir_handlers[ast_operator_type::AST_OP_COMPILE_UNIT] = &IRGenerator::ir_compile_unit;
}

/// @brief 遍历抽象语法树产生线性IR，保存到IRCode中
/// @param root 抽象语法树
/// @param IRCode 线性IR
/// @return true: 成功 false: 失败
bool IRGenerator::run()
{
    ast_node * node;

    // 从根节点进行遍历
    node = ir_visit_ast_node(root);

    return node != nullptr;
}

/// @brief 根据AST的节点运算符查找对应的翻译函数并执行翻译动作
/// @param node AST节点
/// @return 成功返回node节点，否则返回nullptr
ast_node * IRGenerator::ir_visit_ast_node(ast_node * node)
{
    // 空节点
    if (nullptr == node) {
        return nullptr;
    }

    bool result;

    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);
    if (pIter == ast2ir_handlers.end()) {
        // 没有找到，则说明当前不支持
        result = (this->ir_default)(node);
    } else {
        result = (this->*(pIter->second))(node);
    }

    if (!result) {
        // 语义解析错误，则出错返回
        node = nullptr;
    }

    return node;
}

/// @brief 访问AST节点，用于将其作为条件处理，并根据结果跳转。
/// @param node AST节点。
/// @param true_target 如果 node 求值为真，则跳转到此标签。
/// @param false_target 如果 node 求值为假，则跳转到此标签。
/// @return 处理是否成功。node->blockInsts 将包含生成的指令。
bool IRGenerator::ir_visit_for_condition(ast_node * node,
                                         LabelInstruction * true_target,
                                         LabelInstruction * false_target)
{
    if (!node || !true_target || !false_target) {
        // 适当的错误记录或返回
        return false;
    }
    if (!module->getCurrentFunction()) { // 确保在函数上下文中
        return false;
    }

    // 保存外部的 true/false 目标，因为递归调用可能会修改它们
    LabelInstruction * prev_true_target = m_current_true_target;
    LabelInstruction * prev_false_target = m_current_false_target;

    m_current_true_target = true_target;
    m_current_false_target = false_target;

    ast_node * result_node = ir_visit_ast_node_recursive(node); // 调用核心的递归访问

    // 恢复之前的 true/false 目标，以支持嵌套的逻辑运算
    m_current_true_target = prev_true_target;
    m_current_false_target = prev_false_target;

    return (result_node != nullptr);
}

/// @brief 访问AST节点的核心递归函数
/// @param node AST节点
/// @return 处理后的节点，可能是 nullptr
ast_node * IRGenerator::ir_visit_ast_node_recursive(ast_node * node)
{
    // 空节点检查
    if (nullptr == node) {
        return nullptr;
    }

    bool success_of_handler; // 用于接收具体处理器函数的返回值

    // 从映射表中查找对应当前节点类型的处理函数
    std::unordered_map<ast_operator_type, ast2ir_handler_t>::const_iterator pIter;
    pIter = ast2ir_handlers.find(node->node_type);

    if (pIter == ast2ir_handlers.end()) {
        // 没有找到特定的处理器，使用默认处理器
        success_of_handler = (this->ir_default)(node);
    } else {
        success_of_handler = (this->*(pIter->second))(node);
    }

    // 检查处理器函数是否成功执行
    if (!success_of_handler) {
        // 具体的处理器函数（如 ir_lt, ir_add）应该在失败时记录错误信息
        // 这里可以选择进一步处理或直接返回nullptr表示失败
        return nullptr;
    }

    if (node->val && m_current_true_target && m_current_false_target) {
        bool is_control_flow_sensitive_op =
            (node->node_type == ast_operator_type::AST_OP_AND || node->node_type == ast_operator_type::AST_OP_OR ||
             node->node_type == ast_operator_type::AST_OP_NOT || node->node_type == ast_operator_type::AST_OP_IF ||
             node->node_type == ast_operator_type::AST_OP_EQ || node->node_type == ast_operator_type::AST_OP_NE ||
             node->node_type == ast_operator_type::AST_OP_LT || node->node_type == ast_operator_type::AST_OP_LE ||
             node->node_type == ast_operator_type::AST_OP_GT || node->node_type == ast_operator_type::AST_OP_GE);
        // 注意: return 语句也是控制流敏感的，但它不使用真假出口，而是直接跳转或结束。
        // func_call 如果有返回值，其返回值可以作为条件，所以它不是控制流敏感的（除非它永不返回）。

        if (!is_control_flow_sensitive_op) {
            Function * currentFunc = module->getCurrentFunction();
            if (currentFunc) { // 确保有当前函数上下文来创建指令
                node->blockInsts.addInst(
                    new BranchInstruction(currentFunc, node->val, m_current_true_target, m_current_false_target));
                node->val = nullptr; // 结果已经通过控制流体现，此节点不应再向上层传递值
                                     // 除非上层逻辑有特殊需求。
            } else {
                // 错误：在没有当前函数的情况下尝试创建分支指令
                minic_log(LOG_ERROR,
                          "Attempted to create branch instruction outside of a function context for node type %d",
                          (int) node->node_type);
                return nullptr; // 指示错误
            }
        }
    }

    // 如果一切顺利，返回处理过的节点
    return node;
}

/// @brief 未知节点类型的节点处理
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_default(ast_node * node)
{
    // 未知的节点
    printf("Unknown node(%d)\n", (int) node->node_type);
    return true;
}

/// @brief 编译单元AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_compile_unit(ast_node * node)
{
    module->setCurrentFunction(nullptr);

    for (auto son: node->sons) {

        // 遍历编译单元，要么是函数定义，要么是语句
        ast_node * son_node = ir_visit_ast_node(son);
        if (!son_node) {
            // TODO 自行追加语义错误处理
            return false;
        }
    }

    return true;
}

/// @brief 函数定义AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_define(ast_node * node)
{
    bool result;

    // 创建一个函数，用于当前函数处理
    if (module->getCurrentFunction()) {
        // 函数中嵌套定义函数，这是不允许的，错误退出
        // TODO 自行追加语义错误处理
        return false;
    }

    // 函数定义的AST包含四个孩子
    // 第一个孩子：函数返回类型
    // 第二个孩子：函数名字
    // 第三个孩子：形参列表
    // 第四个孩子：函数体即block
    ast_node * type_node = node->sons[0];
    ast_node * name_node = node->sons[1];
    ast_node * param_node = node->sons[2];
    ast_node * block_node = node->sons[3];

    // 创建一个新的函数定义
    Function * newFunc = module->newFunction(name_node->name, type_node->type);
    if (!newFunc) {
        // 新定义的函数已经存在，则失败返回。
        // TODO 自行追加语义错误处理
        return false;
    }

    // 当前函数设置有效，变更为当前的函数
    module->setCurrentFunction(newFunc);

    // 进入函数的作用域
    module->enterScope();

    // 获取函数的IR代码列表，用于后面追加指令用，注意这里用的是引用传值
    InterCode & irCode = newFunc->getInterCode();

    // 这里也可增加一个函数入口Label指令，便于后续基本块划分
    LabelInstruction * entryLabelInst = new LabelInstruction(newFunc);

    irCode.addInst(entryLabelInst);

    // 创建并加入Entry入口指令
    irCode.addInst(new EntryInstruction(newFunc));

    // 创建出口指令并不加入出口指令，等函数内的指令处理完毕后加入出口指令
    LabelInstruction * exitLabelInst = new LabelInstruction(newFunc);

    // 函数出口指令保存到函数信息中，因为在语义分析函数体时return语句需要跳转到函数尾部，需要这个label指令
    newFunc->setExitLabel(exitLabelInst);

    m_collected_formal_params.clear(); // 清空为当前函数收集的形参列表
    this->current_formal_param_index_ = 0;

    // 遍历形参，没有IR指令，不需要追加
    result = ir_function_formal_params(param_node);
    if (!result) {
        // 形参解析失败
        // TODO 自行追加语义错误处理
        return false;
    }
    node->blockInsts.addInst(param_node->blockInsts);

    newFunc->setFormalParams(m_collected_formal_params);

    // 新建一个Value，用于保存函数的返回值，如果没有返回值可不用申请
    LocalVariable * retValue = nullptr;
    if (!type_node->type->isVoidType()) {

        // 保存函数返回值变量到函数信息中，在return语句翻译时需要设置值到这个变量中
        retValue = static_cast<LocalVariable *>(module->newVarValue(type_node->type));
    }
    newFunc->setReturnValue(retValue);

    // 这里最好设置返回值变量的初值为0，以便在没有返回值时能够返回0

    // 函数内已经进入作用域，内部不再需要做变量的作用域管理
    block_node->needScope = false;

    // 遍历block
    result = ir_block(block_node);
    if (!result) {
        // block解析失败
        // TODO 自行追加语义错误处理
        return false;
    }

    // IR指令追加到当前的节点中
    node->blockInsts.addInst(block_node->blockInsts);

    // 此时，所有指令都加入到当前函数中，也就是node->blockInsts

    // node节点的指令移动到函数的IR指令列表中
    irCode.addInst(node->blockInsts);

    // 添加函数出口Label指令，主要用于return语句跳转到这里进行函数的退出
    irCode.addInst(exitLabelInst);

    // 函数出口指令
    irCode.addInst(new ExitInstruction(newFunc, retValue));

    // 恢复成外部函数
    module->setCurrentFunction(nullptr);

    // 退出函数的作用域
    module->leaveScope();

    return true;
}

/// @brief 形式列表AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_params(ast_node * node)
{
    bool result = true;
    // 形参列表的AST节点包含多个形参节点
    for (auto son: node->sons) {
        // 遍历形参节点
        result = ir_function_formal_param(son);
        if (!result) {
            // TODO 自行追加语义错误处理
            break;
        }
    }

    return result;
}

/// @brief 形式参数AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_formal_param(ast_node * node)

// 每个形参变量都创建对应的临时变量 (param_temp_var)，用于表达实参转递的值
// 而真实的形参则创建函数内的局部变量 (param_var)。
// 然后产生赋值指令，用于把表达实参值的临时变量拷贝到形参局部变量上 (param_var = param_temp_var)。
// 请注意这些指令要放在Entry指令后面，因此处理的先后上要注意。

{
    // 获取当前函数
    Function * current_function = module->getCurrentFunction();

    ast_node * type_ast_node = node->sons[0]; // 类型节点
    ast_node * declaration_node = node->sons[1];

    std::string param_name;
    ast_node * id_ast_node; // 指向代表参数名的那个叶子节点

    Type * base_type = type_ast_node->type;
    Type * effective_param_type = nullptr;
    if (declaration_node->node_type == ast_operator_type::AST_OP_ARRAY_DECL)
        {
        // 参数是数组

        // 提取名字和维度信息节点
        id_ast_node = declaration_node->sons[0];
        param_name = id_ast_node->name;
        ast_node * dimensions_node = declaration_node->sons[1];

        Type * final_type = base_type;

        // 从内到外包裹类型，所以反向遍历维度列表
        for (auto it = dimensions_node->sons.rbegin(); it != dimensions_node->sons.rend(); ++it) {
            ast_node * dim_expr_node = *it;
            uint64_t dim_size = dim_expr_node->integer_val;

            final_type = new ArrayType(final_type, dim_size);
        }

        effective_param_type = final_type;

    } else {
        // 参数是普通变量
        id_ast_node = declaration_node;
        param_name = id_ast_node->name;
        effective_param_type = base_type;
    }

    // 创建 FormalParam 对象。这个对象代表了参数传递的接口。
    FormalParam * formal_param_value = new FormalParam(effective_param_type, param_name);

    // 将创建的 FormalParam 对象添加到 IRGenerator 的临时收集中
    m_collected_formal_params.push_back(formal_param_value);

    // 将这个 FormalParam 对象 (formal_param_value) 与形式参数的AST节点 (node) 关联。
    // 这样，如果上层结构（如函数调用处生成实参传递）需要引用这个参数槽，可以通过 node->val 访问。
    node->val = formal_param_value;

    // 为函数体内部使用创建一个局部变量 (param_local_var)。
    LocalVariable * param_local_var = static_cast<LocalVariable *>(module->newVarValue(effective_param_type, param_name));
    // 将此函数内部的局部变量与参数的标识符AST节点(id_ast_node)关联。
    id_ast_node->val = param_local_var;

    // 创建一个赋值指令 (MoveInstruction)，用于将实际参数的值（存储在 formal_param_value 代表的槽中）
    // 拷贝到函数体内部使用的局部变量 (param_local_var) 上。
    MoveInstruction * moveInst = new MoveInstruction(current_function, param_local_var, formal_param_value);

    // 将此 MoveInstruction 添加到当前函数的IR代码列表中。
    // 这应该在函数入口指令 (EntryInstruction) 之后，实际函数体代码之前。
    current_function->getInterCode().addInst(moveInst);

    return true;
}

/// @brief 函数调用AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_function_call(ast_node * node)
{
    std::vector<Value *> realParams;

    // 获取当前正在处理的函数
    Function * currentFunc = module->getCurrentFunction();

    // 函数调用的节点包含两个节点：
    // 第一个节点：函数名节点
    // 第二个节点：实参列表节点

    std::string funcName = node->sons[0]->name;
    int64_t lineno = node->sons[0]->line_no;

    ast_node * paramsNode = node->sons[1];

    // 根据函数名查找函数，看是否存在。若不存在则出错
    // 这里约定函数必须先定义后使用
    auto calledFunction = module->findFunction(funcName);
    if (nullptr == calledFunction) {
        minic_log(LOG_ERROR, "函数(%s)未定义或声明", funcName.c_str());
        return false;
    }

    // 当前函数存在函数调用
    currentFunc->setExistFuncCall(true);

    // 如果没有孩子，也认为是没有参数
    if (!paramsNode->sons.empty()) {

        int32_t argsCount = (int32_t) paramsNode->sons.size();

        // 当前函数中调用函数实参个数最大值统计，实际上是统计实参传参需在栈中分配的大小
        // 因为目前的语言支持的int和float都是四字节的，只统计个数即可
        if (argsCount > currentFunc->getMaxFuncCallArgCnt()) {
            currentFunc->setMaxFuncCallArgCnt(argsCount);
        }
        auto & formal_params = calledFunction->getParams();
        // 遍历参数列表，孩子是表达式
        // 这里自左往右计算表达式
        for (size_t i = 0; i < paramsNode->sons.size(); ++i) {
            ast_node * son = paramsNode->sons[i];
            ast_node * temp= son;
            const Type * formal_type = formal_params[i]->getType();
            if (son->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS) {
                if (formal_type->isArrayType()) {
                    bool success = ir_array_declare(temp);
                    if (!success) {
                        return false;
                    }
                } else {
                    temp = ir_visit_ast_node(son);
                    if (!temp) {
                        return false;
                    }
                }
            }
            else
            {
                temp = ir_visit_ast_node(son);
                if (!temp) {
                    return false;
                }

            }
            // 遍历Block的每个语句，进行显示或者运算
            // ast_node * temp = ir_visit_ast_node(son);
            // if (!temp) {
            //     return false;
            // }
            realParams.push_back(temp->val);
            node->blockInsts.addInst(temp->blockInsts);
        }
    }

    // TODO 这里请追加函数调用的语义错误检查，这里只进行了函数参数的个数检查等，其它请自行追加。
    if (realParams.size() != calledFunction->getParams().size()) {
        // 函数参数的个数不一致，语义错误
        minic_log(LOG_ERROR, "第%lld行的被调用函数(%s)未定义或声明", (long long) lineno, funcName.c_str());
        return false;
    }

    // 返回调用有返回值，则需要分配临时变量，用于保存函数调用的返回值
    Type * type = calledFunction->getReturnType();

    FuncCallInstruction * funcCallInst = new FuncCallInstruction(currentFunc, calledFunction, realParams, type);

    // 创建函数调用指令
    node->blockInsts.addInst(funcCallInst);

    // 函数调用结果Value保存到node中，可能为空，上层节点可利用这个值
    node->val = funcCallInst;

    return true;
}

/// @brief 语句块（含函数体）AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_block(ast_node * node)
{
    // 进入作用域
    if (node->needScope) {
        module->enterScope();
    }

    std::vector<ast_node *>::iterator pIter;
    for (pIter = node->sons.begin(); pIter != node->sons.end(); ++pIter) {

        // 遍历Block的每个语句，进行显示或者运算
        ast_node * temp = ir_visit_ast_node(*pIter);
        if (!temp) {
            return false;
        }

        node->blockInsts.addInst(temp->blockInsts);
    }

    // 离开作用域
    if (node->needScope) {
        module->leaveScope();
    }

    return true;
}

/// @brief 整数加法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_add(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * addInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_ADD_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(addInst);

    node->val = addInst;

    return true;
}

/// @brief 整数减法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_sub(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 加法节点，左结合，先计算左节点，后计算右节点

    // 加法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 加法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * subInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_SUB_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(subInst);

    node->val = subInst;

    return true;
}

/// @brief 求负运算AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_minus(ast_node * node)
{

    Function * currentFunc = module->getCurrentFunction(); // Get current function context
    ast_node * src1_node = node->sons[0];

    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    // 一元减法节点，直接计算左节点

    // 一元减法的操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    node->blockInsts.addInst(left->blockInsts);
    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * minusInst = new BinaryInstruction(currentFunc,
                                                          IRInstOperator::IRINST_OP_NEG_I,
                                                          left->val,
                                                          nullptr,
                                                          IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    // node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(minusInst);

    node->val = minusInst;

    return true;
}

/// @brief 整数乘法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mul(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 乘法节点，左结合，先计算左节点，后计算右节点

    // 乘法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 乘法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * mulInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_MUL_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(mulInst);

    node->val = mulInst;

    return true;
}
/// @brief 整数除法AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_div(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 除法节点，左结合，先计算左节点，后计算右节点

    // 除法的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 除法的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * divInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_DIV_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(divInst);

    node->val = divInst;

    return true;
}
/// @brief 整数取模AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_mod(ast_node * node)
{
    ast_node * src1_node = node->sons[0];
    ast_node * src2_node = node->sons[1];

    // 取模节点，左结合，先计算左节点，后计算右节点

    // 取模的左边操作数
    ast_node * left = ir_visit_ast_node(src1_node);
    if (!left) {
        // 某个变量没有定值
        return false;
    }

    // 取模的右边操作数
    ast_node * right = ir_visit_ast_node(src2_node);
    if (!right) {
        // 某个变量没有定值
        return false;
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理

    BinaryInstruction * modInst = new BinaryInstruction(module->getCurrentFunction(),
                                                        IRInstOperator::IRINST_OP_MOD_I,
                                                        left->val,
                                                        right->val,
                                                        IntegerType::getTypeInt());

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(left->blockInsts);
    node->blockInsts.addInst(right->blockInsts);
    node->blockInsts.addInst(modInst);

    node->val = modInst;

    return true;
}

/// @brief 整数等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_eq(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_...
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction = new BinaryInstruction(currentFunc,
                                                                IRInstOperator::IRINST_OP_EQ_I, // 整数等于比较的操作码
                                                                left_val_node->val,             // %l1 (a)
                                                                right_val_node->val,            // %l2 (b)
                                                                comparison_result_type          // 比较结果的类型
    );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 整数不等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ne(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_...
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction =
        new BinaryInstruction(currentFunc,
                              IRInstOperator::IRINST_OP_NE_I, // 整数不等于比较的操作码
                              left_val_node->val,             // %l1 (a)
                              right_val_node->val,            // %l2 (b)
                              comparison_result_type          // 比较结果的类型
        );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 整数大于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_ge(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_...
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction =
        new BinaryInstruction(currentFunc,
                              IRInstOperator::IRINST_OP_GE_I, // 整数大于等于比较的操作码
                              left_val_node->val,             // %l1 (a)
                              right_val_node->val,            // %l2 (b)
                              comparison_result_type          // 比较结果的类型
        );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 整数大于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_gt(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_...
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction = new BinaryInstruction(currentFunc,
                                                                IRInstOperator::IRINST_OP_GT_I, // 整数大于比较的操作码
                                                                left_val_node->val,             // %l1 (a)
                                                                right_val_node->val,            // %l2 (b)
                                                                comparison_result_type          // 比较结果的类型
    );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 整数小于等于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_le(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_...
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction =
        new BinaryInstruction(currentFunc,
                              IRInstOperator::IRINST_OP_LE_I, // 整数小于等于比较的操作码
                              left_val_node->val,             // %l1 (a)
                              right_val_node->val,            // %l2 (b)
                              comparison_result_type          // 比较结果的类型
        );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 整数小于AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_lt(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * left_child_node = node->sons[0];
    ast_node * right_child_node = node->sons[1];

    // 递归访问左右子节点以计算它们的值。
    LabelInstruction * original_true_target = m_current_true_target;
    LabelInstruction * original_false_target = m_current_false_target;
    m_current_true_target = nullptr;
    m_current_false_target = nullptr;

    ast_node * left_val_node = ir_visit_ast_node_recursive(left_child_node);

    ast_node * right_val_node = ir_visit_ast_node_recursive(right_child_node);

    // 恢复 m_current_... 给当前的 LT 运算符使用
    m_current_true_target = original_true_target;
    m_current_false_target = original_false_target;

    // 将子节点生成的指令添加到当前 LT 节点的指令列表中
    node->blockInsts.addInst(left_val_node->blockInsts);
    node->blockInsts.addInst(right_val_node->blockInsts);

    // 创建比较指令 (cmp lt)
    Type * comparison_result_type = IntegerType::getTypeBool();

    BinaryInstruction * cmp_instruction = new BinaryInstruction(currentFunc,
                                                                IRInstOperator::IRINST_OP_LT_I, // 整数小于比较的操作码
                                                                left_val_node->val,             // %l1 (a)
                                                                right_val_node->val,            // %l2 (b)
                                                                comparison_result_type          // 比较结果的类型
    );
    node->blockInsts.addInst(cmp_instruction);
    Value * comparison_result_val = cmp_instruction; // cmp 指令本身就是结果 Value (%t1)

    // 检查是否在条件上下文中 (即父节点是否传递了真/假出口标签)
    if (m_current_true_target && m_current_false_target) {
        // 如果是，则生成条件跳转指令 (bc)
        node->blockInsts.addInst(new BranchInstruction(currentFunc,
                                                       comparison_result_val, // %t1 (比较结果)
                                                       m_current_true_target, // label .L0 (真出口)
                                                       m_current_false_target // label .L1 (假出口)
                                                       ));
        node->val = nullptr; // 结果已经通过控制流体现，此节点不向上层传递值
    } else {
        node->val = comparison_result_val;
    }

    return true;
}

/// @brief 逻辑与运算AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_and(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    // 1. 检查前提条件
    if (!currentFunc) {
        minic_log(LOG_ERROR,
                  "ir_and: Current function is null for node at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }
    // 逻辑与必须在条件上下文中使用，即其父节点（如if）必须已通过 ir_visit_for_condition 设置了真假出口
    if (!m_current_true_target || !m_current_false_target) {
        minic_log(LOG_ERROR,
                  "ir_and: Not called in a conditional context (true/false targets missing from m_current_... members) "
                  "for node at line %lld.",
                  (long long) (node ? node->line_no : -1));

        return false;
    }
    if (!node || node->sons.size() != 2) {
        minic_log(LOG_ERROR,
                  "ir_and: Invalid AND node or insufficient children at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }

    ast_node * left_expr_node = node->sons[0];  // BoolExpr0 (A)
    ast_node * right_expr_node = node->sons[1]; // BoolExpr1 (B)

    // 保存外部（即当前AND表达式整体）的真假出口目标，
    // 因为在递归访问子表达式时，我们会修改 m_current_true_target / m_current_false_target
    LabelInstruction * outer_true_target_for_and = m_current_true_target;
    LabelInstruction * outer_false_target_for_and = m_current_false_target;

    // 2. 创建中间标签 L3 (遵循您流程图的命名)
    // L3: 如果 BoolExpr0 (A) 为真，则跳转到此标签以继续评估 BoolExpr1 (B)
    LabelInstruction * l3_a_true_eval_b_label = new LabelInstruction(currentFunc);

    // --- 翻译 BoolExpr0 (A) ---
    // 为了翻译 A，我们需要设置 A 的真假出口：
    // - 如果 A 为真，跳转到 l3_a_true_eval_b_label (继续评估 B)
    // - 如果 A 为假，短路，直接跳转到整个 AND 表达式的假出口 (outer_false_target_for_and)
    m_current_true_target = l3_a_true_eval_b_label;
    m_current_false_target = outer_false_target_for_and; // 短路！

    // 递归调用核心遍历函数来处理左操作数 A
    if (!ir_visit_ast_node_recursive(left_expr_node)) {
        minic_log(LOG_ERROR,
                  "Failed to generate IR for left operand of AND at line %lld.",
                  (long long) left_expr_node->line_no);
        // 恢复外部的真假目标，以防万一（尽管这里失败了）
        m_current_true_target = outer_true_target_for_and;
        m_current_false_target = outer_false_target_for_and;
        delete l3_a_true_eval_b_label; // 清理未使用的标签
        return false;
    }
    // 将 A 生成的指令添加到当前 AND 节点的指令列表中
    node->blockInsts.addInst(left_expr_node->blockInsts);
    // left_expr_node->val 在这里不直接使用，因为 ir_visit_ast_node_recursive 的尾部逻辑
    // (或者 left_expr_node 本身如果是关系/逻辑运算) 已经根据 A 的结果和
    // 当前设置的 m_current_true_target/m_current_false_target 生成了跳转指令。

    // --- 插入 L3 标签 ---
    // 如果 A 为真，控制流会到达这里
    node->blockInsts.addInst(l3_a_true_eval_b_label);

    // --- 翻译 BoolExpr1 (B) ---
    // 为了翻译 B，我们需要设置 B 的真假出口：
    // - 如果 B 为真，跳转到整个 AND 表达式的真出口 (outer_true_target_for_and)
    // - 如果 B 为假，跳转到整个 AND 表达式的假出口 (outer_false_target_for_and)
    m_current_true_target = outer_true_target_for_and;
    m_current_false_target = outer_false_target_for_and;

    // 递归调用核心遍历函数来处理右操作数 B
    if (!ir_visit_ast_node_recursive(right_expr_node)) {
        minic_log(LOG_ERROR,
                  "Failed to generate IR for right operand of AND at line %lld.",
                  (long long) right_expr_node->line_no);
        // 此时 m_current_... 已经被设置为外部目标，无需再次恢复到 outer_...
        // l3_a_true_eval_b_label 已经被 addInst，由 InterCode 管理
        return false;
    }
    // 将 B 生成的指令添加到当前 AND 节点的指令列表中
    node->blockInsts.addInst(right_expr_node->blockInsts);
    // 同样，right_expr_node->val 不直接使用，跳转已由内部处理。

    node->val = nullptr;

    return true;
}

/// @brief 逻辑或运算AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_or(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    // 1. 检查前提条件
    if (!currentFunc) {
        minic_log(LOG_ERROR,
                  "ir_or: Current function is null for node at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }
    // 逻辑或必须在条件上下文中使用，即其父节点（如if）必须已通过 ir_visit_for_condition 设置了真假出口
    if (!m_current_true_target || !m_current_false_target) {
        minic_log(LOG_ERROR,
                  "ir_or: Not called in a conditional context (true/false targets missing from m_current_... members) "
                  "for node at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }
    if (!node || node->sons.size() != 2) {
        minic_log(LOG_ERROR,
                  "ir_or: Invalid OR node or insufficient children at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }

    ast_node * left_expr_node = node->sons[0];  // BoolExpr0 (A)
    ast_node * right_expr_node = node->sons[1]; // BoolExpr1 (B)

    // 保存外部（即当前OR表达式整体）的真假出口目标，
    // 因为在递归访问子表达式时，我们会修改 m_current_true_target / m_current_false_target
    LabelInstruction * outer_true_target_for_or = m_current_true_target;
    LabelInstruction * outer_false_target_for_or = m_current_false_target;

    // 2. 创建中间标签 L3 (遵循您流程图的命名)
    // L3: 如果 BoolExpr0 (A) 为假，则跳转到此标签以继续评估 BoolExpr1 (B)
    LabelInstruction * l3_a_false_eval_b_label = new LabelInstruction(currentFunc);

    // --- 翻译 BoolExpr0 (A) ---
    // 为了翻译 A，我们需要设置 A 的真假出口：
    // - 如果 A 为真，短路，直接跳转到整个 OR 表达式的真出口 (outer_true_target_for_or)
    m_current_true_target = outer_true_target_for_or; // 短路！
    // - 如果 A 为假，跳转到 l3_a_false_eval_b
    m_current_false_target = l3_a_false_eval_b_label; // 如果 A 为假，跳转到 l3_a_false_eval_b_label

    // 递归调用核心遍历函数来处理左操作数 A
    if (!ir_visit_ast_node_recursive(left_expr_node)) {
        minic_log(LOG_ERROR,
                  "Failed to generate IR for left operand of OR at line %lld.",
                  (long long) left_expr_node->line_no);
        // 恢复外部的真假目标，以防万一（尽管这里失败了）
        m_current_true_target = outer_true_target_for_or;
        m_current_false_target = outer_false_target_for_or;
        delete l3_a_false_eval_b_label; // 清理未使用的标签
        return false;
    }
    // 将 A 生成的指令添加到当前 OR 节点的指令列表中
    node->blockInsts.addInst(left_expr_node->blockInsts);

    // --- 插入 L3 标签 ---
    // 如果 A 为假，控制流会到达这里
    node->blockInsts.addInst(l3_a_false_eval_b_label);
    // --- 翻译 BoolExpr1 (B) ---
    // 为了翻译 B，我们需要设置 B 的真假出口：
    // - 如果 B 为真，跳转到整个 OR 表达式的真出口 (outer_true_target_for_or)
    // - 如果 B 为假，跳转到整个 OR 表达式的假出口 (outer_false_target_for_or)
    m_current_true_target = outer_true_target_for_or;
    m_current_false_target = outer_false_target_for_or;

    // 递归调用核心遍历函数来处理右操作数 B
    if (!ir_visit_ast_node_recursive(right_expr_node)) {
        minic_log(LOG_ERROR,
                  "Failed to generate IR for right operand of OR at line %lld.",
                  (long long) right_expr_node->line_no);
        // 此时 m_current_... 已经被设置为外部目标，无需再次恢复到 outer_...
        // l3_a_false_eval_b_label 已经被 addInst，由 InterCode 管理
        return false;
    }
    // 将 B 生成的指令添加到当前 OR 节点的指令列表中
    node->blockInsts.addInst(right_expr_node->blockInsts);
    // 同样，right_expr_node->val 不直接使用，跳转已由内部处理。

    node->val = nullptr;

    return true;
}

/// @brief 非运算AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_not(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();

    ast_node * operand_A_node = node->sons[0];


    if (m_current_true_target && m_current_false_target) {
        LabelInstruction * original_true_target = m_current_true_target;
        LabelInstruction * original_false_target = m_current_false_target;


        m_current_true_target = original_false_target;
        m_current_false_target = original_true_target;


        if (!ir_visit_ast_node_recursive(operand_A_node)) {

            m_current_true_target = original_true_target;
            m_current_false_target = original_false_target;
            return false;
        }
        node->blockInsts.addInst(operand_A_node->blockInsts);

        m_current_true_target = original_true_target;
        m_current_false_target = original_false_target;

        node->val = nullptr;
    } else {
        LabelInstruction * temp_true_target_for_operand = m_current_true_target;
        LabelInstruction * temp_false_target_for_operand = m_current_false_target;

        m_current_true_target = nullptr;
        m_current_false_target = nullptr;

        ast_node * processed_operand_A = ir_visit_ast_node_recursive(operand_A_node);

        m_current_true_target = temp_true_target_for_operand;
        m_current_false_target = temp_false_target_for_operand;

        node->blockInsts.addInst(processed_operand_A->blockInsts);


        Value * zero = module->newConstInt(0);
        Type * bool_type = IntegerType::getTypeBool();

        BinaryInstruction * eq_instr =
            new BinaryInstruction(currentFunc,
                                  IRInstOperator::IRINST_OP_EQ_I,
                                  processed_operand_A->val,
                                  zero,
                                  bool_type);
        node->blockInsts.addInst(eq_instr);
        node->val = eq_instr;
    }
    return true;
}

/// @brief 赋值AST节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_assign(ast_node * node)
{
    ast_node * son1_node = node->sons[0];
    ast_node * son2_node = node->sons[1];

    // 赋值节点，自右往左运算

    ast_node * right = ir_visit_ast_node(son2_node);
    if (!right) {

        return false;
    }
    // 左值判断是否是数组，是的话调用ir_array_assign，不调用ir_visit_ast_node

    ast_node * left = son1_node;
    //左值为数组,无LOAD指令
    if (son1_node->node_type == ast_operator_type::AST_OP_ARRAY_ACCESS)
    {
		bool success = ir_array_declare(left);
		if (!success) {
			return false;
        }
    }else{
        left = ir_visit_ast_node(son1_node);
        if (!left) {
            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    MoveInstruction * movInst = new MoveInstruction(module->getCurrentFunction(), left->val, right->val);

    // 创建临时变量保存IR的值，以及线性IR指令
    node->blockInsts.addInst(right->blockInsts);
	node->blockInsts.addInst(left->blockInsts);
	node->blockInsts.addInst(movInst);
    // 这里假定赋值的类型是一致的
    node->val = movInst;

    return true;
}

/// @brief return节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_return(ast_node * node)
{
    ast_node * right = nullptr;

    // return语句可能没有没有表达式，也可能有，因此这里必须进行区分判断
    if (!node->sons.empty()) {

        ast_node * son_node = node->sons[0];

        // 返回的表达式的指令保存在right节点中
        right = ir_visit_ast_node(son_node);
        if (!right) {

            // 某个变量没有定值
            return false;
        }
    }

    // 这里只处理整型的数据，如需支持实数，则需要针对类型进行处理
    Function * currentFunc = module->getCurrentFunction();

    // 返回值存在时则移动指令到node中
    if (right) {

        // 创建临时变量保存IR的值，以及线性IR指令
        node->blockInsts.addInst(right->blockInsts);

        // 返回值赋值到函数返回值变量上，然后跳转到函数的尾部
        node->blockInsts.addInst(new MoveInstruction(currentFunc, currentFunc->getReturnValue(), right->val));

        node->val = right->val;
    } else {
        // 没有返回值
        node->val = nullptr;
    }

    // 跳转到函数的尾部出口指令上
    node->blockInsts.addInst(new GotoInstruction(currentFunc, currentFunc->getExitLabel()));

    return true;
}

/// @brief if节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_if(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        minic_log(LOG_ERROR, "IRGenerator::ir_if called outside of a function context.");
        return false;
    }

    // AST结构: node->sons[0]=condition, node->sons[1]=then_block, node->sons[2]=else_block (optional)
    if (node->sons.empty()) {
        minic_log(LOG_ERROR,
                  "Invalid if statement AST node: missing condition  %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }

    ast_node * cond_node = node->sons[0];
    ast_node * then_node = node->sons[1];
    ast_node * else_node = nullptr;
    bool has_else = (node->sons.size() > 2 && node->sons[2] != nullptr);

    if (has_else) {
        else_node = node->sons[2];
    }

    // 1. 创建需要的标签
    LabelInstruction * then_label = new LabelInstruction(currentFunc); // 真出口 (then 块的开始)
    LabelInstruction * else_actual_label = nullptr;                    // 假出口 (else 块的开始, 如果有 else)
    LabelInstruction * end_if_label = new LabelInstruction(currentFunc); // if 语句结束后的汇合点

    if (has_else) {
        else_actual_label = new LabelInstruction(currentFunc);
    }

    // 确定条件为假时的跳转目标：
    // - 如果有 else 块，则跳转到 else_actual_label
    // - 如果没有 else 块，则直接跳转到 end_if_label (跳过 then 块)
    LabelInstruction * false_target_for_condition = has_else ? else_actual_label : end_if_label;

    // 2. 翻译条件表达式 (cond_node)，并传递真/假出口标签
    //    ir_visit_for_condition 会负责让 cond_node 生成跳转到 then_label (如果为真)
    //    或 false_target_for_condition (如果为假) 的指令。
    if (!ir_visit_for_condition(cond_node, then_label, false_target_for_condition)) {
        minic_log(LOG_ERROR, "Failed to generate IR for if-condition at line %lld.", (long long) cond_node->line_no);
        // 清理已创建的标签
        delete then_label;
        delete else_actual_label;
        delete end_if_label;
        return false;
    }
    // 将条件表达式生成的所有指令 (包括其内部的 cmp 和 bc/BranchInstruction)
    // 添加到 if 语句节点的指令列表中。
    node->blockInsts.addInst(cond_node->blockInsts);

    // 3. 添加 then_label 标记 then 块的开始，并翻译 then_block
    node->blockInsts.addInst(then_label);
    // 翻译 then_block
    if (then_node) {
        ast_node * processed_then_node = ir_visit_ast_node(then_node);
        if (!processed_then_node) {
            minic_log(LOG_ERROR,
                      "Failed to generate IR for if-then block at line %lld.",
                      (long long) then_node->line_no);
            // 标签可能已加入 InterCode，应由其管理，但如果这里要手动清理，需小心
            // delete else_actual_label; (if created and not added)
            // delete end_if_label; (if not added)
            return false;
        }
        node->blockInsts.addInst(processed_then_node->blockInsts);
    }


    // 4. 处理 else_block (如果存在)
    if (has_else) {
        // 在 then_block 执行完毕后，必须无条件跳转到 end_if_label，以跳过 else_block
        node->blockInsts.addInst(new GotoInstruction(currentFunc, end_if_label));

        // 添加 else_actual_label 标记 else 块的开始
        node->blockInsts.addInst(else_actual_label);

        // 翻译 else_block
        ast_node * processed_else_node = ir_visit_ast_node(else_node);
        if (!processed_else_node) {
            minic_log(LOG_ERROR,
                      "Failed to generate IR for if-else block at line %lld.",
                      (long long) else_node->line_no);
            // delete end_if_label; (if not added)
            return false;
        }
        node->blockInsts.addInst(processed_else_node->blockInsts);
        // else_block 执行完毕后会自然流向 end_if_label
    }
    // 如果没有 else, 并且原始条件为假，则 false_target_for_condition (即 end_if_label) 会被跳转到。
    // 如果原始条件为真，then_block 执行完毕后会自然流向 end_if_label (因为没有 else 分支后的 goto)。

    // 5. 添加 end_if_label 作为 if 语句的汇合点
    node->blockInsts.addInst(end_if_label);

    // if 语句本身不产生值
    node->val = nullptr;

    return true;
}

/// @brief while节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_while(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        minic_log(LOG_ERROR, "IRGenerator::ir_while called outside of a function context.");
        return false;
    }

    // AST结构: node->sons[0]=condition, node->sons[1]=block
    if (node->sons.empty()) {
        minic_log(LOG_ERROR,
                  "Invalid while statement AST node: missing condition at line %lld.",
                  (long long) (node ? node->line_no : -1));
        return false;
    }

    ast_node * cond_node = node->sons[0];
    ast_node * block_node = node->sons[1];

    // 1. 创建需要的标签
    LabelInstruction * loop_start_label = new LabelInstruction(currentFunc); // 循环入口
    LabelInstruction * loop_body_label = new LabelInstruction(currentFunc);  // 循环体入口
    LabelInstruction * loop_end_label = new LabelInstruction(currentFunc);   // 循环出口

    // 2. 添加循环开始标签
    node->blockInsts.addInst(loop_start_label);
    m_current_loop_start_label = loop_start_label; // 记录当前循环的开始标签
    m_current_loop_end_label = loop_end_label;     // 记录当前循环的结束标签

    // 3. 翻译条件表达式 (cond_node)，并传递真/假出口标签
    if (!ir_visit_for_condition(cond_node, loop_body_label, loop_end_label)) {
        minic_log(LOG_ERROR, "Failed to generate IR for while-condition at line %lld.", (long long) cond_node->line_no);
        delete loop_start_label;
        delete loop_body_label;
        delete loop_end_label;
        return false;
    }
    // 将条件表达式生成的所有指令添加到 while 语句节点的指令列表中。
    node->blockInsts.addInst(cond_node->blockInsts);

    // 4. 添加循环体标签，并翻译循环体块
    node->blockInsts.addInst(loop_body_label);
    ast_node * processed_block_node = ir_visit_ast_node(block_node);
    if (!processed_block_node) {
        minic_log(LOG_ERROR, "Failed to generate IR for while-block at line %lld.", (long long) block_node->line_no);
        delete loop_start_label;
        delete loop_body_label;
        delete loop_end_label;
        return false;
    }
    node->blockInsts.addInst(processed_block_node->blockInsts);
    // 5. 添加跳转到循环开始标签的指令
    node->blockInsts.addInst(new GotoInstruction(currentFunc, loop_start_label));
    // 6. 添加循环结束标签
    node->blockInsts.addInst(loop_end_label);
    // 7. while 语句本身不产生值
    node->val = nullptr;
    return true;
}

/// @brief break节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_break(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        minic_log(LOG_ERROR, "IRGenerator::ir_break called outside of a function context.");
        return false;
    }

    // 1. 检查当前是否在循环中
    if (!m_current_loop_end_label) {
        minic_log(LOG_ERROR, "IRGenerator::ir_break called outside of a loop context.");
        return false;
    }

    // 2. 添加跳转到循环结束标签的指令
    node->blockInsts.addInst(new GotoInstruction(currentFunc, m_current_loop_end_label));

    // 3. break 语句本身不产生值
    node->val = nullptr;

    return true;
}

/// @brief continue节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_continue(ast_node * node)
{
    Function * currentFunc = module->getCurrentFunction();
    if (!currentFunc) {
        minic_log(LOG_ERROR, "IRGenerator::ir_continue called outside of a function context.");
        return false;
    }

    // 1. 检查当前是否在循环中
    if (!m_current_loop_start_label) {
        minic_log(LOG_ERROR, "IRGenerator::ir_continue called outside of a loop context.");
        return false;
    }

    // 2. 添加跳转到循环开始标签的指令
    node->blockInsts.addInst(new GotoInstruction(currentFunc, m_current_loop_start_label));

    // 3. continue 语句本身不产生值
    node->val = nullptr;

    return true;
}

/// @brief 类型叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_type(ast_node * node)
{
    // 不需要做什么，直接从节点中获取即可。

    return true;
}

/// @brief 标识符叶子节点翻译成线性中间IR，变量声明的不走这个语句
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_var_id(ast_node * node)
{
    Value * val;

    // 查找ID型Value
    // 变量，则需要在符号表中查找对应的值

    val = module->findVarValue(node->name);

    node->val = val;

    return true;
}

/// @brief 无符号整数字面量叶子节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_leaf_node_uint(ast_node * node)
{
    ConstInt * val;

    // 新建一个整数常量Value
    val = module->newConstInt((int32_t) node->integer_val);

    node->val = val;

    return true;
}

/// @brief 变量声明语句节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_declare_statment(ast_node * node)
{
    bool result = false;

    for (auto & child: node->sons) {

        // 遍历每个变量声明
        result = ir_variable_declare(child);
        if (!result) {
            break;
        }
        node->blockInsts.addInst(child->blockInsts);
    }

    return result;
}

/// @brief 变量定声明节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_variable_declare(ast_node * node)
{
    // 共有两个孩子，第一个类型，第二个变量名
    // 第二个可能为变量名或赋值语句
    // TODO 这里可强化类型等检查

    ast_node * right_node = node->sons[1];
    ast_node * left_node = node->sons[0];
    if (right_node->node_type == ast_operator_type::AST_OP_ASSIGN) {

        ast_node * var_name_node = right_node->sons[0];
        ast_node * init_val_node_ast = right_node->sons[1];
        std::string var_name;
        Value * var_ir_value;
        var_name = var_name_node->name;

        // 创建变量的IR Value
        var_ir_value = module->newVarValue(left_node->type, var_name);
        node->val = var_ir_value;

        // 翻译初始化表达式
        ast_node * translated_init_val_node = ir_visit_ast_node(init_val_node_ast);
        if (!translated_init_val_node) {
            // 初始化表达式翻译失败
            return false;
        }

        // 添加翻译初始化表达式所生成的指令
        node->blockInsts.addInst(translated_init_val_node->blockInsts);

        // 创建Move指令，将初始化表达式的结果赋给新声明的变量
        MoveInstruction * movInst =
            new MoveInstruction(module->getCurrentFunction(), var_ir_value, translated_init_val_node->val);
        node->blockInsts.addInst(movInst);
    } else if (right_node->node_type == ast_operator_type::AST_OP_ARRAY_DECL) {
        ast_node * var_name_node = right_node->sons[0];
        ast_node * array_size_node = right_node->sons[1];
        std::string var_name;
        var_name = var_name_node->name;

        std::vector<uint64_t> dims;
        for (ast_node * size_node: array_size_node->sons) {
            dims.push_back(size_node->integer_val);
        }
        Type * baseElementType = left_node->type;
        Type * finalArrayType = baseElementType;

        for (auto it = dims.rbegin(); it != dims.rend(); ++it) {
            finalArrayType =new ArrayType(finalArrayType, *it);
        }
        node->val = module->newVarValue(finalArrayType, var_name);
    } else {
        // 直接声明变量，没有初始化
        node->val = module->newVarValue(left_node->type, right_node->name);
    }

    return true;
}

/// @brief 数组元素声明节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_array_declare(ast_node * node)
{
    ast_node * array_name_node = node->sons[0];
    ast_node * index_node = node->sons[1];

    Value * base_addr_val = module->findVarValue(array_name_node->name);
    // 从基地址指针获取数组类型
    const Type * base_type = base_addr_val->getType();
    const Type * type_iterator = nullptr;

    if (base_type->isArrayType() && static_cast<const ArrayType *>(base_type)->getNumElements() == 0) {
        // 函数参数
        type_iterator = base_type;
    } else {
        // 局部/全局数组
        type_iterator = base_type;
    }

    // 翻译所有下标表达式
    std::vector<Value *> index_vals;
    for (ast_node * index_expr_ast: index_node->sons) {
        ast_node * translated_index_node = ir_visit_ast_node(index_expr_ast);
        if (!translated_index_node)
            return false;
        node->blockInsts.addInst(translated_index_node->blockInsts);
        index_vals.push_back(translated_index_node->val);
    }

    // 迭代计算元素偏移量
    Value* element_offset = nullptr;
    if (index_vals.empty()) {
        // 错误：数组访问必须有下标
        return false;
    }
    // if (!index_vals.empty() && type_iterator->isArrayType()) {
    //     type_iterator = static_cast<const ArrayType *>(type_iterator)->getElementType();
    // }
    // TODO int array[20][100] 乘数组大小 用的20，而不是100
    // 偏移量从第一个下标开始
    element_offset = index_vals[0];
    if (type_iterator->isArrayType()) {
        type_iterator = static_cast<const ArrayType *>(type_iterator)->getElementType();
    }
    for (size_t i = 1; i < index_vals.size(); ++i) {

        Value * current_index_val = index_vals[i];
        const ArrayType * currentArrayTy = static_cast<const ArrayType *>(type_iterator);

        Value * dim_size_val = new ConstInt(currentArrayTy->getNumElements());
        element_offset = new BinaryInstruction(module->getCurrentFunction(),
                                               IRInstOperator::IRINST_OP_MUL_I,
                                               element_offset,
                                               dim_size_val,
                                               IntegerType::getTypeInt());
        node->blockInsts.addInst(static_cast<Instruction *>(element_offset));

        // running_offset = running_offset + idxi (idxi 是当前维度的下标值)
        element_offset = new BinaryInstruction(module->getCurrentFunction(),
                                               IRInstOperator::IRINST_OP_ADD_I,
                                               element_offset,
                                               current_index_val,
                                               IntegerType::getTypeInt());
        node->blockInsts.addInst(static_cast<Instruction *>(element_offset));

        // 更新类型迭代器，为下一次循环做准备
        type_iterator = currentArrayTy->getElementType();
    }
    // if (type_iterator->isArrayType()) {
    //     type_iterator = static_cast<const ArrayType *>(type_iterator)->getElementType();
    // }

    // ✅ 关键：在整个循环结束后，type_iterator 指向的是最后一维访问的元素类型。
    // 我们还需要最后再推进一次，来获得最终的基础元素类型。
    // if (!index_vals.empty() && type_iterator->isArrayType()) {
    //     type_iterator = static_cast<const ArrayType *>(type_iterator)->getElementType();
    // }
    // 计算最终的字节偏移量
    int32_t element_size = type_iterator->getSize();
    Value * element_size_val = new ConstInt(element_size);
    Value * byte_offset = new BinaryInstruction(module->getCurrentFunction(),
                                                IRInstOperator::IRINST_OP_MUL_I,
                                                element_offset,
                                                element_size_val,
                                                IntegerType::getTypeInt());
    node->blockInsts.addInst(static_cast<Instruction *>(byte_offset));

    // 计算最终地址 = 基地址 + 字节偏移量
    const Type * final_address_type = PointerType::get(type_iterator);
    BinaryInstruction * final_address = new BinaryInstruction(module->getCurrentFunction(),
                                                  IRInstOperator::IRINST_OP_ADD_I,
                                                  base_addr_val,
                                                  byte_offset,
                                                  const_cast<Type *> (final_address_type));
    node->blockInsts.addInst(final_address);

    node->val = final_address;

    return true;
}
/// @brief 数组元素访问节点翻译成线性中间IR
/// @param node AST节点
/// @return 翻译是否成功，true：成功，false：失败
bool IRGenerator::ir_array_access(ast_node * node)
{
    // 调用地址计算函数，获取地址
    bool success = ir_array_declare(node);
    if (!success) {
        return false;
    }
    Function * currentFunc = module->getCurrentFunction();
    // 从上一步的结果中拿到地址指针
    Value * address_ptr = node->val;
    // TODO 该方法待修改，不能直接new value
    Instruction * load_inst = new MoveInstruction(currentFunc, address_ptr);
    node->blockInsts.addInst(load_inst);

    node->val = load_inst;

    return true;
}
