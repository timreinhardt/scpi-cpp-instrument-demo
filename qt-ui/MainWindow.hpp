#pragma once

#include <QWidget>

class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;

class MainWindow : public QWidget
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUi();
    void connectSignals();
    void logToConsole(const QString& message);
    void sendScpiCommand();
    void syncCommandFromDropdown(int index);

    QLineEdit *hostInput_ = nullptr;
    QLineEdit *portInput_ = nullptr;
    QLineEdit *commandInput_ = nullptr;

    QComboBox *commandSelect_ = nullptr;
    QPushButton *sendButton_ = nullptr;

    QTextEdit *responseBox_ = nullptr;
    QTextEdit *consoleBox_ = nullptr;
};