// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import QtQuick 1.0
import com.nokia.meego 1.0

// FIXME delete; -> ConfigurationActivityPage.qml

PageStack {
    id: pageStack

    Component.onCompleted: {
        // FIXME check necessary?
        pageStack.push( Qt.createComponent( "SettingsListPage.qml" ) )
    }

    function back() {
        if( pageStack.depth == 1 ) {
            pageStack.visible = false
            activitySelection.visible = true
            activitySelection.activity = -1
        }
        else {
            pageStack.pop();
        }
    }
}