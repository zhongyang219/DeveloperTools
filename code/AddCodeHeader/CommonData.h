#pragma once
#include <QString>

enum eOutputFormat      //������ı���ʽ
{
    OF_UTF8,
    OF_ANSI,
    OF_UTF16
};


//�ļ�ͷ��Ŀ
enum eHeadItem
{
    HI_COPYRIGHT,       //��Ȩ
    HI_FILE,            //�ļ���
    HI_BRIEF,           //�ļ�����
    HI_AUTHOR,          //����
    HI_EMAL,            //����
    HI_VERSION,         //�汾��
    HI_DATE,            //����
    HI_LICENSE,         //���֤
    HI_USER_DEFINE,     //�Զ���Ĺ̶����ݣ�ͨ��������Ȩ����Ϣ
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
