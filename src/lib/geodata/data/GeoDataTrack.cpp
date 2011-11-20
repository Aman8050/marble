//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Guillaume Martres <smarter@ubuntu.com>
//

#include "GeoDataTrack.h"

#include "GeoDataLatLonAltBox.h"
#include "GeoDataTypes.h"
#include "MarbleDebug.h"
#include "Quaternion.h"

#include "GeoDataLineString.h"

#include <QtCore/QMap>
#include <QtCore/QLinkedList>
#include "GeoDataExtendedData.h"

namespace Marble {

class GeoDataTrackPrivate
{
public:
    GeoDataTrackPrivate()
        : m_lineString( new GeoDataLineString() ),
          m_lineStringNeedsUpdate( false ),
          m_interpolate( false )
    {
    }

    GeoDataLineString *m_lineString;
    bool m_lineStringNeedsUpdate;

    QMap<QDateTime, GeoDataCoordinates> m_pointMap;

    QLinkedList<QDateTime> m_whenStack;
    QList<GeoDataCoordinates> m_coordStack;
    GeoDataExtendedData m_extendedData;

    bool m_interpolate;
};

GeoDataTrack::GeoDataTrack()
    : d( new GeoDataTrackPrivate() )
{

}

GeoDataTrack::GeoDataTrack( const GeoDataGeometry &other )
    : GeoDataGeometry( other )
{

}

int GeoDataTrack::size() const
{
    return d->m_pointMap.size() + d->m_coordStack.size();
}

bool GeoDataTrack::interpolate() const
{
    return d->m_interpolate;
}

void GeoDataTrack::setInterpolate(bool on)
{
    d->m_interpolate = on;
}

QDateTime GeoDataTrack::firstWhen() const
{
    if ( d->m_pointMap.isEmpty() ) {
        return QDateTime();
    }

    return d->m_pointMap.constBegin().key();
}

QDateTime GeoDataTrack::lastWhen() const
{
    if ( d->m_pointMap.isEmpty() ) {
        return QDateTime();
    }

    return ( d->m_pointMap.constEnd() - 1).key();
}

QList<GeoDataCoordinates> GeoDataTrack::coordinatesList() const
{
    return d->m_pointMap.values() + d->m_coordStack;
}

QList<QDateTime> GeoDataTrack::whenList() const
{
    return d->m_pointMap.keys();
}

GeoDataCoordinates GeoDataTrack::coordinatesAt( const QDateTime &when ) const
{
    if ( d->m_pointMap.isEmpty() ) {
        return GeoDataCoordinates();
    }

    QMap<QDateTime, GeoDataCoordinates>::const_iterator nextEntry = d->m_pointMap.upperBound( when );

    // No tracked point happened before "when"
    if ( nextEntry == d->m_pointMap.constBegin() ) {
        mDebug() << "No tracked point before " << when;
        return GeoDataCoordinates();
    }

    QMap<QDateTime, GeoDataCoordinates>::const_iterator previousEntry = nextEntry - 1;
    GeoDataCoordinates previousCoord = previousEntry.value();

    GeoDataCoordinates coord;

    if ( nextEntry != d->m_pointMap.constEnd() && interpolate() ) {
        QDateTime previousWhen = previousEntry.key();
        QDateTime nextWhen = nextEntry.key();
        GeoDataCoordinates nextCoord = nextEntry.value();

#if QT_VERSION < 0x040700	
        int interval = 1000 * previousWhen.secsTo( nextWhen );
        int position = 1000 * previousWhen.secsTo( when );
#else	
        int interval = previousWhen.msecsTo( nextWhen );
        int position = previousWhen.msecsTo( when );
#endif	
        qreal t = (qreal)position / (qreal)interval;

        Quaternion interpolated;
        interpolated.slerp( previousCoord.quaternion(), nextCoord.quaternion(), t );
        qreal lon, lat;
        interpolated.getSpherical( lon, lat );

        qreal alt = previousCoord.altitude() + ( nextCoord.altitude() - previousCoord.altitude() ) * t;

        coord = GeoDataCoordinates( lon, lat, alt );
    } else {
        coord = previousCoord;
    }

    return coord;
}

GeoDataCoordinates GeoDataTrack::coordinatesAt( int index ) const
{
    if ( index < d->m_pointMap.count() ) {
        return d->m_pointMap.values().at( index );
    } else {
        return d->m_coordStack.at( index - d->m_pointMap.count() );
    }
}

void GeoDataTrack::addPoint( const QDateTime &when, const GeoDataCoordinates &coord )
{
    d->m_lineStringNeedsUpdate = true;
    d->m_pointMap.insert( when, coord );
}

void GeoDataTrack::appendCoordinates( const GeoDataCoordinates &coord )
{
    if ( !d->m_whenStack.isEmpty() ) {
        d->m_pointMap.insert( d->m_whenStack.takeFirst(), coord );
    } else {
        d->m_coordStack.append( coord );
    }
    d->m_lineStringNeedsUpdate = true;
}

void GeoDataTrack::appendAltitude( qreal altitude )
{
    if ( d->m_coordStack.isEmpty() ) {
        Q_ASSERT(0);
    } else {
        GeoDataCoordinates coordinates = d->m_coordStack.last();
        coordinates.setAltitude( altitude );
        d->m_coordStack.removeLast();
        d->m_coordStack.append( coordinates );
    }
    d->m_lineStringNeedsUpdate = true;
}

void GeoDataTrack::appendWhen( const QDateTime &when )
{
    if ( !d->m_coordStack.isEmpty() ) {
        d->m_pointMap.insert( when, d->m_coordStack.takeFirst() );
        d->m_lineStringNeedsUpdate = true;
    } else {
        d->m_whenStack.append( when );
    }
}

void GeoDataTrack::clear()
{
    d->m_pointMap.clear();
    d->m_coordStack.clear();
    d->m_whenStack.clear();
    d->m_lineStringNeedsUpdate = true;
}

void GeoDataTrack::removeBefore( const QDateTime &when )
{
    QMap<QDateTime, GeoDataCoordinates>::iterator end = d->m_pointMap.end();
    QMap<QDateTime, GeoDataCoordinates>::iterator it = d->m_pointMap.begin();

    while ( it != end && it.key() < when ) {
        it = d->m_pointMap.erase( it );
    }
}

void GeoDataTrack::removeAfter( const QDateTime &when )
{
    QMap<QDateTime, GeoDataCoordinates>::iterator end = d->m_pointMap.end();
    QMap<QDateTime, GeoDataCoordinates>::iterator it = d->m_pointMap.upperBound( when );

    while ( it != end ) {
        it = d->m_pointMap.erase( it );
    }
}

GeoDataLineString *GeoDataTrack::lineString() const
{
    if ( d->m_lineStringNeedsUpdate ) {
        delete d->m_lineString;
        d->m_lineString = new GeoDataLineString();
        foreach ( const GeoDataCoordinates &coordinates, coordinatesList() ) {
            d->m_lineString->append( coordinates );
        }
        d->m_lineStringNeedsUpdate = false;
    }
    return d->m_lineString;
}

GeoDataExtendedData& GeoDataTrack::extendedData() const
{
    return d->m_extendedData;
}

void GeoDataTrack::setExtendedData( const GeoDataExtendedData& extendedData )
{
    d->m_extendedData = extendedData;
}

const char* GeoDataTrack::nodeType() const
{
    return GeoDataTypes::GeoDataTrackType;
}

EnumGeometryId GeoDataTrack::geometryId() const
{
    return GeoDataTrackId;
}

GeoDataLatLonAltBox GeoDataTrack::latLonAltBox() const
{
    return lineString()->latLonAltBox();
}

//TODO
void GeoDataTrack::pack( QDataStream& stream ) const
{
    GeoDataGeometry::pack( stream );
}
//TODO
void GeoDataTrack::unpack( QDataStream& stream )
{
    GeoDataGeometry::unpack( stream );
}

}
