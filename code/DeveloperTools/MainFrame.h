#pragma once

#include <QWidget>
#include "ribbonframewindow.h"

class MainFrame : public RibbonFrameWindow
{
    Q_OBJECT
public:
    explicit MainFrame(QWidget* parent = nullptr);

signals:

    // MainFrameWindow interface
private:
    virtual bool OnCommand(const QString& strCmd, bool checked) override;
};
