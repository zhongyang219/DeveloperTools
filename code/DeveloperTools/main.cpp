#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置字体
    QFont font("Microsoft YaHei", 9, QFont::Normal);
    a.setFont(font);

    //设置样式表
    QFile qssFile(":/DeveloperTools/res/style.qss");
    qssFile.open(QIODevice::ReadOnly);
    QByteArray fileContents(qssFile.readAll());
    qssFile.close();

    MainFrame w;
    w.setStyleSheet(QString::fromUtf8(fileContents));
    w.show();
    return a.exec();
}
