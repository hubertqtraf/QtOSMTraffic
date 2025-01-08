/******************************************************************
 * project:	OSM Traffic
 *
 * (C)		Schmid Hubert 2024
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

#include "trmapview.h"

#include <tr_map_list.h>
#include <tr_map_net_road.h>
#include <tr_document.h>
#include <QPrintDialog>

#include "tr_import_osm.h"

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "profiledialog.h"

#include <QDebug>
#include <QDockWidget>
#include <QFileDialog>
#include <QSettings>
#include <QPrinter>
#include <QFontDialog>
#include <QMessageBox>
//#include <QSvgGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_profile_dlg(nullptr)
    , m_file_options(nullptr)
    , m_disp_option(nullptr)
    , m_about(nullptr)
    , m_net_option(nullptr)
    , m_net_dock(nullptr)
{
    ui->setupUi(this);

    view = new QScrollArea(this);
    setCentralWidget(view);
    m_map_view = new TrMapView(view);

    view->setWidget(m_map_view);
    view->setWidgetResizable(true);

    m_file_options = new FileOptions(this);
    readSettings();

    m_map_view->getDocument().setFileName("unloaded.osm");
    m_profile_dlg = new ProfileDialog(this);
    QString profile = m_file_options->getProfileFileName();
    TR_INF << profile;
    m_profile_dlg->read(profile);
    connect(m_profile_dlg, &ProfileDialog::accepted, this,  &MainWindow::on_updateWorld);

    m_map_view->getDocument().addLayerType(m_profile_dlg->getElemStringList("modes"));

    QObject::connect(m_map_view, SIGNAL(sendMessage(const QString &, int)), statusBar(),
                            SLOT(showMessage(const QString &, int)));

    m_net_dock = new QDockWidget(tr("Networks"), this);
    m_net_option = new TrNetDock(this);
    m_net_dock->setWidget(m_net_option);
    addDockWidget(Qt::RightDockWidgetArea, m_net_dock);
    connect(m_net_option, SIGNAL(selectModeChanged(uint64_t)), this, SLOT(on_updateNetOptions(uint64_t)));
    ui->menuSettings->addAction(m_net_dock->toggleViewAction());

    m_node_dock = new QDockWidget(tr("Point"), this);
    m_node_option = new TrNodeDock(this);
    m_node_dock->setWidget(m_node_option);
    addDockWidget(Qt::RightDockWidgetArea, m_node_dock);
    ui->menuSettings->addAction(m_node_dock->toggleViewAction());
    m_map_view->setElementDock(m_node_option, 1);

    m_map_view->setFont(&m_font);

    window()->setWindowTitle("OSM Traffic: " + m_map_view->getDocument().getFileName());
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::createRoadNetObjects(const QStringList & list, TrImportOsm & filter)
{
    for(int i = 0; i < list.size(); i++)
    {
        TrMapNet * road_net = new TrMapNetRoad();
        filter.createNet(road_net, list[i]);
        m_map_view->getDocument().addMapLayerObjectByName(list[i], road_net);
    }
}

void MainWindow::createNetObjects(const QStringList & list, TrImportOsm & filter)
{
    for(int i = 0; i < list.size(); i++)
    {
        TrMapNet * net = new TrMapNet();
        filter.createNet(net, list[i]);
        m_map_view->getDocument().addMapLayerObjectByName(list[i], net);
    }
}

void MainWindow::createFaceObjects(const QStringList & list, TrImportOsm & filter)
{
    for(int i = 0; i < list.size(); i++)
    {
        TrMapList * tr_list = new TrMapList();
        if(filter.createFaceList(tr_list, list[i]))
            m_map_view->getDocument().addMapLayerObjectByName(list[i], tr_list);
    }
}

void MainWindow::writeSettings()
{
    QSettings settings("trafalgar", "QTraf");

    settings.beginGroup("MainWindow");
    settings.setValue("size", size());
    settings.setValue("pos", pos());
    settings.setValue("font", m_font.toString());
    settings.endGroup();
    if(m_file_options != nullptr)
        m_file_options->manageSettings(settings, false);
}

void MainWindow::readSettings()
{
    QSettings settings("trafalgar", "QTraf");

    settings.beginGroup("MainWindow");
    resize(settings.value("size", QSize(400, 400)).toSize());
    move(settings.value("pos", QPoint(200, 200)).toPoint());
    m_font.fromString(settings.value("font").toString());
    settings.endGroup();
    if(m_file_options != nullptr)
        m_file_options->manageSettings(settings, true);
}

void MainWindow::on_actionOpen_triggered()
{
    if(m_file_options == nullptr)
    {
        m_file_options = new FileOptions(this);
    }
    TR_INF << m_file_options->getOsmDir();
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open OSM File"),
              m_file_options->getOsmDir(), tr("OSM File (*.osm)"));
    on_loadWorld(fileName, m_file_options->getShiftOption());
}

void MainWindow::on_actionPrint_triggered()
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setCreator("Me");
    printer.setDocName("Test");
    QPrintDialog printDialog(&printer, this);
    if(printDialog.exec() == QDialog::Accepted)
    {
        QPainter painter;

        painter.begin(&printer);
        painter.setRenderHint(QPainter::Antialiasing);
        TR_INF << "DPI: " << printer.resolution();
        QRect printer_rec = printer.pageLayout().fullRectPixels(printer.resolution());
        double xscale = printer_rec.width() / double(m_map_view->width());
        double yscale = printer_rec.height() / double(m_map_view->height());
        double scale = qMin(xscale, yscale);
        painter.translate(printer_rec.x() + printer_rec.width()/2,
                        printer_rec.y() + printer_rec.height()/2);
        painter.scale(scale, scale);
        painter.translate(-m_map_view->width()/2, -m_map_view->height()/2);

        m_map_view->render(&painter);
        painter.end();
    }
}

void MainWindow::on_actionSVG_triggered()
{
    // TODO: file dialog
    /*QString path = "test.svg";

    QSvgGenerator generator;
    generator.setFileName(path);
    generator.setSize(QSize(width(), height()));
    generator.setViewBox(QRect(0, 0, width(), height()));
    generator.setTitle(tr("SVG Generator Trafgar Drawing"));
    generator.setDescription(tr("An SVG drawing created by the Qt SVG Generator"));
    m_map_view->paintSvg(generator);*/
}

void MainWindow::on_actionReset_triggered()
{
    m_map_view->resetZoom();
}

void MainWindow::on_actionZoon_in_triggered()
{
    m_map_view->zoomChange(true);
}

void MainWindow::on_actionZoom_out_triggered()
{
    m_map_view->zoomChange(false);
}

void MainWindow::on_actionExit_triggered()
{
    writeSettings();
    QApplication::quit();
}

void MainWindow::on_actionAboutQt_triggered()
{
    QApplication::aboutQt();
}

void MainWindow::on_actionAbout_triggered()
{
    if(!m_about)
    {
         m_about = new About(this);
    }
    m_about->show();
}

void MainWindow::on_actionConfigure_triggered()
{
    if(m_profile_dlg)
    {
        m_profile_dlg->show();
        QStringList list = m_profile_dlg->getElemStringList("modes");
        TR_INF << list;
        m_map_view->update();
    }
}

void MainWindow::on_actionFonts_triggered()
{
    bool ok;

    QString f = m_font.toString();
    if(f.size())
        m_font = QFontDialog::getFont(&ok, QFont(f, m_font.pointSize()), this);
    else
        m_font = QFontDialog::getFont(&ok, QFont("Helvetica [Cronyx]", 10), this);
    if(ok)
    {
        TR_INF << m_font.key();
        if(m_map_view != nullptr)
        {
            m_map_view->setFont(&m_font);
            m_map_view->update();
        }
    }
    else
    {
        // set the default (Helvetica [Cronyx])
    }
}

void MainWindow::on_loadWorld(const QString & filename, int shift)
{
    if(m_file_options == nullptr)
        return;

    setCursor(Qt::WaitCursor);
    if(m_map_view->getDocument().m_is_loaded)
    {
        // TODO: check if file name is same -> Dialog box?
        m_map_view->getDocument().clean();
        m_map_view->getDocument().addLayerType(m_profile_dlg->getElemStringList("modes"));
    }
    if(shift != 0)
        TrGeoObject::s_mask |= TR_MASK_MOVE_LINE;

    TrImportOsm osm_filter;
    if(osm_filter.read(filename, m_map_view->getDocument().getNameList(), 0) == false)
    {
        QMessageBox msgBox;
        msgBox.setIcon(QMessageBox::Critical);
        msgBox.setText("Document Error");
        msgBox.exec();
        unsetCursor();
        return;
    }

    QStringList rlist = m_profile_dlg->getElemStringList("layer", "roadnet");
    QStringList llist = m_profile_dlg->getElemStringList("layer", "net");
    createRoadNetObjects(rlist, osm_filter);
    createNetObjects(llist, osm_filter);
    QStringList flist = m_profile_dlg->getElemStringList("layer", "face");
    createFaceObjects(flist, osm_filter);
    m_map_view->getDocument().setFileName(filename);

    m_map_view->getDocument().m_is_loaded = true;

    TrMapList * poi_map = osm_filter.createPoiMap("poi");
    if(poi_map != nullptr)
        m_map_view->getDocument().addMapLayerObjectByName("poi", poi_map);
    m_map_view->setSettingsData(m_profile_dlg->getElemStringList("modes"),
                                m_profile_dlg->getElemStringList("layer"));

    on_updateWorld();
    on_updateLayerView();
    on_updateNetOptions(m_net_option->getNetFlags());

    m_map_view->recalcExtRect();

    // view option dialog
    if(m_disp_option == nullptr)
    {
        m_disp_option = new TrDispOptionDialog(this);
        connect(m_disp_option, &TrDispOptionDialog::accepted, this,  &MainWindow::on_updateLayerView);
    }
    QStringList alist = m_profile_dlg->getElemStringList("layer");
    m_disp_option->setLayerList(alist, m_map_view->getDocument().getLayerNames());
    for (int i = 0; i < m_disp_option->getViewList().size(); ++i)
    {
        m_disp_option->setLayerItemList(m_disp_option->getViewList()[i],
            m_profile_dlg->getElemStringList(m_disp_option->getViewList()[i]));
    }

    window()->setWindowTitle("OSM Traffic: " + m_map_view->getDocument().getFileName());
    m_map_view->setLoadedFlag(true);

    unsetCursor();

}

void MainWindow::on_updateWorld()
{
    QStringList llist = m_profile_dlg->getElemStringList("layer");
    ProfileDialog::TrMapCol b_map = m_profile_dlg->getElemColorMap("base");
    TR_INF << "all layers: " << llist;

    for(int i = 0; i < llist.size(); i++)
    {
        TrGeoObject * obj = m_map_view->getDocument().getLayerObjectByName(llist[i]);
        if(obj != nullptr)
        {
            ProfileDialog::TrMapCol c_map = m_profile_dlg->getElemColorMap(llist[i]);
            obj->setColorGroup(llist[i], c_map);
            obj->setColorGroup("base", b_map);
        }
    }
    m_map_view->setBackgroundColor(b_map);
    m_map_view->initObjects(TR_INIT_COLORS);

    m_map_view->update();
}

void MainWindow::on_updateLayerView()
{
    QMap<QString, uint64_t> layers;

    if(m_disp_option != nullptr)
    {
        m_disp_option->getLayerMasks(layers);
        QStringList list = m_disp_option->getLayerActList();
        TR_INF << m_disp_option->getLayerActList();
        // this will show the layers on given string list
        m_map_view->getDocument().setFunctionOrder(list, "draw");
    }
    m_map_view->getDocument().setLayerItemData(layers);
}

void MainWindow::on_updateNetOptions(uint64_t flags)
{
    TrGeoObject::setGlobelFlags(flags);
    m_map_view->getDocument().setMask(flags);
    m_map_view->initObjects(TR_INIT_GEOMETRY);
    m_map_view->update();
}

void MainWindow::on_actionOptions_triggered()
{
    if(m_disp_option == nullptr)
    {
        m_disp_option = new TrDispOptionDialog(this);
    }
    m_disp_option->show();
}

void MainWindow::on_actionDirectories_triggered()
{
    if(m_file_options == nullptr)
    {
        m_file_options = new FileOptions(this);
    }
    m_file_options->show();
}
