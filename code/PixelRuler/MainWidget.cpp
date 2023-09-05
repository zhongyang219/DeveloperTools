#include "MainWidget.h"
#include <QPainter>
#include "RulerDrawer.h"
#include "PixelRuler.h"
#include "define.h"

CMainWidget::CMainWidget(QWidget* parent)
    : QWidget(parent)
{
}

CMainWidget::~CMainWidget()
{}

void CMainWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    int scalePercent = PixelRuler::Instance()->GetScalePercent();
    CRulerDrawer drawer(painter, rect(), scalePercent);
    drawer.Draw(CRulerDrawer::Top);
    drawer.Draw(CRulerDrawer::Bottom);
    drawer.Draw(CRulerDrawer::Left);
    drawer.Draw(CRulerDrawer::Right);

    QRect scaleRect = rect();
    scaleRect.setLeft(scaleRect.right() - DPI(170));
    scaleRect.setTop(scaleRect.bottom() - DPI(65));
    painter.save();
    QString unitStr = QString(u8"当前缩放比例：%1%").arg(scalePercent);
    painter.drawText(scaleRect, unitStr);
    painter.restore();

}
