#ifndef TRWORLDLAYER_H
#define TRWORLDLAYER_H

#include <QWidget>

namespace Ui {
class TrWorldLayer;
}

class TrWorldLayer : public QWidget
{
    Q_OBJECT

public:
    explicit TrWorldLayer(QWidget *parent = nullptr);
    ~TrWorldLayer();

private:
    Ui::TrWorldLayer *ui;
};

#endif // TRWORLDLAYER_H
