#include "include/gui/ViewConfiguration.h"
#include "ui_ViewConfiguration.h"

ViewConfiguration::ViewConfiguration(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ViewConfiguration)
{
    ui->setupUi(this);
}

ViewConfiguration::~ViewConfiguration()
{
    delete ui;
}

QCheckBox *ViewConfiguration::getCkColors(){
    return ui->ckColors;
}

QCheckBox *ViewConfiguration::getCkStdObserver(){
    return ui->ckStdObserver;
}

QCheckBox *ViewConfiguration::getCkIlluminant(){
    return ui->ckIlluminant;
}

QCheckBox *ViewConfiguration::getCkXYZ(){
    return ui->ckXYZ;
}

QCheckBox *ViewConfiguration::getCkxyY(){
    return ui->ckxyY;
}

QCheckBox *ViewConfiguration::getCkLuv(){
    return ui->ckLuv;
}

QCheckBox *ViewConfiguration::getCkLab(){
    return ui->ckLab;
}

QCheckBox *ViewConfiguration::getCkRGB(){
    return ui->ckRGB;
}

