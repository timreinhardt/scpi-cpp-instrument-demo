# Build Guide — SCPI C++ Instrument Demo

## Mental model



| x                                     | Analogy |
| `CMakeLists.txt`                      | Recipe |
| `cmake`                               | Recipe translator |
| `Makefile` / generated build files    | Kitchen instructions |
| `cmake --build build`                 | Cook follows instructions |
| Compiler/linker                       | Oven/tools |
| Executable                            | Finished meal |

---

## Scripts

### 1. Full clean

Use this when you want to delete all generated build output.

```bash
./scripts/clean.sh