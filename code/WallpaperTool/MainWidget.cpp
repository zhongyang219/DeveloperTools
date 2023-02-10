#include "MainWidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

MainWidget::~MainWidget()
{}

void MainWidget::SetWallpaper(const QString & strPath)
{
    ui.wallpaperPathLineEdit->setText(strPath);
    ui.imageLabel->SetImage(QPixmap(strPath));
}
