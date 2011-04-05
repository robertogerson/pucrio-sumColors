#ifndef VIEWCONFIGURATION_H
#define VIEWCONFIGURATION_H

#include <QDockWidget>
#include <QCheckBox>

namespace Ui {
    class ViewConfiguration;
}

class ViewConfiguration : public QDockWidget
{
    Q_OBJECT

public:
    explicit ViewConfiguration(QWidget *parent = 0);
    ~ViewConfiguration();

    QCheckBox *getCkColors();
    QCheckBox *getCkStdObserver();
    QCheckBox *getCkIlluminant();
    QCheckBox *getCkXYZ();
    QCheckBox *getCkxyY();
    QCheckBox *getCkLuv();
    QCheckBox *getCkLab();
    QCheckBox *getCkRGB();

private:
    Ui::ViewConfiguration *ui;
};

#endif // VIEWCONFIGURATION_H
