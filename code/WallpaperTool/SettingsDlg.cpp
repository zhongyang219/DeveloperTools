#include "SettingsDlg.h"
#include "../CCommonTools/Config.h"
#include "WallpaperTool.h"
#include "define.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QLabel>
#include <QScrollArea>

void SettingsDlg::Data::Save() const
{
    //保存设置
    CConfig settings(QString::fromUtf8(WallpaperTool::Instance()->GetModuleName()));
    settings.WriteValue("wallpaperAcquireMethod", static_cast<int>(wallpaperAcquireMethod));
}

void SettingsDlg::Data::Load()
{
    CConfig settings(QString::fromUtf8(WallpaperTool::Instance()->GetModuleName()));
    wallpaperAcquireMethod = static_cast<WallpaperAcquireMethod>(settings.GetValue("wallpaperAcquireMethod", 1).toInt());
}

//////////////////////////////////////////////////////////////////////////////
SettingsDlg::SettingsDlg(QWidget *parent)
    : QDialog(parent)
{
    //ui.setupUi(this);
    //if (layout() != nullptr)
    //    layout()->setMargin(DPI(8));

    setWindowTitle(u8"壁纸工具设置");

    QVBoxLayout* pLayout = new QVBoxLayout();
    setLayout(pLayout);
    QScrollArea* pScrollArea = new QScrollArea();
    pLayout->addWidget(pScrollArea);
    pLayout->addWidget(new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel));

    QVBoxLayout* pScrollLayout = new QVBoxLayout();
    pScrollArea->setLayout(pScrollLayout);
    pScrollLayout->addWidget(m_radioButtonReg = new QRadioButton(u8"注册表"));
    pScrollLayout->addWidget(m_radioButtonWallPaperInterface = new QRadioButton(u8"IDesktopWallpaper接口"));
    pScrollLayout->addStretch();

    resize(DPI(340), DPI(230));

    //非Windows下禁用壁纸获取方式选项
#ifndef Q_OS_WIN
    ui.radioButtonReg->setEnabled(false);
    ui.radioButtonWallPaperInterface->setEnabled(false);
#endif
}

SettingsDlg::~SettingsDlg()
{}

void SettingsDlg::SetData(const Data & data)
{
    if (data.wallpaperAcquireMethod == Data::Registry)
        m_radioButtonReg->setChecked(true);
    else
        m_radioButtonWallPaperInterface->setChecked(true);
}

SettingsDlg::Data SettingsDlg::GetData() const
{
    Data data;
    data.wallpaperAcquireMethod = m_radioButtonReg->isChecked() ? Data::Registry : Data::WallpaperInterface;
    return data;
}
