#include "WallpapaerWidget.h"
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>

CWallpapaerWidget::CWallpapaerWidget(QWidget *parent)
    : QWidget(parent)
{
    QVBoxLayout* pLayout = new QVBoxLayout();
    pLayout->setMargin(0);
    this->setLayout(pLayout);
    QHBoxLayout* pHLayout = new QHBoxLayout();
    pLayout->addLayout(pHLayout);
    //pHLayout->addWidget(new QLabel(u8"当前壁纸路径："));
    pHLayout->addWidget(&m_pathEdit);
    m_pathEdit.setReadOnly(true);
    pLayout->addWidget(&m_imgLabel);
}

CWallpapaerWidget::~CWallpapaerWidget()
{}

void CWallpapaerWidget::SetWallpaper(const QString & strPath)
{
    m_pathEdit.setText(strPath);
    m_imgLabel.SetImage(QPixmap(strPath));
}

QString CWallpapaerWidget::GetWallpaperPath()
{
    return m_pathEdit.text();
}
