//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010      Dennis Nienhüser <earthwings@gentoo.org>
//

#include "RouteRequest.h"

#include "GeoDataContainer.h"
#include "GeoDataData.h"
#include "GeoDataExtendedData.h"
#include "GeoDataLineString.h"
#include "GeoDataPlacemark.h"
#include "GeoDataTreeModel.h"
#include "MarbleDirs.h"

#include <QMap>
#include <QPainter>
#include <QDebug>

namespace Marble
{

struct PixmapElement
{
    int index;

    int size;

    explicit PixmapElement( int index=-1, int size=0 );

    bool operator < ( const PixmapElement &other ) const;
};

class RouteRequestPrivate
{
public:
    RouteRequestPrivate( GeoDataTreeModel *treeModel, GeoDataContainer *requestFolder );

    GeoDataTreeModel *const m_treeModel;

    GeoDataContainer *const m_requestContainer;

    QMap<PixmapElement, QPixmap> m_pixmapCache;

    RoutingProfile m_routingProfile;

    /** Determines a suitable index for inserting a via point */
    int viaIndex( const GeoDataCoordinates &position ) const;
};

PixmapElement::PixmapElement( int index_, int size_ ) :
    index( index_ ), size( size_ )
{
    // nothing to do
}

bool PixmapElement::operator <(const PixmapElement &other) const
{
    return index < other.index || size < other.size;
}

RouteRequestPrivate::RouteRequestPrivate( GeoDataTreeModel *treeModel, GeoDataContainer *requestContainer ) :
    m_treeModel( treeModel ),
    m_requestContainer( requestContainer )
{
}

int RouteRequestPrivate::viaIndex( const GeoDataCoordinates &position ) const
{
    /** @todo: Works, but does not look elegant at all */

    // Iterates over all ordered trip point pairs (P,Q) and finds the triple
    // (P,position,Q) or (P,Q,position) with minimum length
    qreal minLength = -1.0;
    int result = 0;
    GeoDataLineString viaFirst;
    GeoDataLineString viaSecond;
    for ( int i = 0; i < m_requestContainer->size(); ++i ) {
        Q_ASSERT( viaFirst.size() < 4 && viaSecond.size() < 4 );
        if ( viaFirst.size() == 3 ) {
            viaFirst.remove( 0 );
            viaFirst.remove( 0 );
        }

        if ( viaSecond.size() == 3 ) {
            viaSecond.remove( 0 );
            viaSecond.remove( 0 );
        }

        if ( viaFirst.size() == 1 ) {
            viaFirst.append( position );
        }

        viaFirst.append( dynamic_cast<const GeoDataPlacemark *>( &m_requestContainer->at( i ) )->coordinate() );
        viaSecond.append( dynamic_cast<const GeoDataPlacemark *>( &m_requestContainer->at( i ) )->coordinate() );

        if ( viaSecond.size() == 2 ) {
            viaSecond.append( position );
        }

        if ( viaFirst.size() == 3 ) {
            qreal len = viaFirst.length( EARTH_RADIUS );
            if ( minLength < 0.0 || len < minLength ) {
                minLength = len;
                result = i;
            }
        }

        /** @todo: Assumes that destination is the last point */
        if ( viaSecond.size() == 3 && i + 1 < m_requestContainer->size() ) {
            qreal len = viaSecond.length( EARTH_RADIUS );
            if ( minLength < 0.0 || len < minLength ) {
                minLength = len;
                result = i + 1;
            }
        }
    }

    Q_ASSERT( 0 <= result && result <= m_requestContainer->size() );
    return result;
}

RouteRequest::RouteRequest( GeoDataTreeModel *treeModel, GeoDataContainer *requestContainer, QObject *parent ) :
    QObject( parent ),
    d( new RouteRequestPrivate( treeModel, requestContainer ) )
{
    // nothing to do
}

RouteRequest::~RouteRequest()
{
    delete d;
}

int RouteRequest::size() const
{
    return d->m_requestContainer->size();
}

GeoDataCoordinates RouteRequest::source() const
{
    GeoDataCoordinates result;
    if ( d->m_requestContainer->size() ) {
        result = dynamic_cast<const GeoDataPlacemark *>( &d->m_requestContainer->first() )->coordinate();
    }
    return result;
}

GeoDataCoordinates RouteRequest::destination() const
{
    GeoDataCoordinates result;
    if ( d->m_requestContainer->size() ) {
        result = dynamic_cast<const GeoDataPlacemark *>( &d->m_requestContainer->last() )->coordinate();
    }
    return result;
}

GeoDataCoordinates RouteRequest::at( int position ) const
{
    return dynamic_cast<const GeoDataPlacemark *>( &d->m_requestContainer->at( position ) )->coordinate();
}

QPixmap RouteRequest::pixmap(int position, int size, int margin ) const
{
    PixmapElement const element( position, size );

    if ( !d->m_pixmapCache.contains( element ) ) {
        // Transparent background
        bool const smallScreen = MarbleGlobal::getInstance()->profiles() & MarbleGlobal::SmallScreen;
        int const imageSize = size > 0 ? size : ( smallScreen ? 32 : 16 );
        QImage result( imageSize, imageSize, QImage::Format_ARGB32_Premultiplied );
        result.fill( qRgba( 0, 0, 0, 0 ) );

        // Paint a colored circle
        QPainter painter( &result );
        painter.setRenderHint( QPainter::Antialiasing, true );
        painter.setPen( QColor( Qt::black ) );
        bool const isVisited = visited( position );
        QColor const backgroundColor = isVisited ? Oxygen::aluminumGray4 : Oxygen::forestGreen4;
        painter.setBrush( QBrush( backgroundColor ) );
        painter.setPen( Qt::black );
        int const iconSize = imageSize - 2 * margin;
        painter.drawEllipse( margin, margin, iconSize, iconSize );

        char const text = char( 'A' + position );

        // Choose a suitable font size
        QFont font = painter.font();
        int fontSize = 20;
        while ( fontSize-- > 0 ) {
            font.setPointSize( fontSize );
            QFontMetrics const fontMetric( font );
            if ( fontMetric.width( text ) <= iconSize && fontMetric.height( ) <= iconSize ) {
                break;
            }
        }

        Q_ASSERT( fontSize );
        font.setPointSize( fontSize );
        painter.setFont( font );

        // Paint a character denoting the position (0=A, 1=B, 2=C, ...)
        painter.drawText( 0, 0, imageSize, imageSize, Qt::AlignCenter, QString( text ) );

        d->m_pixmapCache.insert( element, QPixmap::fromImage( result ) );
    }

    return d->m_pixmapCache[element];
}

void RouteRequest::clear()
{
    for ( int i=d->m_requestContainer->size()-1; i>=0; --i ) {
        remove( i );
    }
    Q_ASSERT( dynamic_cast<GeoDataContainer *>( d->m_requestContainer->parent() ) != 0 );
}

void RouteRequest::insert( int index, const GeoDataCoordinates &coordinates, const QString &name )
{
    GeoDataPlacemark *placemark = new GeoDataPlacemark;
    placemark->setCoordinate( coordinates );
    placemark->setName( name );
    d->m_treeModel->addFeature( d->m_requestContainer, placemark, index );
    emit positionAdded( index );
}

void RouteRequest::append( const GeoDataCoordinates &coordinates, const QString &name )
{
    GeoDataPlacemark placemark;
    placemark.setCoordinate( coordinates );
    placemark.setName( name );
    append( placemark );
}

void RouteRequest::append( const GeoDataPlacemark &otherPlacemark )
{
    GeoDataPlacemark *placemark = new GeoDataPlacemark( otherPlacemark );
    d->m_treeModel->addFeature( d->m_requestContainer, placemark );
    emit positionAdded( d->m_requestContainer->size()-1 );
}

void RouteRequest::remove( int index )
{
    const bool removed = d->m_treeModel->removeFeature( d->m_requestContainer, index );
    if ( removed ) {
        emit positionRemoved( index );
    }
}

void RouteRequest::addVia( const GeoDataCoordinates &position )
{
    int index = d->viaIndex( position );
    GeoDataPlacemark *placemark = new GeoDataPlacemark;
    placemark->setCoordinate( position );
    d->m_treeModel->addFeature( d->m_requestContainer, placemark, index );
    emit positionAdded( index );
}

void RouteRequest::setPosition( int index, const GeoDataCoordinates &position, const QString &name )
{
    qDebug() << Q_FUNC_INFO << position.toString() << name;
    if ( index >= 0 && index < d->m_requestContainer->size() ) {
        GeoDataPlacemark *placemark = dynamic_cast<GeoDataPlacemark *>( d->m_requestContainer->child( index ) );
        placemark->setName( name );
        if ( placemark->coordinate() != position ) {
            placemark->setCoordinate( position );
            setVisited( index, false );
            emit positionChanged( index, position );
        }
        d->m_treeModel->updateFeature( placemark );
    }
}

void RouteRequest::setName( int index, const QString &name )
{
    if ( index >= 0 && index < d->m_requestContainer->size() ) {
        GeoDataFeature *const feature = d->m_requestContainer->child( index );
        feature->setName( name );
        d->m_treeModel->updateFeature( feature );
    }
}

QString RouteRequest::name( int index ) const
{
    QString result;
    if ( index >= 0 && index < d->m_requestContainer->size() ) {
        result = d->m_requestContainer->child( index )->name();
    }
    return result;
}

void RouteRequest::setVisited( int index, bool visited )
{
    if ( index >= 0 && index < d->m_requestContainer->size() ) {
        d->m_requestContainer->child( index )->extendedData().addValue( GeoDataData( "routingVisited", visited ) );
        QMap<PixmapElement, QPixmap>::iterator iter = d->m_pixmapCache.begin();
        while ( iter != d->m_pixmapCache.end() ) {
             if ( iter.key().index == index ) {
                 iter = d->m_pixmapCache.erase( iter );
             } else {
                 ++iter;
             }
         }
        d->m_treeModel->updateFeature( d->m_requestContainer->child( index ) );
        emit positionChanged( index, static_cast<const GeoDataPlacemark *>( d->m_requestContainer->child( index ) )->coordinate() );
    }
}

bool RouteRequest::visited( int index ) const
{
    bool visited = false;
    if ( index >= 0 && index < d->m_requestContainer->size() ) {
        if ( d->m_requestContainer->child( index )->extendedData().contains( "routingVisited" ) ) {
            visited = d->m_requestContainer->child( index )->extendedData().value( "routingVisited" ).value().toBool();
        }
    }
    return visited;
}

void RouteRequest::reverse()
{
    d->m_treeModel->removeFeature( d->m_requestContainer );
    d->m_requestContainer->reverse();
    for ( int i = 0; i < d->m_requestContainer->size(); ++i ) {
        setVisited( i, false );
    }
    Q_ASSERT( dynamic_cast<GeoDataContainer *>( d->m_requestContainer->parent() ) != 0 );
    d->m_treeModel->addFeature( static_cast<GeoDataContainer *>( d->m_requestContainer->parent() ), d->m_requestContainer );
}

void RouteRequest::setRoutingProfile( const RoutingProfile &profile )
{
    d->m_routingProfile = profile;
    emit routingProfileChanged();
}

RoutingProfile RouteRequest::routingProfile() const
{
    return d->m_routingProfile;
}

const GeoDataPlacemark &RouteRequest::operator [](int index) const
{
    return *static_cast<const GeoDataPlacemark *>( d->m_requestContainer->child( index ) );
}

} // namespace Marble

#include "RouteRequest.moc"
