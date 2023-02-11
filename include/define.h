#pragma once
#include <QApplication>
#include <QDesktopWidget>

#define DPI(x) (QApplication::desktop()->logicalDpiX() * (x) / 96)

#define APP_NAME "DeveloperTools"
#define APP_VERSION "0.1.0"

///////////////////////////////////////////////////////////////////////////
//���尲ȫɾ��ָ��ĺ�
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

// �򵥽ӿڶ����
#if !defined(DECLARE_CLASS_ATTR)

//�������Get��Set�ļ򵥽ӿ�
//func_name: �����������ˡ�Get����Set������Ĳ��֣�
//mem_name: ��Ա������
//type: ��������
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
//�������Set�ļ򵥽ӿ�
#define DECLARE_CLASS_SET_ATTR(func_name,mem_name,type)		\
    void Set##func_name(const type& input)	\
    {                                       \
        mem_name = input;					\
    }

#endif

#if !defined(DECLARE_CLASS_GET_ATTR)
//�������Get�ļ򵥽ӿ�
#define DECLARE_CLASS_GET_ATTR(func_name,mem_name,type)		\
    type Get##func_name()const					\
    {										\
        return mem_name;					\
    }
#endif
