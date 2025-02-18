#include "MainWidget.h"
#include "define.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent)
{
    QWidget* pWidget = new QWidget(this);
    setCentralWidget(pWidget);

    QVBoxLayout* pLayout = new QVBoxLayout();
    pWidget->setLayout(pLayout);
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pLayout->addLayout(pHLayout);
    pHLayout->addWidget(new QLabel(u8"当前壁纸路径："));
    pHLayout->addWidget(&m_pathEdit);

    pLayout->addWidget(&m_imgLabel);
}

MainWidget::~MainWidget()
{}

void MainWidget::SetWallpaper(const QString & strPath)
{
    m_pathEdit.setText(strPath);
    m_imgLabel.SetImage(QPixmap(strPath));
}
