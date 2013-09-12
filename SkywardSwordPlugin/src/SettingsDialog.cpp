// This file is part of WiiKing2 Editor.
//
// WiiKing2 Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Wiiking2 Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WiiKing2 Editor.  If not, see <http://www.gnu.org/licenses/>

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
