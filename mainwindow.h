/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024-2025
 ******************************************************************/

/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see
 * <https://www.gnu.org/licenses/>.
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "profiledialog.h"
#include "tr_document.h"
#include "trdispoptiondialog.h"
#include "trmapview.h"
#include "trnetdock.h"
#include "trnodedock.h"
#include "trlinkdock.h"
#include "fileoptions.h"
#include "about.h"

#include <QMainWindow>
#include <QScrollArea>
#include <tr_import_osm.h>

#define DEF_MASK TR_MASK_SELECT_LINK|TR_MASK_CLASS_FILTER|0x0000000000090000

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void loadFile(const QString &file);

private slots:
    void on_actionOpen_triggered();

    void on_actionExit_triggered();

    void on_actionPrint_triggered();

    void on_actionSVG_triggered();

    void on_actionReset_triggered();

    void on_actionZoon_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionAboutQt_triggered();

    void on_actionAbout_triggered();

    void on_actionConfigure_triggered();

    void on_actionFonts_triggered();

    void on_loadWorld(const QString &filename, int shift);

    void on_updateWorld();

    void on_actionOptions_triggered();

    void on_actionDirectories_triggered();

    void on_actionSolar_triggered();

    void on_actionload_overlay_triggered();

    void on_actionsave_overlay_triggered();

public slots:
    void on_updateNetOptions(uint64_t flags);

    void on_updateFileOptions();

    void on_updateLayerView();

private:
    Ui::MainWindow *ui;

    QScrollArea * view;
    TrMapView * m_map_view;

    ProfileDialog * m_profile_dlg;
    FileOptions * m_file_options;
    TrDispOptionDialog * m_disp_option;
    About * m_about;

    TrNetDock * m_net_option;
    QDockWidget * m_net_dock;
    QDockWidget * m_element_dock;

    QFont m_font;

    void createNetObjects(const QStringList &list, TrImportOsm &filter);
    void createRoadNetObjects(const QStringList &list, TrImportOsm &filter);
    void createFaceObjects(const QStringList &list, TrImportOsm &filter);

    void writeSettings();
    void readSettings();
};
#endif // MAINWINDOW_H
