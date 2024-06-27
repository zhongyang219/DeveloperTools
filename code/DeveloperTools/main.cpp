#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "define.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);

    QStringList cmdLine;
    for (int i = 0; i < argc; i++)
        cmdLine.push_back(argv[i]);

    MainFrame w(cmdLine);
    w.show();

    return a.exec();
}
