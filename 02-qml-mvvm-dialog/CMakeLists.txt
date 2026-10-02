qt_add_executable(qml_mvvm_dialog WIN32
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/PersonDialog.cpp src/PersonDialog.h
)

qt_add_qml_module(qml_mvvm_dialog
    URI MvvmQml
    QML_FILES qml/PersonForm.qml
    SOURCES
        src/Person.h
        src/PersonViewModel.cpp src/PersonViewModel.h
)

target_include_directories(qml_mvvm_dialog PRIVATE src)
target_link_libraries(qml_mvvm_dialog PRIVATE Qt6::Widgets Qt6::QuickWidgets Qt6::QuickControls2)

qt_example_deploy(qml_mvvm_dialog QML_DIR "${CMAKE_CURRENT_SOURCE_DIR}/qml")
