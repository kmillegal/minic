///
/// @file GlobalVariable.h
/// @brief 全局变量描述类
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
#pragma once

#include "Constant.h"
#include "GlobalValue.h"
#include "IRConstant.h"
#include "ArrayType.h"
#include "ConstInt.h"
#include <string>

///
/// @brief 全局变量，寻址时通过符号名或变量名来寻址
///
class GlobalVariable : public GlobalValue {

public:
    ///
    /// @brief 构建全局变量，默认对齐为4字节
    /// @param _type 类型
    /// @param _name 名字
    ///
    explicit GlobalVariable(Type * _type, std::string _name, ConstInt * _initializer = nullptr)
        : GlobalValue(_type, _name), initializer(_initializer)
    {
        // 设置对齐大小
        setAlignment(4);
    }

    ///
    /// @brief  检查是否是函数
    /// @return true 是函数
    /// @return false 不是函数
    ///
    [[nodiscard]] bool isGlobalVarible() const override
    {
        return true;
    }

    ///
    /// @brief 是否属于BSS段的变量，即未初始化过的变量，或者初值都为0的变量
    /// @return true
    /// @return false
    ///
    [[nodiscard]] bool isInBSSSection() const
    {
        return this->inBSSSection;
    }

    ///
    /// @brief 取得变量所在的作用域层级
    /// @return int32_t 层级
    ///
    int32_t getScopeLevel() override
    {
        return 0;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    int32_t getLoadRegId() override
    {
        return this->loadRegNo;
    }

    ///
    /// @brief 对该Value进行Load用的寄存器编号
    /// @return int32_t 寄存器编号
    ///
    void setLoadRegId(int32_t regId) override
    {
        this->loadRegNo = regId;
    }

    ///
    /// @brief 是否初始化
    /// @return true
    /// @return false
    ///
    [[nodiscard]] bool hasInitializer() const
    {
        return initializer != nullptr;
    }

    ///
    /// @brief 获取初始值
    /// @return 初始值
    ///
    [[nodiscard]] ConstInt * getInitializer() const
    {
        return initializer;
    }

    ///
    /// @brief 对该全局变量初始化
    /// @param ConstInt * val
    ///
    void setInitializer(ConstInt * val)
	{
		this->initializer = val;
        // 设置不在BSS段
        if (initializer->getVal() != 0)
            this->inBSSSection = false;
	}

    ///
    /// @brief Declare指令IR显示
    /// @param str
    ///
    void toDeclareString(std::string & str)
    {
        Type * varType = getType();
        str = "declare ";
        if (varType->isArrayType()) {
            // 处理数组类型
            Type * currentType = varType;
            std::vector<uint64_t> dims;
            // 循环拆解类型，获取所有维度和最终的基础类型
            while (currentType->isArrayType()) {
                ArrayType * arrayTy = static_cast<ArrayType *>(currentType);
                dims.push_back(arrayTy->getNumElements());
                currentType = arrayTy->getElementType();
            }

            str += currentType->toString();
            str += " " + getIRName();

            for (uint64_t dim: dims) {
                str += "[" + std::to_string(dim) + "]";
            }
        } else {
            // 非数组类型
            str += varType->toString() + " " + getIRName();
            if (hasInitializer()) {
                str += " = " + std::to_string(initializer->getVal());
            }
        }
	}

private:
    ///
    /// @brief 变量加载到寄存器中时对应的寄存器编号
    ///
    int32_t loadRegNo = -1;

    ///
    /// @brief 默认全局变量在BSS段，没有初始化，或者即使初始化过，但都值都为0
    ///
    bool inBSSSection = true;

    ///
    /// @brief 初始化值
    ///
    ConstInt * initializer = nullptr;
};
