//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Thibaut Gridel <tgridel@free.fr>

#include "CachePlugin.h"
#include "CacheRunner.h"

namespace Marble
{

CachePlugin::CachePlugin( QObject *parent ) : RunnerPlugin( parent )
{
    setCapabilities( Parsing );
    setName( tr( "Cache File Parser" ) );
    setNameId( "Cache" );
    setDescription( tr( "Create GeoDataDocument from Cache Files" ) );
    setGuiString( tr( "Cache Parser" ) );
}

QString CachePlugin::version() const
{
    return "1.0";
}

QString CachePlugin::copyrightYears() const
{
    return "2011";
}

QList<PluginAuthor> CachePlugin::pluginAuthors() const
{
    return QList<PluginAuthor>()
            << PluginAuthor( "Thibaut Gridel", "tgridel@free.fr" );
}

MarbleAbstractRunner* CachePlugin::newRunner() const
{
    return new CacheRunner;
}

}

Q_EXPORT_PLUGIN2( CachePlugin, Marble::CachePlugin )

#include "CachePlugin.moc"
