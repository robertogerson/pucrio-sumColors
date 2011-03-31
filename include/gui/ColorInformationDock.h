#ifndef COLORINFORMATIONDOCK_H
#define COLORINFORMATIONDOCK_H

#include <QDockWidget>
#include <QListWidget>
#include <QDebug>

namespace Ui {
    class ColorInformationDock;
}

class ColorInformationDock : public QDockWidget
{
    Q_OBJECT

public:
    explicit ColorInformationDock(QWidget *parent = 0);
    ~ColorInformationDock();

    void setCIEXYZ(double X, double Y, double Z);
    void setCIExyY(double x, double y, double Y);
    void setsRGB(int R, int G, int B);
    void setCIELab(double L, double a, double b);
    void setCIELuv(double L, double u, double v);
    void setSampleColor(int R, int G, int B);

    void setCIEXYZ_2(double X, double Y, double Z);
    void setCIExyY_2(double x, double y, double Y);
    void setsRGB_2(int R, int G, int B);
    void setCIELab_2(double L, double a, double b);
    void setCIELuv_2(double L, double u, double v);
    void setSampleColor_2(int R, int G, int B);

    void setCIEXYZ_Res(double X, double Y, double Z);
    void setCIExyY_Res(double x, double y, double Y);
    void setsRGB_Res(int R, int G, int B);
    void setCIELab_Res(double L, double a, double b);
    void setCIELuv_Res(double L, double u, double v);
    void setSampleColor_Res(int R, int G, int B);

private:
    Ui::ColorInformationDock *ui;
};

#endif // COLORINFORMATIONDOCK_H
