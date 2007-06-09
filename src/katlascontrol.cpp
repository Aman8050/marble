//
// This file is part of the Marble Desktop Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2007 Torsten Rahn <tackat@kde.org>"
// Copyright 2007      Inge Wallin  <ingwa@kde.org>"
//

#include "katlascontrol.h"

#include <QtGui/QLayout>
#include <QtGui/QSplitter>
#include <QtGui/QStringListModel>

#include <lib/MarbleWidget.h>
#include <lib/MarbleModel.h>
#include "MarbleControlBox.h"


KAtlasControl::KAtlasControl(QWidget *parent)
   : QWidget(parent)
{
    setWindowTitle(tr("Marble - Desktop Globe"));
    //	resize(640, 640);
    resize( 680, 640 );

    m_control = new MarbleControlBox( this );
//    m_control->setFixedWidth( 185 );
    m_splitter = new QSplitter (this);

    m_marbleWidget = new MarbleWidget( this );

    m_marbleWidget->setSizePolicy( QSizePolicy( QSizePolicy::MinimumExpanding,
                                                QSizePolicy::MinimumExpanding ) );
    m_marbleWidget->setMinimumZoom( m_control->minimumZoom() );

    QVBoxLayout *vlayout = new QVBoxLayout( this );

    QHBoxLayout *hlayout = new QHBoxLayout();	

    m_splitter->addWidget( m_control );
    m_splitter->addWidget( m_marbleWidget );
    m_splitter->setSizes( QList<int>() << 180 << width()-180 );
    m_splitter->setStretchFactor(m_splitter->indexOf(m_control), 0);
    m_splitter->setStretchFactor(m_splitter->indexOf(m_marbleWidget), 1);

    vlayout->addWidget( m_splitter );
    vlayout->setMargin(0);

    m_control->addMarbleWidget( m_marbleWidget );
}


void KAtlasControl::zoomIn()
{
    m_marbleWidget->zoomIn();
}

void KAtlasControl::zoomOut()
{
    m_marbleWidget->zoomOut();
}

void KAtlasControl::moveLeft()
{
    m_marbleWidget->moveLeft();
}

void KAtlasControl::moveRight()
{
    m_marbleWidget->moveRight();
}

void KAtlasControl::moveUp()
{
    m_marbleWidget->moveUp();
}

void KAtlasControl::moveDown()
{
    m_marbleWidget->moveDown();
}


#include "katlascontrol.moc"
