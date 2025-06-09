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
#include "Use.h"
#include "Instruction.h"

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
        // 创建一个 Use 对象。
        Use * conditionUse = new Use(condition, this);


        // 将这个 Use 对象注册到被使用的 Value (condition) 上。
        condition->addUse(conditionUse);


        Instruction::addUse(conditionUse); // 调用 Instruction 基类的 addUse


    }
}

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