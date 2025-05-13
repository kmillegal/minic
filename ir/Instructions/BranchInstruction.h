///
/// @file BranchInstruction.h
/// @brief 条件分支指令，根据条件跳转到不同的目标
///
/// @author HuangKm
/// @version 1.0
/// @date 2025-5-13
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-5-13 <td>1.0     <td>HuangKm  <td>新建
/// </table>
///
#pragma once

#include <string>

#include "Instruction.h"
#include "LabelInstruction.h"
#include "Value.h" // 条件是一个 Value
#include "Function.h"

///
/// @brief 条件分支指令
/// @details 根据提供的条件值，跳转到 true_target 或 false_target。
///
class BranchInstruction final : public Instruction {

public:
    ///
    /// @brief 条件分支指令的构造函数
    /// @param _func 当前指令所属的函数
    /// @param _condition 用于判断条件的 Value*
    /// @param _true_target 条件为真时跳转的目标 LabelInstruction*
    /// @param _false_target 条件为假时跳转的目标 LabelInstruction*
    ///
    BranchInstruction(Function * _func,
                      Value * _condition,
                      LabelInstruction * _true_target,
                      LabelInstruction * _false_target);

    /// @brief 转换成字符串形式，用于IR打印
    /// @param str 输出字符串
    void toString(std::string & str) override;

    ///
    /// @brief 获取条件 Value
    /// @return Value* 条件
    ///
    [[nodiscard]] Value * getCondition() const;

    ///
    /// @brief 获取条件为真时的跳转目标 Label 指令
    /// @return LabelInstruction* 真目标标签
    ///
    [[nodiscard]] LabelInstruction * getTrueTarget() const;

    ///
    /// @brief 获取条件为假时的跳转目标 Label 指令
    /// @return LabelInstruction* 假目标标签
    ///
    [[nodiscard]] LabelInstruction * getFalseTarget() const;

private:
    ///
    /// @brief 用于判断条件的 Value
    ///
    Value * condition;

    ///
    /// @brief 条件为真时跳转到的目标 Label 指令
    ///
    LabelInstruction * true_target;

    ///
    /// @brief 条件为假时跳转到的目标 Label 指令
    ///
    LabelInstruction * false_target;
};