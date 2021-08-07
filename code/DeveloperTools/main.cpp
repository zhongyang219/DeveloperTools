#include "MainFrame.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QFont font("Microsoft YaHei", 9, QFont::Normal);
    a.setFont(font);

    MainFrame w;
    w.show();
    return a.exec();
}
