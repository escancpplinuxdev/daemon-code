#include <QApplication>
#include "calculator.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    Calculator calc;
    calc.setWindowTitle("Qt Calculator");
    calc.setFixedSize(280, 380);
    calc.show();
    
    return app.exec();
}
