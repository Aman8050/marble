// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import org.kde.edu.marble.qtcomponents 0.12
import org.kde.edu.marble 0.11
import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
    id: main
    width: 800
    height: 480

    MarbleSettings {
        id: settings
    }
    
    ActivitySelectionView {
        id: activitySelection
        anchors.fill: parent
    }

    SearchBar {
        id: searchBar
        visible: false
        anchors.top: parent.top
        anchors.left: mainWidget.left
        anchors.right: parent.right
        height: 35
        property bool activated: false
        Keys.onPressed: {
            if( event.key == Qt.Key_Return || event.key == Qt.Key_Enter ) {
                console.log( "search triggered: ", text )
                mainWidget.find( text )
            }
        }
    }

    MainWidget {
        id: mainWidget
        visible: false
        anchors.top: parent.top
        anchors.bottom: mainToolBar.top
        anchors.left: routingDialog.right
        anchors.right: parent.right
    }
    
    SettingsPage {
        id: settingsPage
        visible: false
        anchors.top: parent.top
        anchors.bottom: mainToolBar.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    
    RouteRequestView {
        id: routeRequestView
        visible: false
        anchors.top: parent.top
        anchors.bottom: mainToolBar.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    
    WaypointView {
        id: waypointView
        visible: false
        anchors.top: parent.top
        anchors.bottom: mainToolBar.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
    
    RoutingDialog {
        id: routingDialog
        visible: false
        anchors.top: main.top
        anchors.left: main.left
        anchors.bottom: mainToolBar.top
        width: visible ? 240 : 0
    }
    
    Rectangle {
        color: "grey"
        opacity: 0.5
        visible: !searchBar.visible && mainWidget.visible
        anchors.top: routingDialog.top
        anchors.left: routingDialog.right
        width: routingButton.width
        height: routingButton.height
        radius: 5
        Image {
            id: routingButton
            opacity: 1.0
            source: "image://theme/icon-m-common-drilldown-arrow"
            smooth: true
            transform: Rotation { origin.x: 16; origin.y: 16; angle: routingDialog.visible ? 180 : 0 }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    routingDialog.visible = !routingDialog.visible
                }
            }
        }
    }

    ToolBar {
        id: mainToolBar
        visible: false
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        tools: ToolBarLayout {
            id: toolBarLayout
            anchors.fill: parent
            // FIXME icons
            Row {
                ToolIcon {
                    id: backButton
                    iconId: "toolbar-back"
                    onClicked: { settingsPage.back() }
                }
                ToolIcon {
                    id: waypointButton
                    iconId: "common-location-picker"
                    onClicked: { waypointView.visible = !waypointView.visible }
                }
                ToolIcon {
                    id: routeRequestButton
                    iconId: "toolbar-callhistory"
                    onClicked: { routeRequestView.visible = !routeRequestView.visible }
                }
                ToolIcon {
                    id: virtualGlobeButton
                    iconId: "ovi-service-maps"
                    onClicked: { main.state = "Virtual Globe" }
                }
                ToolIcon {
                    id: driveButton
                    iconId: "content-automobile"
                    visible: !settingsPage.visible
                    onClicked: { main.state = "Drive" }
                }
                ToolIcon {
                    id: cycleButton
                    iconId: "common-clock"
                    onClicked: { main.state = "Cycle" }
                }
                ToolIcon {
                    id: walkButton
                    iconId: "camera-scene-sports-screen"
                    onClicked: { main.state = "Walk" }
                }
                ToolIcon {
                    id: guidanceButton
                    iconId: "telephony-content-sms-dimmed"
                    onClicked: { main.state = "Guidance" }
                }
                ToolIcon {
                    id: bookmarksButton
                    iconId: "content-bookmark"
                    onClicked: { main.state = "Bookmarks" }
                }
                ToolIcon {
                    id: aroundMeButton
                    iconId: "transfer-sync"
                    onClicked: { main.state = "Around Me" }
                }
                ToolIcon {
                    id: weatherButton
                    iconId: "weather-sunny-thunder"
                    onClicked: { main.state = "Weather" }
                }
                ToolIcon {
                    id: trackingButton
                    iconId: "content-feed"
                    onClicked: { main.state = "Tracking" }
                }
                ToolIcon {
                    id: geocachingButton
                    iconId: "email-combined-mailbox"
                    onClicked: { main.state = "Geocaching" }
                }
                ToolIcon {
                    id: friendsButton
                    iconId: "conversation-group-chat"
                    onClicked: { main.state = "Friends" }
                }
                ToolIcon {
                    id: downloadButton
                    iconId: "transfer-download"
                    onClicked: { main.state = "Download" }
                }
                ToolIcon {
                    id: searchButton
                    iconId: "toolbar-search"
                    onClicked: { main.state = "Search" }
                }
                ToolIcon {
                    id: wikipediaButton
                    iconId: "content-wikipedia"
                    onClicked: { main.togglePlugin( "wikipedia" ) }
                }
                ToolIcon {
                    id: photoButton
                    iconId: "content-photoalbum"
                    onClicked: { main.togglePlugin( "photo" ) }
                }
                ToolIcon {
                    id: settingsButton
                    iconId: "toolbar-settings"
                    onClicked: { main.state = "Configuration" }
                }
                ToolIcon {
                    id: activityButton
                    iconId: "toolbar-view-menu-dimmed-white"
                    onClicked: { activitySelection.visible = true; activitySelection.activity = -1 }
                }
            }
        }
        states: [
            State {
                name: "activitySelection"
                when: main.state == name
            },
            State {
                name: "Virtual Globe"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: false }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: true }
                PropertyChanges { target: wikipediaButton; visible: true }
                PropertyChanges { target: photoButton; visible: true }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Drive"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: true }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Cycle"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: true }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Walk"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: true }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Guidance"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: true }
                PropertyChanges { target: cycleButton; visible: true }
                PropertyChanges { target: walkButton; visible: true }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Search"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: true }
                PropertyChanges { target: aroundMeButton; visible: true }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: true }
                PropertyChanges { target: photoButton; visible: true }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Bookmarks"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: true }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: true }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Around Me"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: true }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: true }
                PropertyChanges { target: wikipediaButton; visible: true }
                PropertyChanges { target: photoButton; visible: true }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Weather"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Tracking"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Geocaching"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: true }
                PropertyChanges { target: walkButton; visible: true }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: true }
                PropertyChanges { target: photoButton; visible: true }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Friends"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Download"
                when: main.state == name
                PropertyChanges { target: backButton; visible: false }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: true }
                PropertyChanges { target: activityButton; visible: true }
            },
            State {
                name: "Configuration"
                when: main.state == name
                PropertyChanges { target: backButton; visible: true }
                PropertyChanges { target: waypointButton; visible: false }
                PropertyChanges { target: routeRequestButton; visible: false }
                PropertyChanges { target: virtualGlobeButton; visible: true }
                PropertyChanges { target: driveButton; visible: false }
                PropertyChanges { target: cycleButton; visible: false }
                PropertyChanges { target: walkButton; visible: false }
                PropertyChanges { target: guidanceButton; visible: false }
                PropertyChanges { target: bookmarksButton; visible: false }
                PropertyChanges { target: aroundMeButton; visible: false }
                PropertyChanges { target: weatherButton; visible: false }
                PropertyChanges { target: trackingButton; visible: false }
                PropertyChanges { target: geocachingButton; visible: false }
                PropertyChanges { target: friendsButton; visible: false }
                PropertyChanges { target: downloadButton; visible: false }
                PropertyChanges { target: searchButton; visible: false }
                PropertyChanges { target: wikipediaButton; visible: false }
                PropertyChanges { target: photoButton; visible: false }
                PropertyChanges { target: settingsButton; visible: false }
                PropertyChanges { target: activityButton; visible: true }
            }
        ]
    }

    // see http://techbase.kde.org/Projects/Marble/Devices_and_Use_Cases
    states: [
        State {
            name: "activitySelection"
            when: activitySelection.visible && activitySelection.activity == -1
            PropertyChanges { target: mainWidget; visible: false }
            PropertyChanges { target: searchBar;  visible: false }
            PropertyChanges { target: settingsPage; visible: false }
            PropertyChanges { target: waypointView;  visible: false }
            PropertyChanges { target: routeRequestView; visible: false }
            PropertyChanges { target: routingDialog;  visible: false }
            PropertyChanges { target: mainToolBar; visible: false }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Virtual Globe"
            when: activitySelection.activity == 0
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; projection: "Spherical" }
            PropertyChanges { target: settings; mapTheme: "earth/bluemarble/bluemarble.dgml" }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Drive"
            when: activitySelection.activity == 1
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: routingDialog; visible: true }
            PropertyChanges { target: settings; projection: "Mercator" }
            PropertyChanges { target: settings; mapTheme: "earth/openstreetmap/openstreetmap.dgml" }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Cycle"
            when: activitySelection.activity == 2
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: routingDialog; visible: true }
            PropertyChanges { target: settings; projection: "Mercator" }
            PropertyChanges { target: settings; mapTheme: "earth/openstreetmap/openstreetmap.dgml" }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Walk"
            when: activitySelection.activity == 3
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: routingDialog; visible: true }
            PropertyChanges { target: settings; projection: "Mercator" }
            PropertyChanges { target: settings; mapTheme: "earth/openstreetmap/openstreetmap.dgml" }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Guidance"
            when: activitySelection.activity == 4
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Search"
            when: activitySelection.activity == 5
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainWidget; anchors.top: searchBar.bottom }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: searchBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Bookmarks"
            when: activitySelection.activity == 6
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Around Me"
            when: activitySelection.activity == 7
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Weather"
            when: activitySelection.activity == 8
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; projection: "Spherical" }
            PropertyChanges { target: settings; mapTheme: "earth/bluemarble/bluemarble.dgml" }
            StateChangeScript { script: enablePlugin( "weather" ) }
        },
        State {
            name: "Tracking"
            when: activitySelection.activity == 9
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; projection: "Mercator" }
            PropertyChanges { target: settings; mapTheme: "earth/openstreetmap/openstreetmap.dgml" }
            PropertyChanges { target: settings; gpsTracking: true }
            PropertyChanges { target: settings; showPosition: true }
            PropertyChanges { target: settings; showTrack: true }
            PropertyChanges { target: settings; autoCenter: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Geocaching"
            when: activitySelection.activity == 10
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainWidget; visible: true }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; projection: "Mercator" }
            PropertyChanges { target: settings; mapTheme: "earth/openstreetmap/openstreetmap.dgml" }
            StateChangeScript { script: enablePlugin( "opencaching" ) }
        },
        State {
            name: "Friends"
            when: activitySelection.activity == 11
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Download"
            when: activitySelection.activity == 12
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        },
        State {
            name: "Configuration"
            when: activitySelection.activity == 13
            PropertyChanges { target: activitySelection; visible: false }
            PropertyChanges { target: mainToolBar; visible: true }
            PropertyChanges { target: mainWidget; visible: false }
            PropertyChanges { target: settingsPage; visible: true }
            PropertyChanges { target: settings; activeRenderPlugins: settings.defaultRenderPlugins }
        }
    ]

    function routeRequestModel() {
        return mainWidget.routeRequestModel()
    }
    
    function waypointModel() {
        return mainWidget.waypointModel()
    }
    
    function getRouting() {
        return mainWidget.getRouting()
    }

    function getSearch() {
        return mainWidget.getSearch()
    }
    
    // FIXME only enable default+passed?
    function enablePlugin( name ) {
        var tmp = settings.defaultRenderPlugins
        if( tmp.indexOf( name ) == -1 ) {
            tmp.push( name )
            settings.activeRenderPlugins = tmp
        }
    }
    
    function disablePlugin( name ) {
        var tmp = new Array()
        for( var i = 0; i < settings.activeRenderPlugins.length; i++ ) {
            if( settings.activeRenderPlugins[i] != name ) {
                tmp.push( settings.activeRenderPlugins[i] )
            }
        }
        settings.activeRenderPlugins = tmp
    }
    
    function togglePlugin( name ) {
        if( settings.activeRenderPlugins.indexOf( name ) == -1 ) {
            enablePlugin( name )
        }
        else {
            disablePlugin( name )
        }
    }

}