// myclass.cpp
#include "myclass.h"

MyClass::MyClass(QObject *parent) 
    : QObject(parent)
{
    qDebug() << "MyClass created!";
}

void MyClass::doSomething()
{
    qDebug() << "Emitting signal...";
    emit mySignal("Hello from MyClass!");
}

void MyClass::mySlot()
{
    qDebug() << "mySlot called!";
}
