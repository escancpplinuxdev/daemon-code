// myclass.h
#ifndef MYCLASS_H
#define MYCLASS_H
#include <QObject>
class MyClass : public QObject {
    Q_OBJECT
public:
    MyClass();
signals:
    void mySignal();
};
#endif

// myclass.cpp
#include "myclass.h"
MyClass::MyClass() {}

// Build system compiles moc_myclass.cpp separately
