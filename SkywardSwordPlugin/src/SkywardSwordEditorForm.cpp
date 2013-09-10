#include "SkywardSwordEditorForm.hpp"
#include "ui_SkywardSwordEditorForm.h"
#include "SkywardSwordGameFile.hpp"
#include "SettingsManager.hpp"
#include "SkywardSwordPlugin.hpp"
#include <QtEndian>
#include <QDebug>

SkywardSwordEditorForm::SkywardSwordEditorForm(SkywardSwordGameFile* file, const char *data, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SkywardSwordEditorForm),
      m_gameFile(file),
      m_gameData((char*)data)
{
    ui->setupUi(this);
    // Make sure the selected tab is "Player Info"
    ui->tabWidget->setCurrentIndex(0);
    connect(this, SIGNAL(modified()), this, SLOT(onModified()));
    updateData();
}

SkywardSwordEditorForm::~SkywardSwordEditorForm()
{
    delete ui;
}

char* SkywardSwordEditorForm::gameData()
{
    return m_gameData;
}

void SkywardSwordEditorForm::onDelete()
{
    memset(m_gameData, 0x00, 0x53C0);
    setNew(true);
    emit modified();
}

void SkywardSwordEditorForm::onCreate()
{
    memset(m_gameData, 0x00, 0x53C0);
    setPlayerName(SkywardSwordPlugin::instance()->settings()->defaultPlayerName());
    emit modified();
}

QString SkywardSwordEditorForm::playerName() const
{
    if (!m_gameData)
        return QString("");

    ushort tmpName[8];
    for (int i = 0, j=0; i < 8; ++i, j+= 2)
    {
        tmpName[i] = *(quint16*)(m_gameData + (0x08D4 + j));
        tmpName[i] = qFromBigEndian(tmpName[i]);
    }

    return QString(QString::fromUtf16(tmpName));
}

void SkywardSwordEditorForm::setPlayerName(const QString &name)
{
    if (!m_gameData)
        return;


    if (name == playerName())
        return;

    for (int i = 0, j = 0; i < 8; ++i, ++j)
    {
        if (i > name.length())
        {
            *(ushort*)(m_gameData + (0x08D4 + j++)) = 0;
            continue;
        }
        *(ushort*)(m_gameData + (0x08D4 + j++)) = qToBigEndian<quint16>(name.utf16()[i]);
    }

    //this->updateChecksum();
    emit modified();
}

void SkywardSwordEditorForm::onModified()
{
    m_gameFile->setDirty(true);
    this->setUpdatesEnabled(false);
    updateData();
    this->setUpdatesEnabled(true);
}

bool SkywardSwordEditorForm::isNew() const
{
    if (!m_gameData)
        return true;

    return (*(char*)(m_gameData + 0x53AD)) != 0;
}

void SkywardSwordEditorForm::setNew(bool isNew)
{
    if (this->isNew() == isNew)
        return;

    (*(char*)(m_gameData + 0x53AD)) = isNew;
    emit modified();
}

uint SkywardSwordEditorForm::checksum()
{
    return qFromBigEndian(*(quint32*)(m_gameData + 0x53BC));
}

void SkywardSwordEditorForm::updateData()
{
    ui->deletePushButton->setEnabled(!isNew());
    ui->createPushButton->setEnabled(isNew());
    ui->tabWidget->setEnabled(!isNew());
    ui->nameLineEdit->setText(playerName());
    ui->rupeeSpinBox->setValue(qFromBigEndian(*(quint16*)(m_gameData + 0x0A5E)));
    ui->checksumValueLbl->setText("0x" + QString("%1").arg((uint)checksum(), 8, 16, QChar('0')).toUpper());
}
