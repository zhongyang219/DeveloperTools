#pragma once
#include <QString>

enum eOutputFormat      //输出的文本格式
{
    OF_UTF8,
    OF_ANSI,
    OF_UTF16
};


//文件头项目
enum eHeadItem
{
    HI_COPYRIGHT,       //版权
    HI_FILE,            //文件名
    HI_BRIEF,           //文件描述
    HI_AUTHOR,          //作者
    HI_EMAL,            //邮箱
    HI_VERSION,         //版本号
    HI_DATE,            //日期
    HI_LICENSE,         //许可证
    HI_USER_DEFINE,     //自定义的固定内容，通常包括版权等信息
    HI_MAX
};

struct HeadItemItem
{
    eHeadItem item;
    QString value;
    bool enable{ true };

    HeadItemItem()
    {}

    HeadItemItem(eHeadItem _item)
        : item(_item)
    {}

    QString name() const;

    static QString GetHeadItemName(eHeadItem item);

};
