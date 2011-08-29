//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2009      Patrick Spendrin <ps_ml@gmx.de>
//

#ifndef MARBLE_GEODATAFEATUREPRIVATE_H
#define MARBLE_GEODATAFEATUREPRIVATE_H

#include <QtCore/QString>
#include <QtCore/QAtomicInt>

#include "GeoDataExtendedData.h"
#include "GeoDataAbstractView.h"
#include "GeoDataFeature.h"
#include "GeoDataRegion.h"
#include "GeoDataTimeStamp.h"
#include "GeoDataTimeSpan.h"
#include "GeoDataTypes.h"
#include "GeoDataStyle.h"
#include "MarbleDirs.h"

namespace Marble
{

class GeoDataFeaturePrivate
{
  public:
    GeoDataFeaturePrivate() :
        m_name(),
        m_description(),
        m_descriptionCDATA(),
        m_address(),
        m_phoneNumber(),
        m_styleUrl(),
        m_abstractView(),
        m_popularity( 0 ),
        m_popularityIndex( 19 ),
        m_visible( true ),
        m_visualCategory( GeoDataFeature::Default ),
        m_role(" "),
        m_style( 0 ),
        m_styleMap( 0 ),
        m_extendedData(),
        m_timeSpan(),
        m_timeStamp(),
        m_region(),
        ref( 0 )
    {
    }

    GeoDataFeaturePrivate( const GeoDataFeaturePrivate& other ) :
        m_name( other.m_name ),
        m_description( other.m_description ),
        m_descriptionCDATA( other.m_descriptionCDATA),
        m_address( other.m_address ),
        m_phoneNumber( other.m_phoneNumber ),
        m_styleUrl( other.m_styleUrl ),
        m_popularity( other.m_popularity ),
        m_popularityIndex( other.m_popularityIndex ),
        m_visible( other.m_visible ),
        m_visualCategory( other.m_visualCategory ),
        m_role( other.m_role ),
        m_style( other.m_style ),               //FIXME: both style and stylemap need to be reworked internally!!!!
        m_styleMap( other.m_styleMap ),
        m_extendedData( other.m_extendedData ),
        m_timeSpan( other.m_timeSpan ),
        m_timeStamp( other.m_timeStamp ),
        m_region( other.m_region ),
        ref( 0 )
    {
    }

    void operator=( const GeoDataFeaturePrivate& other )
    {
        m_name = other.m_name;
        m_description = other.m_description;
        m_descriptionCDATA = other.m_descriptionCDATA;
        m_address = other.m_address;
        m_phoneNumber = other.m_phoneNumber;
        m_styleUrl = other.m_styleUrl;
        m_popularity = other.m_popularity;
        m_popularityIndex = other.m_popularityIndex;
        m_visible = other.m_visible;
        m_role = other.m_role;
        m_style = other.m_style;
        m_styleMap = other.m_styleMap;
        m_timeSpan = other.m_timeSpan;
        m_timeStamp = other.m_timeStamp;
        m_visualCategory = other.m_visualCategory;
        m_extendedData = other.m_extendedData;
        m_region = other.m_region;
    }
    
    virtual GeoDataFeaturePrivate* copy()
    { 
        GeoDataFeaturePrivate* copy = new GeoDataFeaturePrivate;
        *copy = *this;
        return copy;
    }

    virtual EnumFeatureId featureId() const
    {
        return InvalidFeatureId;
    }

    virtual ~GeoDataFeaturePrivate()
    {
    }

    virtual const char* nodeType() const
    {
        return GeoDataTypes::GeoDataFeatureType;
    }

    static GeoDataStyle* createOsmPOIStyle( const QFont &font, const QString &bitmap, 
                                         const QColor &color = QColor( 0xBE, 0xAD, 0xAD ),
                                         const QColor &outline = QColor( 0xBE, 0xAD, 0xAD ).darker())
    {
        GeoDataStyle *style = new GeoDataStyle();
        QPixmap const pixmap = QPixmap( MarbleDirs::path( "bitmaps/poi/" + bitmap + ".png" ) );
        GeoDataPolyStyle polyStyle( color );
        GeoDataLineStyle lineStyle( outline );
        //if(color != Qt::transparent)
        polyStyle.setFill( true );
        polyStyle.setOutline( true );
        style->setIconStyle( GeoDataIconStyle( pixmap ) );
        style->setPolyStyle( polyStyle );
        style->setLineStyle( lineStyle );
        style->setLabelStyle( GeoDataLabelStyle( font, Qt::black ) );
        return style;
    }
    
    static GeoDataStyle* createHighwayStyle( const QColor& color, qreal width = 1, qreal realWidth = 0.0, 
                                             Qt::PenStyle penStyle = Qt::SolidLine, 
                                             Qt::PenCapStyle capStyle = Qt::RoundCap )
    {
        GeoDataStyle *style = new GeoDataStyle;
        GeoDataLineStyle lineStyle( color );
        GeoDataPolyStyle polyStyle( color.lighter() );
        lineStyle.setCapStyle( capStyle );
        lineStyle.setPenStyle( penStyle );
        lineStyle.setWidth( width );
        lineStyle.setPhysicalWidth( realWidth );
        style->setLineStyle( lineStyle );
        style->setPolyStyle( polyStyle );
        return style;
    }
    
    static GeoDataStyle* createWayStyle( const QColor& color, const QColor& outlineColor, 
                                         bool fill = true, bool outline = true,
                                         Qt::BrushStyle brushStyle = Qt::SolidPattern )
    {
        GeoDataStyle *style = new GeoDataStyle;
        GeoDataPolyStyle polyStyle( color );
        GeoDataLineStyle lineStyle( outlineColor );
        polyStyle.setFill( fill );
        polyStyle.setOutline( outline );
        polyStyle.setBrushStyle( brushStyle );
        style->setLineStyle( lineStyle );
        style->setPolyStyle( polyStyle );
        return style;
    }

    QString             m_name;         // Name of the feature. Is shown on screen
    QString             m_description;  // A longer textual description
    bool                m_descriptionCDATA; // True if description should be considered CDATA
    QString             m_address;      // The address.  Optional
    QString             m_phoneNumber;  // Phone         Optional
    QString             m_styleUrl;     // styleUrl     Url#tag to a document wide style
    GeoDataAbstractView m_abstractView; // AbstractView  Optional
    qint64              m_popularity;   // Population(!)
    int                 m_popularityIndex; // Index of population

    bool        m_visible;      // True if this feature should be shown.
    GeoDataFeature::GeoDataVisualCategory  m_visualCategory; // the visual category


    QString       m_role;

    GeoDataStyle*    m_style;
    GeoDataStyleMap* m_styleMap;

    GeoDataExtendedData m_extendedData;

    GeoDataTimeSpan  m_timeSpan;
    GeoDataTimeStamp m_timeStamp;

    GeoDataRegion m_region;
    
    QAtomicInt  ref;
};

} // namespace Marble

#endif

