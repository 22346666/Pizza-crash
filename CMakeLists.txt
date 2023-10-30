cmake_minimum_required(VERSION 3.16)
project(Server LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS HttpServer)

qt_standard_project_setup()

qt_add_executable(Server
    main.cpp
    http_server.h
    http_server.cpp
)

target_link_libraries(Server PRIVATE
    Qt6::HttpServer
)

install(TARGETS Server
    RUNTIME DESTINATION "${INSTALL_EXAMPLEDIR}"
    BUNDLE DESTINATION "${INSTALL_EXAMPLEDIR}"
    LIBRARY DESTINATION "${INSTALL_EXAMPLEDIR}"
)

qt_add_resources(Server "assets"
    PREFIX
        "/"
    FILES
        assets/server_certificate.crt
        assets/server.key

)
