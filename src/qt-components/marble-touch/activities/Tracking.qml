// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Dennis Nienhüser <earthwings@gentoo.org>
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import QtQuick 1.0
import com.nokia.meego 1.0
import org.kde.edu.marble 0.11
import org.kde.edu.marble.qtcomponents 0.12

/*
 * Page for geocaching activity.
 */
Page {
    id: trackingActivityPage
    anchors.fill: parent

    tools: ToolBarLayout {
        ToolIcon {
            iconId: "toolbar-back";
            onClicked: pageStack.pop()
        }
        ToolIcon {
            iconId: "toolbar-home"
            onClicked: marbleWidget.centerOn( marbleWidget.getTracking().lastKnownPosition.longitude, marbleWidget.getTracking().lastKnownPosition.latitude )
        }
        ToolIconCheckable {
            id: searchButton
            iconId: "toolbar-search";
        }
        ToolIcon {
            iconId: "toolbar-view-menu"
            onClicked: pageMenu.open()
        }
    }

    Menu {
        id: pageMenu
        content: MenuLayout {
            MenuItem {
                text: "Map Theme"
                onClicked: {
                    pageStack.push( "qrc:/MapThemeSelectionPage.qml" )
                }
            }
            MenuItem {
                text: "Save Track"
                onClicked: {
                    pageStack.push( "qrc:/SaveTrackPage.qml" )
                }
            }
            MenuItemSwitch {
                text: "Online"
                checked: !settings.workOffline
                onClicked: settings.workOffline = !settings.workOffline
            }
            MenuItemSwitch {
                text: "Auto Center"
                checked: false
                onCheckedChanged: {
                    marbleWidget.getTracking().autoCenter = checked
                }
            }
            MenuItemSwitch {
                text: "Auto Zoom"
                checked: false
                onCheckedChanged: {
                    marbleWidget.getTracking().autoZoom = checked
                }
            }
        }
    }

    Column {
        width: parent.width
        height: parent.height

        SearchField {
            id: searchField
            visible: searchButton.checked
            width: parent.width
            onSearch: marbleWidget.find( term )
        }

        Item {
            id: mapContainer
            width: parent.width
            height: parent.height - searchField.height
            clip: true

            Component.onCompleted: {
                marbleWidget.parent = mapContainer
                settings.projection = "Mercator"
                var plugins = settings.defaultRenderPlugins
                settings.removeElementsFromArray(plugins, ["coordinate-grid", "sun", "stars", "compass"])
                plugins.push( "speedometer" )
                settings.activeRenderPlugins =  plugins
                settings.mapTheme = "earth/openstreetmap/openstreetmap.dgml"
                settings.gpsTracking = true
                settings.showPosition = true
                settings.showTrack = true
                marbleWidget.visible = true
            }

            Component.onDestruction: {
                marbleWidget.parent = null
                marbleWidget.visible = false
            }
        }
    }
}
