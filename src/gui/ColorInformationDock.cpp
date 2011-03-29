#include "include/gui/ColorInformationDock.h"
#include "ui_colorinformationdock.h"

ColorInformationDock::ColorInformationDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ColorInformationDock) {

    ui->setupUi(this);
}

ColorInformationDock::~ColorInformationDock() {
    delete ui;
}

void ColorInformationDock::setCIEXYZ(double X, double Y, double Z) {
    ui->CIEXYZ_X->setText(QString::number(X, 10, 4));
    ui->CIEXYZ_Y->setText(QString::number(Y, 10, 4));
    ui->CIEXYZ_Z->setText(QString::number(Z, 10, 4));
}

void ColorInformationDock::setCIEXYZ_2(double X, double Y, double Z) {
    ui->CIEXYZ_X_2->setText(QString::number(X, 10, 4));
    ui->CIEXYZ_Y_2->setText(QString::number(Y, 10, 4));
    ui->CIEXYZ_Z_2->setText(QString::number(Z, 10, 4));
}

void ColorInformationDock::setCIEXYZ_Res(double X, double Y, double Z) {
    ui->CIEXYZ_X_3->setText(QString::number(X, 10, 4));
    ui->CIEXYZ_Y_3->setText(QString::number(Y, 10, 4));
    ui->CIEXYZ_Z_3->setText(QString::number(Z, 10, 4));
}

void ColorInformationDock::setCIExyY(double x, double y, double Y) {
    ui->CIExy_x->setText(QString::number(x, 10, 4));
    ui->CIExy_y->setText(QString::number(y, 10, 4));
    ui->CIExy_Y->setText(QString::number(Y, 10, 4));
}

void ColorInformationDock::setCIExyY_2(double x, double y, double Y) {
    ui->CIExy_x_2->setText(QString::number(x, 10, 4));
    ui->CIExy_y_2->setText(QString::number(y, 10, 4));
    ui->CIExy_Y_2->setText(QString::number(Y, 10, 4));
}

void ColorInformationDock::setCIExyY_Res(double x, double y, double Y) {
    ui->CIExy_x_3->setText(QString::number(x, 10, 4));
    ui->CIExy_y_3->setText(QString::number(y, 10, 4));
    ui->CIExy_Y_3->setText(QString::number(Y, 10, 4));
}

void ColorInformationDock::setsRGB(int R, int G, int B) {
    ui->sRGB_R->setText(QString::number(R));
    ui->sRGB_G->setText(QString::number(G));
    ui->sRGB_B->setText(QString::number(B));
}

void ColorInformationDock::setsRGB_2(int R, int G, int B) {
    ui->sRGB_R_2->setText(QString::number(R));
    ui->sRGB_G_2->setText(QString::number(G));
    ui->sRGB_B_2->setText(QString::number(B));
}

void ColorInformationDock::setsRGB_Res(int R, int G, int B) {
    ui->sRGB_R_3->setText(QString::number(R));
    ui->sRGB_G_3->setText(QString::number(G));
    ui->sRGB_B_3->setText(QString::number(B));
}

void ColorInformationDock::setSampleColor(int R, int G, int B) {
    QString bgColorStyle("background-color:rgb(");
    bgColorStyle +=  QString::number(R);
    bgColorStyle += ",";

    bgColorStyle += QString::number(G);
    bgColorStyle += ",";

    bgColorStyle += QString::number(B);
    bgColorStyle += ");";

    ui->sampleColor->setStyleSheet(bgColorStyle);
}

void ColorInformationDock::setSampleColor_2(int R, int G, int B) {
    QString bgColorStyle("background-color:rgb(");
    bgColorStyle +=  QString::number(R);
    bgColorStyle += ",";

    bgColorStyle += QString::number(G);
    bgColorStyle += ",";

    bgColorStyle += QString::number(B);
    bgColorStyle += ");";

    ui->sampleColor_2->setStyleSheet(bgColorStyle);
}

void ColorInformationDock::setSampleColor_Res(int R, int G, int B) {
    QString bgColorStyle("background-color:rgb(");
    bgColorStyle +=  QString::number(R);
    bgColorStyle += ",";

    bgColorStyle += QString::number(G);
    bgColorStyle += ",";

    bgColorStyle += QString::number(B);
    bgColorStyle += ");";

    ui->sampleColor_3->setStyleSheet(bgColorStyle);
}

