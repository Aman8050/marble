//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>
// Copyright 2007-2008 Inge Wallin  <ingwa@kde.org>
//

#ifndef MARBLE_VECTORMAP_H
#define MARBLE_VECTORMAP_H

#include <QtCore/QPointF>
#include <QtGui/QPen>
#include <QtGui/QBrush>

#include "global.h"
#include "Quaternion.h"
#include "GeoDataCoordinates.h"
#include "ScreenPolygon.h"

class QPaintDevice;

namespace Marble
{

class GeoPainter;
class PntMap;
class ViewportParams;

class VectorMap : public ScreenPolygon::Vector
{
 public:
    VectorMap();
    ~VectorMap();
    void createFromPntMap( const PntMap*, const ViewportParams *viewport );

    /**
     * @brief Paint the background, i.e. the water.
     */
    void paintMap( GeoPainter *painter );
    void drawMap( GeoPainter *painter );

    void setzBoundingBoxLimit ( const qreal zBoundingBoxLimit ) {
        m_zBoundingBoxLimit = zBoundingBoxLimit; }
    void setzPointLimit ( const qreal zPointLimit ) {
        m_zPointLimit = zPointLimit; }

    //	void clearNodeCount(){ m_debugNodeCount = 0; }
    //	int nodeCount(){ return m_debugNodeCount; }

 private:
    void sphericalCreateFromPntMap( const PntMap*, const ViewportParams *viewport );
    void rectangularCreateFromPntMap( const PntMap*, const ViewportParams *viewport );
    void mercatorCreateFromPntMap( const PntMap*, const ViewportParams *viewport );

    void createPolyLine( GeoDataCoordinates::Vector::ConstIterator const &,
                         GeoDataCoordinates::Vector::ConstIterator const &, const int,
                         const ViewportParams *viewport );
    void sphericalCreatePolyLine( GeoDataCoordinates::Vector::ConstIterator const &,
				  GeoDataCoordinates::Vector::ConstIterator const &,
                                  const int detail, const ViewportParams *viewport );
    void rectangularCreatePolyLine( GeoDataCoordinates::Vector::ConstIterator const &,
				    GeoDataCoordinates::Vector::ConstIterator const &,
                                    const int detail, const ViewportParams *viewport );
    void mercatorCreatePolyLine( GeoDataCoordinates::Vector::ConstIterator const &,
				 GeoDataCoordinates::Vector::ConstIterator const &,
                                 const int detail, const ViewportParams *viewport );

    void           manageCrossHorizon( const ViewportParams *viewport );
    QPointF  horizonPoint( const ViewportParams *viewport ) const;
    void           createArc( const ViewportParams *viewport );

    int            getDetailLevel( int radius ) const;

 private:
    qreal            m_zlimit;
    qreal            m_plimit;
    qreal            m_zBoundingBoxLimit;	
    qreal            m_zPointLimit;	

    //	Quaternion m_invRotAxis;
    matrix            m_rotMatrix;

    //	int m_debugNodeCount;

    ScreenPolygon     m_polygon;

    QPointF           m_currentPoint;
    QPointF           m_lastPoint; 

    // Dealing with the horizon for spherical projection.
    bool              m_firsthorizon;
    bool              m_lastvisible;
    bool              m_currentlyvisible;
    bool              m_horizonpair;
    QPointF           m_firstHorizonPoint;
    QPointF           m_horizona;
    QPointF           m_horizonb;
	
    int               m_rlimit;

    // Needed for repetition in the X direction for flat projection
    int         m_lastSign;
    int         m_offset;
    qreal      m_lastLon;
    qreal      m_lastLat;
};

}

#endif
