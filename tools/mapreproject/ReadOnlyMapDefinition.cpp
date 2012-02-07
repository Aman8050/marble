#include "ReadOnlyMapDefinition.h"

#include "NwwMapImage.h"
#include "SimpleMapImage.h"

ReadOnlyMapDefinition::ReadOnlyMapDefinition()
    : m_mapType( UnknownMapSource ),
      m_interpolationMethod( UnknownInterpolation ),
      m_baseDirectory(),
      m_tileLevel( -1 ),
      m_cacheSizeBytes(),
      m_filename()
{
}

ReadOnlyMapImage * ReadOnlyMapDefinition::createReadOnlyMap() const
{
    if ( m_mapType == NasaWorldWindMap ) {
        NwwMapImage * const source = new NwwMapImage( m_baseDirectory, m_tileLevel );
        source->setInterpolationMethod( m_interpolationMethod );
        source->setCacheSizeBytes( m_cacheSizeBytes );
        return source;
    }
    else if ( m_mapType == BathymetryMap ) {
        SimpleMapImage * const source = new SimpleMapImage( m_filename );
        return source;
    }
    else
        return NULL;
}
