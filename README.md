# SCPI C++ Instrument Demo

Modular C++ instrument-control prototype using SCPI over TCP/IP sockets with transport abstraction, live Qt visualization, and a Python mock spectrum-analyzer simulator.

## Features

- SCPI communication over TCP sockets
- Persistent instrument connection
- Modular transport abstraction layer
- Qt6 desktop GUI
- Live trace polling
- CSV trace parsing into numeric vectors
- Real-time trace statistics
- Live 2D spectrum-style graph rendering
- Mock Python SCPI instrument simulator

---

## Architecture

```text
Qt GUI / CLI
      ↓
ScpiClient
      ↓
ITransport
      ↓
TcpTransport
      ↓
TCP Socket
      ↓
Mock SCPI Instrument
```

---

## Project Structure

```text
core/       SCPI logic and trace parsing/statistics
transport/  Transport abstraction and TCP implementation
qt-ui/      Qt GUI and live trace widget
ui/         CLI interface
mock/       Python SCPI instrument simulator
scripts/    Build/configure/run scripts
```

---

## Build Workflow

Clean:

```bash
./scripts/clean.sh
```

Configure Qt + CMake:

```bash
./scripts/configure_qt.sh
```

Build:

```bash
./scripts/build.sh
```

Build outputs:

```text
build/scpi_cpp_demo
build/scpi_cpp_qt_demo
```

---

## Run Mock SCPI Instrument

```bash
python3 mock/mock_scpi_server.py
```

Default:
- Host: localhost
- Port: 5025

---

## Run CLI Demo

```bash
./build/scpi_cpp_demo localhost 5025 "*IDN?"
```

---

## Run Qt GUI

```bash
./build/scpi_cpp_qt_demo
```

Example commands:
- `*IDN?`
- `:MEAS:VOLT?`
- `:TRAC:DATA?`

---

## Live Trace Visualization

The Qt GUI supports live polling, parsed trace statistics, and real-time spectrum-style plotting.

Example summary:

```text
Trace Summary
Points: 51
Min: -71.49 dBm
Max: -44.74 dBm
Avg: -63.86 dBm
```

---

## Current Status

Implemented:
- SCPI TCP communication
- Persistent connection handling
- Qt GUI controls
- Mock spectrum analyzer
- Live polling timer
- CSV trace parsing
- Trace statistics
- Live 2D graph rendering

Planned:
- Fixed dBm scaling
- Grid/axis improvements
- Waterfall visualization
- Heatmap / field mapping
- 3D visualization
- Multi-trace overlays

---

## Notes

- Developed on Apple Silicon macOS
- Qt installed via Homebrew
- Intended to remain cross-platform
- Core logic is platform-agnostic C++
