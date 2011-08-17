// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Daniel Marth <danielmarth@gmx.at>

import QtQuick 1.0
import com.nokia.meego 1.0

/*
 * A textfield for searching locations.
 */
TextField {
    id: textField
    placeholderText: "Search..."
    // Icon to clear text in the textfield.
    ToolIcon {
        id: clearButton
        iconId: "input-clear"
        anchors.top: textField.top
        anchors.right: textField.right
        anchors.rightMargin: 5
        height: parent.height - 2
        width: parent.height - 2
        // Reset text and clear search.
        onClicked: {
            textField.text = ""
            mainWidget.find( text )
        }
    }
}