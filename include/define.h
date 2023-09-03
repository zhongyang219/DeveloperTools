#pragma once
#include <QGuiApplication>
#include <QScreen>

inline int DPI(int x)
{
    return QGuiApplication::primaryScreen()->logicalDotsPerInch() * x / 96;
}

#define APP_NAME "DeveloperTools"
#define APP_VERSION "0.2.0"

///////////////////////////////////////////////////////////////////////////
//定义安全删除指针的宏
#ifndef SAFE_DELETE
#define SAFE_DELETE(p) do \
{\
    if(p != nullptr) \
    { \
        delete p; \
        p = nullptr; \
    } \
} while (false)
#endif

// 简单接口定义宏
#if !defined(DECLARE_CLASS_ATTR)

//定义类的Get和Set的简单接口
//func_name: 函数名（除了“Get”或“Set”以外的部分）
//mem_name: 成员变量名
//type: 变量类型
#define DECLARE_CLASS_ATTR(func_name,mem_name,type)		\
    type Get##func_name()const					\
    {										\
        return mem_name;					\
    }										\
    void Set##func_name(const type& input)	\
    {                                       \
        mem_name = input;					\
    }

#endif

#if !defined(DECLARE_CLASS_SET_ATTR)
//定义类的Set的简单接口
#define DECLARE_CLASS_SET_ATTR(func_name,mem_name,type)		\
    void Set##func_name(const type& input)	\
    {                                       \
        mem_name = input;					\
    }

#endif

#if !defined(DECLARE_CLASS_GET_ATTR)
//定义类的Get的简单接口
#define DECLARE_CLASS_GET_ATTR(func_name,mem_name,type)		\
    type Get##func_name()const					\
    {										\
        return mem_name;					\
    }
#endif
