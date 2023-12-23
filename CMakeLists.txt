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
    files/scripts/MainMenu.js
    files/html/Profile.html
    pizza.h pizza.cpp

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
    files/css/StylesPizza.css files/html/401.html files/html/About.html files/html/Home.html
    files/html/laba_4.html files/html/laba_4_2.html files/html/laba_5.html files/html/laba_5_si.html
    files/html/laba_5_su.html files/html/Login.html files/html/MainMenu.html files/html/Order.html
    files/html/Register.html files/photos/BackgrShape.png files/photos/BackgrShapeC.png files/photos/BackgrShapeR.png
    files/photos/Cart.png files/photos/DefaultPizza.png files/photos/Facebook.png
    files/photos/fish.png files/photos/Instagram.png files/photos/Phone.png files/photos/PizzaBackgr.png
    files/scripts/MainMenu.js   files/html/Profile.html  files/photos/diabolo.png files/photos/four_cheese.png
    files/photos/margh.png files/photos/pepperoni.png files/photos/veggie.png
)
