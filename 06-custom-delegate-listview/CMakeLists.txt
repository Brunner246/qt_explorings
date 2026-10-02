qt_add_executable(custom_delegate_listview WIN32
    src/main.cpp
    src/Contact.h
    src/ContactDelegate.cpp src/ContactDelegate.h
    src/ContactListModel.cpp src/ContactListModel.h
)

target_link_libraries(custom_delegate_listview PRIVATE Qt6::Widgets)

qt_example_deploy(custom_delegate_listview)
