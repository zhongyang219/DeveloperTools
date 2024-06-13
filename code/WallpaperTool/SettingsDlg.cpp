#include "SettingsDlg.h"
#include "../CCommonTools/Config.h"
#include "WallpaperTool.h"
#include "define.h"

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
    ui.setupUi(this);
    if (layout() != nullptr)
        layout()->setMargin(DPI(8));

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
        ui.radioButtonReg->setChecked(true);
    else
        ui.radioButtonWallPaperInterface->setChecked(true);
}

SettingsDlg::Data SettingsDlg::GetData() const
{
    Data data;
    data.wallpaperAcquireMethod = ui.radioButtonReg->isChecked() ? Data::Registry : Data::WallpaperInterface;
    return data;
}
