// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import org.kde.edu.marble.qtcomponents 0.12
import org.kde.edu.marble 0.11
import QtQuick 1.0
import com.nokia.meego 1.0

/* 
 * Main window of the application. Also contains activity properties,
 * settings and manages plugin states on activity changes.
 */
PageStackWindow {
    id: main
    // Use full screen.
    // FIXME Fix for desktop screens.
    width: screen.displayWidth
    height: screen.displayHeight
    platformStyle: defaultStyle
    initialPage: activitySelection
    property alias activityModel: activitySelection.model
    property alias marbleWidget: mainWidget
    
    // System dependent style for the main window.
    PageStackWindowStyle {
        id: defaultStyle
    }

    // Stores the settings of the application.
    MarbleSettings {
        id: settings
    }
    
    // Default toolbar layout for pages if there is no other defined.
    ToolBarLayout {
        id: commonToolBar
        visible: false
        // ToolIcon { iconId: "toolbar-back"; onClicked: { pageStack.pop() } }
    }
    
    // Displays all available activities and starts them if the user clicks on them.
    ActivitySelectionView {
        id: activitySelection
        onActivityChanged: {
            console.log( "onActivityChanged", oldActivity, newActivity )
            main.changeActivity( oldActivity, newActivity )
        }
    }

    MainWidget {
        id: mainWidget
    }

    // Returns the model which contains routing instructions.
    function routeRequestModel() {
        return mainWidget.routeRequestModel()
    }
    
    // Returns the model which contains points on the route.
    function waypointModel() {
        return mainWidget.waypointModel()
    }
    
    // Returns object to define and clear route.
    function getRouting() {
        return mainWidget.getRouting()
    }

    // Returns object to define and clear searches.
    function getSearch() {
        return mainWidget.getSearch()
    }
}
