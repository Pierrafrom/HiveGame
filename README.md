# HiveGame

[![Tests](https://github.com/Pierrafrom/HiveGame/actions/workflows/unit-tests-pipeline.yml/badge.svg)](https://github.com/Pierrafrom/HiveGame/actions/workflows/unit-tests-pipeline.yml)
[![Docs](https://github.com/Pierrafrom/HiveGame/actions/workflows/generate-docs.yml/badge.svg)](https://pierrafrom.github.io/HiveGame/)
[![C++](https://img.shields.io/badge/C%2B%2B-CMake-00599C?logo=cplusplus)](CMakeLists.txt)
[![Qt](https://img.shields.io/badge/GUI-Qt-41CD52?logo=qt)](#build-modes)

A full C++ implementation of **Hive** (John Yianni's tile-laying board
game where players place and slide bug pieces to surround the opponent's
queen bee, no board or dice), playable through a console (CLI) or a
Qt graphical interface — same game engine underneath both. Built as a
4-person student project for the LO21 (advanced OOP/design patterns)
course, applying MVC, Factory, Strategy and Singleton where each pattern
actually earns its place rather than as a checklist. Full documentation
is generated from the code with Doxygen — see the
[Documentation](https://pierrafrom.github.io/HiveGame/) link above.

## Build Modes

Configuration is handled using CMake. A parameter allows you to enable or disable the graphical interface:

```cmake
option(ENABLE_GUI "Enable GUI mode" ON)
```

When `ENABLE_GUI` is enabled, Qt sources and dependencies are included during configuration. Otherwise, only the CLI version is built. The path to your Qt installation can be defined in an optional `cmake/UserConfig.cmake` file, for example:

```cmake
set(QT_DIR "<path to Qt>")
```

Example to build CLI only:

```bash
cmake -B build -S . -DENABLE_GUI=OFF
cmake --build build
```

To build the GUI version, simply leave `ENABLE_GUI` set to `ON` (default) and specify `QT_DIR`.

## Project Structure

The project is organized into cleanly separated modules:

* **models/**: Core game logic (board, pieces, rules, etc.). Key classes like `Game`, `GameRules`, and `Serializer` are implemented as Singletons to ensure a single global instance.
* **controllers/**: User interaction handling. Contains `cli` and `gui` subfolders for the interface choice.
* **views/**: CLI display or Qt graphical components depending on the selected mode.
* **utils/**: Various utilities, including JSON serialization.
* **config/**: Configuration constants, including maximum piece quantities.

Pieces are created using a **Factory pattern** (`PieceFactory`) and utilize a **Strategy pattern** for movement behavior, making it easy to add extensions and maintain the code.

## Quality and Maintainability

The project is extensively documented (see `docs/mainpage.md`), and the code is annotated with Doxygen comments. Unit tests are available in the `tests` folder, using GoogleTest integrated with CMake:

```cmake
add_executable(HiveGameTests ${TEST_SOURCES})
target_link_libraries(HiveGameTests gtest gtest_main)
enable_testing()
add_test(NAME AllTests COMMAND HiveGameTests)
```

These tests cover key classes (`Board`, `Game`, `Move`, etc.) and can be reviewed in `tests/models/GameTest.cpp`.

Two GitHub Actions workflows are provided:

* **unit-tests-pipeline.yml**: Builds the project and runs `ctest` on each push or pull request to `main`.
* **generate-docs.yml**: Generates Doxygen documentation and deploys it on GitHub Pages.

## Running Tests

After configuring the project:

```bash
cmake -B build -S .
cmake --build build
cd build
ctest --output-on-failure
```

## Development Team

* Pierre Fromont
* Alberic Peveraud de Vaumas
* Benoit de Miscault
* Robin Lanfranchi
