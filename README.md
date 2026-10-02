# Qt Explorings

A collection of small, self-contained Qt 6 examples for teaching. Every folder covers one topic and has its own
`README.md` that explains the concept and walks through the code.

| Folder | Topic | UI technology |
|---|---|---|
| [01-widgets-mvvm-dialog](01-widgets-mvvm-dialog) | MVVM with a classic `QDialog` | Widgets |
| [02-qml-mvvm-dialog](02-qml-mvvm-dialog) | The same MVVM, View written in QML inside a `QQuickWidget` | Widgets + QML |
| [03-widgets-drag-drop-table](03-widgets-drag-drop-table) | Drag & drop between two tables, target limited to *N* items | Widgets |
| [04-qml-drag-drop-table](04-qml-drag-drop-table) | The same drag & drop in QML inside a `QQuickWidget` | Widgets + QML |
| [05-tree-view](05-tree-view) | Custom `QAbstractItemModel` shown in a `QTreeView`, with filtering | Widgets |
| [06-custom-delegate-listview](06-custom-delegate-listview) | `QStyledItemDelegate` that paints cards in a `QListView` | Widgets |
| [07-network-access](07-network-access) | REST client for [jsonplaceholder.typicode.com](https://jsonplaceholder.typicode.com/) | Widgets |

## Prerequisites

- Qt 6.8 or newer (Widgets, Quick, QuickWidgets, QuickControls2, Qml, Network)
- CMake 3.28 or newer, Ninja
- MSVC (Visual Studio 2022 or newer) with C++20

## Building

The presets follow the same layout as our other projects:

- `CMakePresets.json` (committed) defines the generator, compiler and build types (`msvc-debug`, `msvc-release`, …).
- `CMakeUserPresets.json` (local, git-ignored) adds machine-specific paths, in particular where Qt is installed,
  and defines the `local-*` presets you actually use.

Adjust `CMAKE_PREFIX_PATH` in `CMakeUserPresets.json` to your Qt installation. Then, from a
*Developer PowerShell for Visual Studio* (so that `cl.exe` is on the `PATH`):

```powershell
cmake --preset local-debug
cmake --build --preset local-debug
```

Every example builds into its own folder, e.g. `out/build/local-debug/05-tree-view/tree_view.exe`.
After each build, `windeployqt` copies the Qt runtime next to the executable (see
[`cmake/QtExampleDeploy.cmake`](cmake/QtExampleDeploy.cmake)), so the programs can be started directly.

QML files are checked with `qmllint`:

```powershell
cmake --build --preset local-debug --target all_qmllint
```

## Project layout

```
CMakeLists.txt          superbuild: finds Qt once and adds every example
cmake/                  shared CMake helpers
NN-topic/
    CMakeLists.txt      the example's target
    README.md           explanation
    src/                C++ sources
    qml/                QML sources (QML examples only)
```

The examples share no code on purpose. Each folder can be read on its own, so some small types
(for example `Person` or `Item`) exist in more than one folder.
