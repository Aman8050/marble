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

Page {
    tools: commonToolBar

    ListView {
        id: mapListView
        anchors.fill: parent
        anchors.margins: UiConstants.DefaultMargin
        highlightFollowsCurrentItem: true
        model: themes.mapThemes()
        currentIndex: themes.mapThemes().indexOf( settings.mapTheme )
        
        MapThemeManager {
            id: themes
        }

        delegate:
            Rectangle {
                id: themeItem
                width: mapListView.width
                height: mapImage.height + 20
                color: model.modelData.id == settings.mapTheme ? "lightsteelblue" : "white"
                Row {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 15
                    Image {
                        id: mapImage
                        source: "image://maptheme/" + model.modelData.id
                    }
                    Label {
                        id: themeLabel
                        text: model.modelData.name
                        anchors.verticalCenter: parent.verticalCenter
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        mapListView.currentIndex = index
                        settings.mapTheme = themes.mapThemes()[index].id
                    }
                }
            }

    }
}