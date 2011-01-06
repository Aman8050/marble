//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2011 Utku Aydın <utkuaydin34@gmail.com>
//

#include "DgmlLayerTagWriter.h"

#include "GeoSceneTypes.h"
#include "GeoWriter.h"
#include "GeoSceneLayer.h"
#include "DgmlElementDictionary.h"
#include <GeoSceneTexture.h>

namespace Marble
{

static GeoTagWriterRegistrar s_writerData( GeoTagWriter::QualifiedName( GeoSceneTypes::GeoSceneLayerType,
                                                                            dgml::dgmlTag_nameSpace20 ),
                                               new DgmlLayerTagWriter() );


bool DgmlLayerTagWriter::write(const GeoNode *node, GeoWriter& writer) const
{
    const GeoSceneLayer *layer = static_cast<const GeoSceneLayer*>( node );
    writer.writeStartElement( dgml::dgmlTag_Layer );
    
    GeoSceneAbstractDataset** iterator = layer->datasets().begin();
    while( iterator != layer->datasets().end() )
    {
        GeoSceneAbstractDataset* dataset = *iterator;
        writeElement( dataset, writer );
        ++iterator;
    }
    writer.writeEndElement();
    return true;
}

};