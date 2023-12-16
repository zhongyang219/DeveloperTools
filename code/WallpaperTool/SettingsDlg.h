#pragma once

#include <QDialog>
#include "ui_SettingsDlg.h"

class SettingsDlg : public QDialog
{
    Q_OBJECT

public:
    SettingsDlg(QWidget *parent = nullptr);
    ~SettingsDlg();

    struct Data
    {
        enum WallpaperAcquireMethod
        {
            Registry,
            WallpaperInterface
        };

        WallpaperAcquireMethod wallpaperAcquireMethod;

        void Save() const;
        void Load();
    };

    void SetData(const Data& data);
    Data GetData() const;

private:
    Ui::SettingsDlgClass ui;
};
