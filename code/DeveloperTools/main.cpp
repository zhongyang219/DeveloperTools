#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "define.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);

    MainFrame w;
    w.show();

    return a.exec();
}
