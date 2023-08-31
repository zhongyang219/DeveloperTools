#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(ADDCODEHEADER_LIB)
#  define ADDCODEHEADER_EXPORT Q_DECL_EXPORT
# else
#  define ADDCODEHEADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define ADDCODEHEADER_EXPORT
#endif


//菜单命令
#define CMD_SCAN_FILE "AddCodeHeaderScanFile"
#define CMD_REMOVE_COMMENT_EXCUTE "RemoveCommentExcute"
#define CMD_SHOW_ADD_CODE_HEADER "ShowAddCodeHeader"
#define CMD_ADD_CODE_HEADER_EXCUTE "AddCodeHeaderExcute"
#define CMD_USER_GUID "AddCodeHeaderUserGuid"

#define CMD_RemoveCommentCheck "RemoveCommentCheck"
#define CMD_RemoveSpaceCheck "RemoveSpaceCheck"
#define CMD_RemoveEmptyLineCheck "RemoveEmptyLineCheck"
#define CMD_KeepEmptyLineNum "KeepEmptyLineNum"
