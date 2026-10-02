qt_add_executable(widgets_drag_drop_table WIN32
    src/main.cpp
    src/Item.h
    src/ItemTableModel.cpp src/ItemTableModel.h
    src/SelectionDialog.cpp src/SelectionDialog.h
    src/SelectionViewModel.cpp src/SelectionViewModel.h
)

target_link_libraries(widgets_drag_drop_table PRIVATE Qt6::Widgets)

qt_example_deploy(widgets_drag_drop_table)
