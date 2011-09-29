//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2010 Niko Sams <niko.sams@gmail.com>
//


#include "AltitudeProfile.h"
#include <GeoPainter.h>
#include <GeoSceneLayer.h>
#include <ViewportParams.h>
#include <routing/RoutingManager.h>
#include <routing/AlternativeRoutesModel.h>
#include <GeoDataDocument.h>
#include <MarbleDebug.h>
#include <KPlotWidget>
#include <KPlotObject>
#include <KPlotAxis>
#include <AltitudeModel.h>
#include <MarbleModel.h>
#include <GeoDataParser.h>
#include <QFile>

#include "MarbleGraphicsGridLayout.h"
#include "WidgetGraphicsItem.h"
#include <QLabel>
#include <QLayout>
#include <GeoDataFolder.h>

using namespace Marble;

AltitudeProfile::AltitudeProfile(const QPointF& point, const QSizeF& size)
    : AbstractFloatItem(point, size), m_isInitialized(false)
{

}

QStringList AltitudeProfile::backendTypes() const
{
    return QStringList( "altitudeProfile" );
}

bool AltitudeProfile::isInitialized() const
{
    return m_isInitialized;
}

void AltitudeProfile::initialize()
{
    m_isInitialized = true;

    m_graph = new KPlotWidget();
    m_graph->setAntialiasing( true );
    m_graph->axis( KPlotWidget::TopAxis )->setVisible( false );
    m_graph->axis( KPlotWidget::RightAxis )->setVisible( false );
    m_graph->resetPlot();
    //m_graph->axis( KPlotWidget::LeftAxis )->setLabel( QString() );
    //m_graph->axis( KPlotWidget::LeftAxis )->setLabel(tr("Altitude"));
    m_graph->axis( KPlotWidget::BottomAxis )->setTickLabelsShown(false);

    m_plot = new KPlotObject(Qt::red, KPlotObject::Lines, 3);
    m_graph->addPlotObject(m_plot);
    m_graph->setMaximumSize( QSize( 300, 100 ) );
    m_graph->setMinimumSize( QSize( 300, 100 ) );

    m_stats = new QLabel("Stats");
    QWidget *w = new QWidget();
    w->setMaximumSize( QSize( 400, 100 ) );
    w->setMinimumSize( QSize( 400, 100 ) );
    QHBoxLayout* l = new QHBoxLayout;
    w->setLayout( l );
    l->addWidget( m_graph, 3 );
    l->addWidget( m_stats, 1 );

    m_widgetItem = new WidgetGraphicsItem( this );
    m_widgetItem->setWidget( w );

    MarbleGraphicsGridLayout *layout = new MarbleGraphicsGridLayout( 1, 1 );
    layout->addItem( m_widgetItem, 0, 0 );

    setLayout( layout );


    currentRouteChanged( marbleModel()->routingManager()->alternativeRoutesModel()->currentRoute() );
    connect( marbleModel()->routingManager()->alternativeRoutesModel(), SIGNAL( currentRouteChanged( GeoDataDocument* ) ), SLOT( currentRouteChanged( GeoDataDocument* ) ) );

    connect( marbleModel()->altitudeModel(), SIGNAL( loadCompleted() ), SLOT( altitudeDataLoadCompleted() ) );

#if 0
    GeoDataParser parser( GeoData_UNKNOWN );

    QFile file( "/home/niko/tracks/2011-06-12-mattighofen.gpx" );
    file.open( QIODevice::ReadOnly );
    Q_ASSERT( parser.read( &file ) );
    GeoDataDocument* document = static_cast<GeoDataDocument*>( parser.releaseDocument() );
    Q_ASSERT( document );
    file.close();
    GeoDataPlacemark* routePlacemark = document->placemarkList().first();
    qDebug() << routePlacemark->geometry()->geometryId();
    Q_ASSERT(routePlacemark->geometry()->geometryId() ==  GeoDataMultiGeometryId);
    GeoDataMultiGeometry* routeWaypoints = static_cast<GeoDataMultiGeometry*>(routePlacemark->geometry());
    qDebug() << "size" << routeWaypoints->size(); // << "length" << routeWaypoints->length( EARTH_RADIUS );
    for(int i=0; i < routeWaypoints->size(); ++i) {
        GeoDataGeometry* route2 = routeWaypoints->child(i);
        qDebug() << "route2.geometryId" << route2->geometryId();
        Q_ASSERT(route2->geometryId() == GeoDataLineStringId);
        GeoDataLineString* routeWaypoints2 = static_cast<GeoDataLineString*>(route2);
        qDebug() << "size" << routeWaypoints2->size() << "length" << routeWaypoints2->length(EARTH_RADIUS);
        qreal previousAlt = 0;
        qreal totalIncrease = 0;
        for(int j=0; j< routeWaypoints2->size(); ++j) {
            GeoDataCoordinates coordinate = routeWaypoints2->at( j );
            qDebug() << coordinate.latitude(Marble::GeoDataCoordinates::Degree) << coordinate.longitude(Marble::GeoDataCoordinates::Degree) << coordinate.altitude();
            if (previousAlt && coordinate.altitude() > previousAlt) {
                totalIncrease += coordinate.altitude() - previousAlt;
            }
            previousAlt = coordinate.altitude();
        }
        qDebug() << "totalIncrease" << totalIncrease << "vs. 254m von garmin gemessen";
    }
#endif
}

void AltitudeProfile::altitudeDataLoadCompleted()
{
    currentRouteChanged( marbleModel()->routingManager()->alternativeRoutesModel()->currentRoute() );
}

void AltitudeProfile::currentRouteChanged( GeoDataDocument* route )
{
    m_plot->clearPoints();
    m_stats->setText( QString() );

    if (!route) return;

    qint32 minY = INT_MAX;
    qint32 maxY = 0;
    quint32 numDataPoints = 0;
    qDebug() << "*************************";

    GeoDataPlacemark* routePlacemark = 0;
    Q_ASSERT(route->size());
    if ( !route->placemarkList().count() ) {
        qDebug() << "no placemarks found?!";
        for(int i=0; i<route->size(); ++i) {
            qDebug() << "nodeType" << i << route->child( i )->nodeType();
            if ( dynamic_cast<GeoDataFolder*>( route->child( i ) ) ) {
                Q_ASSERT( static_cast<GeoDataFolder*>( route->child( i ) )->placemarkList().size() );
                routePlacemark = static_cast<GeoDataFolder*>( route->child( i ) )->placemarkList().first();
            }
        }
    } else {
        routePlacemark = route->placemarkList().first();
    }
    Q_ASSERT(routePlacemark);
    Q_ASSERT(routePlacemark->geometry()->geometryId() ==  GeoDataLineStringId);
    GeoDataLineString* routeWaypoints = static_cast<GeoDataLineString*>(routePlacemark->geometry());
    //qDebug() << routeWaypoints->length( EARTH_RADIUS );
    qreal totalIncrease = 0;
    qreal totalIncreaseAvg = 0;
    qreal totalDecreaseAvg = 0;
    qreal lastAltitude = -100000;
    qreal lastAvgAltitude = -100000;
    QList<qreal> allAltitudes;
    for(int i=1; i < routeWaypoints->size(); ++i) {
        GeoDataCoordinates coordinate = routeWaypoints->at( i );
        GeoDataCoordinates coordinatePrev = routeWaypoints->at( i - 1 );
        //qreal altitude = marbleModel()->altitudeModel()->height(coordinate.latitude(Marble::GeoDataCoordinates::Degree), coordinate.longitude(Marble::GeoDataCoordinates::Degree));
        QList<qreal> altitudes = marbleModel()->altitudeModel()->heightProfile(
            coordinatePrev.latitude(Marble::GeoDataCoordinates::Degree),
            coordinatePrev.longitude(Marble::GeoDataCoordinates::Degree),
            coordinate.latitude(Marble::GeoDataCoordinates::Degree),
            coordinate.longitude(Marble::GeoDataCoordinates::Degree)
        );
        foreach(const qreal altitude, altitudes) {
            //qDebug() << "POINT" << numDataPoints << coordinate.longitude(Marble::GeoDataCoordinates::Degree) << coordinate.latitude(Marble::GeoDataCoordinates::Degree)
            //        << "height" << altitude;
            allAltitudes << altitude;
            if ( allAltitudes.count() >= 10 ) {
                qreal avgAltitude = 0;
                for(int j=0; j<10; ++j) {
                    avgAltitude += allAltitudes.at(allAltitudes.count()-j-1);
                }
                avgAltitude = avgAltitude / 10;
                if (lastAvgAltitude != -100000 && avgAltitude > lastAvgAltitude) {
                    totalIncreaseAvg += avgAltitude-lastAvgAltitude;
                }
                if (lastAvgAltitude != -100000 && avgAltitude < lastAvgAltitude) {
                    totalDecreaseAvg -= avgAltitude-lastAvgAltitude;
                }
                lastAvgAltitude = avgAltitude;
            }
            if ( lastAltitude != -100000 && altitude > lastAltitude ) {
                totalIncrease += altitude - lastAltitude;
                //qDebug() << "INCREASE +=" << altitude - lastAltitude << "totalIncrease is now" << totalIncrease;
            }

            double value = altitude;
            //qDebug() << "value" << value;
            m_plot->addPoint(numDataPoints++, value);
            if (value > maxY) maxY = value;
            if (value < minY) minY = value;
            lastAltitude = altitude;
        }
    }
    //qDebug() << "TOTAL INCREASE" << totalIncrease;
    //qDebug() << "TOTAL INCREASE AVG" << totalIncreaseAvg;

    m_graph->setLimits( 0, numDataPoints, minY - minY / 5, maxY + maxY / 5 );

    m_stats->setText( tr( "Gain:<br>%0m<br>Loss:<br>%1m" ).arg( totalIncreaseAvg ).arg( totalDecreaseAvg ) );
}


QIcon AltitudeProfile::icon() const
{
    return QIcon();
}

QString AltitudeProfile::description() const
{
    return tr( "This is a float item that displays the altitude profile of a track." );
}

QString AltitudeProfile::nameId() const
{
    return QString( "altitudeProfile" );
}

QString AltitudeProfile::guiString() const
{
    return tr( "&Altitude Profile" );
}

QString AltitudeProfile::name() const
{
    return QString( "Altitude Profile" );
}

Q_EXPORT_PLUGIN2( AltitudeProfile, Marble::AltitudeProfile )

#include "AltitudeProfile.moc"

