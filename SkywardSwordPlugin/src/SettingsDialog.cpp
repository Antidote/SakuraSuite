#include "SettingsDialog.hpp"
#include "SettingsManager.hpp"
#include "ui_SettingsDialog.h"

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SettingsDialog)
{
    ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete ui;
}

void SettingsDialog::accept()
{
    SettingsManager* settings = SettingsManager::instance();

    QString regionBtn = ui->regionBtnGrp->checkedButton()->objectName();
    settings->setDefaultRegion(regionBtn == "ntscURB" ? SettingsManager::NTSCU :
                              (regionBtn == "ntscJRB" ? SettingsManager::NTSCJ :
                               SettingsManager::PAL));

    if (ui->ntscUNameLE->isModified() && !ui->ntscUNameLE->text().isEmpty())
        settings->setDefaultPlayerNameForRegion(SettingsManager::NTSCU, ui->ntscUNameLE->text());
    if (ui->ntscJNameLE->isModified() && !ui->ntscJNameLE->text().isEmpty())
        settings->setDefaultPlayerNameForRegion(SettingsManager::NTSCJ, ui->ntscJNameLE->text());
    if (ui->palNameLE->isModified() && !ui->palNameLE->text().isEmpty())
        settings->setDefaultPlayerNameForRegion(SettingsManager::PAL, ui->palNameLE->text());

    QDialog::accept();
}

void SettingsDialog::showEvent(QShowEvent* se)
{
    QDialog::showEvent(se);

    // Region Settings
    SettingsManager* settings = SettingsManager::instance();
    switch(settings->defaultRegion())
    {
        case SettingsManager::NTSCU: ui->ntscURB->setChecked(true); break;
        case SettingsManager::NTSCJ: ui->ntscJRB->setChecked(true); break;
        case SettingsManager::PAL:   ui->palRB  ->setChecked(true); break;
    }

    ui->ntscUNameLE->setText(settings->defaultPlayerNameForRegion(SettingsManager::NTSCU));
    ui->ntscJNameLE->setText(settings->defaultPlayerNameForRegion(SettingsManager::NTSCJ));
    ui->palNameLE  ->setText(settings->defaultPlayerNameForRegion(SettingsManager::PAL  ));
}
