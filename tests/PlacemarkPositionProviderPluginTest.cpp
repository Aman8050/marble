//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012       Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#include <QtTest/QtTest>
#include "GeoDataTrack.h"
#include "GeoDataPlacemark.h"
#include "PositionProviderPlugin.h"
#include "PluginManager.h"
#include "MarbleModel.h"

#define addRow() QTest::newRow( QString("line %1").arg( __LINE__ ).toAscii().data() )

Q_DECLARE_METATYPE( Marble::GeoDataPlacemark * )

namespace Marble
{

class PlacemarkPositionProviderPluginTest : public QObject
{
    Q_OBJECT

 public:
    PlacemarkPositionProviderPluginTest();

    static PositionProviderPlugin *createUninitializedPlugin( const PluginManager *pluginManager );
    static PositionProviderPlugin *createInitializedPlugin( const MarbleModel *model );

 private slots:
    void setClockDateTime();

 private:
    const QDateTime m_minTime;
    const QDateTime m_maxTime;
    GeoDataPlacemark m_placemark1;
    GeoDataPlacemark m_placemark2;
};

PlacemarkPositionProviderPluginTest::PlacemarkPositionProviderPluginTest() :
    m_minTime( QDate( 2000, 1, 1 ), QTime( 0, 0 ) ),
    m_maxTime( QDate( 2000, 1, 2 ), QTime( 23, 59 ) )
{
    qRegisterMetaType<GeoDataAccuracy>( "GeoDataAccuracy" );
    qRegisterMetaType<PositionProviderStatus>( "PositionProviderStatus" );

    GeoDataTrack *track = new GeoDataTrack;
    track->setInterpolate( true );
    track->addPoint( m_minTime, GeoDataCoordinates() );
    track->addPoint( m_maxTime, GeoDataCoordinates() );
    m_placemark1.setGeometry( track );

    m_placemark2 = m_placemark1;
}

PositionProviderPlugin *PlacemarkPositionProviderPluginTest::createUninitializedPlugin( const PluginManager *pluginManager )
{
    QList<PositionProviderPlugin *> plugins = pluginManager->createPositionProviderPlugins();

    while ( !plugins.isEmpty() ) {
        PositionProviderPlugin *const plugin = plugins.takeFirst();

        if ( plugin->nameId() == "Placemark" ) {
            qDeleteAll( plugins );
            return plugin;
        }

        delete plugin;
    }

    return 0;
}

PositionProviderPlugin *PlacemarkPositionProviderPluginTest::createInitializedPlugin( const MarbleModel *model )
{
    PositionProviderPlugin *const plugin = createUninitializedPlugin( model->pluginManager() );

    if ( plugin == 0 ) {
        return 0;
    }

    plugin->setMarbleModel( model );
    plugin->initialize();

    return plugin;
}

void PlacemarkPositionProviderPluginTest::setClockDateTime()
{
    MarbleModel model;

    model.setClockDateTime( m_minTime ); // FIXME crashes when this line is removed

    model.setTrackedPlacemark( &m_placemark1 );

    PositionProviderPlugin *const plugin = createInitializedPlugin( &model );
    QVERIFY2( plugin != 0, "Need a PlacemarkPositionProviderPlugin!" );

    QSignalSpy positionChangedSpy( plugin, SIGNAL( positionChanged( GeoDataCoordinates, GeoDataAccuracy ) ) );

    model.setClockDateTime( m_minTime );

    QCOMPARE( positionChangedSpy.count(), 1 );

    model.setClockDateTime( m_minTime ); // FIXME crashes when using m_maxTime

    QCOMPARE( positionChangedSpy.count(), 2 );

    delete plugin;
}

}

QTEST_MAIN( Marble::PlacemarkPositionProviderPluginTest )

#include "PlacemarkPositionProviderPluginTest.moc"
