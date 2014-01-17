PROJECT(placemarks)

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
