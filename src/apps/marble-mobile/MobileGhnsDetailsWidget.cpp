//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#include "MobileGhnsDetailsWidget.h"

#include "NewstuffModel.h"

namespace Marble {

MobileGhnsDetailsDialog::MobileGhnsDetailsDialog( NewstuffModel *model, const QModelIndex &index, QWidget *parent ) :
    QDialog( parent ),
    m_model( model ),
    m_index( index )
{
    m_ui.setupUi( this );

    connect( model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateData(QModelIndex,QModelIndex)) );
    connect( m_ui.downloadButton, SIGNAL(clicked()), this, SLOT(startDownload()) );
    connect( m_ui.cancelButton, SIGNAL(clicked()), this, SLOT(cancelDownload()) );
    connect( m_ui.uninstallButton, SIGNAL(clicked()), this, SLOT(uninstall()) );

    m_ui.nameLabel->setText( "<b>" + m_model->data( index, NewstuffModel::Name ).toString() + "</b>" );
    m_ui.descriptionLabel->setText( "<i>" + m_model->data( index, NewstuffModel::Summary ).toString() + "</i>" );
    m_ui.licenseLabel->setText( m_model->data( index, NewstuffModel::License ).toString() );
    m_ui.authorLabel->setText( m_model->data( index, NewstuffModel::Author ).toString() );
    m_ui.versionLabel->setText( m_model->data( index, NewstuffModel::Version ).toString() );
    m_ui.releaseDateLabel->setText( m_model->data( index, NewstuffModel::ReleaseDate ).toString() );

    updateEphemeralValues();
}

void MobileGhnsDetailsDialog::updateData( const QModelIndex &start, const QModelIndex &end )
{
    if ( m_index.row() < start.row() )
        return;

    if ( m_index.row() > end.row() )
        return;

    updateEphemeralValues();
}

void MobileGhnsDetailsDialog::startDownload()
{
    m_model->install( m_index.row() );

    updateEphemeralValues();
}

void MobileGhnsDetailsDialog::cancelDownload()
{
    m_model->cancel( m_index.row() );

    updateEphemeralValues();
}

void MobileGhnsDetailsDialog::uninstall()
{
    m_ui.uninstallButton->setEnabled( false );
    m_model->uninstall( m_index.row() );
}

void MobileGhnsDetailsDialog::updateEphemeralValues()
{
    m_ui.previewImage->setPixmap( m_model->data( m_index, Qt::DecorationRole ).value<QIcon>().pixmap( 150 ) );
    const qint64 numBytes = m_model->data( m_index, NewstuffModel::PayloadSize ).value<qint64>();
    const qreal megaBytes = numBytes / 1024.0 / 1024.0;

    m_ui.sizeLabel->setText( megaBytes > 0 ? QString( "%1 MB" ).arg( megaBytes, 0, 'f', 1 ) : QString() );

    const bool isInstalled = m_model->data( m_index, NewstuffModel::IsInstalled ).toBool();
    const bool isUpgradable = m_model->data( m_index, NewstuffModel::IsUpgradable ).toBool();
    const bool isTransitioning = m_model->data( m_index, NewstuffModel::IsTransitioning ).toBool();
    m_ui.downloadProgress->setVisible( isTransitioning );
    m_ui.downloadButton->setVisible( !isTransitioning );
    m_ui.cancelButton->setVisible( (!isInstalled) || isTransitioning );
    m_ui.uninstallButton->setVisible( isInstalled && (!isTransitioning) );

    m_ui.downloadProgress->setValue( ( m_model->data( m_index, NewstuffModel::DownloadedSize ).value<qint64>() * 100 ) / numBytes );
    m_ui.downloadButton->setText( isInstalled ? tr( "Upgrade" ) : tr( "Install" ) );
    m_ui.downloadButton->setEnabled( (!isInstalled) || isUpgradable );
    m_ui.cancelButton->setEnabled( isTransitioning );
    m_ui.uninstallButton->setEnabled( isInstalled );
}

}

#include "MobileGhnsDetailsWidget.moc"
