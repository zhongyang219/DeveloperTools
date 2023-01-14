#pragma once
#include <QApplication>
#include <QDesktopWidget>

#define DPI(x) (QApplication::desktop()->logicalDpiX() * (x) / 96)

//�˵�����
#define CMD_SCAN_FILE "AddCodeHeaderScanFile"
#define CMD_REMOVE_COMMENT_EXCUTE "RemoveCommentExcute"
#define CMD_SHOW_ADD_CODE_HEADER "ShowAddCodeHeader"
#define CMD_ADD_CODE_HEADER_EXCUTE "AddCodeHeaderExcute"
#define CMD_USER_GUID "AddCodeHeaderUserGuid"

///////////////////////////////////////////////////////////////////////////
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
