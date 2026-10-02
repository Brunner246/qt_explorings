qt_add_executable(widgets_mvvm_dialog WIN32
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/Person.h
    src/PersonDialog.cpp src/PersonDialog.h
    src/PersonViewModel.cpp src/PersonViewModel.h
)

target_link_libraries(widgets_mvvm_dialog PRIVATE Qt6::Widgets)

qt_example_deploy(widgets_mvvm_dialog)
