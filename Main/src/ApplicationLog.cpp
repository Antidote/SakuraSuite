#include "include/ApplicationLog.hpp"
#include "ui_ApplicationLog.h"

ApplicationLog::ApplicationLog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationLog)
{
    ui->setupUi(this);
}

ApplicationLog::~ApplicationLog()
{
    delete ui;
}

void ApplicationLog::message(const QString& message)
{
    addEntry(Message, message);
}

void ApplicationLog::warning(const QString& warning)
{
    addEntry(Warning, warning);
}

void ApplicationLog::error(const QString& error)
{
    addEntry(Error, error);
}

void ApplicationLog::fatal(const QString& fatal)
{
    addEntry(Fatal, fatal);
}

void ApplicationLog::addEntry(ApplicationLog::Level level, const QString& message)
{
    m_messages << Entry{level, QDateTime::currentDateTime(), message};
    QTreeWidgetItem* item = new QTreeWidgetItem;
    switch(level)
    {
        case Message:
            item->setText(0, "Message");
            break;
        case Warning:
            item->setText(0, "Warning");
            break;
        case Error:
            item->setText(0, "Error");
            break;
        case Fatal:
            item->setText(0, "FATAL!!!!");
            break;
    }

    item->setText(1, message);
    item->setText(2, m_messages.last().dateTime.toLocalTime().toString());
    ui->treeWidget->addTopLevelItem(item);
    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
}
