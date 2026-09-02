#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QString>

class Calculator : public QWidget
{
    Q_OBJECT

public:
    explicit Calculator(QWidget *parent = nullptr);

private slots:
    void digitClicked();
    void operatorClicked();
    void equalsClicked();
    void clearClicked();
    void backspaceClicked();
    void percentClicked();
    void signClicked();
    void decimalClicked();

private:
    // UI Components
    QLineEdit *display;
    QLineEdit *expressionDisplay;
    
    // Buttons
    QPushButton *buttons[20];
    
    // Calculator state
    double firstNumber;
    double secondNumber;
    double result;
    QString currentOperator;
    bool isNewNumber;
    bool isResultDisplayed;
    
    // Helper methods
    void createUI();
    void connectSignals();
    QString formatNumber(double num);
    double calculate(double num1, double num2, const QString &op);
    void updateDisplay();
    void clearAll();
};

#endif // CALCULATOR_H
