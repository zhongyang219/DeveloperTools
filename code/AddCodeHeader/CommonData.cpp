#include "CommonData.h"

QString HeadItemItem::name() const
{
    return GetHeadItemName(item);
}

QString HeadItemItem::GetHeadItemName(eHeadItem item)
{
    switch (item)
    {
    case HI_COPYRIGHT:
        return u8"版权";
    case HI_FILE:
        return u8"文件名";
    case HI_BRIEF:
        return u8"文件描述";
    case HI_AUTHOR:
        return u8"作者";
    case HI_EMAL:
        return u8"邮箱";
    case HI_VERSION:
        return u8"版本号";
    case HI_DATE:
        return u8"日期";
    case HI_LICENSE:
        return u8"许可证";
    case HI_USER_DEFINE:
        return u8"自定义";
    }
    return QString();
}
