//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#include "MobileGhnsDownloadWidget.h"

#include "MobileGhnsDetailsWidget.h"

#include "NewstuffModel.h"

#include <QListView>

namespace Marble {

MobileGhnsDownloadWidget::MobileGhnsDownloadWidget( QWidget *parent ) :
    QDialog( parent ),
    m_model( 0 )
{
    m_ui.setupUi( this );

    m_ui.listView->setIconSize( QSize( 48, 48 ) );
    m_ui.listView->setAlternatingRowColors( true );

    connect( m_ui.listView, SIGNAL(clicked(QModelIndex)), this, SLOT(showDetails(QModelIndex)) );
}

void MobileGhnsDownloadWidget::setModel( NewstuffModel *model )
{
    m_model = model;
    m_ui.listView->setModel( model );
}

void MobileGhnsDownloadWidget::showDetails( const QModelIndex &index )
{
    QPointer<QDialog> dialog = createDetailsDialog( m_model, index );
#ifdef Q_WS_MAEMO_5
    dialog->setAttribute( Qt::WA_Maemo5StackedWindow );
    dialog->setWindowFlags( Qt::Window );
    dialog->setAttribute( Qt::WA_DeleteOnClose, true );
    dialog->show();
#else // Q_WS_MAEMO_5
    dialog->exec();
    delete dialog;
#endif // Q_WS_MAEMO_5
}

QDialog *MobileGhnsDownloadWidget::createDetailsDialog( NewstuffModel *model, const QModelIndex &index )
{
    return new MobileGhnsDetailsDialog( model, index, this );
}

}

#include "MobileGhnsDownloadWidget.moc"
