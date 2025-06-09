///
/// @file ArrayType.cpp
/// @brief 数组类型类
///
/// @author HuangKm
/// @version 1.0
/// @date 2025-6-6
///
/// @copyright Copyright (c) 2025
///
/// @par 修改日志:
/// <table>
/// <tr><th>Date       <th>Version <th>Author  <th>Description
/// <tr><td>2025-6-6 <td>1.0     <td>HuangKm  <td>新建
/// </table>
///

#include <string>

#include "ArrayType.h"


// 构造函数的实现
ArrayType::ArrayType(Type * elemTy, uint64_t numElems)
    : Type(Type::ArrayTyID),
      elementType(elemTy),
      numElements(numElems)
{}


// toString() 方法的实现
std::string ArrayType::toString() const
{
    // 递归调用 elementType 的 toString() 方法
    return "[" + std::to_string(numElements) + " x " + elementType->toString() + "]";
}

// getSize() 方法的实现
int32_t ArrayType::getSize() const
{
    // 数组总大小 = 元素数量 * 单个元素的大小
    if (elementType->getSize() <= 0) {
        return -1;
    }
    return numElements * elementType->getSize();
}