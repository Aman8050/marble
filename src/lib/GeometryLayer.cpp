//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008-2009      Patrick Spendrin  <ps_ml@gmx.de>
// Copyright 2010           Thibaut Gridel <tgridel@free.fr>
//

#include "GeometryLayer.h"

// Marble
#include "GeoDataDocument.h"
#include "GeoDataFolder.h"
#include "GeoDataLineStyle.h"
#include "GeoDataObject.h"
#include "GeoDataPlacemark.h"
#include "GeoDataPolygon.h"
#include "GeoDataPolyStyle.h"
#include "GeoDataStyle.h"
#include "GeoDataStyleMap.h"
#include "MarbleDebug.h"
#include "GeoDataTypes.h"
#include "GeoDataFeature.h"
#include "GeoPainter.h"
#include "ViewportParams.h"
#include "GeoGraphicsScene.h"
#include "GeoGraphicsItem.h"
#include "GeoLineStringGraphicsItem.h"
#include "GeoPolygonGraphicsItem.h"

// Qt
#include <QtCore/QTime>
#include <QtCore/QAbstractItemModel>

namespace Marble
{
int GeometryLayer::s_defaultZValues[GeoDataFeature::LastIndex];
int GeometryLayer::s_defaultLODValues[GeoDataFeature::LastIndex];
bool GeometryLayer::s_defaultValuesInitialized = false;
int GeometryLayer::s_defaultZValue = 50;

class GeometryLayerPrivate
{
public:
    void createGraphicsItems( GeoDataObject *object );
    void createGraphicsItemFromGeometry( GeoDataGeometry *object, GeoDataPlacemark *placemark );

    QBrush m_currentBrush;
    QPen m_currentPen;
    GeoGraphicsScene m_scene;
    QAbstractItemModel *m_model;
};

GeometryLayer::GeometryLayer( QAbstractItemModel *model )
        : d( new GeometryLayerPrivate() )
{
    if ( !s_defaultValuesInitialized )
        initializeDefaultValues();

    d->m_model = model;
    GeoDataObject *object = static_cast<GeoDataObject*>( d->m_model->index( 0, 0, QModelIndex() ).internalPointer() );
    if ( object && object->parent() )
        d->createGraphicsItems( object->parent() );
}

GeometryLayer::~GeometryLayer()
{
    delete d;
}

QStringList GeometryLayer::renderPosition() const
{
    return QStringList( "HOVERS_ABOVE_SURFACE" );
}

void GeometryLayer::initializeDefaultValues()
{
    for ( int i = 0; i < GeoDataFeature::LastIndex; i++ )
        s_defaultZValues[i] = s_defaultZValue;
    
    for ( int i = 0; i < GeoDataFeature::LastIndex; i++ )
        s_defaultLODValues[i] = -1;

    s_defaultZValues[GeoDataFeature::None]                = 0;
    
    for ( int i = GeoDataFeature::LanduseAllotments; i <= GeoDataFeature::LanduseRetail; i++ )
        s_defaultZValues[(GeoDataFeature::GeoDataVisualCategory)i] = s_defaultZValue - 16;
    
    s_defaultZValues[GeoDataFeature::NaturalWater]        = s_defaultZValue - 16;
    s_defaultZValues[GeoDataFeature::NaturalWood]         = s_defaultZValue - 15;
    
    //Landuse
    
    s_defaultZValues[GeoDataFeature::LeisurePark]         = s_defaultZValue - 14; 
    
    s_defaultZValues[GeoDataFeature::TransportParking]    = s_defaultZValue - 13;
    
    s_defaultZValues[GeoDataFeature::HighwayTertiaryLink] = s_defaultZValue - 12;
    s_defaultZValues[GeoDataFeature::HighwaySecondaryLink]= s_defaultZValue - 12;
    s_defaultZValues[GeoDataFeature::HighwayPrimaryLink]  = s_defaultZValue - 12;
    s_defaultZValues[GeoDataFeature::HighwayTrunkLink]    = s_defaultZValue - 12;
    s_defaultZValues[GeoDataFeature::HighwayMotorwayLink] = s_defaultZValue - 12;

    s_defaultZValues[GeoDataFeature::HighwayUnknown]      = s_defaultZValue - 11;
    s_defaultZValues[GeoDataFeature::HighwayPath]         = s_defaultZValue - 10;
    s_defaultZValues[GeoDataFeature::HighwayTrack]        = s_defaultZValue - 9;
    s_defaultZValues[GeoDataFeature::HighwaySteps]        = s_defaultZValue - 8;
    s_defaultZValues[GeoDataFeature::HighwayPedestrian]   = s_defaultZValue - 8;
    s_defaultZValues[GeoDataFeature::HighwayService]      = s_defaultZValue - 7;
    s_defaultZValues[GeoDataFeature::HighwayRoad]         = s_defaultZValue - 6;
    s_defaultZValues[GeoDataFeature::HighwayTertiary]     = s_defaultZValue - 5;
    s_defaultZValues[GeoDataFeature::HighwaySecondary]    = s_defaultZValue - 4;
    s_defaultZValues[GeoDataFeature::HighwayPrimary]      = s_defaultZValue - 3;
    s_defaultZValues[GeoDataFeature::HighwayTrunk]        = s_defaultZValue - 2;
    s_defaultZValues[GeoDataFeature::HighwayMotorway]     = s_defaultZValue - 1;
    s_defaultZValues[GeoDataFeature::RailwayRail]         = s_defaultZValue - 1;

    s_defaultValuesInitialized = true;
}


bool GeometryLayer::render( GeoPainter *painter, ViewportParams *viewport,
                            const QString& renderPos, GeoSceneLayer * layer )
{
    painter->save();
    painter->autoMapQuality();
    QList<GeoGraphicsItem*> items = d->m_scene.items( viewport->viewLatLonAltBox() );
    foreach( GeoGraphicsItem* item, items )
    {
        if ( item->visible() )
            item->paint( painter, viewport, renderPos, layer );
    }
    painter->restore();
    return true;
}

void GeometryLayerPrivate::createGraphicsItems( GeoDataObject *object )
{
    GeoDataFeature *feature = dynamic_cast<GeoDataFeature*>( object );

    if ( dynamic_cast<GeoDataPlacemark*>( object ) )
    {
        GeoDataPlacemark *placemark = static_cast<GeoDataPlacemark*>( object );
        createGraphicsItemFromGeometry( placemark->geometry(), placemark );
    }

    // parse all child objects of the container
    GeoDataContainer *container = dynamic_cast<GeoDataContainer*>( object );
    if ( container )
    {
        int rowCount = container->size();
        for ( int row = 0; row < rowCount; ++row )
        {
            createGraphicsItems( container->child( row ) );
        }
    }
}

void GeometryLayerPrivate::createGraphicsItemFromGeometry( GeoDataGeometry* object, GeoDataPlacemark *placemark )
{
    GeoGraphicsItem *item = 0;
    if ( dynamic_cast<GeoDataLinearRing*>( object ) )
    {
    }
    else if ( GeoDataLineString* line = dynamic_cast<GeoDataLineString*>( object ) )
    {
        item = new GeoLineStringGraphicsItem();
        static_cast<GeoLineStringGraphicsItem*>( item )->setLineString( *line );
    }
    else if ( GeoDataPolygon *poly = dynamic_cast<GeoDataPolygon*>( object ) )
    {
        item = new GeoPolygonGraphicsItem();
        static_cast<GeoPolygonGraphicsItem*>( item )->setPolygon( *poly );
    }
    else if ( dynamic_cast<GeoDataMultiGeometry*>( object ) )
    {
        GeoDataMultiGeometry *multigeo = dynamic_cast<GeoDataMultiGeometry*>( object );
        int rowCount = multigeo->size();
        for ( int row = 0; row < rowCount; ++row )
        {
            createGraphicsItemFromGeometry( multigeo->child( row ), placemark );
        }
    }
    if ( !item )
        return;
    item->setStyle( placemark->style() );
    item->setVisible( placemark->isVisible() );
    item->setZValue( GeometryLayer::s_defaultZValues[placemark->visualCategory()] );
    item->setMinLodPixels( GeometryLayer::s_defaultLODValues[placemark->visualCategory()] );
    m_scene.addIdem( item );
}

void GeometryLayer::invalidateScene()
{
    QList<GeoGraphicsItem*> items = d->m_scene.items();
    foreach( GeoGraphicsItem* item, items )
    {
        delete item;
    }
    d->m_scene.clear();
    GeoDataObject *object = static_cast<GeoDataObject*>( d->m_model->index( 0, 0, QModelIndex() ).internalPointer() );
    if ( object && object->parent() )
        d->createGraphicsItems( object->parent() );
}

}

#include "GeometryLayer.moc"
