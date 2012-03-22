//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Most of the code is taken from MarbleRenderPluginInterface.h
// by Torsten Rahn and Inge Wallin.
//
// Copyright 2009 Jens-Michael Hoffmann <jensmh@gmx.de>
// Copyright 2012 Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//
#ifndef MARBLE_PLUGININTERFACE_H
#define MARBLE_PLUGININTERFACE_H

#include <QtCore/QList>
#include <QtCore/QObject> // for QObject::tr()
#include <QtCore/QString>
#include <QtCore/QtPlugin>
#include <QtGui/QIcon>

#include "marble_export.h"

namespace Marble
{

struct MARBLE_EXPORT PluginAuthor
{
    PluginAuthor( const QString &name_, const QString &email_, const QString &task_ = QObject::tr( "Developer" ) ) :
        name( name_ ),
        task( task_ ),
        email( email_ )
    {}

    QString name;
    QString task;
    QString email;
};

/**
 * @short This class specifies interface of a Marble plugin.
 */

class MARBLE_EXPORT PluginInterface
{
 public:
    virtual ~PluginInterface();

    /**
     * @brief Returns the "real name" of the backend.
     *.
     * Example: "Starry Sky Plugin"
     */
    virtual QString name() const = 0;

    /**
     * @brief Returns the string that should appear in the UI / in the menu.
     *.
     * Using a "&" you can suggest key shortcuts
     *
     * Example: "&Stars"
     */
    virtual QString guiString() const = 0;

    /**
     * @brief Returns the name ID of the backend.
     *.
     * Examples: "starrysky", "QNetworkAccessManager"
     */
    virtual QString nameId() const = 0;

    virtual QString version() const = 0;

    /**
     * @brief Returns a user description of the plugin.
     */
    virtual QString description() const = 0;

    /**
     * @brief Returns an icon for the plugin.
     */
    virtual QIcon icon() const = 0;

    virtual QString copyrightYears() const = 0;

    virtual QList<PluginAuthor> pluginAuthors() const = 0;

    /**
     * @brief Returns about text (credits) for external data the plugin uses.
     *
     * The default implementation returns the empty string. Please override
     * this method to give credits for all data from 3rd-partys.
     */
    virtual QString aboutDataText() const;

    virtual void initialize() = 0;

    virtual bool isInitialized() const = 0;
};

}

Q_DECLARE_INTERFACE( Marble::PluginInterface, "org.kde.Marble.PluginInterface/1.1" )

#endif
