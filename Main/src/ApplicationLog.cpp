#include "include/ApplicationLog.hpp"
#include "ui_ApplicationLog.h"
#include <QAbstractButton>
#include <QDebug>
#include <QFileDialog>
#include "Constants.hpp"
#include <iostream>

ApplicationLog* ApplicationLog::m_instance = NULL;
ApplicationLog::ApplicationLog() :
    ui(new Ui::ApplicationLog),
    m_stdoutMonitor(std::cout),
    m_stdlogMonitor(std::clog),
    m_stderrMonitor(std::cerr)
{
    ui->setupUi(this);
    connect(&m_stdoutMonitor, SIGNAL(messageRecieved(QString)), this, SLOT(onStdOut(QString)));
    connect(&m_stdlogMonitor, SIGNAL(messageRecieved(QString)), this, SLOT(onStdLog(QString)));
    connect(&m_stderrMonitor, SIGNAL(messageRecieved(QString)), this, SLOT(onStdErr(QString)));
}

ApplicationLog::~ApplicationLog()
{
    saveLog(Constants::SAKURASUITE_HOME_PATH + QDir::separator() + "shutdown.log");
    qInstallMessageHandler(0);
    delete ui;
    ui = NULL;
}

ApplicationLog*ApplicationLog::instance()
{
    if (!m_instance)
        m_instance = new ApplicationLog;
    return m_instance;
}

void ApplicationLog::debug(const QString& message)
{
    addEntry(Debug, message);
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

void ApplicationLog::saveLog(const QString& logFilename)
{
    QFile file(logFilename);
    if (file.open(QFile::WriteOnly))
    {
        QTextStream stream(&file);
        stream << Constants::SAKURASUITE_APP_NAME << " ";
#ifdef Q_OS_LINUX
        stream << "Linux ";
#elif Q_OS_MAC
        stream << "Mac ";
#elif Q_OS_WIN
        stream << "Windows ";
#endif
        stream << QSysInfo::WordSize << "bit\n";
        stream << "Built " << Constants::SAKURASUITE_BUILD_DATE << "\n";
        stream << "Version " << Constants::SAKURASUITE_APP_VERSION << "\n";

        foreach (Entry entry, m_messages)
        {
            switch (entry.level)
            {
                case Debug:
                    stream << "[Debug  ]: ";
                    break;
                case Warning:
                    stream << "[Warning]: ";
                    break;
                case Error:
                    stream << "[Error  ]: ";
                    break;
                case Fatal:
                    stream << "[Fatal  ]: ";
                    break;
            }

            stream << entry.dateTime.toLocalTime().toString() << " ";
            stream << entry.message << "\n";
        }
    }
}

void ApplicationLog::onButtonPressed()
{
    if (sender() == ui->saveLogPushButton)
    {
        QString rd = QSettings().value(Constants::Settings::SAKURASUITE_RECENT_DIRECTORY).toString();
        QString logFilename = QFileDialog::getSaveFileName(this, "Save log...", rd, "*.log");
        if (logFilename.isEmpty())
            return;

        if (QFileInfo(logFilename).suffix() != "log")
            logFilename += ".log";

        QSettings().setValue(Constants::Settings::SAKURASUITE_RECENT_DIRECTORY, QFileInfo(logFilename).absolutePath());
        saveLog(logFilename);
    }
}

void ApplicationLog::onStdOut(QString msg)
{
    debug(QString("[stdout] %1").arg(msg));
}

void ApplicationLog::onStdLog(QString msg)
{
    debug(QString("[stdlog] %1").arg(msg));
}

void ApplicationLog::onStdErr(QString msg)
{
    error(QString("[stderr] %1").arg(msg));
}

void ApplicationLog::addEntry(ApplicationLog::Level level, const QString& message)
{
    m_messages << Entry{level, QDateTime::currentDateTime(), message};
    if (!ui)
        return;

    QTreeWidgetItem* item = new QTreeWidgetItem;
    switch(level)
    {
        case Debug:
            item->setIcon(0, QIcon(":/icons/information-white.png"));
            break;
        case Warning:
            item->setIcon(0, QIcon(":/icons/exclamation-diamond.png"));
            break;
        case Error:
        case Fatal:
            item->setIcon(0, QIcon(":/icons/exclamation-red.png"));
            break;
    }

    item->setText(0, message);
    item->setText(1, m_messages.last().dateTime.toLocalTime().toString());
    ui->treeWidget->addTopLevelItem(item);
    ui->treeWidget->header()->resizeSections(QHeaderView::ResizeToContents);
}
