#include "mainwindow.h"
#include <DApplication>
#include <DWidgetUtil>

int main(int argc, char *argv[])
{
    DApplication::loadDXcbPlugin();
    DApplication a(argc, argv);
    a.setTheme("light");

    MainWindow w;
    w.resize(1000, 600);
    Dtk::Widget::moveToCenter(&w);
    w.show();

    return a.exec();
}
