//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2013      Bernhard Beschow <bbeschow@cs.tu-berlin.de>
//

#ifndef MARBLE_MOBILEGHNSDETAILSWIDGET_H
#define MARBLE_MOBILEGHNSDETAILSWIDGET_H

#include <QDialog>

#include "ui_MobileGhnsDetailsWidget.h"

#include <QModelIndex>

namespace Marble
{

class NewstuffModel;

class MobileGhnsDetailsDialog : public QDialog
{
    Q_OBJECT

public:
    MobileGhnsDetailsDialog( NewstuffModel *model, const QModelIndex &index, QWidget *parent = 0 );

private Q_SLOTS:
    void updateData( const QModelIndex &start, const QModelIndex &end );
    void startDownload();
    void cancelDownload();
    void uninstall();

private:
    void updateEphemeralValues();

private:
    Ui::MobileGhnsDetailsWidget m_ui;
    NewstuffModel *const m_model;
    const QModelIndex m_index;
};

}

#endif
