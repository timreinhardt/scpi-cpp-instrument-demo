#pragma once

#include <QWidget>

#include <memory>

class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;
class QTimer;
class QLabel;
class HeatmapWidget;

#include "TcpTransport.hpp"
#include "ScpiClient.hpp"
class TraceWidget;
class MainWindow : public QWidget
{
public:
    explicit MainWindow(QWidget *parent = nullptr);

private:
    void setupUi();
    void connectSignals();
    void logToConsole(const QString& message);

    void connectToInstrument();
    void disconnectFromInstrument();
    void sendScpiCommand();
    void syncCommandFromDropdown(int index);
    void updateConnectionState(bool connected);
    void startLiveTrace();
    void stopLiveTrace();
    void pollTraceData();

    QLineEdit *hostInput_ = nullptr;
    QLineEdit *portInput_ = nullptr;
    QLineEdit *commandInput_ = nullptr;
    QLabel *traceGraphLabel_ = nullptr;
    QComboBox *commandSelect_ = nullptr;

    QPushButton *connectButton_ = nullptr;
    QPushButton *disconnectButton_ = nullptr;
    QPushButton *sendButton_ = nullptr;

    QPushButton *startLiveButton_ = nullptr;
    QPushButton *stopLiveButton_ = nullptr;
    QTimer *pollTimer_ = nullptr;

    QTextEdit *responseBox_ = nullptr;
    TraceWidget *traceWidget_ = nullptr;
    QTextEdit *consoleBox_ = nullptr;
    HeatmapWidget *heatmapWidget_ = nullptr;
    QLabel *heatmapLabel_ = nullptr;
    std::unique_ptr<TcpTransport> transport_;
    std::unique_ptr<ScpiClient> client_;
};