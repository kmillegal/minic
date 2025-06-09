///
/// @file MoveInstruction.cpp
/// @brief Move指令，也就是DragonIR的Asssign指令
///
/// @author zenglj (zenglj@live.com)
/// @version 1.0
/// @date 2024-09-29
///
/// @copyright Copyright (c) 2024
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2024-09-29 <td>1.0     <td>zenglj  <td>新建
/// </table>
///

#include "Instruction.h"
#include "VoidType.h"
#include "PointerType.h"
#include "MoveInstruction.h"

///
/// @brief 构造函数
/// @param _func 所属的函数
/// @param result 结构操作数
/// @param srcVal1 源操作数
///
MoveInstruction::MoveInstruction(Function * _func, Value * _result, Value * _srcVal1)
    : Instruction(_func, IRInstOperator::IRINST_OP_ASSIGN, VoidType::getType())
{
    addOperand(_result);
    addOperand(_srcVal1);
    if (_result->getType()->isPointerType()) {
        this->op = IRInstOperator::IRINST_OP_STORE;

    } else if (_srcVal1->getType()->isPointerType()) {
        this->op = IRInstOperator::IRINST_OP_LOAD;
        Type * pointer_type = _srcVal1->getType();
        PointerType * src_ptr_type = static_cast<PointerType *>(pointer_type);
        this->type = const_cast<Type *>(src_ptr_type->getPointeeType());
    }
}

/// @brief 转换成字符串显示
/// @param str 转换后的字符串
void MoveInstruction::toString(std::string & str)
{
    Value *resVal = getOperand(0), *srcVal = getOperand(1);
    switch (op) {
        case IRInstOperator::IRINST_OP_STORE:
            // STORE 操作
            str = "*" + resVal->getIRName() + " = " + srcVal->getIRName();
            break;
        case IRInstOperator::IRINST_OP_LOAD:
            // LOAD 操作
            str = resVal->getIRName() + " = *" + srcVal->getIRName();
            break;
        case IRInstOperator::IRINST_OP_ASSIGN:
            // 赋值操作
            str = resVal->getIRName() + " = " + srcVal->getIRName();
            break;
        default:
            // 未知指令
            Instruction::toString(str);
            break;
    }
}

