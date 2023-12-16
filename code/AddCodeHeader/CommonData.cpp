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
        return u8"��Ȩ";
    case HI_FILE:
        return u8"�ļ���";
    case HI_BRIEF:
        return u8"�ļ�����";
    case HI_AUTHOR:
        return u8"����";
    case HI_EMAL:
        return u8"����";
    case HI_VERSION:
        return u8"�汾��";
    case HI_DATE:
        return u8"����";
    case HI_LICENSE:
        return u8"���֤";
    case HI_USER_DEFINE:
        return u8"�Զ���";
    }
    return QString();
}
