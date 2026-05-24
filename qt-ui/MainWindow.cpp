#include "MainWindow.hpp"

#include "FieldGrid.hpp"
#include "HeatmapWidget.hpp"
#include "ScpiClient.hpp"
#include "TcpTransport.hpp"
#include "TraceData.hpp"
#include "TraceWidget.hpp"

#include <QComboBox>
#include <QCoreApplication>
#include <QDateTime>
#include <QElapsedTimer>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>

#include <exception>
#include <memory>
#include <string>

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

    // -----------------------------------------------------
    // Connection controls
    // -----------------------------------------------------

    hostInput_ = new QLineEdit("localhost");
    portInput_ = new QLineEdit("5025");

    connectButton_ = new QPushButton("Connect");
    disconnectButton_ = new QPushButton("Disconnect");

    layout->addWidget(new QLabel("Host"));
    layout->addWidget(hostInput_);

    layout->addWidget(new QLabel("Port"));
    layout->addWidget(portInput_);

    layout->addWidget(connectButton_);
    layout->addWidget(disconnectButton_);

    // -----------------------------------------------------
    // SCPI command controls
    // -----------------------------------------------------

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
    commandSelect_->addItem(":FIELD:GRID? - Get fake field grid", ":FIELD:GRID?");
    commandSelect_->addItem(":SYST:ERR? - Query error queue", ":SYST:ERR?");
    commandSelect_->addItem("Custom text command", "");

    commandInput_ = new QLineEdit("*IDN?");
    sendButton_ = new QPushButton("Send Command");

    layout->addWidget(new QLabel("SCPI Command"));
    layout->addWidget(commandSelect_);
    layout->addWidget(commandInput_);
    layout->addWidget(sendButton_);

    // -----------------------------------------------------
    // Live trace controls
    // -----------------------------------------------------

    startLiveButton_ = new QPushButton("Start Live Acquisition");
    stopLiveButton_ = new QPushButton("Stop Live Acquisition");

    pollTimer_ = new QTimer(this);
    pollTimer_->setInterval(500);

    layout->addWidget(startLiveButton_);
    layout->addWidget(stopLiveButton_);

    // -----------------------------------------------------
    // Trace graph
    // -----------------------------------------------------

    traceGraphLabel_ = new QLabel("Trace Graph");
    traceWidget_ = new TraceWidget();

    traceGraphLabel_->hide();
    traceWidget_->hide();

    layout->addWidget(traceGraphLabel_);
    layout->addWidget(traceWidget_);

    // -----------------------------------------------------
    // Field heatmap
    // -----------------------------------------------------

    heatmapLabel_ = new QLabel("Field Heatmap");
    heatmapWidget_ = new HeatmapWidget();

    heatmapLabel_->hide();
    heatmapWidget_->hide();

    layout->addWidget(heatmapLabel_);
    layout->addWidget(heatmapWidget_);

    // -----------------------------------------------------
    // Response and debug output
    // -----------------------------------------------------

    responseBox_ = new QTextEdit();
    responseBox_->setReadOnly(true);

    consoleBox_ = new QTextEdit();
    consoleBox_->setReadOnly(true);
    consoleBox_->setMinimumHeight(140);

    layout->addWidget(new QLabel("Response"));
    layout->addWidget(responseBox_);

    layout->addWidget(new QLabel("Debug Console"));
    layout->addWidget(consoleBox_);

    updateConnectionState(false);

    resize(760, 900);
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

    connect(startLiveButton_, &QPushButton::clicked,
            this, &MainWindow::startLiveTrace);

    connect(stopLiveButton_, &QPushButton::clicked,
            this, &MainWindow::stopLiveTrace);

    connect(pollTimer_, &QTimer::timeout,
            this, &MainWindow::pollTraceData);
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

    if (pollTimer_)
    {
        pollTimer_->stop();
    }

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
    startLiveButton_->setEnabled(connected);
    stopLiveButton_->setEnabled(false);

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

        if (command == ":TRAC:DATA?")
        {
            auto values = parseTraceCsv(response);
            auto summary = summarizeTraceData(values);

            traceGraphLabel_->show();
            traceWidget_->show();
            traceWidget_->setTraceData(values);

            responseBox_->setText(
                QString::fromStdString(formatTraceSummary(summary, values))
            );
        }
        else if (command == ":FIELD:GRID?")
        {
            auto grid = parseFieldGrid(response);

            heatmapLabel_->show();
            heatmapWidget_->show();
            heatmapWidget_->setGridData(grid);

            responseBox_->setText(
                QString("Field Grid Summary\nRows: %1\nColumns: %2")
                    .arg(fieldGridRows(grid))
                    .arg(fieldGridCols(grid))
            );
        }
        else
        {
            responseBox_->setText(QString::fromStdString(response));
        }
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

void MainWindow::startLiveTrace()
{
    if (!client_)
    {
        logToConsole("ERROR: Cannot start live trace while disconnected");
        responseBox_->setText("ERROR: Not connected");
        return;
    }

    logToConsole("----------------------------------------");
    logToConsole("Starting live acquisition every 500 ms");

    pollTimer_->start();

    startLiveButton_->setEnabled(false);
    stopLiveButton_->setEnabled(true);
}

void MainWindow::stopLiveTrace()
{
    logToConsole("----------------------------------------");
    logToConsole("Stopping live acquisition");

    pollTimer_->stop();

    startLiveButton_->setEnabled(true);
    stopLiveButton_->setEnabled(false);
}

void MainWindow::pollTraceData()
{
    if (!client_)
    {
        logToConsole("ERROR: Live acquisition attempted while disconnected");

        pollTimer_->stop();
        updateConnectionState(false);

        return;
    }

    try
    {
        // -------------------------------------------------
        // Poll 1D spectrum trace
        // -------------------------------------------------

        std::string traceResponse = client_->query(":TRAC:DATA?");

        auto values = parseTraceCsv(traceResponse);
        auto summary = summarizeTraceData(values);

        traceGraphLabel_->show();
        traceWidget_->show();
        traceWidget_->setTraceData(values);

        // -------------------------------------------------
        // Poll 2D field grid / heatmap
        // -------------------------------------------------

        std::string gridResponse = client_->query(":FIELD:GRID?");

        auto grid = parseFieldGrid(gridResponse);

        heatmapLabel_->show();
        heatmapWidget_->show();
        heatmapWidget_->setGridData(grid);

        // -------------------------------------------------
        // Human-readable status output
        // -------------------------------------------------

        responseBox_->setText(
            QString("Live Acquisition Summary\n"
                    "Trace points: %1\n"
                    "Trace min: %2 dBm\n"
                    "Trace max: %3 dBm\n"
                    "Trace avg: %4 dBm\n"
                    "Field rows: %5\n"
                    "Field cols: %6")
                .arg(summary.pointCount)
                .arg(summary.minValue, 0, 'f', 2)
                .arg(summary.maxValue, 0, 'f', 2)
                .arg(summary.averageValue, 0, 'f', 2)
                .arg(fieldGridRows(grid))
                .arg(fieldGridCols(grid))
        );

        logToConsole(
            "Live acquisition update received: " +
            QString::number(summary.pointCount) +
            " trace points, " +
            QString::number(fieldGridRows(grid)) +
            "x" +
            QString::number(fieldGridCols(grid)) +
            " field grid"
        );
    }
    catch (const std::exception& ex)
    {
        QString error = QString("LIVE ACQUISITION ERROR: ") + ex.what();

        logToConsole(error);
        responseBox_->setText(error);

        pollTimer_->stop();

        client_.reset();
        transport_.reset();

        updateConnectionState(false);
    }
}