project( marble-mobile )

include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_SOURCE_DIR}/..
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
)
include(${QT_USE_FILE})

macro_optional_find_package(QtMobility COMPONENTS SystemInfo)
marble_set_package_properties( QtMobility PROPERTIES DESCRIPTION "a collection of APIs and frameworks" )
marble_set_package_properties( QtMobility PROPERTIES URL "http://qt.digia.com/" )
marble_set_package_properties( QtMobility PROPERTIES TYPE OPTIONAL PURPOSE "Required for QtLocation to work" )

set( marblemobile_SRCS
  main.cpp
  MobileMainWindow.cpp
  StackableWindow.cpp
  ../MarbleTest.cpp
)

add_definitions(-DMARBLE_MOBILE_VERSION_STRING="1.6.20")
IF(QTMOBILITY_SYSTEMINFO_INCLUDE_DIR)
    add_definitions(-DMARBLE_WITH_QTSYSTEMINFO)
    include_directories(${QTMOBILITY_INCLUDE_DIR})
    include_directories(${QTMOBILITY_SYSTEMINFO_INCLUDE_DIR})
ENDIF(QTMOBILITY_SYSTEMINFO_INCLUDE_DIR)

qt4_automoc( ${marblemobile_SRCS} )
add_executable( marble-mobile ${marblemobile_SRCS} )
target_link_libraries( marble-mobile ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTMAIN_LIBRARY} ${QTMOBILITY_SYSTEMINFO_LIBRARY} marblewidget )
install( TARGETS marble-mobile RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
