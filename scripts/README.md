# Build Guide — SCPI C++ Instrument Demo

## Mental model

| Item | Analogy |
|---|---|
| `CMakeLists.txt` | Recipe |
| `cmake` | Recipe translator |
| `Makefile` / generated build files | Kitchen instructions |
| `cmake --build build` | Cook follows instructions |
| Compiler/linker | Oven/tools |
| Executable | Finished meal |

---

## Scripts

### 1. Use Apple Silicon Homebrew + Qt

Ensures ARM Homebrew + Qt are used instead of Intel `/usr/local`.

Run:

```bash
source ./scripts/use_arm_brew.sh
```

Check:

```bash
which brew
```

Expected:

```text
/opt/homebrew/bin/brew
```

---

### 2. Full clean

Deletes generated build output.

```bash
./scripts/clean.sh
```

---

### 3. Configure CMake + Qt

Generates build system files using Qt.

```bash
./scripts/configure_qt.sh
```

Conceptually:

```text
CMakeLists.txt
      ↓
cmake
      ↓
generated Makefiles/build files
```

---

### 4. Build

```bash
./scripts/build.sh
```

Outputs:

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

- Host: `localhost`
- Port: `5025`

---

## Run CLI Version

```bash
./build/scpi_cpp_demo localhost 5025 "*IDN?"
```

---

## Run Qt GUI Version

```bash
./build/scpi_cpp_qt_demo
```

Features:

- SCPI command interface
- Live acquisition
- 1D trace graph
- 2D RF heatmap
- 3D RF surface
- Debug console

---

## Notes

- Developed on Apple Silicon macOS
- Qt installed via Homebrew
- Uses:
  - Qt Widgets
  - Qt DataVisualization
  - C++17
  - CMake

- Current data is mock/simulated instrumentation data.
- Architecture separated into:
  - transport
  - SCPI client
  - parsing
  - visualization