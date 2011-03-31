#ifndef COLORCHOOSEDOCK_H
#define COLORCHOOSEDOCK_H

#include <QDockWidget>
#include <QStringListModel>
#include <QListWidget>
#include <QDebug>

namespace Ui {
    class ColorChooseDock;
}

class ColorChooseDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ColorChooseDock(QWidget *parent = 0);
    ~ColorChooseDock();

    void clearColours();
    void addColor(int index, QString &color_name);
    QListWidget *getListWidgetColor1();
    QListWidget *getListWidgetColor2();

private:
    Ui::ColorChooseDock *ui;
};

#endif // COLORCHOOSEDOCK_H
