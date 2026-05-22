# SCPI C++ Instrument Demo

Modular C++ instrument-control prototype using SCPI over TCP/IP sockets with transport abstraction and a mock spectrum-analyzer simulator.

The project is intended as a learning/demo platform for:
- SCPI instrument communication
- TCP socket programming
- C++ architecture layering
- CMake-based builds
- Qt desktop GUI integration
- Instrumentation and test-equipment software concepts

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

The backend transport and SCPI logic are separated so different communication methods can later be added such as:
- Ethernet/TCP
- Serial/COM
- USB
- VISA-style abstraction

---

# Project Structure

```text
core/
    SCPI command logic

transport/
    Transport abstraction and TCP implementation

ui/
    Command-line interface

qt-ui/
    Qt Widgets desktop GUI

mock/
    Python mock SCPI instrument simulator

scripts/
    Build and clean scripts
```

---

# Build System

This project uses:
- CMake
- C++17
- Qt6 Widgets

Builds are performed out-of-source into:

```text
build/
```

---

# Build Workflow

## Full clean

Removes all generated build files.

```bash
./scripts/clean.sh
```

---

## Configure CMake + Qt

```bash
./scripts/configure_qt.sh
```

Equivalent conceptually to:

```text
CMakeLists.txt
      ↓
cmake
      ↓
generated Makefiles/build files
```

---

## Build

```bash
./scripts/build.sh
```

Build outputs:

```text
build/scpi_cpp_demo
build/scpi_cpp_qt_demo
```

---

# Run Mock SCPI Instrument

```bash
python3 mock/mock_scpi_server.py
```

Default:
- Host: localhost
- Port: 5025

---

# Run CLI Version

```bash
./build/scpi_cpp_demo localhost 5025 "*IDN?"
```

---

# Run Qt GUI Version

```bash
./build/scpi_cpp_qt_demo
```

Then:
- Host: localhost
- Port: 5025
- Command: *IDN?

Press:
Send Command

Expected response:

```text
YIC,MOCK-SPECTRUM-ANALYZER,MOCK12345,1.0
```

---

# Notes

- Developed on Apple Silicon macOS
- Qt installed via Homebrew
- Intended to later run cross-platform on:
  - Windows
  - Linux
  - Embedded Linux/Raspberry Pi

- Current Qt implementation is desktop-oriented.
- Core transport and SCPI layers are platform-agnostic C++.
