#ifndef PASSWORDGENERATORWIDGET_H
#define PASSWORDGENERATORWIDGET_H

#include <QWidget>

namespace Ui {
class PasswordGeneratorWidget;
}

class PasswordGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PasswordGeneratorWidget(QWidget *parent = nullptr);
    ~PasswordGeneratorWidget();

    void LoadConfig();
    void SaveConfig() const;

    void SetPasswordLength(int length);
    QString GetGeneratedPassword() const;

public slots:
    void onGenerateClicked();
    void onCopyClicked();
    void onSaveClicked();
    void onRestoreDefaultClicked();
    void onGenerateGUIDToggled(bool checked);
    void onIncludeSpecCharToggled(bool checked);

private:
    void updateControlStates();

private:
    Ui::PasswordGeneratorWidget *ui;
    QString m_lastPassword;
};

#endif // PASSWORDGENERATORWIDGET_H
