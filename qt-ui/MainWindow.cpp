#include "MainWindow.hpp"

#include "ScpiClient.hpp"
#include "TcpTransport.hpp"

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

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent)
{
    setupUi();
    connectSignals();

    logToConsole("Application started");
    logToConsole("Ready");
}

void MainWindow::setupUi()
{
    setWindowTitle("SCPI C++ Instrument Demo");

    auto *layout = new QVBoxLayout(this);

    hostInput_ = new QLineEdit("localhost");
    portInput_ = new QLineEdit("5025");

    commandSelect_ = new QComboBox();
    commandSelect_->addItem("*IDN? - Identify instrument", "*IDN?");
    commandSelect_->addItem(":MEAS:VOLT? - Read voltage", ":MEAS:VOLT?");
    commandSelect_->addItem(":MEAS:CURR? - Read current", ":MEAS:CURR?");
    commandSelect_->addItem(":FREQ:CENT? - Get centre frequency", ":FREQ:CENT?");
    commandSelect_->addItem(":FREQ:CENT 2400000000 - Set centre frequency", ":FREQ:CENT 2400000000");
    commandSelect_->addItem(":FREQ:SPAN? - Get span", ":FREQ:SPAN?");
    commandSelect_->addItem(":FREQ:SPAN 100000000 - Set span", ":FREQ:SPAN 100000000");
    commandSelect_->addItem(":INIT - Start sweep/acquisition", ":INIT");
    commandSelect_->addItem(":TRAC:DATA? - Get fake trace data", ":TRAC:DATA?");
    commandSelect_->addItem(":SYST:ERR? - Query error queue", ":SYST:ERR?");
    commandSelect_->addItem("Custom text command", "");

    commandInput_ = new QLineEdit("*IDN?");

    connectButton_ = new QPushButton("Connect");
    disconnectButton_ = new QPushButton("Disconnect");
    sendButton_ = new QPushButton("Send Command");

    layout->addWidget(connectButton_);
    layout->addWidget(disconnectButton_);
    layout->addWidget(sendButton_);

    updateConnectionState(false);

    sendButton_ = new QPushButton("Send Command");

    responseBox_ = new QTextEdit();
    responseBox_->setReadOnly(true);

    consoleBox_ = new QTextEdit();
    consoleBox_->setReadOnly(true);
    consoleBox_->setMinimumHeight(140);

    layout->addWidget(new QLabel("Host"));
    layout->addWidget(hostInput_);

    layout->addWidget(new QLabel("Port"));
    layout->addWidget(portInput_);

    layout->addWidget(new QLabel("SCPI Command"));
    layout->addWidget(commandSelect_);
    layout->addWidget(commandInput_);

    layout->addWidget(sendButton_);

    layout->addWidget(new QLabel("Response"));
    layout->addWidget(responseBox_);

    layout->addWidget(new QLabel("Debug Console"));
    layout->addWidget(consoleBox_);

    resize(620, 620);
}

void MainWindow::connectSignals()
{
    connect(commandSelect_, &QComboBox::currentIndexChanged,
            this, &MainWindow::syncCommandFromDropdown);

    connect(connectButton_, &QPushButton::clicked,
            this, &MainWindow::connectToInstrument);

    connect(disconnectButton_, &QPushButton::clicked,
            this, &MainWindow::disconnectFromInstrument);

    connect(sendButton_, &QPushButton::clicked,
            this, &MainWindow::sendScpiCommand);
}
void MainWindow::connectToInstrument()
{
    QString hostQ = hostInput_->text();
    QString portQ = portInput_->text();

    logToConsole("----------------------------------------");
    logToConsole("Connect button clicked");
    logToConsole("Host: " + hostQ);
    logToConsole("Port: " + portQ);

    try
    {
        std::string host = hostQ.toStdString();
        int port = portQ.toInt();

        transport_ = std::make_unique<TcpTransport>(host, port);
        transport_->connectToTarget();

        client_ = std::make_unique<ScpiClient>(std::move(transport_));

        logToConsole("Connected to instrument");
        updateConnectionState(true);
    }
    catch (const std::exception& ex)
    {
        QString error = QString("CONNECT ERROR: ") + ex.what();
        logToConsole(error);
        responseBox_->setText(error);

        client_.reset();
        transport_.reset();
        updateConnectionState(false);
    }
}

void MainWindow::disconnectFromInstrument()
{
    logToConsole("----------------------------------------");
    logToConsole("Disconnect button clicked");

    client_.reset();
    transport_.reset();

    logToConsole("Disconnected");
    updateConnectionState(false);
}

void MainWindow::updateConnectionState(bool connected)
{
    connectButton_->setEnabled(!connected);
    disconnectButton_->setEnabled(connected);
    sendButton_->setEnabled(connected);

    hostInput_->setEnabled(!connected);
    portInput_->setEnabled(!connected);
}

void MainWindow::logToConsole(const QString& message)
{
    QString timestamp =
        QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss.zzz");

    consoleBox_->append("[" + timestamp + "] " + message);
}

void MainWindow::syncCommandFromDropdown(int index)
{
    QString command = commandSelect_->itemData(index).toString();

    if (!command.isEmpty())
    {
        commandInput_->setText(command);
        logToConsole("Dropdown selected command: " + command);
    }

    commandInput_->setFocus();
}

void MainWindow::sendScpiCommand()
{
    if (!client_)
    {
        responseBox_->setText("ERROR: Not connected");
        logToConsole("ERROR: Send attempted while not connected");
        return;
    }

    sendButton_->setEnabled(false);
    sendButton_->setText("Sending...");
    QCoreApplication::processEvents();

    QElapsedTimer timer;
    timer.start();

    QString commandQ = commandInput_->text();

    logToConsole("----------------------------------------");
    logToConsole("Send button clicked");
    logToConsole("Command: " + commandQ);

    try
    {
        std::string command = commandQ.toStdString();

        logToConsole("Sending SCPI query...");
        std::string response = client_->query(command);

        logToConsole("Response received");
        logToConsole("Elapsed ms: " + QString::number(timer.elapsed()));

        responseBox_->setText(QString::fromStdString(response));
    }
    catch (const std::exception& ex)
    {
        QString error = QString("ERROR: ") + ex.what();

        logToConsole(error);
        logToConsole("Elapsed ms: " + QString::number(timer.elapsed()));

        responseBox_->setText(error);

        client_.reset();
        transport_.reset();
        updateConnectionState(false);
    }

    if (client_)
    {
        sendButton_->setText("Send Command");
        sendButton_->setEnabled(true);
    }
}