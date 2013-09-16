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

#include "SkywardSwordEditorForm.hpp"
#include "ui_SkywardSwordEditorForm.h"
#include "SkywardSwordGameFile.hpp"
#include "SettingsManager.hpp"
#include "SkywardSwordPlugin.hpp"
#include "Constants.hpp"

#include <QtEndian>
#include <QDebug>
#include <Checksums.hpp>
#include <utility.hpp>

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

#ifdef SS_PREVIEW
    ui->previewLabel->setText("<center><b>PREVIEW BUILD</b></center>");
#elif defined(SS_INTERNAL)
    ui->previewLabel->setText("<center><b>INTERNAL BUILD</b></center>");
#else
    ui->previewLabel->setVisible(false);
#endif
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

    emit modified();
}

quint32 SkywardSwordEditorForm::rupees() const
{
    if (!m_gameData)
        return 0;

    return qFromBigEndian(*(quint16*)(m_gameData + 0x0A5E));
}

void SkywardSwordEditorForm::setRupees(int val)
{
    if (!m_gameData)
        return;

    if ((quint32)val == rupees())
        return;

    *(quint16*)(m_gameData + 0x0A5E) = qToBigEndian((quint16)val);
    emit modified();
}

int SkywardSwordEditorForm::totalHP()
{
    return (int)qFromBigEndian(*(quint16*)(m_gameData + 0x5302));
}

void SkywardSwordEditorForm::setTotalHP(int val)
{
    if (val == totalHP())
        return;

    *(quint16*)(m_gameData + 0x5302) = qToBigEndian((quint16)val);
    emit modified();
}

quint32 SkywardSwordEditorForm::unkHP()
{
    return (int)qFromBigEndian(*(quint16*)(m_gameData + 0x5304));
}

void SkywardSwordEditorForm::setUnkHP(int val)
{
    if (val == (int)unkHP())
        return;

    *(quint16*)(m_gameData + 0x5304) = qToBigEndian((quint16)val);
    emit modified();
}

quint32 SkywardSwordEditorForm::currentHP()
{
    return (int)qFromBigEndian(*(quint16*)(m_gameData + 0x5306));
}

void SkywardSwordEditorForm::setCurrentHP(int val)
{
    if (val == (int)currentHP())
        return;

    *(quint16*)(m_gameData + 0x5306) = qToBigEndian((quint16)val);
    emit modified();
}

float SkywardSwordEditorForm::playerX()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0010));
}

void SkywardSwordEditorForm::setPlayerX(double x)
{
    if (x == playerX())
        return;

    *(float*)(m_gameData + 0x0010) = zelda::utility::swapFloat(x);
    emit modified();
}

float SkywardSwordEditorForm::playerY()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0014));
}

void SkywardSwordEditorForm::setPlayerY(double y)
{
    if (y == playerY())
        return;

    *(float*)(m_gameData + 0x0014) = zelda::utility::swapFloat(y);
    emit modified();
}

float SkywardSwordEditorForm::playerZ()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0018));
}

void SkywardSwordEditorForm::setPlayerZ(double z)
{
    if (z == playerZ())
        return;

    *(float*)(m_gameData + 0x0018) = zelda::utility::swapFloat(z);
    emit modified();
}

float SkywardSwordEditorForm::playerRoll()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x001C));
}

void SkywardSwordEditorForm::setPlayerRoll(double roll)
{
    if (roll == playerRoll())
        return;

    *(float*)(m_gameData + 0x001C) = zelda::utility::swapFloat(roll);
    emit modified();
}

float SkywardSwordEditorForm::playerPitch()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0020));
}

void SkywardSwordEditorForm::setPlayerPitch(double pitch)
{
    if (pitch == playerPitch())
        return;

    *(float*)(m_gameData + 0x0020) = zelda::utility::swapFloat(pitch);
    emit modified();
}

float SkywardSwordEditorForm::playerYaw()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0024));
}

void SkywardSwordEditorForm::setPlayerYaw(double yaw)
{
    if (yaw == playerYaw())
        return;

    *(float*)(m_gameData + 0x0024) = zelda::utility::swapFloat(yaw);
    emit modified();
}

float SkywardSwordEditorForm::cameraX()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0028));
}

void SkywardSwordEditorForm::setCameraX(double x)
{
    if (x == cameraX())
        return;

    *(float*)(m_gameData + 0x0028) = zelda::utility::swapFloat(x);
    emit modified();
}

float SkywardSwordEditorForm::cameraY()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x002C));
}

void SkywardSwordEditorForm::setCameraY(double y)
{
    if (y == cameraY())
        return;

    *(float*)(m_gameData + 0x002C) = zelda::utility::swapFloat(y);
    emit modified();
}

float SkywardSwordEditorForm::cameraZ()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0030));
}

void SkywardSwordEditorForm::setCameraZ(double z)
{
    if (z == cameraZ())
        return;

    *(float*)(m_gameData + 0x0030) = zelda::utility::swapFloat(z);
    emit modified();
}


float SkywardSwordEditorForm::cameraRoll()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0034));
}

void SkywardSwordEditorForm::setCameraRoll(double roll)
{
    if (roll == cameraRoll())
        return;

    *(float*)(m_gameData + 0x0034) = zelda::utility::swapFloat(roll);
    emit modified();
}


float SkywardSwordEditorForm::cameraPitch()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0038));
}

void SkywardSwordEditorForm::setCameraPitch(double pitch)
{
    if (pitch == cameraPitch())
        return;

    *(float*)(m_gameData + 0x0038) = zelda::utility::swapFloat(pitch);
    emit modified();
}


float SkywardSwordEditorForm::cameraYaw()
{
    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x003C));
}

void SkywardSwordEditorForm::setCameraYaw(double yaw)
{
    if (yaw == cameraYaw())
        return;

    *(float*)(m_gameData + 0x003C) = zelda::utility::swapFloat(yaw);
    emit modified();
}

bool SkywardSwordEditorForm::beetle()
{
    return flag(0x09E6, 0x20);
}

void SkywardSwordEditorForm::setBeetle(bool val)
{
    if (val == beetle())
        return;
    setFlag(0x09E6, 0x20, val);
}

bool SkywardSwordEditorForm::hookBeetle()
{
    return flag(0x09EB, 0x02);
}

void SkywardSwordEditorForm::setHookBeetle(bool val)
{
    if (val == hookBeetle())
        return;

    setFlag(0x09EB, 0x02, val);
}

bool SkywardSwordEditorForm::quickBeetle()
{
    return flag(0x09EB, 0x04);
}

bool SkywardSwordEditorForm::toughBeetle()
{
    return flag(0x09EB, 0x08);
}

bool SkywardSwordEditorForm::bow()
{
    return flag(0x09E4, 0x10);
}

bool SkywardSwordEditorForm::ironBow()
{
    return flag(0x09ED, 0x01);
}

bool SkywardSwordEditorForm::sacredBow()
{
    return flag(0x09ED, 0x02);
}

bool SkywardSwordEditorForm::slingshot()
{
    return flag(0x09E6, 0x10);
}

bool SkywardSwordEditorForm::scattershot()
{
    return flag(0x09EC, 0x80);
}

bool SkywardSwordEditorForm::practiceSword()
{
    return flag(0x09F2, 0x01);
}

bool SkywardSwordEditorForm::goddessSword()
{
    return flag(0x09E4, 0x01);
}

bool SkywardSwordEditorForm::longSword()
{
    return flag(0x09E4, 0x02);
}

bool SkywardSwordEditorForm::whiteSword()
{
    return flag(0x09FB, 0x10);
}

bool SkywardSwordEditorForm::masterSword()
{
    return flag(0x09E4, 0x04);
}

bool SkywardSwordEditorForm::trueMasterSword()
{
    return flag(0x09F3, 0x80);
}

bool SkywardSwordEditorForm::bugnet()
{
    return flag(0x09E8, 0x01);
}

bool SkywardSwordEditorForm::bigBugnet()
{
    return flag(0x09F2, 0x02);
}

bool SkywardSwordEditorForm::gustBellows()
{
    return flag(0x09E6, 0x02);
}

bool SkywardSwordEditorForm::harp()
{
    return flag(0x09F4, 0x02);
}

bool SkywardSwordEditorForm::whip()
{
    return flag(0x09F3, 0x10);
}

bool SkywardSwordEditorForm::clawshot()
{
    return flag(0x09E4, 0x20);
}

bool SkywardSwordEditorForm::diggingMitts()
{
    return flag(0x09E6, 0x40);
}

bool SkywardSwordEditorForm::moleMitts()
{
    return flag(0x09EC, 0x02);
}

bool SkywardSwordEditorForm::sailCloth()
{
    return flag(0x09F4, 0x01);
}

bool SkywardSwordEditorForm::waterDragonScale()
{
    return flag(0x09E9, 0x20);
}

bool SkywardSwordEditorForm::fireShieldEarings()
{
    return flag(0x09F3, 0x20);
}

bool SkywardSwordEditorForm::bomb()
{
    return flag(0x09ED, 0x04);
}

void SkywardSwordEditorForm::onModified()
{
    if (!this->updatesEnabled())
        return;

    m_gameFile->setDirty(true);
    this->setUpdatesEnabled(false);
    updateChecksum();
    updateData();
    this->setUpdatesEnabled(true);
}

Playtime SkywardSwordEditorForm::playtime() const
{
    if (!m_gameData)
        return Playtime();
    Playtime playTime;
    quint64 tmp = qFromBigEndian(*(quint64*)(m_gameData));
    playTime.Days    = (((tmp / TICKS_PER_SECOND) / 60) / 60) / 24;
    playTime.Hours   = (((tmp / TICKS_PER_SECOND) / 60) / 60) % 24;
    playTime.Minutes = (( tmp / TICKS_PER_SECOND) / 60) % 60;
    playTime.Seconds = (( tmp / TICKS_PER_SECOND) % 60);
    return playTime;
}

void SkywardSwordEditorForm::setPlaytime(Playtime val)
{
    if (!m_gameData)
        return;
    if (val == playtime())
        return;

    quint64 totalSeconds = 0;
    totalSeconds += ((val.Days    * 60) * 60) * 24;
    totalSeconds += ( val.Hours   * 60) * 60;
    totalSeconds += ( val.Minutes * 60);
    totalSeconds +=   val.Seconds;
    *(quint64*)(m_gameData) = qToBigEndian(TICKS_PER_SECOND * totalSeconds);

    emit modified();
}

QDateTime SkywardSwordEditorForm::savetime() const
{
    if (!m_gameData)
        return QDateTime::currentDateTime();

    return fromWiiTime(qFromBigEndian(*(quint64*)(m_gameData + 0x0008)));
}


void SkywardSwordEditorForm::setSavetime(const QDateTime& time)
{
    if (!m_gameData)
        return;
    if (time == savetime())
        return;

    *(qint64*)(m_gameData + 0x0008) = qToBigEndian<qint64>(toWiiTime(time));
    emit modified();
}

QString SkywardSwordEditorForm::currentMap() const
{
    return QString::fromAscii(m_gameData + 0x531C);
}

void SkywardSwordEditorForm::setCurrentMap(const QString& map)
{
    if (map == currentMap())
        return;

    memcpy(m_gameData + 0x531C, map.toAscii().data(), 32);
    emit modified();
}

QString SkywardSwordEditorForm::currentArea() const
{
    return QString::fromAscii(m_gameData + 0x533C);
}

void SkywardSwordEditorForm::setCurrentArea(const QString& area)
{
    if (area == currentArea())
        return;

    memcpy(m_gameData + 0x533C, area.toAscii().data(), 32);
    emit modified();
}

QString SkywardSwordEditorForm::currentRoom() const
{
    return QString::fromAscii(m_gameData + 0x535C);
}

void SkywardSwordEditorForm::setCurrentRoom(const QString& room)
{
    if (room == currentRoom())
        return;

    memcpy(m_gameData + 0x535C, room.toAscii().data(), 32);
    emit modified();
}

int SkywardSwordEditorForm::roomID() const
{
    return (uint)(*(uchar*)(m_gameData + 0x5309));
}

void SkywardSwordEditorForm::setRoomID(int id)
{
    if (id == roomID())
        return;

    (*(char*)(m_gameData + 0x5309)) = (char)id;
    emit modified();
}

bool SkywardSwordEditorForm::watchedIntro()
{
    return flag(0x0941, 0x02);
}

void SkywardSwordEditorForm::setIntroWatched(bool val)
{
    if (val == watchedIntro())
        return;

    setFlag(0x0941, 0x02, val);
}

bool SkywardSwordEditorForm::isNight()
{
    return flag(0x53B3, 0x01);
}

void SkywardSwordEditorForm::setNight(bool val)
{
    if (val == isNight())
        return;

    setFlag(0x53B3, 0x01, val);
}

bool SkywardSwordEditorForm::isHeroMode()
{
    return flag(0x08FE, 0x08);
}

void SkywardSwordEditorForm::setHeroMode(bool val)
{
    setFlag(0x08FE, 0x08, val);
}


bool SkywardSwordEditorForm::isNew() const
{
    if (!m_gameData)
        return true;

    return (*(char*)(m_gameData + 0x53AD)) != 0;
}

void SkywardSwordEditorForm::setNew(bool isNew)
{
    if (!m_gameData)
        return;

    if (this->isNew() == isNew)
        return;

    (*(char*)(m_gameData + 0x53AD)) = isNew;
    emit modified();
}

int SkywardSwordEditorForm::checksum()
{
    return qFromBigEndian(*(quint32*)(m_gameData + 0x53BC));
}

void SkywardSwordEditorForm::updateChecksum()
{
    int oldChecksum = checksum();
    *(quint32*)(m_gameData + 0x53BC) = qToBigEndian((quint32)zelda::Checksums::crc32((Uint8*)m_gameData, 0x53BC));

    if (checksum() != oldChecksum)
        emit modified();
}

void SkywardSwordEditorForm::setQuantity(bool isRight, int offset, quint32 val)
{
    if (!m_gameData)
        return;

    quint16 oldVal = qFromBigEndian(*(quint16*)(m_gameData + offset));
    if (!isRight)
    {
        quint16 newVal = (oldVal&127)|(((quint16)val << 7));
        *(quint16*)(m_gameData + offset) = qToBigEndian(newVal);
    }

    if (isRight)
    {
        oldVal = (oldVal >> 7) & 127;
        quint16 newVal = (val|(oldVal << 7));
        *(quint16*)(m_gameData + offset) = qToBigEndian(newVal);
    }

    emit modified();
}

quint32 SkywardSwordEditorForm::quantity(bool isRight, int offset) const
{
    if (!m_gameData)
        return 0;

    if (!isRight)
        return (quint32)(qFromBigEndian((*(quint16*)(m_gameData + offset))) >> 7) & 127;
    else
        return (quint32)(qFromBigEndian(*(quint16*)(m_gameData + offset))) & 127;

}

bool SkywardSwordEditorForm::flag(quint32 offset, quint32 flag)
{
    return (*(char*)(m_gameData + offset) & flag) == flag;
}

void SkywardSwordEditorForm::setFlag(quint32 offset, quint32 flag, bool val)
{
    if (val)
        *(char*)(m_gameData + offset) |= flag;
    else
        *(char*)(m_gameData + offset) &= ~flag;

    emit modified();
}

void SkywardSwordEditorForm::updateData()
{
    this->setUpdatesEnabled(false);
    // "Toolbar" Items
    ui->deletePushButton->setEnabled(!isNew());
    ui->createPushButton->setEnabled(isNew());
    ui->checksumValueLbl->setText("0x" + QString("%1").arg((uint)checksum(), 8, 16, QChar('0')).toUpper());
    ui->tabWidget->setEnabled(!isNew());

    // Play Stats
    ((PlaytimeWidget*)ui->playtime)->setPlaytime(playtime());
    ui->saveTimeEdit->setDateTime(savetime());
    ui->curMapLineEdit->setText(currentMap());
    ui->curAreaLineEdit->setText(currentArea());
    ui->curRoomLineEdit->setText(currentRoom());
    ui->roomIDSpinBox->setValue(roomID());
    ui->introViewedChkBox->setChecked(watchedIntro());
    ui->nightChkbox->setChecked(isNight());
    ui->heroModeChkBox->setChecked(isHeroMode());

    // Player
    ui->nameLineEdit->setText(playerName());
    ui->rupeeSpinBox->setValue(rupees());
    ui->totalHPSpinBox->setValue(totalHP());
    ui->unkHPSpinBox->setValue(unkHP());
    ui->curHPSpinBox->setValue(currentHP());

    // Player Location
    ui->playerXSpinBox->setValue(playerX());
    ui->playerYSpinBox->setValue(playerY());
    ui->playerZSpinBox->setValue(playerZ());
    ui->playerRollSpinBox->setValue(playerRoll());
    ui->playerPitchSpinBox->setValue(playerPitch());
    ui->playerYawSpinBox->setValue(playerYaw());

    // Camera Location
    ui->cameraXSpinBox->setValue(cameraX());
    ui->cameraYSpinBox->setValue(cameraY());
    ui->cameraZSpinBox->setValue(cameraZ());
    ui->cameraRollSpinBox->setValue(cameraRoll());
    ui->cameraPitchSpinBox->setValue(cameraPitch());
    ui->cameraYawSpinBox->setValue(cameraYaw());

    // Equipment
    ui->beetleChkBox->setChecked(beetle());
    ui->hookBeetleChkBox->setChecked(hookBeetle());
    ui->quickBeetleChkBox->setChecked(quickBeetle());
    ui->toughBeetleChkBox->setChecked(toughBeetle());
    ui->bowChkBox->setChecked(bow());
    ui->ironBowChkBox->setChecked(ironBow());
    ui->sacredBowChkBox->setChecked(sacredBow());
    ui->bugNetChkBox->setChecked(bugnet());
    ui->bigBugNetChkBox->setChecked(bigBugnet());
    ui->gustBellowsChkBox->setChecked(gustBellows());
    ui->harpChkBox->setChecked(harp());
    ui->whipChkBox->setChecked(whip());
    ui->clawShotChkBox->setChecked(clawshot());
    ui->diggingMittsChkBox->setChecked(diggingMitts());
    ui->moleMittsChkBox->setChecked(moleMitts());
    ui->sailClothChkBox->setChecked(sailCloth());
    ui->dragonScaleChkBox->setChecked(waterDragonScale());
    ui->fireEaringsChkBox->setChecked(fireShieldEarings());
    ui->bombChkBox->setChecked(bomb());
    ui->slingShotChkBox->setChecked(slingshot());
    ui->scatterShotChkBox->setChecked(scattershot());

    // Swords
    ui->practiceSwdChkBox->setChecked(practiceSword());
    ui->goddessSwdChkBox->setChecked(goddessSword());
    ui->longSwdChkBox->setChecked(longSword());
    ui->whiteSwdChkBox->setChecked(whiteSword());
    ui->masterSwdChkBox->setChecked(masterSword());
    ui->trueMasterSwdChkBox->setChecked(trueMasterSword());
    this->setUpdatesEnabled(true);
}
