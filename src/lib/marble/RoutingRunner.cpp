//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2012,2013 Bernhard Beschow <bbeschow@cs.tu-berlin.de>

#include "RoutingRunner.h"

#include "GeoDataPlacemark.h"
#include "GeoDataExtendedData.h"

#include <QString>

namespace Marble
{

RoutingRunner::RoutingRunner( QObject *parent ) :
    QObject( parent )
{
}

const GeoDataExtendedData RoutingRunner::routeData(qreal length, const QTime& duration) const
{
    GeoDataExtendedData result;
    GeoDataData lengthData;
    lengthData.setName( "length" );
    lengthData.setValue( length );
    result.addValue( lengthData );
    GeoDataData durationData;
    durationData.setName( "duration" );
    durationData.setValue( duration.toString( Qt::ISODate ) );
    result.addValue( durationData );
    return result;
}

}

#include "RoutingRunner.moc"
