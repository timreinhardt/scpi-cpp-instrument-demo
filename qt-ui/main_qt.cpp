#include "ScpiClient.hpp"
#include "TcpTransport.hpp"

#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QComboBox>
#include <QDateTime>
#include <QElapsedTimer>
#include <QCoreApplication>

#include <memory>
#include <string>
#include <exception>

static void logToConsole(QTextEdit *console, const QString& message)
{
    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    console->append("[" + timestamp + "] " + message);
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("SCPI C++ Instrument Demo");

    auto *layout = new QVBoxLayout(&window);

    auto *hostInput = new QLineEdit("localhost");
    auto *portInput = new QLineEdit("5025");

    auto *commandSelect = new QComboBox();
    commandSelect->addItem("*IDN? - Identify instrument", "*IDN?");
    commandSelect->addItem(":MEAS:VOLT? - Read voltage", ":MEAS:VOLT?");
    commandSelect->addItem(":MEAS:CURR? - Read current", ":MEAS:CURR?");
    commandSelect->addItem(":FREQ:CENT? - Get centre frequency", ":FREQ:CENT?");
    commandSelect->addItem(":FREQ:CENT 2400000000 - Set centre frequency", ":FREQ:CENT 2400000000");
    commandSelect->addItem(":FREQ:SPAN? - Get span", ":FREQ:SPAN?");
    commandSelect->addItem(":FREQ:SPAN 100000000 - Set span", ":FREQ:SPAN 100000000");
    commandSelect->addItem(":INIT - Start sweep/acquisition", ":INIT");
    commandSelect->addItem(":TRAC:DATA? - Get fake trace data", ":TRAC:DATA?");
    commandSelect->addItem(":SYST:ERR? - Query error queue", ":SYST:ERR?");
    commandSelect->addItem("Custom text command", "");

    auto *commandInput = new QLineEdit("*IDN?");
    auto *sendButton = new QPushButton("Send Command");

    auto *responseBox = new QTextEdit();
    responseBox->setReadOnly(true);

    auto *consoleBox = new QTextEdit();
    consoleBox->setReadOnly(true);
    consoleBox->setMinimumHeight(140);

    layout->addWidget(new QLabel("Host"));
    layout->addWidget(hostInput);

    layout->addWidget(new QLabel("Port"));
    layout->addWidget(portInput);

    layout->addWidget(new QLabel("SCPI Command"));
    layout->addWidget(commandSelect);
    layout->addWidget(commandInput);

    layout->addWidget(sendButton);

    layout->addWidget(new QLabel("Response"));
    layout->addWidget(responseBox);

    layout->addWidget(new QLabel("Debug Console"));
    layout->addWidget(consoleBox);

    QObject::connect(
        commandSelect,
        &QComboBox::currentIndexChanged,
        [&](int index)
        {
            QString command = commandSelect->itemData(index).toString();

            if (!command.isEmpty())
            {
                commandInput->setText(command);
                logToConsole(consoleBox, "Dropdown selected command: " + command);
            }

            commandInput->setFocus();
        });

    QObject::connect(
        sendButton,
        &QPushButton::clicked,
        [&]()
        {
            sendButton->setEnabled(false);
            sendButton->setText("Sending...");
            QCoreApplication::processEvents();

            QElapsedTimer timer;
            timer.start();

            QString hostQ = hostInput->text();
            QString portQ = portInput->text();
            QString commandQ = commandInput->text();

            logToConsole(consoleBox, "----------------------------------------");
            logToConsole(consoleBox, "Send button clicked");
            logToConsole(consoleBox, "Host: " + hostQ);
            logToConsole(consoleBox, "Port: " + portQ);
            logToConsole(consoleBox, "Command: " + commandQ);

            try
            {
                std::string host = hostQ.toStdString();
                int port = portQ.toInt();
                std::string command = commandQ.toStdString();

                logToConsole(consoleBox, "Creating TcpTransport");
                auto transport = std::make_unique<TcpTransport>(host, port);

                logToConsole(consoleBox, "Creating ScpiClient");
                ScpiClient client(std::move(transport));

                logToConsole(consoleBox, "Sending SCPI query...");
                std::string response = client.query(command);

                logToConsole(consoleBox, "Response received");
                logToConsole(consoleBox, "Elapsed ms: " + QString::number(timer.elapsed()));

                responseBox->setText(QString::fromStdString(response));
            }
            catch (const std::exception& ex)
            {
                QString error = QString("ERROR: ") + ex.what();

                logToConsole(consoleBox, error);
                logToConsole(consoleBox, "Elapsed ms: " + QString::number(timer.elapsed()));

                responseBox->setText(error);
            }

            sendButton->setText("Send Command");
            sendButton->setEnabled(true);
        });

    logToConsole(consoleBox, "Application started");
    logToConsole(consoleBox, "Ready");

    window.resize(620, 620);
    window.show();

    return app.exec();
}