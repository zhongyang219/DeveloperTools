#include "MainWidget.h"
#include <QPainter>
#include "RulerDrawer.h"
#include "PixelRuler.h"

CMainWidget::CMainWidget(QWidget* parent)
    : QWidget(parent)
{}

CMainWidget::~CMainWidget()
{}

void CMainWidget::paintEvent(QPaintEvent * event)
{
    QPainter painter(this);
    CRulerDrawer drawer(painter, rect(), PixelRuler::Instance()->GetScalePercent());
    drawer.Draw(CRulerDrawer::Top);
    drawer.Draw(CRulerDrawer::Bottom);
    drawer.Draw(CRulerDrawer::Left);
    drawer.Draw(CRulerDrawer::Right);
}
