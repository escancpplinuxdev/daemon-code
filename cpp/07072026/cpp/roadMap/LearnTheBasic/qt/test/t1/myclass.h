// myclass.h
#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>
#include <QDebug>

class MyClass : public QObject
{
    Q_OBJECT  // ← This requires MOC

public:
    explicit MyClass(QObject *parent = nullptr);
    void doSomething();

signals:
    void mySignal(const QString &message);

public slots:
    void mySlot();
};

#endif // MYCLASS_H
