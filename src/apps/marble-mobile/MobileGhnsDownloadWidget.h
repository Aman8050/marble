//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#ifndef MARBLE_MOBILEGHNSDOWNLOADWIDGET_H
#define MARBLE_MOBILEGHNSDOWNLOADWIDGET_H

#include <QDialog>

#include "ui_MobileGhnsDownloadWidget.h"

class QModelIndex;

namespace Marble
{

class NewstuffModel;

class MobileGhnsDownloadWidget : public QDialog
{
    Q_OBJECT

public:
    explicit MobileGhnsDownloadWidget( QWidget *parent = 0 );

    void setModel( NewstuffModel *model );

protected:
    virtual QDialog *createDetailsDialog( NewstuffModel *model, const QModelIndex &index );

private Q_SLOTS:
    void showDetails( const QModelIndex &index );

private:
    Ui::MobileGhnsDownloadWidget m_ui;
    NewstuffModel *m_model;
};

}

#endif
