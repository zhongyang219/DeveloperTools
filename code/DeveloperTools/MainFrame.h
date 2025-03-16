#pragma once

#include <QWidget>
#include <QSharedMemory>
#include "ribbonframewindow.h"

class MainFrame : public RibbonFrameWindow
{
    Q_OBJECT
public:
    explicit MainFrame(const QStringList& cmdLine, QWidget* parent = nullptr);
    ~MainFrame();
    void SetSharedMemory(QSharedMemory* pShared);

signals:

private:
    int m_timerId;    //1秒定时器的ID
    QSharedMemory* m_pSharedMemory{};   //保存QSharedMemory对象的指针

    // MainFrameWindow interface
private:
    virtual bool OnCommand(const QString& strCmd, bool checked) override;
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent* event) override;
    virtual void timerEvent(QTimerEvent*) override;
};
