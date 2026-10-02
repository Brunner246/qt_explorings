qt_add_executable(qml_drag_drop_table WIN32
    src/main.cpp
    src/SelectionDialog.cpp src/SelectionDialog.h
)

qt_add_qml_module(qml_drag_drop_table
    URI DragDropQml
    QML_FILES
        qml/SelectionView.qml
        qml/ItemTable.qml
        qml/DragGhost.qml
    SOURCES
        src/Item.h
        src/ItemTableModel.cpp src/ItemTableModel.h
        src/SelectionViewModel.cpp src/SelectionViewModel.h
)

target_include_directories(qml_drag_drop_table PRIVATE src)
target_link_libraries(qml_drag_drop_table PRIVATE Qt6::Widgets Qt6::QuickWidgets Qt6::QuickControls2)

qt_example_deploy(qml_drag_drop_table QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")
