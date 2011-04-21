#include "include/gui/About.h"
#include "ui_about.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);
    connect(ui->commandLinkButton, SIGNAL(clicked()), this, SLOT(aboutQt()));
}

void About::aboutQt(){
    QApplication::aboutQt();
}

About::~About()
{
    delete ui;
}
