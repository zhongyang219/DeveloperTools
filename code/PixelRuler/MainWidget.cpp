#include "MainWidget.h"
#include <QPainter>
#include "RulerDrawer.h"
#include "PixelRuler.h"
#include <QVBoxLayout>
#include "define.h"
#include <QFile>

CMainWidget::CMainWidget(QWidget* parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout;
    const int margin = DPI(72);
    pLayout->setContentsMargins(margin, margin, margin, margin);
    setLayout(pLayout);
    pLayout->addWidget(&m_helpEdit);
    m_helpEdit.setReadOnly(true);

    QFile file(":/res/help.md");
    if (file.open(QIODevice::ReadOnly))
    {
        QString helpContents = QString::fromUtf8(file.readAll());
        m_helpEdit.setMarkdown(helpContents);
    }
}

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
