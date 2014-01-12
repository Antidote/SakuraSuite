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

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "PluginsManager.hpp"
#include "AboutDialog.hpp"
#include "PreferencesDialog.hpp"
#include "WiiKeyManager.hpp"
// Updater Includes
#include <Updater.hpp>

// PluginFramework includes
#include <PluginInterface.hpp>
#include <DocumentBase.hpp>
#include <GameDocument.hpp>

// Qt Includes
#include <QDebug> // for qWarning
#include <QLabel>
#include <QMenu>
#include <QFileInfo>
#include <QMessageBox>
#include <QFileDialog>
#include <QStyleFactory>
#include <QDesktopWidget>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_currentFile(NULL),
    m_pluginsManager(new PluginsManager(this)),
    m_aboutDialog(NULL),
    m_updater(new Updater(this)),
    m_updateMBox(this),
    m_cancelClose(false),
    m_keyManager(new WiiKeyManager),
    m_untitledDocs(0)
  #ifdef WK2_PREVIEW
  ,m_previewLayout(NULL),
    m_previewLabel(NULL)
  #endif
{
    ui->setupUi(this);

#ifndef DEBUG
    // lock Application
    if (checkLock())
    {
        qApp->quit();
        return;
    }
#endif

    ui->actionExportWiiSave->setVisible(false);
    m_preferencesDialog = new PreferencesDialog(m_keyManager, this);
    connect(&m_lockTimer, SIGNAL(timeout()), SLOT(onLockTimeout()));

    loadWiiKeys();
    // we set the timer to timeout every 58 seconds
    // Since checkLock expects a stale cookie to be over 60 seconds old
    // this ensures that there is never a race condition
    // Since it's nearly impossible two instances to overlap
    // such a time span (unless you get some REALLY bad lag, again, not likely)
    m_lockTimer.setInterval(58*1000);
    m_lockTimer.start();

    setWindowIcon(QIcon(":/icon/Bomb64x64.png"));
    m_defaultWindowGeometry = this->saveGeometry();
    m_defaultWindowState = this->saveState();

    // lets load the plugins
    m_pluginsManager->loadPlugins();

    // Setup "Styles" menu
    setupStyleActions();

    // Add default filter
    m_fileFilters << "All Files *.* (*.*)";

    // Setup the MRU list
    initMRU();

    // Setup the context menu for documentList
    initDocumentList();

    // On preview and internal builds we inject a label into the menu bar to inform the user
#if defined(SS_PREVIEW) || defined(SS_INTERNAL)
    injectPreviewLabel();
#endif
    // Now load the MRU
    updateRecentFileActions();

    // Restore window from saved states
    restoreDefaultGeometry();

    // Setup the updater's connections
    initUpdater();
    // Setup the filesystem watcher
    initFSWatcher();


    connect(ui->actionPreferences, SIGNAL(triggered()), m_preferencesDialog, SLOT(exec()));
    // Hide the toolbar if it has no actions
    ui->mainToolBar->setVisible((ui->mainToolBar->actions().count() > 0 ? true : false));
}

MainWindow::~MainWindow()
{
#if defined(SS_PREVIEW) || defined(SS_INTERNAL)
    if (m_previewLayout)
    {
        delete m_previewLayout;
        m_previewLayout = NULL;
    }

    if (m_previewLabel)
    {
        delete m_previewLabel;
        m_previewLabel = NULL;
    }
#endif

    QFile(Constants::SAKURASUITE_LOCK_FILE).remove();
    QSettings settings;
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());


    foreach (DocumentBase* file, m_documents.values())
    {
        delete file;
        file = NULL;
    }

    m_keyManager->saveKeys();
    delete m_keyManager;
    delete m_pluginsManager;
    m_pluginsManager = NULL;
    delete ui;
}
void MainWindow::initUpdater()
{
    connect(m_updater, SIGNAL(done()), this, SLOT(onUpdateDone()));
    connect(m_updater, SIGNAL(error(Updater::ErrorType)), this, SLOT(onUpdateError(Updater::ErrorType)));
    connect(m_updater, SIGNAL(warning(QString)), this, SLOT(onUpdateWarning(QString)));
    connect(m_updater, SIGNAL(noUpdate()), this, SLOT(onNoUpdate()));
}

void MainWindow::restoreDefaultGeometry()
{
    QSettings settings;
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());
}

void MainWindow::injectPreviewLabel()
{
#if defined(SS_PREVIEW) || defined(SS_INTERNAL)
    QMenuBar* bar = this->menuBar();
    m_previewLayout = new QHBoxLayout(bar);
    m_previewLayout->addStretch();
    m_previewLabel  = new QLabel(bar);
    m_previewLabel->setObjectName("previewLabel");
#ifdef SS_PREVIEW
    m_previewLabel->setText("<b>PREVIEW BUILD</b>");
#elif defined(SS_INTERNAL)
    m_previewLabel->setText("<b>INTERNAL BUILD</b>");
#endif
    m_previewLayout->setContentsMargins(150, 0, 6, 0);
    m_previewLayout->addWidget(m_previewLabel);
    bar->setLayout(m_previewLayout);
#endif
}

void MainWindow::initDocumentList()
{
    ui->documentList->addAction(ui->actionOpen);
    ui->documentList->addAction(ui->menuNew->menuAction());
    QAction* separator = new QAction(this);
    separator->setSeparator(true);
    ui->documentList->addAction(separator);
    ui->documentList->addAction(ui->menuRecentFiles->menuAction());

    separator = new QAction(this);
    separator->setSeparator(true);
    ui->documentList->addAction(separator);
    ui->documentList->addAction(ui->actionSave);
    ui->documentList->addAction(ui->actionSaveAs);
    separator = new QAction(this);
    separator->setSeparator(true);
    ui->documentList->addAction(separator);
    ui->documentList->addAction(ui->actionClose);
    this->setWindowTitle(Constants::SAKURASUITE_TITLE);
}

void MainWindow::initMRU()
{
    m_recentFileSeparator = ui->menuRecentFiles->insertSeparator(ui->actionClearRecent);

    for (int i = 0; i < MAXRECENT; ++i)
    {
        QAction* act = new QAction(this);
        act->setVisible(false);
        connect(act, SIGNAL(triggered()), this, SLOT(openRecentFile()));
        m_recentFileActions.append(act);
        ui->menuRecentFiles->insertAction(m_recentFileSeparator, m_recentFileActions[i]);
    }
}

void MainWindow::initFSWatcher()
{
    connect(&m_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
}

void MainWindow::addFileFilter(const QString& filter)
{
    if (m_fileFilters.contains(filter))
        return;

    m_fileFilters.push_front(filter);
}

void MainWindow::removeFileFilter(const QString& filter)
{
    if (!m_fileFilters.contains(filter))
        return;

    m_fileFilters.removeOne(filter);
}

void MainWindow::closeFilesFromLoader(PluginInterface* loader)
{
    QList<DocumentBase*> targets;
    foreach(DocumentBase* file, m_documents.values())
    {
        if (file->loadedBy() == loader)
        {
            targets.append(file);
            for (int i = 0; i < ui->documentList->count(); i++)
            {
                if (cleanPath(ui->documentList->item(i)->data(FILEPATH).toString()) == cleanPath(file->filePath()))
                {
                    delete ui->documentList->item(i);
                    break;
                }
            }
        }
    }

    foreach(DocumentBase* file, targets)
    {
        m_fileSystemWatcher.removePath(cleanPath(file->filePath()));
        m_documents.remove(cleanPath(file->filePath()));
        delete file;
    }
    targets.clear();
}

QString MainWindow::cleanPath(const QString& currentFile)
{
    QString filePath = currentFile;
    filePath = filePath.replace("\\", "/");
#ifdef Q_OS_WIN
    filePath = filePath.toLower();
#endif

    return filePath;
}

bool MainWindow::isInternalBuild()
{
#ifdef SS_INTERNAL
    return true;
#else
    return false;
#endif
}

bool MainWindow::isPreviewBuild()
{
#ifdef SS_PREVIEW
    return true;
#else
    return false;
#endif
}

QMenu* MainWindow::fileMenu() const
{
    return ui->menuFile;
}

QMenu* MainWindow::newDocumentMenu() const
{
    return ui->menuNew;
}

QMenu* MainWindow::editMenu() const
{
    return ui->menuEdit;
}

QMenu* MainWindow::helpMenu() const
{
    return ui->menuHelp;
}

QMenu* MainWindow::viewMenu() const
{
    return ui->menuView;
}

QMenuBar* MainWindow::menuBar() const
{
    return QMainWindow::menuBar();
}

QStatusBar* MainWindow::statusBar() const
{
    return ui->statusBar;
}

QToolBar* MainWindow::toolBar() const
{
    return ui->mainToolBar;
}

QMainWindow* MainWindow::mainWindow() const
{
    return (QMainWindow*)this;
}

void MainWindow::onNewDocument(DocumentBase* document)
{
    if (!document)
        return;

    GameDocument* gd = qobject_cast<GameDocument*>(document);
    if (gd && gd->supportsWiiSave())
        gd->setKeyManager(m_keyManager);

    QString filePath = QString("/tmp/Untitled %1 Document %2").arg(document->loadedBy()->name()).arg(++m_untitledDocs);
    m_documents[cleanPath(filePath)] = document;
    PluginInterface* loader = document->loadedBy();
    QListWidgetItem* item = new QListWidgetItem();
    item->setData(FILENAME, QFileInfo(filePath).fileName());
    item->setData(FILEPATH, cleanPath(filePath));
    item->setText(item->data(FILENAME).toString());
    if (!loader->icon().isNull())
        item->setIcon(loader->icon());

    ui->documentList->addItem(item);
    ui->documentList->setCurrentItem(item);
    m_currentFile = document;
    updateWindowTitle();
}

void MainWindow::closeEvent(QCloseEvent* e)
{
    onCloseAll();

    if (m_cancelClose)
        e->ignore();
    else
        e->accept();
}

void MainWindow::dragEnterEvent(QDragEnterEvent* e)
{
    foreach (QUrl url, e->mimeData()->urls())
    {
        PluginInterface* plugin = m_pluginsManager->preferredPlugin(url.toLocalFile());

        if (plugin)
            e->acceptProposedAction();
    }
}

void MainWindow::dropEvent(QDropEvent* e)
{
    foreach (QUrl url, e->mimeData()->urls())
    {
        PluginInterface* plugin = m_pluginsManager->preferredPlugin(url.toLocalFile());

        if (plugin)
            openFile(url.toLocalFile());
    }
}

void MainWindow::loadWiiKeys()
{
    if (!m_keyManager->loadKeys())
    {
        qWarning() << "Unable to load keys from registry, attempting to load from file";
        if (!m_keyManager->open(qApp->applicationDirPath() + "/keys.bin"))
            qWarning() << "Unable to load keys from file";
    }
}

void MainWindow::openFile(const QString& currentFile)
{
    // Change any '\' to '/' for maximum compatibility
    QString filePath = cleanPath(currentFile);
    QMessageBox msgBox(this);
    msgBox.setStandardButtons(QMessageBox::Ok);
    msgBox.setIcon(QMessageBox::Warning);
    if (m_documents.contains(filePath))
    {
        msgBox.setWindowTitle(Constants::SAKURASUITE_ALREADY_OPENED);
        msgBox.setText(Constants::SAKURASUITE_ALREADY_OPENED_MSG.arg(strippedName(filePath)));
        msgBox.exec();
        return;
    }
    PluginInterface* loader = m_pluginsManager->preferredPlugin(filePath);
    if (!loader)
    {
        msgBox.setWindowTitle("Unable to find suitable loader...");
        msgBox.setText(tr("%1 was unable to determine an appropriate plugin to load '%2' with.").arg(Constants::SAKURASUITE_TITLE).arg(filePath));
        msgBox.exec();
        return;
    }

    DocumentBase* file = loader->loadFile(filePath);

    if (!file)
    {
        msgBox.setWindowTitle(Constants::SAKURASUITE_OPEN_FAILED);
        msgBox.setText(Constants::SAKURASUITE_OPEN_FAILED_MSG.arg(strippedName(filePath)).arg(loader->name()));
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.setIcon(QMessageBox::Warning);
        msgBox.exec();
        return;
    }
    connect(file, SIGNAL(modified()), this, SLOT(updateWindowTitle()));
    m_documents[filePath] = file;
    m_fileSystemWatcher.addPath(filePath);

    // If the document is a game document, check for WiiSave support;
    // If the document supports wiisaves give the instance of the key manager
    // this way we can have one global instance that Main maintains.
    GameDocument* gd = qobject_cast<GameDocument*>(file);
    if (gd && gd->supportsWiiSave())
        gd->setKeyManager(m_keyManager);

    QListWidgetItem* item = new QListWidgetItem();
    item->setData(FILENAME, m_documents[filePath]->fileName());
    item->setData(FILEPATH, filePath);
    item->setText(item->data(FILENAME).toString());
    if (!loader->icon().isNull())
        item->setIcon(loader->icon());

    ui->documentList->addItem(item);
    ui->documentList->setCurrentItem(item);
    m_currentFile = file;
    updateMRU(filePath);
    QSettings settings;
    settings.setValue(Constants::Settings::SAKURASUITE_RECENT_DIRECTORY, cleanPath(QFileInfo(filePath).absolutePath()));
    updateWindowTitle();
}

void MainWindow::onDocumentChanged(int row)
{
    ui->actionReload->setEnabled(ui->documentList->count() > 0);

    Q_UNUSED(row);
    if (!ui->documentList->currentItem())
    {
        ui->actionClose->setEnabled(false);
        ui->actionSave->setEnabled(false);
        ui->actionSaveAs->setEnabled(false);
        ui->actionExportWiiSave->setEnabled(false);
        m_currentFile = NULL;
        return;
    }

    ui->actionClose->setEnabled(true);
    ui->actionSave->setEnabled(true);
    ui->actionSaveAs->setEnabled(true);

    // First we need to store the old Widget so we can remove
    // it from the main layout.
    DocumentBase* oldFile = m_currentFile;
    m_currentFile = m_documents[cleanPath(ui->documentList->currentItem()->data(FILEPATH).toString())];

    if (!m_currentFile)
        return;

    GameDocument* gd = qobject_cast<GameDocument*>(m_currentFile);
    ui->actionExportWiiSave->setEnabled((gd && gd->supportsWiiSave()));

    if (oldFile)
    {
        if (oldFile->widget())
        {
            ui->mainLayout->removeWidget(oldFile->widget());
            oldFile->widget()->hide();
        }
    }

    if (m_currentFile->widget())
    {
        ui->mainLayout->addWidget(m_currentFile->widget());
        m_currentFile->widget()->show();
    }

    updateWindowTitle();
}


void MainWindow::onClose()
{
    if (m_documents.count() <= 0)
        return;

    if (m_currentFile->isDirty())
    {
        QMessageBox mbox(this);
        mbox.setWindowTitle("File has been modified...");
        mbox.setText(tr("'%1' has been modified, do you wish to save?").arg(ui->documentList->currentItem()->data(FILENAME).toString()));
        mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::Discard);
        mbox.exec();
        if (mbox.result() == QMessageBox::Yes)
            onSave();
        else if (mbox.result() == QMessageBox::Cancel)
        {
            m_cancelClose = true;
            return;
        }
    }

    QString filePath = cleanPath(ui->documentList->currentItem()->data(FILEPATH).toString());
    m_fileSystemWatcher.removePath(cleanPath(filePath));
    m_documents.remove(cleanPath(filePath));
    delete m_currentFile;
    m_currentFile = NULL;
    delete ui->documentList->currentItem();

    if (ui->documentList->count() <= 0)
        this->setWindowTitle(Constants::SAKURASUITE_TITLE);

    ui->actionReload->setEnabled(ui->documentList->count() > 0);
}

void MainWindow::onCloseAll()
{
    m_cancelClose = false;
    while (m_documents.count() > 0 && !m_cancelClose)
        onClose();
}

void MainWindow::onOpen()
{
    QString MRD = mostRecentDirectory();
    QString file = QFileDialog::getOpenFileName(this, "Open file...", MRD, m_fileFilters.join(";;").trimmed());

    if (!file.isEmpty())
    {
        openFile(cleanPath(file));
    }
}

void MainWindow::onSave()
{
    if (m_documents.count() <= 0)
        return;
    if (!m_currentFile || !m_currentFile->isDirty())
        return;

    if (!m_currentFile->fileName().isEmpty())
        m_fileSystemWatcher.removePath(cleanPath(m_currentFile->filePath()));


    GameDocument* gd = qobject_cast<GameDocument*>(m_currentFile);

    if (m_currentFile->fileName().isEmpty() || (gd && gd->isWiiSave()))
        onSaveAs();

    if (m_currentFile->save())
        statusBar()->showMessage(tr("Save successful"), 2000);
    else
        statusBar()->showMessage(tr("Save failed"), 2000);

    m_fileSystemWatcher.addPath(cleanPath(m_currentFile->filePath()));
    updateWindowTitle();
}

void MainWindow::onSaveAs()
{
    if (m_documents.count() <= 0)
        return;
    if (!m_currentFile)
        return;
    QString MRD = mostRecentDirectory();
    QString file = QFileDialog::getSaveFileName(this, "Save file as...", MRD, m_fileFilters.join(";;").trimmed());

    if (file.isEmpty())
        return;

    cleanPath(file);
    if (cleanPath(m_currentFile->filePath()) != file)
    {
        m_documents.remove(cleanPath(m_currentFile->filePath()));
        m_currentFile->save(file);
        file = cleanPath(m_currentFile->filePath());
        m_documents[file] = m_currentFile;
        QListWidgetItem* item = ui->documentList->currentItem();
        item->setData(FILENAME, cleanPath(m_currentFile->fileName()));
        item->setData(FILEPATH, file);
        item->setText(item->data(FILENAME).toString());
    }
    else
    {
        m_currentFile->save();
    }

    updateWindowTitle();
}

void MainWindow::onExit()
{
    onCloseAll();

    if (m_cancelClose)
        return;

    qApp->quit();
}

void MainWindow::onAbout()
{
    if (!m_aboutDialog)
        m_aboutDialog = new AboutDialog(this);

    m_aboutDialog->exec();
}

void MainWindow::onAboutQt()
{
    qApp->aboutQt();
}

void MainWindow::updateMRU(const QString& file)
{
    QSettings settings;
    QStringList files = settings.value(Constants::Settings::SAKURASUITE_RECENT_FILES).toStringList();
    files.removeAll(cleanPath(file));
    files.prepend(cleanPath(file));

    while (files.size() > MAXRECENT)
        files.removeLast();

    settings.setValue(Constants::Settings::SAKURASUITE_RECENT_FILES, files);
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
    QStringList files = settings.value(Constants::Settings::SAKURASUITE_RECENT_FILES).toStringList();

    int numRecentFiles = qMin(files.size(), (int)MAXRECENT);

    for (int i = 0; i < numRecentFiles; ++i)
    {
        QString text = tr("&%1 %2").arg(i+1).arg(strippedName(files[i]));
        m_recentFileActions[i]->setText(text);
        m_recentFileActions[i]->setData(files[i]);
        m_recentFileActions[i]->setVisible(true);
        m_recentFileActions[i]->setStatusTip(files[i]);
    }

    for (int j = numRecentFiles; j < MAXRECENT; ++j)
        m_recentFileActions[j]->setVisible(false);

    m_recentFileSeparator->setVisible(true);
}

void MainWindow::setupStyleActions()
{
    if (!QSettings().value(Constants::Settings::SAKURASUITE_DEFAULT_STYLE).isValid())
    {
        if(qApp->style())
            QSettings().setValue(Constants::Settings::SAKURASUITE_DEFAULT_STYLE, qApp->style()->objectName());
        else
            QSettings().setValue(Constants::Settings::SAKURASUITE_DEFAULT_STYLE, qApp->desktop()->style()->objectName());
    }
    QStringList styles = QStyleFactory::keys();
    QActionGroup* actionGroup = new QActionGroup(this);
    actionGroup->addAction(ui->actionDefaultStyle);

    QString currentStyle = QSettings().value(Constants::Settings::SAKURASUITE_CURRENT_STYLE).toString();
    qApp->setStyle(currentStyle);

    foreach (QString style, styles)
    {
        QAction* a = ui->menuStyles->addAction(style);
        a->setCheckable(true);
        if (!QString::compare(currentStyle, style, Qt::CaseInsensitive))
            a->setChecked(true);

        a->setObjectName(style.toLower() + "Action");
        a->setActionGroup(actionGroup);
        connect(a, SIGNAL(triggered()), this, SLOT(onStyleChanged()));
    }
}

void MainWindow::openRecentFile()
{
    QAction* action = qobject_cast<QAction*>(sender());

    if (action)
        openFile(action->data().toString());
}

void MainWindow::updateWindowTitle()
{
    QString filename = ui->documentList->currentItem()->data(FILENAME).toString();
    if (m_currentFile->isDirty() && !ui->documentList->currentItem()->text().contains("*"))
        ui->documentList->currentItem()->setText(ui->documentList->currentItem()->text() + "*");
    else if (!m_currentFile->isDirty() && ui->documentList->currentItem()->text().contains("*"))
    {
        ui->documentList->currentItem()->setText(ui->documentList->currentItem()->data(FILENAME).toString());
    }

    setWindowTitle(Constants::SAKURASUITE_TITLE_FILE.arg(filename).arg(m_currentFile->isDirty() ? Constants::SAKURASUITE_TITLE_DIRTY : ""));
}

void MainWindow::onUpdateDone()
{
    this->setEnabled(true);
    m_updateMBox.hide();
    m_updateMBox.setWindowTitle(Constants::SAKURASUITE_NOT_LATEST_VERSION);
    m_updateMBox.setText(Constants::SAKURASUITE_NOT_LATEST_VERSION_MSG
                         .arg(m_updater->updateUrl())
                         .arg(m_updater->md5Sum())
                         .arg(m_updater->changelogUrl()));

    m_updateMBox.setStandardButtons(QMessageBox::Ok);
    m_updateMBox.exec();
}

void MainWindow::onUpdateError(Updater::ErrorType et)
{
    this->setEnabled(true);
    m_updateMBox.hide();
    m_updateMBox.setStandardButtons(QMessageBox::Ok);
    m_updateMBox.setWindowModality(Qt::NonModal);

    switch(et)
    {
        case Updater::UnableToConnect:
            m_updateMBox.setWindowTitle(Constants::SAKURASUITE_UPDATE_CONTACT_ERROR);
            m_updateMBox.setText(Constants::SAKURASUITE_UPDATE_CONTACT_ERROR_MSG);
            break;
        case Updater::ParseError:
            m_updateMBox.setWindowTitle(Constants::SAKURASUITE_UPDATE_PARSE_ERROR);
            m_updateMBox.setText(Constants::SAKURASUITE_UPDATE_PARSE_ERROR_MSG.arg(m_updater->errorString()));
            break;
        case Updater::NoPlatform:
            m_updateMBox.setWindowTitle(Constants::SAKURASUITE_UPDATE_PLATFORM);
            m_updateMBox.setText(Constants::SAKURASUITE_UPDATE_PLATFORM_MSG);
            break;
    }
    m_updateMBox.exec();
}

void MainWindow::onUpdateWarning(QString message)
{
    ui->statusBar->showMessage(message, 2000);
}

void MainWindow::onNoUpdate()
{
    this->setEnabled(true);
    m_updateMBox.hide();
}

void MainWindow::onLockTimeout()
{
    if (QSettings().value("singleInstance", false).toBool())
    {
        QFile file(Constants::SAKURASUITE_LOCK_FILE);
        if (file.open(QFile::WriteOnly))
        {
            file.seek(0);
            file.write(QString(QDateTime::currentDateTime().toString() + "\n").toLatin1());
            file.resize(file.pos());
        }
    }
}

void MainWindow::showEvent(QShowEvent* se)
{
    QMainWindow::showEvent(se);

    if (m_pluginsManager->plugins().count() <= 0)
    {
        QMessageBox mbox;
        mbox.setWindowTitle(Constants::SAKURASUITE_NO_PLUGINS_ERROR);
        mbox.setText(Constants::SAKURASUITE_NO_PLUGINS_ERROR_MSG);
        mbox.setStandardButtons(QMessageBox::Ok);
        mbox.exec();
        exit(1);
        return;
    }

    QSettings settings;
    if (settings.value(Constants::Settings::SAKURASUITE_CHECK_ON_START, false).toBool())
        onCheckUpdate();
}

// Checklock returns true if lock exists
bool MainWindow::checkLock()
{
    // The lock file is pretty simple
    // All the application does is output the current time
    // of course if a malicious program decided to be a jerk
    // it could output a date several years in the future
    // But since we're using the temp directory
    // theoretically the file should be deleted after every boot
    // But some OSes don't abide by this (Windows I'm looking at you)
    // So we should probably check that too but for now this'll do
    // I COULD use some Qt magic and have it output the lock file, but
    // that's neither here nor there, plus this is simple enough for our purposes
    if (QFile::exists(Constants::SAKURASUITE_LOCK_FILE) && QSettings().value("singleInstance", false).toBool())
    {
        QFile file(Constants::SAKURASUITE_LOCK_FILE);
        if (file.open(QFile::ReadOnly))
        {
            QDateTime time = QDateTime::fromString(QString(file.readLine()));
            quint64 msecs = time.msecsTo(QDateTime::currentDateTime());
            if (msecs > (60*1000))
            {
                // We have a stale lock, which means the old
                // instance either crashed, or didn't have a chance
                // to clean up after itself, we can safely ignore it.
                qWarning() << "Found stale lock, ignoring";
                return false;
            }
        }
        QMessageBox mbox(this);
        mbox.setWindowTitle(Constants::SAKURASUITE_INSTANCE_EXISTS);
        mbox.setText(Constants::SAKURASUITE_INSTANCE_EXISTS_MSG);
        mbox.exec();
        return true;
    }
    else if (QSettings().value("singleInstance", false).toBool())
    {
        QFile lock(Constants::SAKURASUITE_LOCK_FILE);
        lock.open(QFile::WriteOnly);
        lock.write(QString(QDateTime::currentDateTime().toString() + "\n").toLatin1());
        lock.close();
        return false;
    }

    return false;
}

void MainWindow::onPlugins()
{
    m_pluginsManager->dialog();
}

void MainWindow::onClearRecent()
{
    QSettings settings;
    settings.setValue(Constants::Settings::SAKURASUITE_RECENT_FILES, QVariant());

    updateRecentFileActions();
}

void MainWindow::onRestoreDefault()
{
    this->restoreState(m_defaultWindowState);
    this->restoreGeometry(m_defaultWindowGeometry);
}

void MainWindow::onCheckUpdate()
{
    QSettings settings;
    if (!settings.value(Constants::Settings::SAKURASUITE_UPDATE_URL).isValid())
        settings.setValue(Constants::Settings::SAKURASUITE_UPDATE_URL, Constants::Settings::SAKURASUITE_UPDATE_URL_DEFAULT);

    m_updateMBox.setWindowTitle(Constants::SAKURASUITE_UPDATE_CHECKING);
    m_updateMBox.setText(Constants::SAKURASUITE_UPDATE_CHECKING_MSG);
    m_updateMBox.setStandardButtons(QMessageBox::NoButton);
    // This prevents the user from clicking away
    m_updateMBox.setWindowModality(Qt::WindowModal);

    // Prevent the user from interrupting the check
    this->setEnabled(false);
    // Using exec blocks everything, is there a work around?
    m_updateMBox.show();

#ifdef SS_INTERNAL
    m_updater->checkForUpdate(Constants::Settings::SAKURASUITE_UPDATE_URL_DEFAULT, Constants::SAKURASUITE_APP_VERSION, Constants::SAKURASUITE_VERSION);
#else
    m_updater->checkForUpdate(settings.value(Constants::Settings::SAKURASUITE_UPDATE_URL).toString(), Constants::SAKURASUITE_APP_VERSION, Constants::SAKURASUITE_VERSION);
#endif
}

void MainWindow::onReload()
{
    if (!m_currentFile)
        return;

    if (!m_currentFile->reload())
    {
        m_documents.remove(cleanPath(m_currentFile->filePath()));
        QListWidgetItem* item = NULL;
        for (int i = 0; i < ui->documentList->count(); i++)
        {
            if (cleanPath(ui->documentList->item(i)->data(FILEPATH).toString()) == cleanPath(m_currentFile->filePath()))
            {
                item = ui->documentList->item(i);
                break;
            }
        }

        delete m_currentFile;
        m_currentFile = NULL;

        if (item)
            delete item;

        ui->statusBar->showMessage(tr("Reload failed..."), 2000);
    }
    else
        ui->statusBar->showMessage(tr("Reload successful..."), 2000);
}

void MainWindow::onExportWiiSave()
{
    if (!m_currentFile)
        return;

    if (!m_keyManager->isValid())
    {
        QMessageBox mbox(this);
        mbox.setWindowTitle("Unable to export WiiSave...");
        mbox.setText(tr("There are no keys specified for WiiSaves<br />"
                        "Please set your keys in Edit->Preferences in order to export WiiSaves (data.bin)"));
        mbox.exec();
        return;
    }


    GameDocument* gd = qobject_cast<GameDocument*>(m_currentFile);

    if (!gd || !gd->supportsWiiSave())
        return;

    if (gd->exportWiiSave())
        ui->statusBar->showMessage(tr("Export successful..."), 2000);
    else
        ui->statusBar->showMessage(tr("Export failed..."), 2000);
}

void MainWindow::onStyleChanged()
{
    QAction* a = qobject_cast<QAction*>(sender());
    if (a)
    {
        QString style = a->text().toLower();
        a->setChecked(true);
        if (style.contains("default"))
        {
            QString tmp = QSettings().value(Constants::Settings::SAKURASUITE_DEFAULT_STYLE).toString();
            qApp->setStyle(tmp);
            foreach (QAction* action, a->actionGroup()->actions())
            {
                if (action->text().toLower() == tmp.toLower())
                {
                    action->setChecked(true);
                    break;
                }
            }
        }
        else
            qApp->setStyle(a->text());
        QSettings().setValue(Constants::Settings::SAKURASUITE_CURRENT_STYLE, (style.contains("default") ?
                                                                               QSettings().value(Constants::Settings::SAKURASUITE_DEFAULT_STYLE).toString()
                                                                             : style));
    }
}

void MainWindow::onFileChanged(const QString& file)
{
    // Remove the path so we can reload the file.
    m_fileSystemWatcher.removePath(file);

    QMessageBox mbox(this);
    mbox.setWindowTitle("Reload?");
    mbox.setText(tr("<b>'%1'</b><br />"
                    "Has been modified outside of the application, do you wish to reload?").arg(QFileInfo(file).fileName()));
    mbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Ignore);
    mbox.exec();

    if (mbox.result() == QMessageBox::Ignore)
    {
        // The user chose to ignore the changes
        // so we need to put the file back
        m_fileSystemWatcher.addPath(file);
        return;
    }

    if (m_documents[cleanPath(file)]->reload())
        // Now we need to put the file back in the FS watcher
        m_fileSystemWatcher.addPath(file);
    else
    {
        DocumentBase* f = m_documents.take(cleanPath(file));
        delete f;
        QListWidgetItem* item = NULL;
        for (int i = 0; i < ui->documentList->count(); i++)
        {
            if (ui->documentList->item(i)->data(FILEPATH) == cleanPath(file))
            {
                item = ui->documentList->item(i);
                break;
            }
        }

        if (item)
            delete item;
    }
}

QString MainWindow::strippedName(const QString& fullFileName) const
{
    return QFileInfo(fullFileName).fileName();
}

QString MainWindow::mostRecentDirectory()
{
    QSettings settings;
    return settings.value(Constants::Settings::SAKURASUITE_RECENT_DIRECTORY).toString();
}
