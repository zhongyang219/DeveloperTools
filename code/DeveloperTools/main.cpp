#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //设置字体
    QFont font("Microsoft YaHei", 9, QFont::Normal);
    a.setFont(font);

    MainFrame w;
    w.show();

    return a.exec();
}
