//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2008 Torsten Rahn <tackat@kde.org>"
//

#include "MarbleTestPlugin.h"

#include <QtGui/QColor>
#include <QtGui/QPixmap>
#include <QtGui/QRadialGradient>
#include "MarbleDirs.h"
#include "GeoPainter.h"
#include "GeoDataCoordinates.h"
#include "GeoDataLineString.h"
#include "GeoDataLinearRing.h"

namespace Marble
{

QStringList MarbleTestPlugin::backendTypes() const
{
    return QStringList( "test" );
}

QString MarbleTestPlugin::renderPolicy() const
{
    return QString( "ALWAYS" );
}

QStringList MarbleTestPlugin::renderPosition() const
{
    return QStringList( "ALWAYS_ON_TOP" );
}

QString MarbleTestPlugin::name() const
{
    return tr( "Test Plugin" );
}

QString MarbleTestPlugin::guiString() const
{
    return tr( "&Test Plugin" );
}

QString MarbleTestPlugin::nameId() const
{
    return QString( "test-plugin" );
}

QString MarbleTestPlugin::description() const
{
    return tr( "This is a simple test plugin." );
}

QIcon MarbleTestPlugin::icon () const
{
    return QIcon();
}


void MarbleTestPlugin::initialize ()
{
}

bool MarbleTestPlugin::isInitialized () const
{
    return true;
}

bool MarbleTestPlugin::render( GeoPainter *painter, ViewportParams *viewport, const QString& renderPos, GeoSceneLayer * layer )
{
    painter->autoMapQuality();

    // Example: draw a straight line

    GeoDataCoordinates northpole1( 0.0, 90.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates northpole2( 0.0, 90.0, 3000000.0, GeoDataCoordinates::Degree );

    painter->setPen( QColor( 255, 255, 255, 255 ) );

    painter->drawLine( northpole1, northpole2 );

    // Example: draw a straight line string ("polyline")

    GeoDataCoordinates madrid( -3.7, 40.4, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates flensburg( 9.4, 54.8, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates linkoeping( 15.6, 58.4, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates istanbul( 28.0, 41.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates moscow( 37.6, 55.75, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates brasilia( -47.9, -15.75, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates orbit( 105.6, 0.0, 3000000.0, GeoDataCoordinates::Degree );

    painter->setPen( QColor( 200, 200, 200, 255 ) );

    GeoDataLineString lineString;
    lineString << &madrid << &flensburg << &linkoeping << &istanbul << &moscow;

    painter->drawPolyline( lineString ); 

    // Example: draw plain filled circles with text on earth and in earth orbit

    painter->setPen( QColor( 99, 198, 99, 255 ) );
    painter->setBrush( QColor( 99, 198, 99, 80 ) );
    painter->drawEllipse( flensburg, 30, 30 ); 

    painter->drawText( flensburg, "Torsten" );

    painter->setPen( QColor( 198, 99, 99, 255 ) );
    painter->setBrush( QColor( 198, 99, 99, 80 ) );
    painter->drawEllipse( linkoeping, 40, 40 ); 

    painter->drawText( linkoeping, "Inge" );

    painter->setPen( QColor( 99, 99, 198, 255 ) );
    painter->setBrush( QColor( 99, 99, 198, 80 ) );
    painter->drawEllipse( orbit, 20, 20 ); 

    painter->drawText( orbit, "Claudiu" );

    // Example: draw plain pixmaps

    painter->drawPixmap( istanbul, QPixmap( MarbleDirs::path( "bitmaps/earth_apollo.jpg" ) ) ); 

    painter->drawImage( brasilia, QImage( MarbleDirs::path( "bitmaps/earth_apollo.jpg" ) ) ); 

    // Example: draw a plain rectangle and a rounded rectangle

    painter->setPen( QColor( 99, 198, 198, 255 ) );
    QBrush brush( QColor( 99, 198, 198, 80 ) );
    painter->setBrush( brush );

    painter->drawRect( madrid, 30, 30 ); 

    painter->setPen( QColor( 198, 99, 198, 255 ) );
    brush.setColor( QColor( 198, 99, 198, 180 ) );
    brush.setStyle( Qt::DiagCrossPattern );
    painter->setBrush( brush );

    painter->drawRoundRect( moscow, 40, 40 ); 

    // Example: draw earth orbit

    GeoDataCoordinates m1(-180.0, 0.0, 3000000.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates m2(-90.0, 0.0, 3000000.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates m3(0.0, 0.0, 3000000.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates m4(+90.0, 0.0, 3000000.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates m5(+180.0, 0.0, 3000000.0, GeoDataCoordinates::Degree );

    GeoDataLinearRing ring( 0, Tessellate | RespectLatitudeCircle );

    ring << &m1 << &m2 << &m3 << &m4 << &m5;

    painter->drawPolyline( ring ); 

    // Example: draw a triangle with lines that follow the coordinate grid

    painter->setPen( QColor( 198, 99, 99, 255 ) );
    brush.setColor( QColor( 198, 99, 99, 180 ) );
    brush.setStyle( Qt::FDiagPattern );
    painter->setBrush( brush );

    GeoDataCoordinates t1(0.0, 90.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t2(-12.5, 45.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t3(-77.5, 45.0, 0.0, GeoDataCoordinates::Degree );

    GeoDataLinearRing triangle( 0, Tessellate | RespectLatitudeCircle );

    triangle << &t1 << &t2 << &t3;

    painter->drawPolygon( triangle, Qt::OddEvenFill ); 

    // Example: draw a triangle with lines that follow the great circles

    GeoDataLinearRing triangle2( 0, Tessellate );

    GeoDataCoordinates t4(0.0, 90.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t5(-102.5, 45.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t6(-167.5, 45.0, 0.0, GeoDataCoordinates::Degree );

    triangle2 << &t4 << &t5 << &t6;

    painter->drawPolygon( triangle2, Qt::OddEvenFill ); 

    // Example: draw a triangle with straight lines

    GeoDataLinearRing triangle3;

    GeoDataCoordinates t7(0.0, 90.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t8(102.5, 45.0, 0.0, GeoDataCoordinates::Degree );
    GeoDataCoordinates t9(167.5, 45.0, 0.0, GeoDataCoordinates::Degree );

    triangle3 << &t7 << &t8 << &t9;

    painter->drawPolygon( triangle3, Qt::OddEvenFill ); 


    // Example: draw a rectangle with lines that follow the coordinate grid

    GeoDataCoordinates rectCenter( -45.0, 20.0, 0.0, GeoDataCoordinates::Degree );
    painter->drawRect( rectCenter, 20.0, 20.0, true ); 


    // Example: draw annotations

    GeoDataCoordinates sotm(-8.6, 52.66, 0.0, GeoDataCoordinates::Degree );

    painter->setPen( QColor( 198, 99, 99, 255 ) );
    brush.setColor( QColor( 255, 255, 255, 200 ) );
    brush.setStyle( Qt::SolidPattern );
    painter->setBrush( brush );

    painter->drawAnnotation (  sotm, "State of the Map,\n  12-13 July 2008,\n OSM conference" );

    GeoDataCoordinates akademy2008(4.5, 51.068, 0.0, GeoDataCoordinates::Degree );

    painter->setPen( QColor( 99, 99, 0 ) );

    QRadialGradient radialGrad(QPointF(100, 100), 100);
    radialGrad.setColorAt(0, QColor( 198, 198, 198, 200 ) );
    radialGrad.setColorAt(0.5, QColor( 199, 198, 99, 200  ) );
    radialGrad.setColorAt( 1, Qt::white );
    radialGrad.setSpread( QGradient::ReflectSpread );

    QBrush gradientBrush( radialGrad );
    painter->setBrush( gradientBrush );

    painter->drawAnnotation (  akademy2008, "Akademy 2008,\n  9-15 August 2008,\n KDE conference", QSize(130, 120), 10, 30, 15, 15 );

    return true;
}

}

Q_EXPORT_PLUGIN2(MarbleTestPlugin, Marble::MarbleTestPlugin)

#include "MarbleTestPlugin.moc"
