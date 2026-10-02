qt_add_executable(tree_view WIN32
    src/main.cpp
    src/MainWindow.cpp src/MainWindow.h
    src/ProjectTreeModel.cpp src/ProjectTreeModel.h
    src/SampleProject.cpp src/SampleProject.h
    src/TreeNode.cpp src/TreeNode.h
)

target_link_libraries(tree_view PRIVATE Qt6::Widgets)

qt_example_deploy(tree_view)
