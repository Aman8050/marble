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

Rectangle {
    id: screen

    // The map widget
    MarbleWidget {
        id: map
        anchors.fill: parent

        property bool autoCenter: settings.autoCenter

        mapThemeId: settings.mapTheme
        zoom: settings.quitZoom
        activeFloatItems: [ "compass", "scalebar", "progress" ]

        // The grouped property tracking provides access to tracking related
        // properties
        tracking {
            // We connect the position source from above with the map
            positionSource: positionProvider
            showPosition: settings.showPosition
            showTrack: settings.showTrack
            // We have our own position marker, the image of a ghost.
            // Marble will take care of positioning it correctly. It will
            // be hidden when there is no current position or it is not
            // visible on the screen
            positionMarker: marker
        }
        
        Component.onCompleted: {
            console.log( "assigning: ", settings.quitLongitude, settings.quitLatitude )
            center.longitude = settings.quitLongitude
            center.latitude = settings.quitLatitude
        }
        
        onZoomChanged: {
            settings.quitZoom = zoom
        }
        
        onVisibleLatLonAltBoxChanged: {
            console.log( "center changed: ", center.longitude, center.latitude )
            settings.quitLongitude = center.longitude
            settings.quitLatitude = center.latitude
        }
        
        // FIXME delegate
        search {
            placemarkDelegate: Rectangle { color: "orange"; width: 10; height: 10 }
        }

    }
    
    // Delivers the current (gps) position
    PositionSource {
        id: positionProvider

        // Can optionally be used to select a specific position provider
        // plugin of marble. Per default the first one is used.
        // The value is the nameId() of an installed Marble PositionProviderPlugin,
        // e.g. Gpsd
        //source: "QtMobilityPositionProviderPlugin"

        // This starts/stops gps tracking
        active: settings.gpsTracking

        // A small grow/shrink animation of the ghost to indicate position updates
        onPositionChanged: {
            growAnimation.running = true
            if ( map.autoCenter ) {
                map.center = positionProvider.position
            }
        }
    }
    
    // A marker indicates the current position
    Image {
        id: marker
        width: 60
        fillMode: Image.PreserveAspectFit
        smooth: true
        source: "qrc:/marker.svg"
        visible: false

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
    
    function find( term ) {
        map.search.find( term )
    }

}
