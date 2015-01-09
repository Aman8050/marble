//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012 Anders Lund <anders@alweb.dk>
//

#include "OpenCachingComModel.h"
#include "OpenCachingComItem.h"

#include "MarbleGlobal.h"
#include "MarbleModel.h"
#include "GeoDataLatLonBox.h"
#include "MarbleDebug.h"

#include <QDebug>
#include <QString>
#include <QUrl>
#include <QScriptEngine>
#include <QScriptValue>

namespace Marble {

OpenCachingComModel::OpenCachingComModel( const MarbleModel *marbleModel, QObject *parent )
    : AbstractDataPluginModel( "opencachingcom", marbleModel, parent )
{
    // translate known values for tags and cache types
    // What would be a nice place for this??? Put here, since this object is only
    // constructed once.
    // note to translators: "cache" in this context means "geocache",
    // please refer to opencaching.com and wikipedia if in doubt.
    tr("Boat required");
    tr("Chirp");
    tr("Diving");
    tr("Exclusive");
    tr("Historic site");
    tr("Letterbox");
    tr("Night");
    tr("Tree climbing");
    tr("UV light");

    tr("Traditional Cache");
    tr("Multi-cache");
    tr("Virtual Cache");
    tr("Puzzle Cache");
    tr("Unknown Cache");
}

OpenCachingComModel::~OpenCachingComModel()
{
}

void OpenCachingComModel::getAdditionalItems( const GeoDataLatLonBox& box, qint32 number, const TileId& tileId )
{
    if( marbleModel()->planetId() != "earth" )
    {
        return;
    }

    QString url("http://www.opencaching.com/api/geocache/?Authorization=");
    url += AUTHKEY + QString("&bbox=%1,%2,%3,%4")
        .arg( box.south( GeoDataCoordinates::Degree ) )
        .arg( box.west(GeoDataCoordinates::Degree ) )
        .arg( box.north(GeoDataCoordinates::Degree ) )
        .arg( box.east(GeoDataCoordinates::Degree ) );
    url += "&limit=" + QString::number( number );
    // TODO Limit to user set tags/types/difficulty - when there is a config dialog...

//     qDebug()<<"Fetching more caches: "<<url;
    downloadDescriptionFile( QUrl( url ), tileId );
}

void OpenCachingComModel::parseFile( const QByteArray& file, const TileId &tileId )
{
    QScriptEngine engine;

    // Qt requires parentheses around json code
    QScriptValue data = engine.evaluate( '(' + QString::fromUtf8( file ) + ')' );
    QVariantList caches = data.toVariant().toList();

//     qDebug()<<"parsing "<<caches.size()<<" items";
    QList<AbstractDataPluginItem*> items;
    while (!caches.isEmpty())
    {
        QVariantMap map = caches.takeFirst().toMap();
        if ( !findItem( map["oxcode"].toString() ) )
        {
            OpenCachingComItem *item = new OpenCachingComItem( map, this );
            items << item;
        }
    }
    addItemsToList(items, tileId);
}

void OpenCachingComModel::fetchData(const QString& url, const QString &type, OpenCachingComItem *item)
{
    downloadItem(url, type, item);
}

const GeoDataCoordinates OpenCachingComModel::home() const
{
    qreal lon, lat;
    int zoom;
    marbleModel()->home( lon, lat, zoom );
    return GeoDataCoordinates(lon, lat, 0, GeoDataCoordinates::Degree);
}

} // namespace Marble

#include "OpenCachingComModel.moc"
