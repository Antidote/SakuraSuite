#include "AboutDialog.hpp"
#include "ui_AboutDialog.h"
#include "Constants.hpp"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AboutDialog)
{
    ui->setupUi(this);
    this->setWindowTitle(Constants::WIIKING2_ABOUT_TITLE.arg(Constants::WIIKING2_TITLE));
    ui->applicationNameLbl->setText(Constants::WIIKING2_APP_NAME);
    ui->applicationVersionLbl->setText(Constants::WIIKING2_APP_VERSION);
    ui->buildDateLbl->setText(Constants::WIIKING2_BUILD_DATE);
}

AboutDialog::~AboutDialog()
{
    delete ui;
}
