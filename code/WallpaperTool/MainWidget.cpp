#include "MainWidget.h"
#include "define.h"

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    ui.verticalLayout->setSpacing(DPI(8));
}

MainWidget::~MainWidget()
{}

void MainWidget::SetWallpaper(const QString & strPath)
{
    ui.wallpaperPathLineEdit->setText(strPath);
    ui.imageLabel->SetImage(QPixmap(strPath));
}
