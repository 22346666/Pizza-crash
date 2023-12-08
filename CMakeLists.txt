cmake_minimum_required(VERSION 3.16)
project(Server LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS HttpServer Sql)

qt_standard_project_setup()

qt_add_executable(Server
    main.cpp
    http_server.h
    http_server.cpp
    database.h database.cpp
    user.h user.cpp
    cookie.h cookie.cpp

)

target_link_libraries(Server PRIVATE
    Qt6::HttpServer
    Qt6::Sql
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
        assets/fullchain.pem
        assets/privkey.pem
)

qt_add_resources(Server "resource"
    PREFIX
        "/"
    FILES
        files/css/laba_4.css files/html/home.html files/html/laba_4.html files/html/laba_4_2.html files/html/laba_5_si.html files/html/laba_5_su.html files/html/laba_5.html
)
