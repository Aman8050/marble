//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Niko Sams <niko.sams@gmail.com>
//


#ifndef MARBLE_ALTITUDEMODEL_H
#define MARBLE_ALTITUDEMODEL_H

#include <QObject>
#include <QCache>
#include <QImage>

#include "marble_export.h"
#include "TileId.h"

namespace Marble {

class MarbleModel;
class HttpDownloadManager;
class GeoSceneGroup;
class GeoSceneTexture;
class MapThemeManager;
class TileLoader;


class MARBLE_EXPORT AltitudeModel : public QObject
{
    Q_OBJECT
public:
    AltitudeModel( MapThemeManager const * const mapThemeManager,
                        HttpDownloadManager * const downloadManager, MarbleModel * const model );

    qreal height(qreal lat, qreal lon);
    QList<qreal> heightProfile( qreal fromLat, qreal fromLon, qreal toLat, qreal toLon );

Q_SIGNALS:
    /**
     * Altitude tiles loaded. You will get more acurate results when quering height
     * for at least one that was queried before.
     **/
    void loadCompleted();

private Q_SLOTS:
    void tileCompleted( const TileId & tileId, const QImage &image );

private:
    void updateTextureLayers();

private: //TODO d pointer
    TileLoader *m_tileLoader;
    const MapThemeManager* m_mapThemeManager;
    const GeoSceneTexture *m_textureLayer;
    MarbleModel *m_model;
    QCache<TileId, const QImage> m_cache;
};

}

#endif // MARBLE_ALTITUDEMODEL_H
