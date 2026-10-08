// main.cpp
#include <QCoreApplication>
#include "myclass.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    MyClass obj;
    // QObject::connect(&obj, &MyClass::mySignal, ...);
    
    return a.exec();
}
