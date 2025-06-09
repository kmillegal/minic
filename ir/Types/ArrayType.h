///
/// @file ArrayType.h
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
#pragma once
#include <cstdint>

#include "Type.h"
class ArrayType final : public Type {

public:
    /// @brief 创建一个数组类型实例
    /// @param elementType 数组元素的类型
    /// @param numElements 数组元素的数量
    ArrayType (Type * elementType, uint64_t numElements);

    /// @brief 获取数组的元素类型
    /// @return Type*
    [[nodiscard]] Type * getElementType() const
    {
        return elementType;
    }

    /// @brief 获取数组的元素数量
    /// @return uint64_t
    [[nodiscard]] uint64_t getNumElements() const
    {
        return numElements;
    }

    /// @brief 获取类型的IR字符串表示, 例如 "[10 x i32]"
    /// @return std::string
    [[nodiscard]] std::string toString() const override;

    /// @brief 获得类型所占内存空间大小（字节数）
    /// @return int32_t
    [[nodiscard]] int32_t getSize() const override;

private:

    /// @brief 数组的元素类型
    Type * elementType;

    /// @brief 数组的元素数量
    uint64_t numElements;

};