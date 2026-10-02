qt_add_executable(network_access WIN32
    src/main.cpp
    src/JsonPlaceholderClient.cpp src/JsonPlaceholderClient.h
    src/MainWindow.cpp src/MainWindow.h
    src/NewPostDialog.cpp src/NewPostDialog.h
    src/Post.h
    src/PostListModel.cpp src/PostListModel.h
)

target_link_libraries(network_access PRIVATE Qt6::Widgets Qt6::Network)

qt_example_deploy(network_access)
