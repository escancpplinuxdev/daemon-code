#include "calculator.h"
#include <QString>
#include <QFont>
#include <QFrame>
#include <QDebug>
#include <cmath>  // ✅ Add this for isnan, isinf
#include <cfloat>

using namespace std;

Calculator::Calculator(QWidget *parent)
    : QWidget(parent)
    , firstNumber(0)
    , secondNumber(0)
    , result(0)
    , currentOperator("")
    , isNewNumber(true)
    , isResultDisplayed(false)
{
    createUI();
    connectSignals();
	    display->setText("|");    // ✅ ADD THIS LINE
}

void Calculator::createUI()
{
    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setSpacing(10);
    mainLayout->setContentsMargins(10, 10, 10, 10);
    
    // Display for expression (smaller, above main display)
    expressionDisplay = new QLineEdit(this);
    expressionDisplay->setReadOnly(true);
    expressionDisplay->setAlignment(Qt::AlignRight);
    expressionDisplay->setStyleSheet(
        "QLineEdit {"
        "    background-color: #f0f0f0;"
        "    color: #666;"
        "    font-size: 14px;"
        "    border: 1px solid #ddd;"
        "    border-radius: 5px;"
        "    padding: 5px;"
        "}"
    );
    expressionDisplay->setFixedHeight(30);
    mainLayout->addWidget(expressionDisplay);
    
    // Main display
    display = new QLineEdit(this);
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setText("0");
    display->setStyleSheet(
        "QLineEdit {"
        "    background-color: #f8f9fa;"
        "    color: #000;"
        "    font-size: 32px;"
        "    font-weight: bold;"
        "    border: 2px solid #dee2e6;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
    );
    display->setFixedHeight(60);
    mainLayout->addWidget(display);
    
    // Grid layout for buttons
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(5);
    
    // Button labels
    QString buttonLabels[5][4] = {
        {"C", "⌫", "%", "÷"},
        {"7", "8", "9", "×"},
        {"4", "5", "6", "-"},
        {"1", "2", "3", "+"},
        {"±", "0", ".", "="}
    };
    
    // Color schemes
    QString numberStyle = 
        "QPushButton {"
        "    background-color: #e9ecef;"
        "    color: #212529;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "    border: 1px solid #ced4da;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #dee2e6;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #ced4da;"
        "}";
    
    QString operatorStyle = 
        "QPushButton {"
        "    background-color: #17a2b8;"
        "    color: white;"
        "    font-size: 20px;"
        "    font-weight: bold;"
        "    border: 1px solid #117a8b;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #138496;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #0f6674;"
        "}";
    
    QString functionStyle = 
        "QPushButton {"
        "    background-color: #fd7e14;"
        "    color: white;"
        "    font-size: 18px;"
        "    font-weight: bold;"
        "    border: 1px solid #e06b0a;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #e06b0a;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #c75e08;"
        "}";
    
    QString equalsStyle = 
        "QPushButton {"
        "    background-color: #28a745;"
        "    color: white;"
        "    font-size: 24px;"
        "    font-weight: bold;"
        "    border: 1px solid #1e7e34;"
        "    border-radius: 8px;"
        "    padding: 10px;"
        "}"
        "QPushButton:hover {"
        "    background-color: #218838;"
        "}"
        "QPushButton:pressed {"
        "    background-color: #1e7e34;"
        "}";
    
    // Create buttons
    int buttonIndex = 0;
    for (int row = 0; row < 5; row++) {
        for (int col = 0; col < 4; col++) {
            QString label = buttonLabels[row][col];
            QPushButton *button = new QPushButton(label, this);
            
            // Set appropriate style
            if (label == "C" || label == "⌫" || label == "%" || label == "±") {
                button->setStyleSheet(functionStyle);
            } else if (label == "=") {
                button->setStyleSheet(equalsStyle);
            } else if (label == "÷" || label == "×" || label == "-" || label == "+") {
                button->setStyleSheet(operatorStyle);
            } else {
                button->setStyleSheet(numberStyle);
            }
            
            button->setFixedHeight(55);
            
            // Store button in array
            buttons[buttonIndex++] = button;
            
            // Add to grid
            gridLayout->addWidget(button, row, col);
        }
    }
    
    mainLayout->addLayout(gridLayout);
    
    // Set the window background
    this->setStyleSheet(
        "QWidget {"
        "    background-color: #ffffff;"
        "}"
    );
}

void Calculator::connectSignals()
{
    // Connect all digit buttons (0-9)
    for (int i = 0; i < 10; i++) 
    {
        // Find button with digit i
        for (int j = 0; j < 20; j++) 
	{
            if (buttons[j]->text() == QString::number(i)) 
	    {
                connect(buttons[j], &QPushButton::clicked, this, &Calculator::digitClicked);
                break;
            }
        }
    }
    
    // Connect operator buttons
    connect(buttons[11], &QPushButton::clicked, this, &Calculator::operatorClicked); // +
    connect(buttons[7], &QPushButton::clicked, this, &Calculator::operatorClicked);  // -
    connect(buttons[3], &QPushButton::clicked, this, &Calculator::operatorClicked);  // ×
    connect(buttons[15], &QPushButton::clicked, this, &Calculator::operatorClicked); // ÷
    
    // Connect function buttons
    connect(buttons[0], &QPushButton::clicked, this, &Calculator::clearClicked);     // C
    connect(buttons[1], &QPushButton::clicked, this, &Calculator::backspaceClicked); // ⌫
    connect(buttons[2], &QPushButton::clicked, this, &Calculator::percentClicked);   // %
    connect(buttons[16], &QPushButton::clicked, this, &Calculator::signClicked);     // ±
    connect(buttons[18], &QPushButton::clicked, this, &Calculator::decimalClicked);  // .
    connect(buttons[19], &QPushButton::clicked, this, &Calculator::equalsClicked);   // =
}

void Calculator::digitClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString digit = button->text();
    
    if (isResultDisplayed) {
        clearAll();
        isResultDisplayed = false;
    }
    
    if (isNewNumber) {
        display->clear();
        isNewNumber = false;
    }
    
    QString currentText = display->text();
    
    // Limit display to 15 characters
    if (currentText.length() >= 15) return;
    
    // Prevent multiple leading zeros
    if (currentText == "0" && digit == "0") return;
    if (currentText == "0" && digit != ".") {
        display->setText(digit);
    } else {
        display->setText(currentText + digit);
    }
}

void Calculator::operatorClicked()
{
    QPushButton *button = qobject_cast<QPushButton*>(sender());
    if (!button) return;
    
    QString op = button->text();
    
    // If there's a pending operation, calculate it first
    if (!currentOperator.isEmpty() && !isNewNumber) {
        equalsClicked();
        // After equals, we want to continue with the current result
        isResultDisplayed = false;
    }
    
    // Get current number
    firstNumber = display->text().toDouble();
    currentOperator = op;
    isNewNumber = true;
    isResultDisplayed = false;
    
    // Update expression display
    QString expr = formatNumber(firstNumber) + " " + op + " ";
    expressionDisplay->setText(expr);
}

void Calculator::equalsClicked()
{
    if (currentOperator.isEmpty()) {
        // No operator - just display the current number
        result = display->text().toDouble();
        display->setText(formatNumber(result));
        return;
    }
    
    // Get second number
    if (isNewNumber && !isResultDisplayed) {
        // If user pressed operator twice, use the first number as second
        secondNumber = firstNumber;
    } else {
        secondNumber = display->text().toDouble();
    }
    
    // Calculate
    double result = calculate(firstNumber, secondNumber, currentOperator);

	    // ✅ ADD THIS: Check if error was set by calculate()
    if (display->text() == "Division by zero is undefined") {
        // Error already displayed, don't overwrite with formatted result
        currentOperator = "";
        isNewNumber = true;
        isResultDisplayed = true;
        expressionDisplay->setText(QString::number(firstNumber) + " ÷ 0 = Error");
        return;
    }

    display->setText(formatNumber(result));
    
    // Update expression display
    QString expr = formatNumber(firstNumber) + " " + currentOperator + " " + 
                   formatNumber(secondNumber) + " =";
    expressionDisplay->setText(expr);


    
    // Store result for future operations
    this->result = result;
    firstNumber = result;
    currentOperator = "";
    isNewNumber = true;
    isResultDisplayed = true;
}

void Calculator::clearClicked()
{
/*    clearAll();
    display->setText("0");
    expressionDisplay->clear();
    isResultDisplayed = false;
*/

if (display->text() == "0") {
        // If already showing 0, clear everything (reset state)
        clearAll();
        display->setText("|");
        expressionDisplay->clear();
    } else {
        clearAll();
        display->setText("|");
        expressionDisplay->clear();
    }
    isResultDisplayed = false;

}

void Calculator::backspaceClicked()
{
    if (isNewNumber || isResultDisplayed) return;
    
    QString text = display->text();
    if (text.length() > 1) {
        text.chop(1);
        display->setText(text);
    } else {
        display->setText("0");
        isNewNumber = true;
    }
}

void Calculator::percentClicked()
{
    double value = display->text().toDouble();
    value = value / 100.0;
    display->setText(formatNumber(value));
}

void Calculator::signClicked()
{
    QString text = display->text();
    if (text == "0") return;
    
    if (text.startsWith("-")) {
        text.remove(0, 1);
    } else {
        text = "-" + text;
    }
    display->setText(text);
}

void Calculator::decimalClicked()
{
    if (isResultDisplayed) {
        clearAll();
        isResultDisplayed = false;
    }
    
    if (isNewNumber) {
        display->setText("0.");
        isNewNumber = false;
        return;
    }
    
    QString text = display->text();
    if (!text.contains(".")) {
        display->setText(text + ".");
    }
}

void Calculator::clearAll()
{
    firstNumber = 0;
    secondNumber = 0;
    result = 0;
    currentOperator = "";
    isNewNumber = true;
    isResultDisplayed = false;
//    display->setText("0");
    display->setText("|");
    expressionDisplay->clear();
}

double Calculator::calculate(double num1, double num2, const QString &op)
{
    double result = 0;
    
    if (op == "+") {
        result = num1 + num2;
    } else if (op == "-") {
        result = num1 - num2;
    } else if (op == "×") {
        result = num1 * num2;
    } else if (op == "÷") {
        if (num2 != 0) {
            result = num1 / num2;
        } else 
	{
            display->setText("Error");
            isNewNumber = true;
            currentOperator = "";
	               expressionDisplay->clear();  // ✅ ADD THIS LINE
		            display->setText("Division by zero is undefined");  // ✅ CHANGE THIS LINE
            return 0;
        }
    }
    
    return result;
}

// ✅ FIXED: formatNumber function
QString Calculator::formatNumber(double num)
{
    // Handle NaN and Infinity - use C math functions
    if (isnan(num) || isinf(num)) {
        return "Error";
    }
    
    // Format the number
    QString str = QString::number(num, 'f', 10);
    
    // Remove trailing zeros
    while (str.contains('.') && (str.endsWith('0') || str.endsWith('.'))) {
        str.chop(1);
    }
    
    // If the string is just "-", return "0"
    if (str == "-") {
        return "0";
    }
    
    return str;
}

void Calculator::updateDisplay()
{
    display->setText(formatNumber(result));
}
