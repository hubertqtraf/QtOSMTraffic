#ifndef TRNODEDOCK_H
#define TRNODEDOCK_H

#include <QWidget>
#include <tr_geo_point.h>

namespace Ui {
class TrNodeDock;
}

class TrNodeDock : public QWidget
{
    Q_OBJECT

public:
    explicit TrNodeDock(QWidget *parent = nullptr);
    ~TrNodeDock();

    void setData(TrGeoPoint *node);
private:
    Ui::TrNodeDock *ui;
};

#endif // TRNODEDOCK_H
