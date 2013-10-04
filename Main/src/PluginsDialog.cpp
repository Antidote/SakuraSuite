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

#include "PluginsDialog.hpp"
#include "ui_PluginsDialog.h"
#include "PluginInterface.hpp"
#include "PluginsManager.hpp"
#include "Constants.hpp"
#include <Updater.hpp>
#include <QMessageBox>

PluginsDialog::PluginsDialog(QWidget *parent, PluginsManager* pluginsManager) :
    QDialog(parent),
    ui(new Ui::PluginsDialog),
    m_pluginsManager(pluginsManager)
{
    ui->setupUi(this);
    ui->groupBox->setEnabled(false);

    connect(&m_statusTimer, SIGNAL(timeout()), this, SLOT(onTimeOut()));
}

PluginsDialog::~PluginsDialog()
{
    delete ui;
}

void PluginsDialog::showEvent(QShowEvent* se)
{
    QDialog::showEvent(se);

    updatePluginData();
}

void PluginsDialog::updatePluginData()
{
    QTreeWidget* tw = ui->treeWidget;
    tw->clear();
    ui->authorValue->clear();
    ui->descriptionTextEdit->clear();
    ui->websiteValue->clear();
    ui->pathValue->clear();
    ui->licenseValue->clear();
    ui->descriptionTextEdit->clear();
    foreach(PluginInterface* plugin, m_pluginsManager->plugins())
    {
        if (!plugin)
            continue;

        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setIcon(NameColumn, plugin->icon());
        item->setText(NameColumn, plugin->name());
        item->setText(VersionColumn, plugin->version());
        item->setText(ExtensionColumn, plugin->extension());
        item->setCheckState(EnabledColumn, (plugin->enabled() ? Qt::Checked : Qt::Unchecked));

        if (plugin->hasUpdater() && plugin->updater())
            connect(plugin->updater(), SIGNAL(warning(QString)), this, SLOT(onPluginWarning(QString)));

        tw->addTopLevelItem(item);
    }
    ui->groupBox->setEnabled(false);
}

void PluginsDialog::onItemSelectionChanged()
{
    if (!ui->treeWidget->currentItem())
    {
        ui->groupBox->setEnabled(false);
        return;
    }

    PluginInterface* plugin = m_pluginsManager->plugin(ui->treeWidget->currentItem()->text(0));
    if (!plugin)
        return;
    ui->authorValue->setText(plugin->author());
    ui->websiteValue->setText("<a href=\""+plugin->website() + "\">" + plugin->website() + "</a>");
    ui->pathValue->setText(plugin->path());
    ui->licenseValue->setText(plugin->license());
    ui->descriptionTextEdit->setHtml(plugin->description());
    ui->settingsPushButton->setEnabled(plugin->settingsDialog() != NULL);
    ui->updatePushButton->setEnabled(plugin->hasUpdater());
    ui->groupBox->setEnabled(true);
}

void PluginsDialog::onItemClicked(QTreeWidgetItem* item, int column)
{
    if (!item)
        return;

    if (column == EnabledColumn)
    {
        PluginInterface* plugin = m_pluginsManager->plugin(item->text(0));
        if (!plugin)
            return;

        bool isChecked = (item->checkState(EnabledColumn) == Qt::Checked);

        if (isChecked != plugin->enabled())
            plugin->setEnabled(isChecked);
    }
}

void PluginsDialog::onSettingsClicked()
{
    if (!ui->treeWidget->currentItem())
        return;

    PluginInterface* plugin = m_pluginsManager->plugin(ui->treeWidget->currentItem()->text(0));
    if (!plugin)
        return;

    if (plugin->settingsDialog())
    {
        plugin->settingsDialog()->exec();
    }
}

void PluginsDialog::onReloadPlugin()
{
    if (!ui->treeWidget->currentItem())
        return;

    QMessageBox mbox;
    mbox.setWindowTitle(Constants::SAKURASUITE_PLUGIN_RELOAD_WARNING);
    mbox.setText(Constants::SAKURASUITE_PLUGIN_RELOAD_WARNING_MSG);
    mbox.setStandardButtons(QMessageBox::Cancel | QMessageBox::Ok);
    mbox.exec();

    if (mbox.result() == QMessageBox::Cancel)
        return;

    if (m_pluginsManager->reloadByName(ui->treeWidget->currentItem()->text(0)))
    {
        this->setStatusTip("Reloaded plugin");
    }
    else
    {
        this->setStatusTip("Failed to reload plugin");
    }

    updatePluginData();
}

void PluginsDialog::onCheckUpdate()
{
    if (!ui->treeWidget->currentItem())
        return;

    PluginInterface* plugin = m_pluginsManager->plugin(ui->treeWidget->currentItem()->text(0));
    if (!plugin)
        return;

    plugin->doUpdate();
}

void PluginsDialog::onPluginWarning(QString warning)
{
    ui->statusLabel->setText(ui->statusLabel->text() + "<br />" + warning);
    m_statusTimer.start(2000);
}

void PluginsDialog::onTimeOut()
{
    ui->statusLabel->clear();
}

