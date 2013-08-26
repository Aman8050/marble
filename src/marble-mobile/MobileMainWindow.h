//
// This file is part of the Marble Virtual Globe.
//
// This program is free software licensed under the GNU LGPL. You can
// find a copy of this license in LICENSE.txt in the top directory of
// the source code.
//
// Copyright 2006-2010 Torsten Rahn <tackat@kde.org>
// Copyright 2007      Inge Wallin  <ingwa@kde.org>
//


#ifndef MARBLE_MAINWINDOW_H
#define MARBLE_MAINWINDOW_H

#include <QMainWindow>
#include <QVariantMap>

#include "MapThemeManager.h"

class QAction;

namespace Marble
{

class DownloadRegionDialog;
class GoToDialog;
class LegendWidget;
class MarbleWidget;
class RoutingWidget;
class StackableWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

#ifdef Q_WS_MAEMO_5
    enum Orientation {
        OrientationAutorotate,
        OrientationLandscape,
        OrientationPortrait
    };
#endif

public:
    explicit MainWindow( const QString &marbleDataPath = QString(),
                         const QVariantMap &cmdLineSettings = QVariantMap(),
                         QWidget *parent = 0 );

    MarbleWidget *marbleWidget();

    void addGeoDataFile( const QString &path );

#ifdef Q_WS_MAEMO_5
    Orientation orientation() const;
#endif

protected:
    void closeEvent( QCloseEvent *event );

private Q_SLOTS:
    void initObject( const QVariantMap &cmdLineSettings );

    void fallBackToDefaultTheme();

    void setWorkOffline( bool );
    void setLegendShown( bool show );

    // Settings Menu
#ifdef Q_WS_MAEMO_5
    void setOrientation( Orientation orientation );
#endif

    // Download region dialog
    void connectDownloadRegionDialog();
    void disconnectDownloadRegionDialog();
    void downloadRegion();

    void showBookmarkManagerDialog();
    void showAboutMarbleDialog();
    void showDownloadRegionDialog();
    void showMapViewDialog();
    void showRoutingDialog();
    void showTrackingDialog();
    void showGoToDialog();

private:
    QString readMarbleDataPath();
    void readSettings( const QVariantMap &overrideSettings = QVariantMap() );
    void writeSettings();
    void initializeTrackingWidget();

    MarbleWidget *const m_marbleWidget;
    LegendWidget *const m_legendWidget;

    DownloadRegionDialog *m_downloadRegionDialog;
    QDialog *m_mapViewDialog;
    StackableWindow *m_routingWindow;
    StackableWindow *m_trackingWindow;
    GoToDialog *m_gotoDialog;
    RoutingWidget *m_routingWidget;

    QAction *m_workOfflineAct;
    QAction *m_showLegendAct;

    MapThemeManager m_mapThemeManager;
    QString m_lastFileOpenPath;
    QStringList m_commandlineFilePaths;
};

} // namespace Marble

#endif
