#include "passwordgeneratorwidget.h"
#include "ui_passwordgeneratorwidget.h"
#include "define.h"
#include "../CCommonTools/Config.h"
#include "PasswordGenerator.h"
#include <QUuid>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QClipboard>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

PasswordGeneratorWidget::PasswordGeneratorWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PasswordGeneratorWidget)
{
    ui->setupUi(this);
    ui->widget->setMaximumWidth(DPI(500));

    connect(ui->generateButton, &QPushButton::clicked, this, &PasswordGeneratorWidget::onGenerateClicked);
    connect(ui->copyButton, &QPushButton::clicked, this, &PasswordGeneratorWidget::onCopyClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &PasswordGeneratorWidget::onSaveClicked);
    connect(ui->restoreDefault, &QPushButton::clicked, this, &PasswordGeneratorWidget::onRestoreDefaultClicked);
    connect(ui->generateGUID, &QCheckBox::toggled, this, &PasswordGeneratorWidget::onGenerateGUIDToggled);
    connect(ui->includeSpecChar, &QCheckBox::toggled, this, &PasswordGeneratorWidget::onIncludeSpecCharToggled);
}           

PasswordGeneratorWidget::~PasswordGeneratorWidget()
{
    delete ui;
}

void PasswordGeneratorWidget::LoadConfig()
{
    CConfig settings(QString::fromUtf8(PasswordGenerator::Instance()->GetModuleName()));
    ui->includeNums->setChecked(settings.GetValue("IncludeNum", true).toBool());
    ui->includeCapital->setChecked(settings.GetValue("IncludeCapital", true).toBool());
    ui->includeLowercase->setChecked(settings.GetValue("IncludeLowercase", true).toBool());
    ui->includeSpecChar->setChecked(settings.GetValue("IncludeSpecCharator", true).toBool());
    ui->passwordLengthBox->setValue(settings.GetValue("PasswordLength", "12").toInt());
    ui->specCharacters->setText(settings.GetValue("SpecCharacters", "~!@#$%^&*()_-+={}[]|\\<>/?").toString());
    ui->charTypeProbEqual->setChecked(settings.GetValue("CharTypeProbEqual", true).toBool());
    ui->charProbEqual->setChecked(!ui->charTypeProbEqual->isChecked());
    ui->generateGUID->setChecked(settings.GetValue("GenerateGUID", false).toBool());
}

void PasswordGeneratorWidget::SaveConfig() const
{
    CConfig settings(QString::fromUtf8(PasswordGenerator::Instance()->GetModuleName()));
    settings.WriteValue("IncludeNum", ui->includeNums->isChecked());
    settings.WriteValue("IncludeCapital", ui->includeCapital->isChecked());
    settings.WriteValue("IncludeLowercase", ui->includeLowercase->isChecked());
    settings.WriteValue("IncludeSpecCharator", ui->includeSpecChar->isChecked());
    settings.WriteValue("PasswordLength", ui->passwordLengthBox->text());
    settings.WriteValue("SpecCharacters", ui->specCharacters->text());
    settings.WriteValue("CharTypeProbEqual", ui->charTypeProbEqual->isChecked());
    settings.WriteValue("GenerateGUID", ui->generateGUID->isChecked());
}

void PasswordGeneratorWidget::SetPasswordLength(int length)
{
    ui->passwordLengthBox->setValue(length);
}

QString PasswordGeneratorWidget::GetGeneratedPassword() const
{
    return ui->passwordBox->text();
}

void PasswordGeneratorWidget::updateControlStates()
{
    bool guidChecked = ui->generateGUID->isChecked();
    bool specChecked = ui->includeSpecChar->isChecked();

    ui->includeNums->setEnabled(!guidChecked);
    ui->includeCapital->setEnabled(!guidChecked);
    ui->includeLowercase->setEnabled(!guidChecked);
    ui->includeSpecChar->setEnabled(!guidChecked);
    ui->label->setEnabled(!guidChecked);
    ui->passwordLengthBox->setEnabled(!guidChecked);
    ui->charTypeProbEqual->setEnabled(!guidChecked);
    ui->charProbEqual->setEnabled(!guidChecked);

    ui->specCharacters->setEnabled(specChecked && !guidChecked);
    ui->restoreDefault->setEnabled(specChecked && !guidChecked);
}

void PasswordGeneratorWidget::onGenerateClicked()
{
    if (ui->generateGUID->isChecked())
    {
        // 生成 GUID (不带大括号)
        ui->passwordBox->setText(QUuid::createUuid().toString(QUuid::WithoutBraces));
        return;
    }

    ui->passwordBox->clear();

    if (ui->passwordLengthBox->text().isEmpty())
    {
        QMessageBox::warning(this, u8"警告", u8"请输入要生成的密码长度！");
        return;
    }

    int passwordLength = ui->passwordLengthBox->text().toInt();
    if (passwordLength == 0)
    {
        QMessageBox::warning(this, u8"警告", u8"密码长度不能为0！");
        return;
    }

    // 收集选中的字符集
    QList<QString> charSets;
    if (ui->includeNums->isChecked()) charSets << "0123456789";
    if (ui->includeCapital->isChecked()) charSets << "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    if (ui->includeLowercase->isChecked()) charSets << "abcdefghijklmnopqrstuvwxyz";
    if (ui->includeSpecChar->isChecked() && !ui->specCharacters->text().isEmpty())
        charSets << ui->specCharacters->text();

    if (charSets.isEmpty())
    {
        QMessageBox::warning(this, u8"警告", u8"请选择一种要包含的字符类型！");
        return;
    }

    QString result;
    if (ui->charTypeProbEqual->isChecked())
    {
        // 每一种字符出现的概率均等
        for (int i = 0; i < passwordLength; ++i)
        {
            int typeIndex = QRandomGenerator::global()->bounded(charSets.size());
            const QString& currentSet = charSets[typeIndex];
            int charIndex = QRandomGenerator::global()->bounded(currentSet.size());
            result += currentSet[charIndex];
        }
    }
    else
    {
        // 每个字符出现的概率均等
        QString allChars = "";
        for (const QString& set : charSets)
        {
            allChars += set;
        }
        for (int i = 0; i < passwordLength; ++i)
        {
            int charIndex = QRandomGenerator::global()->bounded(allChars.size());
            result += allChars[charIndex];
        }
    }

    ui->passwordBox->setText(result);
}

void PasswordGeneratorWidget::onCopyClicked()
{
    if (!ui->passwordBox->text().isEmpty())
    {
        QApplication::clipboard()->setText(ui->passwordBox->text());
        PasswordGenerator::Instance()->GetMainFrame()->SetStatusBarText(u8"密码已复制到剪贴板。", 10000);
    }
}

void PasswordGeneratorWidget::onSaveClicked()
{
    QString currentPwd = ui->passwordBox->text();
    if (!currentPwd.isEmpty() && currentPwd != m_lastPassword)
    {
        QFile file("password.log");
        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream out(&file);
            // 写入格式：时间：密码
            out << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss")
                << u8"：" << currentPwd << "\n";
            file.close();
            m_lastPassword = currentPwd;
            PasswordGenerator::Instance()->GetMainFrame()->SetStatusBarText(u8"密码已保存到password.log中。", 10000);
        }
        else
        {
            QMessageBox::warning(this, u8"错误", u8"无法打开文件进行写入！");
        }
    }
}

void PasswordGeneratorWidget::onRestoreDefaultClicked()
{
    ui->specCharacters->setText("~!@#$%^&*()_-+={}[]|\\<>/?");
}

void PasswordGeneratorWidget::onGenerateGUIDToggled(bool checked)
{
    updateControlStates();
}

void PasswordGeneratorWidget::onIncludeSpecCharToggled(bool checked)
{
    updateControlStates();
}
