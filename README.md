Here is your **HiveGame README rewritten in English** for your public GitHub:

---

# HiveGame

[Documentation](https://pierrafrom.github.io/HiveGame/)

**HiveGame** is a student project developed for the LO21 course. It implements the board game **Hive** (by John Yianni), allowing gameplay either in **console mode (CLI)** or with a **graphical interface (Qt-based GUI)**. Detailed documentation is available in the `docs` folder and can be generated using Doxygen.

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

---

Let me know if you would like a **badge section**, **license section**, or a **feature roadmap** added for your public GitHub presentation.
