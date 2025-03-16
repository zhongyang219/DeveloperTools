#include "MainFrame.h"
#include <QtWidgets/QApplication>
#include "define.h"
#include <QSharedMemory>

void SetSharedMemoryData(QSharedMemory& shared, char data)
{
    if (shared.size() >= 1)
    {
        char* strData = (char*)shared.data();
        strData[0] = data;
    }
}


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setApplicationName(APP_NAME);

    //设置只能运行一个实例
    QSharedMemory shared("{53abf396-999b-438b-b480-7a22a8773409}");
    if (shared.attach())
    {
        //将共享内存的第一个字节为1
        SetSharedMemoryData(shared, 1);
        return 0;
    }
    shared.create(1);
    SetSharedMemoryData(shared, 0);

    QStringList cmdLine;
    for (int i = 0; i < argc; i++)
        cmdLine.push_back(argv[i]);

    MainFrame w(cmdLine);
    w.SetSharedMemory(&shared);
    w.show();

    return a.exec();
}
