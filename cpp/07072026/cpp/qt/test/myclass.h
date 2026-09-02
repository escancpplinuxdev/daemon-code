#ifndef MYCLASS_H
#define MYCLASS_H

#include <QObject>

class MyClass : public QObject
{
    Q_OBJECT  // ← This macro triggers MOC processing

public:
    explicit MyClass(QObject *parent = nullptr);

signals:
    void mySignal();

public slots:
    void mySlot();
};

#endif
