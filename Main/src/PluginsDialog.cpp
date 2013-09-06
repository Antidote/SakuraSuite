#include "PluginsDialog.hpp"
#include "ui_PluginsDialog.h"
#include "PluginInterface.hpp"
#include "PluginsManager.hpp"
#include <QDebug>

PluginsDialog::PluginsDialog(QWidget *parent, PluginsManager* pluginsManager) :
    QDialog(parent),
    ui(new Ui::PluginsDialog),
    m_pluginsManager(pluginsManager)
{
    ui->setupUi(this);
    ui->groupBox->hide();
}

PluginsDialog::~PluginsDialog()
{
    delete ui;
}

void PluginsDialog::showEvent(QShowEvent* se)
{
    QDialog::showEvent(se);

    QTreeWidget* tw = ui->treeWidget;
    tw->clear();
    foreach(PluginInterface* plugin, m_pluginsManager->plugins())
    {
        QTreeWidgetItem* item = new QTreeWidgetItem;
        item->setText(NameColumn, plugin->name());
        item->setText(VersionColumn, plugin->version());
        item->setText(ExtensionColumn, plugin->extension());
        item->setCheckState(EnabledColumn, (plugin->enabled() ? Qt::Checked : Qt::Unchecked));
        tw->addTopLevelItem(item);
    }
}

void PluginsDialog::onItemSelectionChanged()
{
    if (!ui->treeWidget->currentItem())
    {
        ui->groupBox->hide();
        return;
    }

    qDebug() << ui->treeWidget->currentItem()->text(0);
    PluginInterface* plugin = m_pluginsManager->pluginByName(ui->treeWidget->currentItem()->text(0));
    if (!plugin)
        return;
    ui->authorValue->setText(plugin->author());
    ui->websiteValue->setText("<a href=\""+plugin->website() + "\">" + plugin->website() + "</a>");
    ui->pathValue->setText(plugin->path());
    ui->licenseValue->setText(plugin->license());
    ui->descriptionTextEdit->setHtml(plugin->description());
    ui->groupBox->show();
}

void PluginsDialog::onItemClicked(QTreeWidgetItem* item, int column)
{
    if (!item)
        return;

    if (column == EnabledColumn)
    {
        PluginInterface* plugin = m_pluginsManager->pluginByName(item->text(0));
        if (!plugin)
            return;

        bool isChecked = (item->checkState(EnabledColumn) == Qt::Checked);

        if (isChecked != plugin->enabled())
        {
            qDebug() << (isChecked ? "Enabled" : "Disabled") <<  plugin->name();
            plugin->setEnabled(isChecked);
        }
    }
}

