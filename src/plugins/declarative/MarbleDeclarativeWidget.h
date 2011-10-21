//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#ifndef DECLARATIVE_MARBLE_WIDGET_H
#define DECLARATIVE_MARBLE_WIDGET_H

#include "MarbleModel.h"
#include "MarbleMap.h"

#include "Coordinate.h"
#include "Placemark.h"

#include "MapThemeManager.h"

#include <QtDeclarative/QDeclarativeItem>
#include <QtCore/QList>
#include <QtCore/QPoint>
#include <QtDeclarative/QtDeclarative>

using Marble::GeoDataCoordinates; // Ouch. For signal/slot connection across different namespaces

namespace Marble
{
// Forward declarations
class AbstractFloatItem;
class MarbleModel;
class RenderPlugin;
class ViewportParams;
}

class DeclarativeDataPlugin;
class ZoomButtonInterceptor;
/**
  * Wraps a Marble::MarbleWidget, providing access to important properties and methods
  *
  * @todo FIXME: Currently stuffed in a QGraphicsProxyWidget as otherwise it is only
  * displayed in QML when it is the only widget. For performance reasons it would be
  * nice to avoid this.
  */
class MarbleWidget : public QDeclarativeItem
{
    Q_OBJECT

    Q_PROPERTY( Coordinate* center READ center WRITE setCenter NOTIFY visibleLatLonAltBoxChanged )
    Q_PROPERTY( int radius READ radius WRITE setRadius NOTIFY radiusChanged )
    Q_PROPERTY( QString mapThemeId READ mapThemeId WRITE setMapThemeId NOTIFY mapThemeChanged )
    Q_PROPERTY( QString projection READ projection WRITE setProjection NOTIFY projectionChanged )
    Q_PROPERTY( bool inputEnabled READ inputEnabled WRITE setInputEnabled )
    Q_PROPERTY( bool workOffline READ workOffline WRITE setWorkOffline NOTIFY workOfflineChanged )
    Q_PROPERTY( QStringList activeFloatItems READ activeFloatItems WRITE setActiveFloatItems )
    Q_PROPERTY( QStringList activeRenderPlugins READ activeRenderPlugins WRITE setActiveRenderPlugins )
    Q_PROPERTY( QObject* mapThemeModel READ mapThemeModel NOTIFY mapThemeModelChanged )
    Q_PROPERTY( QList<QObject*> renderPlugins READ renderPlugins CONSTANT )
    Q_PROPERTY( QList<QObject*> floatItems READ floatItems CONSTANT )
    Q_PROPERTY(QDeclarativeListProperty<DeclarativeDataPlugin> dataLayers READ dataLayers)

public:
    /** Constructor */
    explicit MarbleWidget( QDeclarativeItem *parent = 0 );

    void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

    ~MarbleWidget();

    Marble::MarbleModel *model();

    const Marble::ViewportParams *viewport() const;

    bool workOffline() const;

    void setWorkOffline( bool workOffline );

    int radius() const;

    void setRadius( int radius );

    void setActiveRenderPlugins( const QStringList &items );

    QStringList activeRenderPlugins() const;

    QDeclarativeListProperty<DeclarativeDataPlugin> dataLayers();

Q_SIGNALS:
    /** Forwarded from MarbleWidget. Zoom value and/or center position have changed */
    void visibleLatLonAltBoxChanged();

    void workOfflineChanged();

    void radiusChanged();

    void mouseClickGeoPosition( qreal longitude, qreal latitude );

    void mapThemeModelChanged();

    void mapThemeChanged();

    void placemarkSelected( Placemark* placemark );

    void projectionChanged();

public Q_SLOTS:
    Coordinate* center();

    void setCenter( Coordinate* center );

    void centerOn( const Marble::GeoDataLatLonBox &bbox );

    void centerOn( const GeoDataCoordinates &coordinates );

    QList<QObject*> renderPlugins() const;

    Marble::RenderPlugin* renderPlugin( const QString & name );

    bool containsRenderPlugin( const QString & name );

    QList<QObject*> floatItems() const;

    Marble::AbstractFloatItem* floatItem( const QString & name );

    bool containsFloatItem( const QString & name );

    /** Returns a list of active (!) float items */
    QStringList activeFloatItems() const;

    /** Activates all of the given float items and deactivates any others */
    void setActiveFloatItems( const QStringList &items );

    /** Returns true if the map accepts keyboard/mouse input */
    bool inputEnabled() const;

    /** Toggle keyboard/mouse input */
    void setInputEnabled( bool enabled );

    /**
      * Returns the currently active map theme id, if any, in the
      * form of e.g. "earth/openstreetmap/openstreetmap.dgml"
      */
    QString mapThemeId() const;

    /**
      * Change the currently active map theme id. Ignored if the given
      * map theme id is invalid (not installed).
      * @see DeclarativeMapThemeManager
      */
    void setMapThemeId( const QString &mapThemeId );

    /**
      * Returns the active projection which can be either "Equirectangular",
      * "Mercator" or "Spherical"
      */
    QString projection( ) const;

    /**
      * Change the active projection. Accepted values are "Equirectangular",
      * "Mercator" and "Spherical"
      */
    void setProjection( const QString &projection );

    /** Zoom in by a fixed amount */
    void zoomIn();

    /** Zoom out by a fixed amount */
    void zoomOut();

    /**
      * Returns the screen position of the given coordinate
      * (can be out of the screen borders)
      */
    QPoint pixel( qreal longitude, qreal latitude ) const;

    /**
      * Returns the coordinate at the given screen position
      */
    Coordinate *coordinate( int x, int y );

    QObject* mapThemeModel();

    void setGeoSceneProperty( const QString &key, bool value );

    void downloadRoute( qreal offset, int topTileLevel, int bottomTileLevel );

    void downloadArea( int topTileLevel, int bottomTileLevel );

    void setDataPluginDelegate( const QString &plugin, QDeclarativeComponent* delegate );

private Q_SLOTS:
    void updateCenterPosition();

    void forwardMouseClick( qreal lon, qreal lat, GeoDataCoordinates::Unit );

private:
    static void addLayer( QDeclarativeListProperty<DeclarativeDataPlugin> *list, DeclarativeDataPlugin *layer );

    Marble::MarbleModel m_model;
    Marble::MarbleMap m_map;

    Marble::MapThemeManager m_mapThemeManager;

    bool m_inputEnabled;

    Coordinate m_center;

    ZoomButtonInterceptor *const m_interceptor;

    QList<DeclarativeDataPlugin*> m_dataLayers;
};

#endif
