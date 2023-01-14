#pragma once
#include <QApplication>
#include <QDesktopWidget>

#define DPI(x) (QApplication::desktop()->logicalDpiX() * (x) / 96)

//菜单命令
#define CMD_SCAN_FILE "AddCodeHeaderScanFile"
#define CMD_REMOVE_COMMENT_EXCUTE "RemoveCommentExcute"
#define CMD_SHOW_ADD_CODE_HEADER "ShowAddCodeHeader"
#define CMD_ADD_CODE_HEADER_EXCUTE "AddCodeHeaderExcute"
#define CMD_USER_GUID "AddCodeHeaderUserGuid"

///////////////////////////////////////////////////////////////////////////
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
