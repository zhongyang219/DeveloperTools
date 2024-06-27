#pragma once

#include <QWidget>
#include "ribbonframewindow.h"

class MainFrame : public RibbonFrameWindow
{
    Q_OBJECT
public:
    explicit MainFrame(const QStringList& cmdLine, QWidget* parent = nullptr);
    ~MainFrame();

signals:

    // MainFrameWindow interface
private:
    virtual bool OnCommand(const QString& strCmd, bool checked) override;
    // QWidget interface
protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
