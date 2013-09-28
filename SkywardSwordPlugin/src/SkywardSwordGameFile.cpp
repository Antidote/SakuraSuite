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

#include "SkywardSwordGameFile.hpp"
#include "SkywardSwordEditorForm.hpp"
#include "SkywardSwordTabWidget.hpp"
#include "SkywardSwordPlugin.hpp"
#include "CopyWidget.hpp"


#include <WiiSaveReader.hpp>
#include <WiiSaveWriter.hpp>
#include <WiiSave.hpp>
#include <WiiFile.hpp>
#include <WiiBanner.hpp>
#include <WiiImage.hpp>
#include <BinaryReader.hpp>
#include <BinaryWriter.hpp>
#include <Exception.hpp>
#include <FileNotFoundException.hpp>
#include <InvalidOperationException.hpp>

#include <WiiKeyManagerBase.hpp>
#include <QMessageBox>
#include <QtEndian>
#include <QApplication>
#include <QFileInfo>
#include <QTabBar>
#include <QDebug>

SkywardSwordGameDocument::SkywardSwordGameDocument(const PluginInterface *loader, const QString &file)
    : GameDocument(loader, file),
      m_skipData(NULL)
{

    m_widget = new SkywardSwordTabWidget;
    m_copyWidget = new CopyWidget(m_widget);
    SkywardSwordTabWidget* tw = qobject_cast<SkywardSwordTabWidget*>(m_widget);
    Q_ASSERT(tw != NULL);
    tw->setIconSize(QSize(32, 32));
    tw->setDocumentMode(true);
    tw->setMovable(true);
    connect(tw->tabBar(), SIGNAL(tabMoved(int,int)), this, SLOT(onTabMoved(int, int)));

    if (file.isEmpty())
    {
        for (int i = 0; i < 3; i++)
        {
            char* data = new char[0x53C0];
            SkywardSwordEditorForm* sw = new SkywardSwordEditorForm(this, data);
            sw->setNew(true);
            tw->addTab(sw, QIcon(QString(":/icon/Game%1").arg(i+1)), tr("&%1 New Game").arg(i + 1));
            connect(sw, SIGNAL(modified()), this, SLOT(onModified()));
        }
        return;
    }
}

SkywardSwordGameDocument::~SkywardSwordGameDocument()
{
    if (m_skipData)
    {
        delete[] m_skipData;
        m_skipData = NULL;
    }
}


bool SkywardSwordGameDocument::loadFile()
{
    if (QFileInfo(filePath()).suffix() == "bin")
    {
        try
        {
            QMessageBox msg((QWidget*)m_widget->parent());
            msg.setWindowTitle("Loading WiiSave...");
            msg.setText("Loading WiiSave please wait....");
            msg.setStandardButtons(QMessageBox::NoButton);
            // This prevents the user from clicking away
            msg.setWindowModality(Qt::WindowModal);
            msg.show();
            qApp->setOverrideCursor(Qt::WaitCursor);
            zelda::io::WiiSaveReader reader(filePath().toStdString());
            zelda::WiiSave* file = reader.readSave();
            qApp->restoreOverrideCursor();
            msg.hide();
            if (file->file("/wiiking2.sav"))
            {
                Uint8* data = file->file("/wiiking2.sav")->data();
                m_isWiiSave = true;
                return loadData(zelda::io::BinaryReader(data, (Uint64)file->file("/wiiking2.sav")->length()));
            }
        }
        catch (...)
        {
            qApp->restoreOverrideCursor();
        }
    }

    try
    {
        return loadData(zelda::io::BinaryReader(filePath().toStdString()));
    }
    catch(...)
    {
        return false;
    }
}

bool SkywardSwordGameDocument::reload()
{
    // First store the current tabs
    QList<int> swCurTabs;
    QTabWidget* tw = qobject_cast<QTabWidget*>(m_widget);
    Q_ASSERT(tw);
    int twCurrentTab = tw->currentIndex();

    for (int i = 0; i < tw->count(); i++)
    {
        SkywardSwordEditorForm* sw = qobject_cast<SkywardSwordEditorForm*>(tw->widget(i));
        swCurTabs.push_back(sw->currentTab());
    }

    bool ret = loadFile();
    // Now set them back
    if (ret)
    {
        for (int i = 0; i < tw->count(); i++)
        {
            SkywardSwordEditorForm* sw = qobject_cast<SkywardSwordEditorForm*>(tw->widget(i));
            sw->setCurrentTab(swCurTabs[i]);
        }
        tw->setCurrentIndex(twCurrentTab);
    }


    // Finally inform Main that we're done
    return ret;
}

bool SkywardSwordGameDocument::supportsWiiSave() const
{
    return true;
}

bool SkywardSwordGameDocument::exportWiiSave()
{
    if (!m_keyManager->isValid())
        return false;

    QTabWidget* tw = qobject_cast<QTabWidget*>(m_widget);
    Q_ASSERT(tw);

    QFile tmp(":/BannerData/banner.tpl");

    zelda::WiiSave* save = new zelda::WiiSave;

    int gameId = ('S' << 24) | ('O' << 16) | ('U' << 8) | (int)m_region;
    quint64 titleId = 0x00010000;
    titleId = (qToBigEndian((quint64)gameId)) | (qToBigEndian(titleId) >> 32);

    qDebug() << hex << qFromBigEndian(titleId);
    zelda::WiiBanner* banner = new zelda::WiiBanner();
    banner->setGameID(qFromBigEndian(titleId));
    if (tmp.open(QFile::ReadOnly))
    {
        QDataStream dataStream(&tmp);
        char* bannerData = new char[192*64*2];
        dataStream.readRawData(bannerData, 192*64*2);
        banner->setBannerImage(new zelda::WiiImage(192, 64, (Uint8*)bannerData));
        tmp.close();
    }
    else
    {
        delete save;
        return false;
    }

    tmp.setFileName(":/BannerData/icon.tpl");

    if (tmp.open(QFile::ReadOnly))
    {
        QDataStream dataStream(&tmp);
        char* iconData = new char[48*48*2];
        dataStream.readRawData(iconData, 48*48*2);
        banner->addIcon(new zelda::WiiImage(48, 48, (Uint8*)iconData));
        tmp.close();
    }
    else
    {
        delete save;
        return false;
    }

    tmp.setFileName(QString(":/BannerData/%1/title.bin").arg(m_region));
    if (tmp.open(QFile::ReadOnly))
    {
        QString titleString = QString::fromUtf16((ushort*)tmp.readAll().data());
        banner->setTitle(titleString.toUtf8().data());
        tmp.close();
    }
    else
    {
        delete save;
        return false;
    }

    tmp.setFileName(QString(":/BannerData/%1/subtitle.bin").arg(m_region));
    if (tmp.open(QFile::ReadOnly))
    {
        QString subtitleString = QString::fromUtf16((ushort*)tmp.readAll().data());
        banner->setSubtitle(subtitleString.toUtf8().data());
        tmp.close();
    }
    else
    {
        delete save;
        return false;
    }
    banner->setPermissions(zelda::WiiFile::GroupRW | zelda::WiiFile::OwnerRW);
    banner->setAnimationSpeed(0);
    save->setBanner(banner);

    try
    {
        zelda::io::BinaryWriter writer(QString(QDir::temp().tempPath() + "/tmp.sav").toStdString());
        writer.setEndianess(zelda::BigEndian);
        writer.writeUInt32(0x534F5500);
        writer.seek(-1);
        writer.writeByte(m_region);
        writer.seek(0x1C, zelda::io::BinaryWriter::Beginning);
        writer.writeUInt32(0x1D);

        for (int i = 0; i < 3; i++)
        {
            SkywardSwordEditorForm* ef = qobject_cast<SkywardSwordEditorForm*>(tw->widget(i));
            if (!ef)
                return false;
            else
            {
                // Let's be sure we have a proper checksum
                ef->updateChecksum();
                writer.writeBytes((Int8*)ef->gameData(), 0x53C0);
            }
        }
        writer.writeBytes((Int8*)m_skipData, 0x80);
        writer.save();

        save->addFile("/wiiking2.sav", new zelda::WiiFile("wiiking2.sav", zelda::WiiFile::GroupRW | zelda::WiiFile::OwnerRW, writer.data(), writer.length()));
        save->addFile("/skip.dat", new zelda::WiiFile("skip.dat", zelda::WiiFile::GroupRW | zelda::WiiFile::OwnerRW, (Uint8*)m_skipData, 0x80));
    }
    catch(...)
    {
        delete save;
        return false;
    }

    try
    {
        zelda::io::WiiSaveWriter writer("./test.bin");
        if (writer.writeSave(save, (Uint8*)keyManager()->macAddr().data(), keyManager()->ngId(), (Uint8*)keyManager()->ngPriv().data(), (Uint8*)keyManager()->ngSig().data(), keyManager()->ngKeyId()))
            qDebug() << "export successful";
    }
    catch(...)
    {
        delete save;
        return false;
    }

    delete save;
    return true;
}

QString SkywardSwordGameDocument::game() const
{
    return "SkywardSword";
}

bool SkywardSwordGameDocument::save(const QString& filename)
{
#ifndef SS_PREVIEW
    if (!filename.isEmpty())
    {
        m_file = QFileInfo(filename).fileName();
        m_path = QFileInfo(filename).absolutePath();
    }

    try
    {
        QTabWidget* tw = (QTabWidget*)m_widget;
        zelda::io::BinaryWriter writer(filePath().toStdString());
        writer.setEndianess(zelda::BigEndian);
        writer.writeUInt32(0x534F5500);
        writer.seek(-1);
        writer.writeByte(m_region);
        writer.seek(0x1C, zelda::io::BinaryWriter::Beginning);
        writer.writeUInt32(0x1D);

        for (int i = 0; i < 3; i++)
        {
            SkywardSwordEditorForm* ef = qobject_cast<SkywardSwordEditorForm*>(tw->widget(i));
            if (!ef)
                return false;
            else
            {
                // Let's be sure we have a proper checksum
                ef->updateChecksum();
                writer.writeBytes((Int8*)ef->gameData(), 0x53C0);
            }
        }

        writer.writeBytes((Int8*)m_skipData, 0x80);
        writer.save();
        m_dirty = false;
        m_isWiiSave = false;
        return true;
    }
    catch (...)
    {
    }
#endif
    return false;
}

void SkywardSwordGameDocument::onModified()
{
    QTabWidget* tw = qobject_cast<QTabWidget*>(m_widget);
    for (int i = 0; i < tw->count(); i++)
    {
        SkywardSwordEditorForm* sw = qobject_cast<SkywardSwordEditorForm*>(tw->widget(i));
        tw->setTabIcon(i, QIcon(QString(":/icon/Game%1").arg(i + 1)));
        if (!sw->isNew())
            tw->setTabText(i, QString("&%1 %2").arg(i+1).arg(sw->playerName()));
        else
            tw->setTabText(i, tr("&%1 New Game").arg(i + 1));
    }
    this->setDirty(true);
    emit modified();
}

void SkywardSwordGameDocument::onCopy(SkywardSwordEditorForm* source)
{
    QTabWidget* tw = qobject_cast<QTabWidget*>(m_widget);
    int index = 0;
    for (int i = 0; i < tw->count(); i++)
    {
        if (tw->widget(i) == source)
            index = i;

        m_copyWidget->setQuestEnabled((CopyWidget::Quest)i, true);
    }

    m_copyWidget->setQuestEnabled((CopyWidget::Quest)index, false);
    m_copyWidget->move(QCursor().pos());
    m_copyWidget->exec();

    if (m_copyWidget->result() == QDialog::Accepted)
    {
        QMessageBox mbox(source);
        mbox.setWindowTitle("Copy data...");
        mbox.setText("Are you sure you wish to copy this data?<br />"
                     "The data at the destination(s) you selected <b>will be lost</b>");
        mbox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
        mbox.exec();
        if (mbox.result() == QMessageBox::No)
            return;

        SkywardSwordEditorForm* dest = NULL;
        if (m_copyWidget->questChecked(CopyWidget::Quest1) && source != tw->widget(0))
        {
            dest = qobject_cast<SkywardSwordEditorForm*>(tw->widget(CopyWidget::Quest1));
            if (dest)
                dest->setGameData(QByteArray(source->gameData(), 0x53C0));
            dest = NULL;
        }
        if (m_copyWidget->questChecked(CopyWidget::Quest2) && source != tw->widget(CopyWidget::Quest2))
        {
            dest = qobject_cast<SkywardSwordEditorForm*>(tw->widget(CopyWidget::Quest2));
            if (dest)
                dest->setGameData(QByteArray(source->gameData(), 0x53C0));
            dest = NULL;
        }
        if (m_copyWidget->questChecked(CopyWidget::Quest3) && source != tw->widget(CopyWidget::Quest3))
        {
            dest = qobject_cast<SkywardSwordEditorForm*>(tw->widget(CopyWidget::Quest3));
            if (dest)
                dest->setGameData(QByteArray(source->gameData(), 0x53C0));
            dest = NULL;
        }
    }
}

void SkywardSwordGameDocument::onTabMoved(int from, int to)
{
    Q_UNUSED(from);
    Q_UNUSED(to);
    onModified();
}

bool SkywardSwordGameDocument::loadData(zelda::io::BinaryReader reader)
{
    SkywardSwordTabWidget* tw = qobject_cast<SkywardSwordTabWidget*>(m_widget);
    Q_ASSERT(tw);
    tw->clear();
    if (filePath().isEmpty())
        return true;

    try
    {
        reader.setEndianess(zelda::BigEndian);

        Uint32 magic = reader.readUInt32();
        m_region = magic & 0x000000FF;
        magic &= 0xFFFFFF00;

        if (magic != 0x534F5500)
            throw zelda::error::InvalidOperationException("Not a valid Skyward Sword file");

        reader.seek(0x1C, zelda::io::BinaryReader::Beginning);

        Uint32 headerSize = reader.readUInt32();
        if (headerSize != 0x1D)
            throw zelda::error::InvalidOperationException("Invalid header size");


        for (int i = 0; i < 3; i++)
        {
            char* data = (char*)reader.readBytes(0x53C0);
            SkywardSwordEditorForm* sw = new SkywardSwordEditorForm(this, data);
            connect(sw, SIGNAL(modified()), this, SLOT(onModified()));
            connect(sw, SIGNAL(copy(SkywardSwordEditorForm*)), this, SLOT(onCopy(SkywardSwordEditorForm*)));
            if (!sw->isNew())
                tw->addTab(sw, QIcon(QString(":/icon/Game%1").arg(i+1)), QString("&%1 %2").arg(i+1).arg(sw->playerName()));
            else
                tw->addTab(sw, QIcon(QString(":/icon/Game%1").arg(i+1)), tr("&%1 New Game").arg(i + 1));
        }
        m_skipData = (char*)reader.readBytes(0x80);
        return true;
    }
    catch (...)
    {
        return false;
    }

    // Shouldn't happen, but just in case
    return false;
}

