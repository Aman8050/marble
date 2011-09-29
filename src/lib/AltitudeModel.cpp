//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Niko Sams <niko.sams@gmail.com>
//


#include "AltitudeModel.h"
#include "TileLoader.h"
#include "MarbleDebug.h"
#include "MapThemeManager.h"
#include <GeoSceneHead.h>
#include <GeoSceneMap.h>
#include <GeoSceneDocument.h>
#include <GeoSceneTexture.h>
#include "TextureTile.h"
#include <QLabel>
#include "TileLoaderHelper.h"
#include "MarbleModel.h"
#include <qmath.h>

namespace Marble {

AltitudeModel::AltitudeModel( MarbleModel *const model )
    : QObject(0), m_model(model)
{
    m_cache.setMaxCost(10); //keep 10 tiles in memory (~17MB)
    m_tileLoader = new TileLoader( model->downloadManager(), model->mapThemeManager() );
    updateTextureLayers();
    connect( m_tileLoader, SIGNAL( tileCompleted( TileId, QImage ) ),
             SLOT( tileCompleted( TileId, QImage ) ) );
}

void AltitudeModel::updateTextureLayers()
{
    m_textureLayer = 0;

    const GeoSceneDocument *srtmTheme = MapThemeManager::loadMapTheme( "earth/srtm2/srtm2.dgml" );
    Q_ASSERT( srtmTheme );

    const GeoSceneHead *head = srtmTheme->head();
    Q_ASSERT( head );

    const GeoSceneMap *map = srtmTheme->map();
    Q_ASSERT( map );

    const GeoSceneLayer *sceneLayer = map->layer( head->theme() );
    Q_ASSERT( sceneLayer );

    m_textureLayer = dynamic_cast<GeoSceneTexture*>( sceneLayer->datasets().first() );
    Q_ASSERT( m_textureLayer );
}

qreal AltitudeModel::height( qreal lat, qreal lon )
{
    const int tileZoomLevel = m_tileLoader->maximumTileLevel( *m_textureLayer );
    Q_ASSERT( tileZoomLevel == 9 );

    const int width = m_textureLayer->tileSize().width();
    const int height = m_textureLayer->tileSize().height();

    const int numTilesX = TileLoaderHelper::levelToColumn( m_textureLayer->levelZeroColumns(), tileZoomLevel );
    const int numTilesY = TileLoaderHelper::levelToRow( m_textureLayer->levelZeroRows(), tileZoomLevel );
    Q_ASSERT( numTilesX > 0 );
    Q_ASSERT( numTilesY > 0 );

    qreal textureX = 180 + lon;
    textureX *= numTilesX * width / 360;

    qreal textureY = 90 - lat;
    textureY *= numTilesY * height / 180;

    qreal ret = 0;
    bool hasHeight = false;
    qreal noData = 0;

    for ( int i = 0; i < 4; ++i ) {
        const int x = static_cast<int>( textureX + ( i % 2 ) );
        const int y = static_cast<int>( textureY + ( i / 2 ) );

        //qDebug() << "x" << x << ( x / width );
        //qDebug() << "y" << y << ( y / height );

        const TileId id( "earth/srtm2", tileZoomLevel, ( x % ( numTilesX * width ) ) / width, ( y % ( numTilesY * height ) ) / height );
        //qDebug() << "LAT" << lat << "LON" << lon << "tile" << ( x % ( numTilesX * width ) ) / width << ( y % ( numTilesY * height ) ) / height;

        const QImage *image = m_cache[id];
        if ( image == 0 ) {
            image = new QImage( m_tileLoader->loadTile( id, DownloadBrowse ) );
            m_cache.insert( id, image );
        }
        Q_ASSERT( image );
        Q_ASSERT( !image->isNull() );
        Q_ASSERT( width == image->width() );
        Q_ASSERT( height == image->height() );

        const qreal dx = ( textureX > (qreal)x ) ? textureX - (qreal)x : (qreal)x - textureX;
        const qreal dy = ( textureY > (qreal)y ) ? textureY - (qreal)y : (qreal)y - textureY;
        if ( !( 0 <= dx && dx <= 1 ) || !( 0 <= dy && dy <= 1 ) ) {
            qDebug() << "dx" << dx << "dy" << dy << "textureX" << textureX << "textureY" << textureY << "x" << x << "y" << y;
        }
        Q_ASSERT( 0 <= dx && dx <= 1 );
        Q_ASSERT( 0 <= dy && dy <= 1 );
        unsigned int pixel;
        pixel = image->pixel( x % width, y % height );
        pixel -= 0xFF000000; //fully opaque
        //qDebug() << "(1-dx)" << (1-dx) << "(1-dy)" << (1-dy);
        if (pixel != 32768) { //no data?
            //qDebug() << "got at x" << x % width << "y" << y % height << "a height of" << pixel << "** RGB" << qRed(pixel) << qGreen(pixel) << qBlue(pixel);
            ret += (qreal)pixel * (1-dx) * (1-dy);
            hasHeight = true;
        } else {
            //qDebug() << "no data at" <<  x % width << "y" << y % height;
            noData += (1-dx) * (1-dy);
        }
    }

    if (!hasHeight) {
        ret = 32768; //no data
    } else {
        if (noData) {
            //qDebug() << "NO DATA" << noData;
            ret += (ret / (1-noData))*noData;
        }
    }

    //qDebug() << ">>>" << lat << lon << "returning an altitude of" << ret;
    return ret;
}

QList<GeoDataCoordinates> AltitudeModel::heightProfile( qreal fromLat, qreal fromLon, qreal toLat, qreal toLon )
{
    const int tileZoomLevel = m_tileLoader->maximumTileLevel( *m_textureLayer );
    const int width = m_textureLayer->tileSize().width();
    const int numTilesX = TileLoaderHelper::levelToColumn( m_textureLayer->levelZeroColumns(), tileZoomLevel );

    qreal distPerPixel = (qreal)360 / ( width * numTilesX );
    //qDebug() << "heightProfile" << fromLat << fromLon << toLat << toLon << "distPerPixel" << distPerPixel;

    qreal lat = fromLat;
    qreal lon = fromLon;
    char dirLat = fromLat < toLat ? 1 : -1;
    char dirLon = fromLon < toLon ? 1 : -1;
    qreal k = qAbs( ( fromLat - toLat ) / ( fromLon - toLon ) );
    //qDebug() << "fromLon" << fromLon << "fromLat" << fromLat;
    //qDebug() << "diff lon" << ( fromLon - toLon ) << "diff lat" << ( fromLat - toLat );
    //qDebug() << "dirLon" << QString::number(dirLon) << "dirLat" << QString::number(dirLat) << "k" << k;
    QList<GeoDataCoordinates> ret;
    while ( lat*dirLat <= toLat*dirLat && lon*dirLon <= toLon*dirLon ) {
        //qDebug() << lat << lon;
        qreal h = height( lat, lon );
        if (h < 32000) {
            ret << GeoDataCoordinates( lon, lat, h, GeoDataCoordinates::Degree );
        }
        if ( k < 0.5 ) {
            //qDebug() << "lon(x) += distPerPixel";
            lat += distPerPixel * k * dirLat;
            lon += distPerPixel * dirLon;
        } else {
            //qDebug() << "lat(y) += distPerPixel";
            lat += distPerPixel * dirLat;
            lon += distPerPixel / k * dirLon;
        }
    }
    //qDebug() << ret;
    return ret;
}

void AltitudeModel::tileCompleted( const TileId & tileId, const QImage &image )
{
    m_cache.insert( tileId, new QImage( image ) );
    emit loadCompleted();
}

}



#include "AltitudeModel.moc"
