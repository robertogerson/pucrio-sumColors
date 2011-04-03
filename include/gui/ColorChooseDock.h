#ifndef COLORCHOOSEDOCK_H
#define COLORCHOOSEDOCK_H

#include <QDockWidget>
#include <QStringListModel>
#include <QListWidget>
#include <QComboBox>
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

    //TODO: Hide this functions. Transform in signals
    QListWidget *getListWidgetColor1();
    QListWidget *getListWidgetColor2();
    QComboBox *getIlluminantsComboBox();

    void clearIlluminants();
    void addIlluminant(QString illuminant);

private:
    Ui::ColorChooseDock *ui;

};

#endif // COLORCHOOSEDOCK_H
