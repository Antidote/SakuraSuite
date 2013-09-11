#include "SkywardSwordGameFile.hpp"
#include "SkywardSwordEditorForm.hpp"
#include <QTabWidget>
#include <QMessageBox>
#include <BinaryReader.hpp>
#include <BinaryWriter.hpp>
#include <Exception.hpp>
#include <FileNotFoundException.hpp>
#include <InvalidOperationException.hpp>
#include <QDebug>
#include <QtEndian>
#include <QApplication>
#include <QFileInfo>

SkywardSwordGameFile::SkywardSwordGameFile(const PluginInterface *loader, const QString &file)
    : GameFile(loader, file)
{

    m_widget = new QTabWidget;
    QTabWidget* tw = (QTabWidget*)m_widget;
    tw->setIconSize(QSize(32, 32));
    if (file.isEmpty())
    {
        for (int i = 0; i < 3; i++)
        {
            char* data = new char[0x53C0];
            SkywardSwordEditorForm* sw = new SkywardSwordEditorForm(this, data);
            sw->setNew(true);
            tw->addTab(sw, QIcon(QString(":/icon/Game%1").arg(i+1)), QString() /*QObject::tr("Game %1").arg(i + 1)*/);
            connect(sw, SIGNAL(modified()), this, SIGNAL(modified()));
        }
        return;
    }

    QMessageBox mbox;
    mbox.setParent(qApp->topLevelWidgets()[0]);
    try
    {
        zelda::io::BinaryReader reader(file.toStdString());
        reader.setEndianess(zelda::BigEndian);
        Uint32 magic = reader.readUInt32();
        qDebug() << hex << magic;
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
            connect(sw, SIGNAL(modified()), this, SIGNAL(modified()));
            tw->addTab(sw, QIcon(QString(":/icon/Game%1").arg(i+1)), QString()/*QObject::tr("Game %1").arg(i + 1)*/);
        }
        m_skipData = (char*)reader.readBytes(0x80);
    }
    catch (zelda::error::FileNotFoundException e)
    {
        mbox.setWindowTitle(mbox.tr("File not found..."));
        mbox.setText(mbox.tr("Unable to locate file <b>'%1'</b><br/>Please ensure that it exists.").arg(e.filename().c_str()));
        mbox.exec();
    }
    catch (zelda::error::InvalidOperationException e)
    {
        mbox.setWindowTitle(mbox.tr("Invalid Operation..."));
        mbox.setText(mbox.tr("%1").arg(e.message().c_str()));
        mbox.exec();
    }
    catch (zelda::error::Exception e)
    {
        mbox.setWindowTitle(mbox.tr("Exception..."));
        mbox.setText(mbox.tr("%1").arg(e.message().c_str()));
        mbox.exec();
    }
}

QString SkywardSwordGameFile::game() const
{
    return "SkywardSword";
}

bool SkywardSwordGameFile::save(const QString& filename)
{
    if (!filename.isEmpty())
    {
        m_file = QFileInfo(filename).fileName();
        m_path = QFileInfo(filename).absolutePath();
    }

    try
    {
        QTabWidget* tw = (QTabWidget*)m_widget;
        zelda::io::BinaryWriter writer(filePath().toStdString());
        qDebug() << writer.filepath().c_str();
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
        return true;
    }
    catch (...)
    {
    }
    return false;
}

void SkywardSwordGameFile::onModified()
{
    emit modified();
}

