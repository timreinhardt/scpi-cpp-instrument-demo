#pragma once

// =====================================================
// Qt Base Widget
// =====================================================
#include <QWidget>

// =====================================================
// Project Includes
// =====================================================
// TCP socket transport layer
#include "TcpTransport.hpp"

// High-level SCPI command/query interface
#include "ScpiClient.hpp"
#include <memory>

// =====================================================
// Forward Declarations
// =====================================================
//
// reduce unnecessary header includes, speed up compile times.
// We only store pointers here, so full class definitions
// are not required in this header.
//

class QLabel;
class QLineEdit;
class QTextEdit;
class QComboBox;
class QPushButton;
class QTimer;

// Custom visualization widgets
class TraceWidget;
class HeatmapWidget;
class Surface3DWidget;

// =====================================================
// Main Application Window
// =====================================================

class MainWindow : public QWidget
{
public:

    // Main window constructor
    explicit MainWindow(QWidget *parent = nullptr);

private:

    // =================================================
    // UI Setup / Wiring
    // =================================================

    // Creates and arranges all UI widgets/layouts
    void setupUi();

    // Connects Qt signals -> slots/functions
    void connectSignals();

    // Writes timestamped messages to debug console
    void logToConsole(const QString& message);

    // =================================================
    // Instrument Connection
    // =================================================

    // Connect to TCP SCPI instrument
    void connectToInstrument();

    // Disconnect and cleanup resources
    void disconnectFromInstrument();

    // =================================================
    // SCPI Command Handling
    // =================================================

    // Sends currently selected SCPI command
    void sendScpiCommand();

    // Clears graphs/heatmaps/3D views
    void clearVisuals();

    // Synchronize command textbox with dropdown selection
    void syncCommandFromDropdown(int index);

    // Enable/disable controls depending on connection state
    void updateConnectionState(bool connected);

    // =================================================
    // Live Acquisition
    // =================================================

    // Start periodic acquisition polling
    void startLiveTrace();

    // Stop periodic acquisition polling
    void stopLiveTrace();

    // Poll live trace + field grid data
    void pollTraceData();

    // =================================================
    // Connection Controls
    // =================================================

    QLineEdit *hostInput_ = nullptr;
    QLineEdit *portInput_ = nullptr;

    // =================================================
    // Command Controls
    // =================================================

    QLineEdit *commandInput_ = nullptr;
    QComboBox *commandSelect_ = nullptr;

    QPushButton *connectButton_ = nullptr;
    QPushButton *disconnectButton_ = nullptr;

    QPushButton *sendButton_ = nullptr;
    QPushButton *clearButton_ = nullptr;

    QPushButton *startLiveButton_ = nullptr;
    QPushButton *stopLiveButton_ = nullptr;

    // =================================================
    // Acquisition Timer
    // =================================================
    //
    // Periodically polls live instrument data.
    //

    QTimer *pollTimer_ = nullptr;

    // =================================================
    // 1D Trace Visualization
    // =================================================
    //
    // Spectrum-analyzer style trace graph.
    //

    QLabel *traceGraphLabel_ = nullptr;
    TraceWidget *traceWidget_ = nullptr;

    // =================================================
    // 2D Heatmap Visualization
    // =================================================
    //
    // RF field intensity map.
    //

    QLabel *heatmapLabel_ = nullptr;
    HeatmapWidget *heatmapWidget_ = nullptr;

    // =================================================
    // 3D Surface Visualization
    // =================================================
    //
    // 3D representation of RF field intensity.
    //

    QLabel *surface3DLabel_ = nullptr;
    Surface3DWidget *surface3DWidget_ = nullptr;

    // =================================================
    // Text Output Areas
    // =================================================

    // Human-readable SCPI responses/status
    QTextEdit *responseBox_ = nullptr;

    // Debug / logging console
    QTextEdit *consoleBox_ = nullptr;

    // =================================================
    // Backend Communication Objects
    // =================================================

    // Low-level TCP transport
    std::unique_ptr<TcpTransport> transport_;

    // High-level SCPI client
    std::unique_ptr<ScpiClient> client_;
};