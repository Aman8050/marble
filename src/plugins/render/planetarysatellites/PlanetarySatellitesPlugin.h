//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012 Rene Kuettner <rene@bitkanal.net>
//

#ifndef MARBLE_NONEARTHSATELLITESPLUGIN_H
#define MARBLE_NONEARTHSATELLITESPLUGIN_H

#include "RenderPlugin.h"

#include <QtCore/QObject>

class QCheckBox;

namespace Ui
{
}

namespace Marble
{

/**
 * @brief This plugin displays planetary satellites, space probes
 *        and their orbits.
 */
class PlanetarySatellitesPlugin : public RenderPlugin
{
    Q_OBJECT
    Q_INTERFACES( Marble::RenderPluginInterface )
    MARBLE_PLUGIN( PlanetarySatellitesPlugin )

public:
    PlanetarySatellitesPlugin();
    explicit PlanetarySatellitesPlugin( const MarbleModel *marbleModel );
    virtual ~PlanetarySatellitesPlugin();
    
    QStringList backendTypes() const;
    QString renderPolicy() const;
    QStringList renderPosition() const;
    QString name() const;
    QString nameId() const;
    QString guiString() const;
    QString version() const;
    QString description() const;
    QString copyrightYears() const;
    QList<PluginAuthor> pluginAuthors() const;
    QString aboutDataText() const;
    QIcon icon() const;
    RenderType renderType() const;
    void initialize();
    bool isInitialized() const;

    bool render( GeoPainter *painter, ViewportParams *viewport, const QString &renderPos, GeoSceneLayer *layer );

private Q_SLOTS:
    void enableModel( bool enabled );
    void visibleModel( bool visible );

private:
    bool m_isInitialized;
};

}

#endif // MARBLE_NONEARTHSATELLITESPLUGIN_H
