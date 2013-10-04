// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#include "SettingsDialog.hpp"
#include "SettingsManager.hpp"
#include "Constants.hpp"
#include "ui_SettingsDialog.h"

#include <QSettings>

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
    if (ui->updateUrlLineEdit->isModified() && !ui->updateUrlLineEdit->text().isEmpty())
        settings->setUpdateUrl(ui->updateUrlLineEdit->text());
    settings->setUpdateCheckOnStart(ui->checkOnStart->isChecked());
    QDialog::accept();
}

void SettingsDialog::onTextChanged(QString text)
{
    QRegExp http("^(http|https)://", Qt::CaseInsensitive);
    if (sender() == ui->updateUrlLineEdit)
    {
        if (text.isEmpty() || !text.contains(http))
        {
            ui->updateUrlLineEdit->setProperty("valid", false);
            ui->statusLabel->setText("Invalid url");
        }
        else
        {
            ui->updateUrlLineEdit->setProperty("valid", true);
            ui->statusLabel->clear();
        }
        style()->polish(ui->updateUrlLineEdit);

        // If the text matches what is currently stored
        // Set the line edit is unmodified
        if (text == QSettings().value(Constants::Settings::SKYWARDSWORD_UPDATE_URL, Constants::Settings::SKYWARDSWORD_UPDATE_URL_DEFAULT).toString())
            ui->updateUrlLineEdit->setModified(false);
    }
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

    ui->ntscUNameLE      ->setText(settings->defaultPlayerNameForRegion(SettingsManager::NTSCU));
    ui->ntscUNameLE      ->setModified(false);
    ui->ntscJNameLE      ->setText(settings->defaultPlayerNameForRegion(SettingsManager::NTSCJ));
    ui->ntscJNameLE      ->setModified(false);
    ui->palNameLE        ->setText(settings->defaultPlayerNameForRegion(SettingsManager::PAL  ));
    ui->palNameLE        ->setModified(false);
    ui->updateUrlLineEdit->setText(settings->updateUrl());
    ui->updateUrlLineEdit->setModified(false);
    ui->checkOnStart     ->setChecked(settings->updateCheckOnStart());
}
