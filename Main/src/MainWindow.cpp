#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "GameFile.hpp"
#include "Constants.hpp"
#include "PluginsManager.hpp"
#include "PluginInterface.hpp"


#include <QDebug>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentFile(NULL),
    m_pluginsManager(new PluginsManager(this))
{
    ui->setupUi(this);
    ui->documentList->addAction(ui->actionClose);
    this->setWindowTitle(tr(Constants::WIIKING2_TITLE));
    // Add default filter
    m_fileFilters << "All Files *.* (*.*)";

    // lets load the plugins
    m_pluginsManager->loadPlugins();

    // Setup the MRU list
    m_recentFileSeparator = ui->menuRecentFiles->insertSeparator(ui->actionClearRecent);
    for (int i = 0; i < MAXRECENT; ++i)
    {
        QAction* act = new QAction(this);
        act->setText(tr("&%1 %2").arg(i + 1).arg("test"));
        //act->setVisible(false);
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_recentFileActions.append(act);
        ui->menuRecentFiles->insertAction(m_recentFileSeparator, m_recentFileActions[i]);
    }

    // Now load the MRU
    updateRecentFileActions();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::addFileFilter(const QString& filter)
{
    if (m_fileFilters.contains(filter))
        return;
    qDebug() << "Adding filter" << filter;
    m_fileFilters << filter;
}

void MainWindow::removeFileFilter(const QString& filter)
{
    if (!m_fileFilters.contains(filter))
        return;

    qDebug() << "Removing filter" << filter;
    m_fileFilters.removeOne(filter);
}

void MainWindow::openFile(const QString& currentFile)
{
    if (m_documents.contains(currentFile))
    {
        QMessageBox msgBox(this);
        msgBox.setWindowTitle(tr(Constants::WIIKING2_ALREADY_OPENED_NAG));
        msgBox.setText(tr(Constants::WIIKING2_ALREADY_OPENED_NAG_MSG).arg(strippedName(currentFile)));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    PluginInterface* loader = m_pluginsManager->preferredPlugin(currentFile);
    if (!loader)
        return;

    GameFile* file = loader->loadFile(currentFile);

    // TODO: Show message upon failure
    if (!file)
        return;

    m_documents[currentFile] = file;
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(FILENAME, m_documents[currentFile]->fileName());
    item->setData(FILEPATH, m_documents[currentFile]->filePath());
    ui->documentList->addItem(item);
    updateMRU(currentFile);
    QSettings settings;
    settings.setValue(tr(Constants::Settings::WIIKING2_RECENT_DIRECTORY), QFileInfo(currentFile).absolutePath());
}

void MainWindow::onDocumentChanged()
{
    if (!ui->documentList->currentItem())
    {
        m_currentFile = NULL;
        return;
    }
    // First we need to store the old Widget so we can remove
    // it from the main layout.
    GameFile* oldFile = m_currentFile;
    m_currentFile = m_documents[ui->documentList->currentItem()->data(FILEPATH).toString()];

    setWindowTitle(tr(Constants::WIIKING2_TITLE_FILE).arg(m_currentFile->fileName()).arg(m_currentFile->isDirty() ? Constants::WIIKING2_TITLE_DIRTY : '\0'));
    if (oldFile)
    {
        ui->mainLayout->removeWidget(oldFile->widget());
        oldFile->widget()->hide();
    }

    ui->mainLayout->addWidget(m_currentFile->widget());
    m_currentFile->widget()->show();
}

void MainWindow::onClose()
{
    qDebug() << "Closing file" << m_currentFile->filePath();
    m_documents.remove(m_currentFile->filePath());
    delete m_currentFile;
    m_currentFile = NULL;
    delete ui->documentList->currentItem();
}

void MainWindow::onOpen()
{
    QSettings settings;
    QString MRD = mostRecentDirectory();
    qDebug() << MRD;
    QString file = QFileDialog::getOpenFileName(this, "Open file...", MRD, m_fileFilters.join(";;").trimmed());

    if (!file.isEmpty())
    {
        openFile(file);
    }
}

void MainWindow::onAboutQt()
{
    qApp->aboutQt();
}

void MainWindow::updateMRU(const QString& file)
{
    QSettings settings;
    QStringList files = settings.value(Constants::Settings::WIIKING2_RECENT_FILES).toStringList();
    files.removeAll(file);
    files.prepend(file);

    while (files.size() > MAXRECENT)
        files.removeLast();

    settings.setValue(Constants::Settings::WIIKING2_RECENT_FILES, files);
    updateRecentFileActions();

    foreach (QWidget* widget, QApplication::topLevelWidgets())
    {
        MainWindow* mainWin = qobject_cast<MainWindow*>(widget);
        if (mainWin)
            mainWin->updateRecentFileActions();
    }
}

void MainWindow::updateRecentFileActions()
{
    QSettings settings;
    QStringList files = settings.value(Constants::Settings::WIIKING2_RECENT_FILES).toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAXRECENT);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i+1).arg(strippedName(files[i]));
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
    }

    for (int j = numRecentFiles; j < MAXRECENT; ++j)
        m_recentFileActions[j]->setVisible(false);

    m_recentFileSeparator->setVisible(true);
}

void MainWindow::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if (action)
        openFile(action->data().toString());
}

void MainWindow::showEvent(QShowEvent* se)
{
    QMainWindow::showEvent(se);

    if (m_pluginsManager->plugins().count() <= 0)
    {
        QMessageBox mbox;
        mbox.setWindowTitle(tr(Constants::WIIKING2_NO_PLUGINS_ERROR));
        mbox.setText(tr(Constants::WIIKING2_NO_PLUGINS_ERROR_MSG));
        mbox.setStandardButtons(QMessageBox::Ok);
        mbox.exec();
        exit(1);
    }
}

void MainWindow::onPlugins()
{
    m_pluginsManager->dialog();
}

void MainWindow::onClearRecent()
{
    QSettings settings;
    settings.setValue(Constants::Settings::WIIKING2_RECENT_FILES, QVariant());

    updateRecentFileActions();
}

QString MainWindow::strippedName(const QString& fullFileName) const
{
    return QFileInfo(fullFileName).fileName();
}

QString MainWindow::mostRecentDirectory()
{
    QSettings settings;
    return settings.value(Constants::Settings::WIIKING2_RECENT_DIRECTORY).toString();
}
