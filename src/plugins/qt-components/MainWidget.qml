// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Dennis Nienhüser <earthwings@gentoo.org>
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import Qt 4.7
import org.kde.edu.marble 0.11
import com.nokia.meego 1.0

/*
 * Main widget containing the map, models for routing, search, etc.
 */
Rectangle {
    id: screen

    // The widget representing the map.
    MarbleWidget {
        id: map
        anchors.fill: parent

        // Load settings.
        property bool autoCenter: settings.autoCenter
        property bool initialized: false
        mapThemeId: settings.mapTheme
        zoom: settings.quitZoom
        projection: settings.projection
        activeFloatItems: [ "compass", "scalebar", "progress" ]
        activeRenderPlugins: settings.activeRenderPlugins

        // The grouped property "tracking" provides access to tracking related
        // properties.
        tracking {
            // Connect the position source from below with the map.
            positionSource: positionProvider
            // Load settings.
            showPosition: settings.showPosition
            showTrack: settings.showTrack
            // We have our own position marker, the image of a ghost.
            // Marble will take care of positioning it correctly. It will
            // be hidden when there is no current position or it is not
            // visible on the screen
            positionMarker: marker
        }
        
        Component.onCompleted: {
            // Load last center of the map.
            center.longitude = settings.quitLongitude
            center.latitude = settings.quitLatitude
            routing.clearRoute()
            initialized = true
        }
        
        onZoomChanged: {
            // Update settings to new zoom level.
            settings.quitZoom = zoom
        }
        
        onVisibleLatLonAltBoxChanged: {
            if( initialized ) {
                // Update last center.
                settings.quitLongitude = center.longitude
                settings.quitLatitude = center.latitude
            }
        }
        
        // The grouped property "search" provides access to search related
        // properties.
        search {
            // Delegate of a search result.
            placemarkDelegate: 
                Image {
                    id: searchDelegate
                    source: "qrc:/placemark.svg"
                    width: 30
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    Rectangle {
                        id: routingOptions
                        visible: false
                        color: "white"
                        width: 350
                        height: nameLabel.height + routingButtons.height + 30
                        border.width: 1
                        border.color: "blue"
                        radius: 10
                        // Name of the search result.
                        Label {
                            id: nameLabel
                            text: name
                            width: 340
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.topMargin: 10
                            anchors.leftMargin: 10
                            anchors.rightMargin: 10
                            platformStyle: LabelStyle { fontPixelSize: 20 }
                        }
                        // Buttons "Directioins from here" and "Directions to here" for routing.
                        Column {
                            id: routingButtons
                            anchors.bottom: parent.bottom
                            anchors.horizontalCenter: parent.horizontalCenter
                            anchors.bottomMargin: 10
                            spacing: 5
                            Rectangle {
                                color: "white"
                                width: 230
                                height: 35
                                border.width: 1
                                border.color: "blue"
                                radius: 10
                                Label {
                                    text: "Directions from here"
                                    anchors.centerIn: parent
                                    platformStyle: LabelStyle { fontPixelSize: 20 }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        routingOptions.visible = false
                                        map.routing.setVia( 0, longitude, latitude )
                                    }
                                }
                            }
                            Rectangle {
                                color: "white"
                                width: 230
                                height: 35
                                border.width: 1
                                border.color: "blue"
                                radius: 10
                                Label {
                                    text: "Directions to here"
                                    anchors.centerIn: parent
                                    platformStyle: LabelStyle { fontPixelSize: 20 }
                                }
                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: {
                                        routingOptions.visible = false
                                        map.routing.setVia( 1, longitude, latitude )
                                    }
                                }
                            }
                        }
                        // Small cross that closes the search result info if its clicked.
                        Image {
                            id: closeImage
                            width: 30
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            source: "image://theme/icon-m-toolbar-close"
                            anchors.top: parent.top
                            anchors.right: parent.right
                            anchors.margins: 5
                            MouseArea {
                                anchors.fill: parent
                                onClicked: {
                                    routingOptions.visible = false
                                }
                            }
                        }
                    }
                    // Show search result info if the placemark is clicked.
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            routingOptions.visible = !routingOptions.visible
                        }
                    }
                }
        }
        
        // Centers map on passed coordinates.
        function centerOn( lon, lat ) {
            map.center.longitude = lon
            map.center.latitude = lat
        }
        
        // Returns the grouped property to access routing functions.
        function getRouting() {
            return map.routing
        }
        
        // Returns the grouped property to access search functions.
        function getSearch() {
            return map.search
        }

    }
    
    // Delivers the current (gps) position.
    PositionSource {
        id: positionProvider

        // Can optionally be used to select a specific position provider
        // plugin of marble. Per default the first one is used.
        // The value is the nameId() of an installed Marble PositionProviderPlugin,
        // e.g. Gpsd
        //source: "QtMobilityPositionProviderPlugin"

        // This starts/stops gps tracking.
        active: settings.gpsTracking

        // Start a small grow/shrink animation of the marker to indicate position updates.
        onPositionChanged: {
            growAnimation.running = true
            if ( map.autoCenter ) {
                map.center = positionProvider.position
            }
        }
    }
    
    // A marker that indicates the current position.
    Image {
        id: marker
        width: 60
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/marker.svg"
        visible: false

        // Animation that grows/shrinks the marker.
        PropertyAnimation on x { duration: 300; easing.type: Easing.OutBounce }
        PropertyAnimation on y { duration: 300; easing.type: Easing.OutBounce }
        SequentialAnimation {
            id: growAnimation
            PropertyAnimation {
                target: marker
                properties: "scale"
                to: 1.2
                duration: 150
            }
            PropertyAnimation {
                target: marker
                properties: "scale"
                to: 1.0
                duration: 150
            }
        }
    }
    
    // Starts a search for the passed term.
    function find( term ) {
        map.search.find( term )
    }
    
    // Returns the model that contains routing instructions.
    function routeRequestModel() {
        return map.routing.routeRequestModel()
    }
    
    // Returns the model that contains points on the route.
    function waypointModel() {
        return map.routing.waypointModel()
    }
    
    // Returns the grouped property to access routing functions.
    function getRouting() {
        return map.getRouting()
    }
    
    // Returns the grouped property to access search functions.
    function getSearch() {
        return map.getSearch()
    }

}
