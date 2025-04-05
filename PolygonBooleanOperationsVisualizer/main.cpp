#include "include/BooleanOperations.h"
#include <iostream>
#include <qapplication>  // Use the full namespace
#include "src/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}