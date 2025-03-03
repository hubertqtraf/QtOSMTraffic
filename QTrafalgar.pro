QT       += core gui xml
QT       += printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += TEST

QMAKE_CXXFLAGS_WARN_ON += -Wno-unused-parameter

INCLUDEPATH += ./UI
INCLUDEPATH += ./geo
INCLUDEPATH += ./osm
INCLUDEPATH += ./trafalgar

SOURCES += \
    UI/about.cpp \
    UI/fileoptions.cpp \
    geo/geo_base.cpp \
    geo/geo_globe.cpp \
    geo/geo_lin.cpp \
    geo/geo_poly.cpp \
    geo/geo_ref.cpp \
    main.cpp \
    UI/mainwindow.cpp \
    osm/tr_import_osm.cpp \
    osm/tr_import_osm_rel.cpp \
    osm/tr_import_osm_stream.cpp \
    osm/tr_osm_link.cpp \
    UI/profile.cpp \
    UI/profiledialog.cpp \
    UI/tr_canvas.cpp \
    UI/tr_document.cpp \
    UI/tr_select_box.cpp \
    UI/tr_set_item.cpp \
    UI/tr_set_model.cpp \
    trafalgar/tr_geo_object.cpp \
    trafalgar/tr_geo_point.cpp \
    trafalgar/tr_geo_poly.cpp \
    trafalgar/tr_geo_segment.cpp \
    trafalgar/tr_layer.cpp \
    trafalgar/tr_map_face.cpp \
    trafalgar/tr_map_link.cpp \
    trafalgar/tr_map_link_road.cpp \
    trafalgar/tr_map_list.cpp \
    trafalgar/tr_map_net.cpp \
    trafalgar/tr_map_net_road.cpp \
    trafalgar/tr_map_node.cpp \
    trafalgar/tr_map_parklane.cpp \
    trafalgar/tr_map_poi.cpp \
    trafalgar/tr_map_transverse.cpp \
    trafalgar/tr_name_element.cpp \
    trafalgar/tr_stack.cpp \
    trafalgar/tr_zoom_map.cpp \
    UI/trdispoptiondialog.cpp \
    UI/trmapview.cpp \
    UI/trnetdock.cpp \
    UI/trlinkdock.cpp \
    UI/trnodedock.cpp

HEADERS += \
    UI/about.h \
    UI/fileoptions.h \
    geo/geo_base.h \
    geo/geo_errno.h \
    geo/geo_globe.h \
    geo/geo_lin.h \
    geo/geo_poly.h \
    geo/geo_ref.h \
    UI/mainwindow.h \
    osm/osm_load.h \
    osm/osm_load_rel.h \
    osm/osm_types.h \
    osm/tr_import_osm.h \
    osm/tr_import_osm_rel.h \
    osm/tr_import_osm_stream.h \
    osm/tr_osm_link.h \
    UI/profile.h \
    UI/profiledialog.h \
    UI/tr_canvas.h \
    UI/tr_document.h \
    UI/tr_select_box.h \
    UI/tr_set_item.h \
    UI/tr_set_model.h \
    trafalgar/tr_defs.h \
    trafalgar/tr_geo_object.h \
    trafalgar/tr_geo_point.h \
    trafalgar/tr_geo_poly.h \
    trafalgar/tr_geo_segment.h \
    trafalgar/tr_layer.h \
    trafalgar/tr_map_face.h \
    trafalgar/tr_map_link.h \
    trafalgar/tr_map_link_road.h \
    trafalgar/tr_map_list.h \
    trafalgar/tr_map_net.h \
    trafalgar/tr_map_net_road.h \
    trafalgar/tr_map_node.h \
    trafalgar/tr_map_parklane.h \
    trafalgar/tr_map_poi.h \
    trafalgar/tr_map_transverse.h \
    trafalgar/tr_name_element.h \
    trafalgar/tr_point.h \
    trafalgar/tr_prof_class_def.h \
    trafalgar/tr_stack.h \
    trafalgar/tr_zoom_map.h \
    UI/trdispoptiondialog.h \
    UI/trmapview.h \
    UI/trnetdock.h \
    UI/trlinkdock.h \
    UI/trnodedock.h

FORMS += \
    UI/about.ui \
    UI/fileoptions.ui \
    UI/mainwindow.ui \
    UI/profiledialog.ui \
    UI/trdispoptiondialog.ui \
    UI/trnetdock.ui  \
    UI/trlinkdock.ui \
    UI/trnodedock.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc
