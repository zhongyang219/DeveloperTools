#pragma once

#include <QDialog>
#include <QRadioButton>

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

        WallpaperAcquireMethod wallpaperAcquireMethod{};

        void Save() const;
        void Load();
    };

    void SetData(const Data& data);
    Data GetData() const;

private:
    QRadioButton* m_radioButtonReg;
    QRadioButton* m_radioButtonWallPaperInterface;

};
