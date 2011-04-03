#include "include/gui/ColorChooseDock.h"
#include "ui_ColorChooseDock.h"

ColorChooseDock::ColorChooseDock(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::ColorChooseDock) {
    ui->setupUi(this);
}

ColorChooseDock::~ColorChooseDock() {
    delete ui;
}

void ColorChooseDock::clearColours() {
    ui->listWidgetColor1->clear();
    ui->listWidgetColor2->clear();
}

void ColorChooseDock::addColor( int index, QString &color_name
                                /*, int R, int G, int B*/) {
    ui->listWidgetColor1->insertItem(index, color_name);
    ui->listWidgetColor2->insertItem(index, color_name);

//    QFrame *frame = new QFrame(layout()->widget());
//    frame->setStyleSheet("background-color: red");
}

QListWidget *ColorChooseDock::getListWidgetColor1(){
    return ui->listWidgetColor1;
}

QListWidget *ColorChooseDock::getListWidgetColor2(){
    return ui->listWidgetColor2;
}

void ColorChooseDock::clearIlluminants(){
    ui->comboBox->clear();
}

void ColorChooseDock::addIlluminant(QString illuminant){
    ui->comboBox->addItem(illuminant);
}

QComboBox * ColorChooseDock::getIlluminantsComboBox(){
    return ui->comboBox;
}
