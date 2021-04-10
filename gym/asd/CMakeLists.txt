cmake_minimum_required (VERSION 3.6)

project(coursework)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#Путь до папки с библиотеками, если не установлены переменные среды
#set(CMAKE_PREFIX_PATH "D:/Qt/5.15.2/msvc2019_64/")
include_directories(${CMAKE_CURRENT_BINARY_DIR})
add_executable (
coursework 
	"addserv.cpp"  
	"addserv.h" 
	"addtariff.cpp"  
	"addtariff.h" 
	"addwindow.cpp" 
	"addwindow.h" 
	"database.cpp" 
	"database.h" 
	"editwindow.h" 
	"editwindow.cpp"    
	"mainwindow.h" 
	"mainwindow.cpp" 
	"main.cpp" 
	"services.h" 
	"services.cpp"
)
find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets Core Gui Sql REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets Core Gui Sql REQUIRED)

target_link_libraries(coursework PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::Core Qt${QT_VERSION_MAJOR}::Gui Qt${QT_VERSION_MAJOR}::Sql)
install(TARGETS coursework DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Core> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Gui> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Widgets> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::Sql> DESTINATION bin)
install(FILES $<TARGET_FILE:Qt5::QSQLiteDriverPlugin> DESTINATION bin/sqldrivers)
install(FILES $<TARGET_FILE:Qt5::QWindowsIntegrationPlugin> DESTINATION bin/platforms)
install(FILES $<TARGET_FILE:Qt5::QWindowsDirect2DIntegrationPlugin> DESTINATION bin/platforms)
