PROJECT(placemarks)

SET (TARGET asc2kml)

include_directories(
 ${CMAKE_CURRENT_SOURCE_DIR}
 ${CMAKE_CURRENT_BINARY_DIR}
 ${QT_INCLUDE_DIR}
)
if( QT4_FOUND )
  include( ${QT_USE_FILE} )
endif()

set( ${TARGET}_SRC asc2kml.cpp )
add_executable( ${TARGET} ${${TARGET}_SRC} )

add_custom_command(OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/cityplacemarks.kml"
  COMMAND sort
  ARGS -t "$(echo -e "\t")" -k 2 cities.txt > "${CMAKE_CURRENT_BINARY_DIR}/cities_sorted.txt"
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  COMMAND ${TARGET}
  ARGS -o "${CMAKE_CURRENT_BINARY_DIR}/cityplacemarks.kml" "${CMAKE_CURRENT_BINARY_DIR}/cities_sorted.txt" statecodes.txt timeZones.txt
  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
  DEPENDS ${TARGET} cities.txt statecodes.txt timeZones.txt
)

ADD_CUSTOM_TARGET(cityplacemarks ALL
  DEPENDS "${CMAKE_CURRENT_BINARY_DIR}/cityplacemarks.kml"
)

if (QT4_FOUND)
  target_link_libraries( ${TARGET} ${QT_QTCORE_LIBRARY} ${QT_QTMAIN_LIBRARY} )
else()
  target_link_libraries( ${TARGET} ${Qt5Core_LIBRARIES} )
endif()

install (FILES
elevplacemarks.cache
otherplacemarks.cache
baseplacemarks.cache
moonlandingsites.cache
boundaryplacemarks.cache
moonterrain.cache
DESTINATION ${MARBLE_DATA_INSTALL_PATH}/placemarks)

if(MOBILE)
    install (FILES cityplacemarks_large_population.cache DESTINATION ${MARBLE_DATA_INSTALL_PATH}/placemarks RENAME cityplacemarks.cache)
else(MOBILE)
    install (FILES cityplacemarks.cache DESTINATION ${MARBLE_DATA_INSTALL_PATH}/placemarks)
endif(MOBILE)
