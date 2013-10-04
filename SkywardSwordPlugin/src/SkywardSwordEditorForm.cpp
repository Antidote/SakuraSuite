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

#include "SkywardSwordEditorForm.hpp"
#include "ui_SkywardSwordEditorForm.h"
#include "SkywardSwordGameFile.hpp"
#include "SettingsManager.hpp"
#include "SkywardSwordPlugin.hpp"
#include "Constants.hpp"

#include <QtEndian>
#include <Checksums.hpp>
#include <utility.hpp>

SkywardSwordEditorForm::SkywardSwordEditorForm(SkywardSwordGameDocument* file, const char *data, QWidget *parent)
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

void SkywardSwordEditorForm::setGameData(const QByteArray& data)
{
    memcpy(m_gameData, data.data(), data.size());
    emit modified();
}

char* SkywardSwordEditorForm::gameData()
{
    return m_gameData;
}

int SkywardSwordEditorForm::currentTab()
{
    return ui->tabWidget->currentIndex();
}

void SkywardSwordEditorForm::setCurrentTab(int index)
{
    ui->tabWidget->setCurrentIndex(index);
}

void SkywardSwordEditorForm::onDelete()
{
    QMessageBox mbox(this);
    mbox.setWindowTitle("Are you sure?");
    mbox.setText("Are you sure you wish to delete this game?<br />"
                 "This cannot be undone.(yet)");
    mbox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    mbox.exec();

    if (mbox.result() == QMessageBox::Cancel)
        return;

    memset(m_gameData, 0x00, 0x53C0);
    setNew(true);
    emit modified();
}

void SkywardSwordEditorForm::onCreate()
{
    memset(m_gameData, 0x00, 0x53C0);
    setPlayerName(SkywardSwordPlugin::instance()->settings()->defaultPlayerName());
    setCurrentMap("F000");
    setCurrentArea("F000");
    setCurrentRoom("F000");
    ui->saveTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->playerXSpinBox->setValue(-4798.150391f);
    ui->playerYSpinBox->setValue(1237.629517f);
    ui->playerZSpinBox->setValue(-6573.722656f);
    ui->cameraXSpinBox->setValue(-4798.150391f);
    ui->cameraYSpinBox->setValue(1237.629517f);
    ui->cameraZSpinBox->setValue(-6573.722656f);
    ui->curHPSpinBox->setValue(24);
    ui->unkHPSpinBox->setValue(24);
    ui->totalHPSpinBox->setValue(24);
    ui->introViewedChkBox->setChecked(true);
    emit modified();
}

void SkywardSwordEditorForm::onCopy()
{
    emit copy(this);
}

Playtime SkywardSwordEditorForm::playtime() const
{
    if (!m_gameData)
        return Playtime();
    Playtime playTime;
    quint64 tmp = qFromBigEndian(*(quint64*)(m_gameData));
    playTime.Hours   = (((tmp / TICKS_PER_SECOND) / 60) / 60);
    playTime.Minutes = (( tmp / TICKS_PER_SECOND) / 60) % 60;
    playTime.Seconds = (( tmp / TICKS_PER_SECOND) % 60);
    return playTime;
}

void SkywardSwordEditorForm::setPlaytime(Playtime val)
{
    quint64 totalSeconds = 0;
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

    return fromWiiTime(qFromBigEndian(*(quint64*)(m_gameData + 0x0008))).addSecs(-(60*60));
}

void SkywardSwordEditorForm::setSavetime(const QDateTime& time)
{
    if (!m_gameData)
        return;
    if (time == savetime())
        return;

    *(qint64*)(m_gameData + 0x0008) = qToBigEndian<qint64>(toWiiTime(time.addSecs((60*60))));
    emit modified();
}

QString SkywardSwordEditorForm::playerName() const
{
    if (!m_gameData)
        return QString("");

    // tmpName has 8 characters
    // the ninth is for the null terminator
    // this prevents garbage data from being displayed
    ushort tmpName[9];
    tmpName[8] = 0;
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

float SkywardSwordEditorForm::player(Coord coord)
{
    switch(coord)
    {
        case XCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0010));
        case YCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0014));
        case ZCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0018));
        case RollCoord: return zelda::utility::swapFloat(*(float*)(m_gameData + 0x001C));
        case PitchCoord:return zelda::utility::swapFloat(*(float*)(m_gameData + 0x001C));
        case YawCoord:  return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0024));
        default:        return 0.f;
    }
}

float SkywardSwordEditorForm::camera(Coord coord)
{
    switch(coord)
    {
        case XCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0028));
        case YCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x002C));
        case ZCoord:    return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0030));
        case RollCoord: return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0034));
        case PitchCoord:return zelda::utility::swapFloat(*(float*)(m_gameData + 0x0038));
        case YawCoord:  return zelda::utility::swapFloat(*(float*)(m_gameData + 0x003C));
        default:        return 0.f;
    }
}

void SkywardSwordEditorForm::setCoord(double val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->playerXSpinBox)
        *(float*)(m_gameData + 0x0010) = zelda::utility::swapFloat(val);
    if (sender() == ui->playerYSpinBox)
        *(float*)(m_gameData + 0x0014) = zelda::utility::swapFloat(val);
    if (sender() == ui->playerZSpinBox)
        *(float*)(m_gameData + 0x0018) = zelda::utility::swapFloat(val);
    if (sender() == ui->playerRollSpinBox)
        *(float*)(m_gameData + 0x001C) = zelda::utility::swapFloat(val);
    if (sender() == ui->playerPitchSpinBox)
        *(float*)(m_gameData + 0x0020) = zelda::utility::swapFloat(val);
    if (sender() == ui->playerYawSpinBox)
        *(float*)(m_gameData + 0x0024) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraXSpinBox)
        *(float*)(m_gameData + 0x0028) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraYSpinBox)
        *(float*)(m_gameData + 0x002C) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraZSpinBox)
        *(float*)(m_gameData + 0x0030) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraRollSpinBox)
        *(float*)(m_gameData + 0x0034) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraPitchSpinBox)
        *(float*)(m_gameData + 0x0038) = zelda::utility::swapFloat(val);
    if (sender() == ui->cameraYawSpinBox)
        *(float*)(m_gameData + 0x003C) = zelda::utility::swapFloat(val);

    emit modified();
}

bool SkywardSwordEditorForm::beetle(BeetleType type)
{
    switch(type)
    {
        case NormalBeetle:return flag(0x09E6, 0x20);
        case HookBeetle:  return flag(0x09EB, 0x02);
        case QuickBeetle: return flag(0x09EB, 0x04);
        case ToughBeetle: return flag(0x09EB, 0x08);
        default:    return false;
    }
}

void SkywardSwordEditorForm::setBeetle(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->beetleChkBox)
    {
        if (!val)
            ui->hookBeetleChkBox->setChecked(false);

        setFlag(0x09E6, 0x20, val);
    }
    else if (sender() == ui->hookBeetleChkBox)
    {
        if (val)
            ui->beetleChkBox->setChecked(true);

        if (!val)
            ui->quickBeetleChkBox->setChecked(false);

        setFlag(0x09EB, 0x02, val);
    }
    else if (sender() == ui->quickBeetleChkBox)
    {
        if (val)
            ui->hookBeetleChkBox->setChecked(true);
        if (!val)
            ui->toughBeetleChkBox->setChecked(false);

        setFlag(0x09EB, 0x04, val);
    }
    else if (sender() == ui->toughBeetleChkBox)
    {
        if (val)
            ui->quickBeetleChkBox->setChecked(true);

        setFlag(0x09EB, 0x08, val);
    }
}

bool SkywardSwordEditorForm::bow(BowType bow)
{
    switch (bow)
    {
        case NormalBow:return flag(0x09E4, 0x10);
        case IronBow:  return flag(0x09ED, 0x01);
        case SacredBow:return flag(0x09ED, 0x02);
        default:       return false;
    }
}


void SkywardSwordEditorForm::setBow(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->bowChkBox)
    {
        if (!val)
            ui->ironBowChkBox->setChecked(false);

        setFlag(0x09E4, 0x10, val);
    }
    else if (sender() == ui->ironBowChkBox)
    {
        if (val)
            ui->bowChkBox->setChecked(true);

        if (!val)
            ui->sacredBowChkBox->setChecked(false);

        setFlag(0x09ED, 0x01, val);
    }
    else if (sender() == ui->sacredBowChkBox)
    {
        if (val)
            ui->ironBowChkBox->setChecked(true);

        setFlag(0x09ED, 0x02, val);
    }
}

bool SkywardSwordEditorForm::slingshot(SlingshotType type)
{
    switch(type)
    {
        case Slingshot:  return flag(0x09E6, 0x10);
        case Scattershot:return flag(0x09EC, 0x80);
        default: return false;
    }
}

void SkywardSwordEditorForm::setSlingshot(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->slingShotChkBox)
    {
        if (!val)
            ui->scatterShotChkBox->setChecked(false);

        setFlag(0x09E6, 0x10, val);
    }
    else if (sender() == ui->scatterShotChkBox)
    {

        if (val)
            ui->slingShotChkBox->setChecked(true);

        setFlag(0x09EC, 0x80, val);
    }
}

int SkywardSwordEditorForm::ammo(SkywardSwordEditorForm::AmmoType type)
{
    quint32 val = qFromBigEndian(*(quint32*)(m_gameData + 0x0A60));
    quint32 ret = 0;

    switch(type)
    {
        case Arrows: ret = (val >> 0 ) & 127; break;
        case Bombs:  ret = (val >> 7 ) & 127; break;
        case Seeds:  ret = (val >> 23) & 127; break;
    }

    return ret;
}

void SkywardSwordEditorForm::setAmmo(int val)
{
    if (!updatesEnabled())
        return;
    quint32 tmp = qFromBigEndian(*(quint32*)(m_gameData + 0x0A60));
    quint32 arrows = (tmp >> 0)  & 127;
    quint32 bombs  = (tmp >> 7)  & 127;
    quint32 seeds  = (tmp >> 23) & 127;

    if (sender() == ui->arrowAmmoSpinBox)
        arrows = val;
    else if (sender() == ui->bombAmmoSpinBox)
        bombs = val;
    else if (sender() == ui->seedAmmoSpinBox)
        seeds = val;

    *(quint32*)(m_gameData + 0x0A60) = qToBigEndian(arrows | (bombs << 7) | (seeds << 23));

    emit modified();
}

bool SkywardSwordEditorForm::practiceSword()
{
    return flag(0x09F2, 0x01);
}

void SkywardSwordEditorForm::setPracticeSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (!val)
        setGoddessSword(false);

    setFlag(0x09F2, 0x01, val);
}

bool SkywardSwordEditorForm::goddessSword()
{
    return flag(0x09E4, 0x01);
}

void SkywardSwordEditorForm::setGoddessSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setPracticeSword(true);

    if (!val)
        setGoddessLongSword(false);

    setFlag(0x09E4, 0x01, val);
}

bool SkywardSwordEditorForm::goddessLongSword()
{
    return flag(0x09E4, 0x02);
}

void SkywardSwordEditorForm::setGoddessLongSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setGoddessSword(true);

    if (!val)
        setGoddessWhiteSword(false);
    setFlag(0x09E4, 0x02, val);
}

bool SkywardSwordEditorForm::goddessWhiteSword()
{
    return flag(0x09FB, 0x10);
}

void SkywardSwordEditorForm::setGoddessWhiteSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setGoddessLongSword(true);

    if (!val)
        setMasterSword(false);

    setFlag(0x09FB, 0x10, val);
}

bool SkywardSwordEditorForm::masterSword()
{
    return flag(0x09E4, 0x04);
}

void SkywardSwordEditorForm::setMasterSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setGoddessWhiteSword(true);
    if (!val)
        setTrueMasterSword(false);

    setFlag(0x09E4, 0x04, val);
}

bool SkywardSwordEditorForm::trueMasterSword()
{
    return flag(0x09F3, 0x80);
}

void SkywardSwordEditorForm::setTrueMasterSword(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setMasterSword(true);

    setFlag(0x09F3, 0x080, val);
}

bool SkywardSwordEditorForm::wallet(WalletType type)
{
    switch(type)
    {
        case SmallWallet:  return true;
        case MediumWallet: return flag(0x09EF, 0x04);
        case BigWallet:    return flag(0x09EF, 0x08);
        case GiantWallet:  return flag(0x09EF, 0x10);
        case TycoonWallet: return flag(0x09EF, 0x20);
    }

    return false;
}

void SkywardSwordEditorForm::setWallet(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->smallWalletChkBox)
        emit modified();
    if (sender() == ui->mediumWalletChkBox)
        setFlag(0x09EF, 0x04, val);
    else if (sender() == ui->bigWalletChkBox)
        setFlag(0x09EF, 0x08, val);
    else if (sender() == ui->giantWalletChkBox)
        setFlag(0x09EF, 0x10, val);
    else if (sender() == ui->tycoonWalletChkBox)
        setFlag(0x09Ef, 0x20, val);
}

bool SkywardSwordEditorForm::triforce(TriforceWidget::TriforcePiece piece)
{
    switch(piece)
    {
        case TriforceWidget::Courage:return flag(0x09ED, 0x20);
        case TriforceWidget::Power:  return flag(0x09ED, 0x40);
        case TriforceWidget::Wisdom: return flag(0x09ED, 0x80);
        default: return false;
    }
}

void SkywardSwordEditorForm::triforceClicked()
{
    if (!updatesEnabled())
        return;

    TriforceWidget* tw = qobject_cast<TriforceWidget*>(ui->triforceWidget);
    if (tw)
    {
        if (tw->isPowerChecked() != triforce(TriforceWidget::Power))
            setFlag(0x09ED, 0x40, tw->isPowerChecked());
        else if (tw->isCourageChecked() != triforce(TriforceWidget::Courage))
            setFlag(0x09ED, 0x20, tw->isCourageChecked());
        else if ((tw->isWisdomChecked() != triforce(TriforceWidget::Wisdom)))
            setFlag(0x09ED, 0x80, tw->isWisdomChecked());
    }
}


bool SkywardSwordEditorForm::bugnet(BugnetType type)
{
    switch(type)
    {
        case Bugnet:    return flag(0x09E8, 0x01);
        case BigBugnet: return flag(0x09F2, 0x02);
        default:        return false;
    }
}

void SkywardSwordEditorForm::setBugnet(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->bugNetChkBox)
    {
        if (!val)
            ui->bigBugNetChkBox->setChecked(false);

        setFlag(0x09E8, 0x01, val);
    }
    else if (sender() == ui->bigBugNetChkBox)
    {
        if (val)
            ui->bugNetChkBox->setChecked(true);

        setFlag(0x09F2, 0x02, val);
    }
}

bool SkywardSwordEditorForm::gustBellows()
{
    return flag(0x09E6, 0x02);
}

void SkywardSwordEditorForm::setGustBellows(bool val)
{
    if (!updatesEnabled())
        return;
    setFlag(0x09E6, 0x02, val);
}

bool SkywardSwordEditorForm::goddessHarp()
{

    return flag(0x09F4, 0x02);
}

void SkywardSwordEditorForm::setGoddessHarp(bool val)
{
    if (!updatesEnabled())
        return;
    setFlag(0x09F4, 0x02, val);
}

bool SkywardSwordEditorForm::whip()
{
    return flag(0x09F3, 0x10);
}

void SkywardSwordEditorForm::setWhip(bool val)
{
    if (!updatesEnabled())
        return;
    setFlag(0x09F3, 0x10, val);
}

bool SkywardSwordEditorForm::clawshot()
{
    return flag(0x09E4, 0x20);
}

void SkywardSwordEditorForm::setClawshot(bool val)
{
    if (!updatesEnabled())
        return;

    setFlag(0x09E4, 0x20, val);
}

bool SkywardSwordEditorForm::diggingMitts()
{
    return flag(0x09E6, 0x40);
}

void SkywardSwordEditorForm::setDiggingMitts(bool val)
{
    if (!updatesEnabled())
        return;

    if (!val)
        setMoleMitts(false);

    setFlag(0x09E6, 0x40, val);
}

bool SkywardSwordEditorForm::moleMitts()
{
    return flag(0x09EC, 0x02);
}

void SkywardSwordEditorForm::setMoleMitts(bool val)
{
    if (!updatesEnabled())
        return;

    if (val)
        setDiggingMitts(true);

    setFlag(0x09EC, 0x02, val);
}

bool SkywardSwordEditorForm::sailCloth()
{
    return flag(0x09F4, 0x01);
}

void SkywardSwordEditorForm::setSailCloth(bool val)
{
    setFlag(0x09F4, 0x01, val);
}

bool SkywardSwordEditorForm::waterDragonScale()
{
    return flag(0x09E9, 0x20);
}

void SkywardSwordEditorForm::setWaterDragonScale(bool val)
{
    setFlag(0x09E9, 0x20, val);
}

bool SkywardSwordEditorForm::fireShieldEarings()
{
    return flag(0x09F3, 0x20);
}

void SkywardSwordEditorForm::setFireShieldEarings(bool val)
{
    setFlag(0x09F3, 0x20, val);
}

bool SkywardSwordEditorForm::bombs()
{
    return flag(0x09ED, 0x04);
}

void SkywardSwordEditorForm::setBombs(bool val)
{
    setFlag(0x09ED, 0x04, val);
}

bool SkywardSwordEditorForm::bug(SkywardSwordEditorForm::BugType bug)
{
    switch(bug)
    {
        case DekuHornet:         return flag(0x08F6, 0x80);
        case BlessedButterfly:   return flag(0x09F2, 0x80);
        case GerudoDragonfly:    return flag(0x09F5, 0x04);
        case StarryFirefly:      return flag(0x09F5, 0x20);
        case WoodlandRhinoBeetle:return flag(0x09F2, 0x08);
        case VolcanicLadybug:    return flag(0x09F2, 0x40);
        case SandCicada:         return flag(0x09F5, 0x02);
        case SkyStagBeetle:      return flag(0x09F5, 0x10);
        case FaronGrasshopper:   return flag(0x09F2, 0x04);
        case SkyloftMantis:      return flag(0x09F2, 0x20);
        case LanayruAnt:         return flag(0x09F5, 0x01);
        case EldinRoller:        return flag(0x09F5, 0x08);
        default:                 return false;
    }
}

void SkywardSwordEditorForm::setBug(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->hornetChkBox)
        setFlag(0x08F6, 0x80, val);
    if (sender() == ui->butterflyChkBox)
        setFlag(0x09F2, 0x80, val);
    if (sender() == ui->dragonflyChkBox)
        setFlag(0x09F5, 0x04, val);
    if (sender() == ui->fireflyChkBox)
        setFlag(0x09F5, 0x20, val);
    if (sender() == ui->rhinoBeetleChkBox)
        setFlag(0x09F2, 0x08, val);
    if (sender() == ui->ladybugChkBox)
        setFlag(0x09F2, 0x40, val);
    if (sender() == ui->sandCicadaChkBox)
        setFlag(0x09F5, 0x02, val);
    if (sender() == ui->stagBeetleChkBox)
        setFlag(0x09F5, 0x10, val);
    if (sender() == ui->grasshopperChkBox)
        setFlag(0x09F2, 0x04, val);
    if (sender() == ui->mantisChkBox)
        setFlag(0x09F2, 0x20, val);
    if (sender() == ui->antChkBox)
        setFlag(0x09F5, 0x01, val);
    if (sender() == ui->eldinRollerChkBox)
        setFlag(0x09F5, 0x08, val);
}

int SkywardSwordEditorForm::bugAmount(SkywardSwordEditorForm::BugType bug)
{
    switch(bug)
    {
        case DekuHornet:          return quantity(true,  0x0A4C);
        case BlessedButterfly:    return quantity(false, 0x0A4A);
        case GerudoDragonfly:     return quantity(true,  0x0A46);
        case StarryFirefly:       return quantity(false, 0x0A44);
        case WoodlandRhinoBeetle: return quantity(false, 0x0A4E);
        case VolcanicLadybug:     return quantity(true,  0x0A4A);
        case SandCicada:          return quantity(false, 0x0A48);
        case SkyStagBeetle:       return quantity(true,  0x0A44);
        case FaronGrasshopper:    return quantity(true,  0x0A4E);
        case SkyloftMantis:       return quantity(false, 0x0A4C);
        case LanayruAnt:          return quantity(true,  0x0A48);
        case EldinRoller:         return quantity(false, 0x0A46);
        default:                  return 0;
    }
}

void SkywardSwordEditorForm::setBugAmount(int val)
{
    if (!updatesEnabled())
        return;

    if (val > 99)
        sender()->setProperty("valid", false);
    else
        sender()->setProperty("valid", true);

    QWidget* w = qobject_cast<QWidget*>(sender());
    if (w)
        style()->polish(w);

    if (sender() == ui->hornetSpinBox)
        setQuantity(true,  0x0A4C, val);
    if (sender() == ui->butterflySpinBox)
        setQuantity(false, 0x0A4A, val);
    if (sender() == ui->dragonflySpinBox)
        setQuantity(true,  0x0A46, val);
    if (sender() == ui->fireflySpinBox)
        setQuantity(false, 0x0A44, val);
    if (sender() == ui->rhinoBeetleSpinBox)
        setQuantity(false, 0x0A4E, val);
    if (sender() == ui->ladybugSpinBox)
        setQuantity(true,  0x0A4A, val);
    if (sender() == ui->sandCicadaSpinBox)
        setQuantity(false, 0x0A48, val);
    if (sender() == ui->stagBeetleSpinBox)
        setQuantity(true,  0x0A44, val);
    if (sender() == ui->grasshopperSpinBox)
        setQuantity(true,  0x0A4E, val);
    if (sender() == ui->mantisSpinBox)
        setQuantity(false, 0x0A4C, val);
    if (sender() == ui->antSpinBox)
        setQuantity(true,  0x0A48, val);
    if (sender() == ui->eldinRollerSpinBox)
        setQuantity(false, 0x0A46, val);
}

bool SkywardSwordEditorForm::material(MaterialType material)
{
    switch(material)
    {
        case DekuHornetLarvae:return flag(0x0934, 0x02);
        case BirdFeather:     return flag(0x0934, 0x04);
        case TumbleWeed:      return flag(0x0934, 0x08);
        case LizardTail:      return flag(0x0934, 0x10);
        case EldinOre:        return flag(0x0934, 0x20);
        case AncientFlower:   return flag(0x0934, 0x40);
        case AmberRelic:      return flag(0x0934, 0x80);
        case DuskRelic:       return flag(0x0937, 0x01);
        case JellyBlob:       return flag(0x0937, 0x02);
        case MonsterClaw:     return flag(0x0937, 0x04);
        case MonsterHorn:     return flag(0x0937, 0x08);
        case OrnamentalSkull: return flag(0x0937, 0x10);
        case EvilCrystal:     return flag(0x0937, 0x20);
        case BlueBirdFeather: return flag(0x0937, 0x40);
        case GoldenSkull:     return flag(0x0937, 0x80);
        case GoddessPlume:    return flag(0x0936, 0x01);
        default:              return false;
    }
}

void SkywardSwordEditorForm::setMaterial(bool val)
{
    if (!updatesEnabled())
        return;

    if (sender() == ui->hornetLarvaeChkBox)
        setFlag(0x0934, 0x02, val);
    else if (sender() == ui->birdFeatherChkBox)
        setFlag(0x0934, 0x04, val);
    else if (sender() == ui->tumbleWeedChkBox)
        setFlag(0x0934, 0x08, val);
    else if (sender() == ui->lizardTailChkBox)
        setFlag(0x0934, 0x10, val);
    else if (sender() == ui->eldinOreChkBox)
        setFlag(0x0934, 0x20, val);
    else if (sender() == ui->ancientFlowerChkBox)
        setFlag(0x0934, 0x40, val);
    else if (sender() == ui->amberRelicChkBox)
        setFlag(0x0934, 0x80, val);
    else if (sender() == ui->duskRelicChkBox)
        setFlag(0x0937, 0x01, val);
    else if (sender() == ui->jellyBlobChkBox)
        setFlag(0x0937, 0x02, val);
    else if (sender() == ui->monsterClawChkBox)
        setFlag(0x0937, 0x04, val);
    else if (sender() == ui->monsterHornChkBox)
        setFlag(0x0937, 0x08, val);
    else if (sender() == ui->decoSkullChkBox)
        setFlag(0x0937, 0x10, val);
    else if (sender() == ui->evilCrystalChkBox)
        setFlag(0x0937, 0x20, val);
    else if (sender() == ui->blueBirdFeatherChkBox)
        setFlag(0x0937, 0x40, val);
    else if (sender() == ui->goldenSkullChkBox)
        setFlag(0x0937, 0x80, val);
    else if (sender() == ui->goddessPlumeChkBox)
        setFlag(0x0936, 0x01, val);
}

int SkywardSwordEditorForm::materialAmount(MaterialType material)
{
    switch(material)
    {
        case DekuHornetLarvae: return quantity(true, 0x0A42);
        case BirdFeather:      return quantity(false,0x0A42);
        case TumbleWeed:       return quantity(true, 0x0A40);
        case LizardTail:       return quantity(false,0x0A40);
        case EldinOre:         return quantity(true, 0x0A3E);
        case AncientFlower:    return quantity(false,0x0A3E);
        case AmberRelic:       return quantity(true, 0x0A3C);
        case DuskRelic:        return quantity(false,0x0A3C);
        case JellyBlob:        return quantity(true, 0x0A3A);
        case MonsterClaw:      return quantity(false,0x0A3A);
        case MonsterHorn:      return quantity(true, 0x0A38);
        case OrnamentalSkull:  return quantity(false,0x0A38);
        case EvilCrystal:      return quantity(true, 0x0A36);
        case BlueBirdFeather:  return quantity(false,0x0A36);
        case GoldenSkull:      return quantity(true, 0x0A34);
        case GoddessPlume:     return quantity(false,0x0A34);
        default:               return 0;
    }
}

void SkywardSwordEditorForm::setMaterialAmount(int val)
{
    if (!updatesEnabled())
        return;

    if (val > 99)
        sender()->setProperty("valid", false);
    else
        sender()->setProperty("valid", true);

    QWidget* w = qobject_cast<QWidget*>(sender());
    if (w)
        style()->polish(w);

    if (sender() == ui->hornetLarvaeSpinBox)
        setQuantity(true,  0x0A42, val);
    else if (sender() == ui->birdFeatherSpinBox)
        setQuantity(false, 0x0A42, val);
    else if (sender() == ui->tumbleWeedSpinBox)
        setQuantity(true,  0x0A40, val);
    else if (sender() == ui->lizardTailSpinBox)
        setQuantity(false, 0x0A40, val);
    else if (sender() == ui->eldinOreSpinBox)
        setQuantity(true,  0x0A3E, val);
    else if (sender() == ui->ancientFlowerSpinBox)
        setQuantity(false, 0x0A3E, val);
    else if (sender() == ui->amberRelicSpinBox)
        setQuantity(true,  0x0A3C, val);
    else if (sender() == ui->duskRelicSpinBox)
        setQuantity(false, 0x0A3C, val);
    else if (sender() == ui->jellyBlobSpinBox)
        setQuantity(true,  0x0A3A, val);
    else if (sender() == ui->monsterClawSpinBox)
        setQuantity(false, 0x0A3A, val);
    else if (sender() == ui->monsterHornSpinBox)
        setQuantity(true,  0x0A38, val);
    else if (sender() == ui->decoSkullSpinBox)
        setQuantity(false, 0x0A38, val);
    else if (sender() == ui->evilCrystalSpinBox)
        setQuantity(true,  0x0A36, val);
    else if (sender() == ui->blueBirdFeatherSpinBox)
        setQuantity(false, 0x0A36, val);
    else if (sender() == ui->goldenSkullSpinBox)
        setQuantity(true,  0x0A34, val);
    else if (sender() == ui->goddessPlumeSpinBox)
        setQuantity(false, 0x0A34, val);
}

int SkywardSwordEditorForm::gratitudeCrystals()
{
    return (int)((qFromBigEndian<quint16>(*(quint16*)(m_gameData + 0x0A50)) >> 3) & 127);
}

void SkywardSwordEditorForm::setGratitudeCrystals(int val)
{
    if (!updatesEnabled())
        return;

    if (val > 127)
    {
        ui->gratitudeCrystalsSpinBox->setProperty("valid", false);
        style()->polish(ui->gratitudeCrystalsSpinBox);
        return;
    }
    else
    {
        ui->gratitudeCrystalsSpinBox->setProperty("valid", true);
        style()->polish(ui->gratitudeCrystalsSpinBox);
    }

    quint16 oldVal = qFromBigEndian(*(quint16*)(m_gameData + 0x0A50)) & 0xFC00;
    *(quint16*)(m_gameData + 0x0A50) = qToBigEndian((quint16)(oldVal | (((quint16)val << 3) & 0x03FF)));
    emit modified();
}

void SkywardSwordEditorForm::onModified()
{
    if (!this->updatesEnabled())
        return;

    this->setUpdatesEnabled(false);
    updateChecksum();
    updateData();
    this->setUpdatesEnabled(true);
}

QString SkywardSwordEditorForm::currentMap() const
{
    return QString::fromLatin1(m_gameData + 0x531C);
}

void SkywardSwordEditorForm::setCurrentMap(const QString& map)
{
    if (map == currentMap())
        return;

    memcpy(m_gameData + 0x531C, map.toLatin1().data(), map.length());
    if (map.length() < 32)
        memset(m_gameData + (0x531C + map.length()), 0, 32 - map.length());

    emit modified();
}

QString SkywardSwordEditorForm::currentArea() const
{
    return QString::fromLatin1(m_gameData + 0x533C);
}

void SkywardSwordEditorForm::setCurrentArea(const QString& area)
{
    if (area == currentArea())
        return;

    memcpy(m_gameData + 0x533C, area.toLatin1().data(), area.length());
    if (area.length() < 32)
        memset(m_gameData + (0x533C + area.length()), 0, 32 - area.length());

    emit modified();
}

QString SkywardSwordEditorForm::currentRoom() const
{
    return QString::fromLatin1(m_gameData + 0x535C);
}

void SkywardSwordEditorForm::setCurrentRoom(const QString& room)
{
    if (room == currentRoom())
        return;

    memcpy(m_gameData + 0x535C, room.toLatin1().data(), room.length());
    if (room.length() < 32)
        memset(m_gameData + (0x535C + room.length()), 0, 32 - room.length());
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
    PlaytimeWidget* ptw = qobject_cast<PlaytimeWidget*>(ui->playtime);
    if (ptw)
        ptw->setPlaytime(playtime());

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
    ui->playerXSpinBox->setValue(player(XCoord));
    ui->playerYSpinBox->setValue(player(YCoord));
    ui->playerZSpinBox->setValue(player(ZCoord));
    ui->playerRollSpinBox->setValue(player(RollCoord));
    ui->playerPitchSpinBox->setValue(player(PitchCoord));
    ui->playerYawSpinBox->setValue(player(YawCoord));

    // Camera Location
    ui->cameraXSpinBox->setValue(camera(XCoord));
    ui->cameraYSpinBox->setValue(camera(YCoord));
    ui->cameraZSpinBox->setValue(camera(ZCoord));
    ui->cameraRollSpinBox->setValue(camera(RollCoord));
    ui->cameraPitchSpinBox->setValue(camera(PitchCoord));
    ui->cameraYawSpinBox->setValue(camera(YawCoord));

    // Equipment
    ui->beetleChkBox->setChecked(beetle(NormalBeetle));
    ui->hookBeetleChkBox->setChecked(beetle(HookBeetle));
    ui->quickBeetleChkBox->setChecked(beetle(QuickBeetle));
    ui->toughBeetleChkBox->setChecked(beetle(ToughBeetle));
    ui->bowChkBox->setChecked(bow(NormalBow));
    ui->ironBowChkBox->setChecked(bow(IronBow));
    ui->sacredBowChkBox->setChecked(bow(SacredBow));
    ui->arrowAmmoSpinBox->setValue(ammo(Arrows));
    ui->bugNetChkBox->setChecked(bugnet(Bugnet));
    ui->bigBugNetChkBox->setChecked(bugnet(BigBugnet));
    ui->gustBellowsChkBox->setChecked(gustBellows());
    ui->harpChkBox->setChecked(goddessHarp());
    ui->whipChkBox->setChecked(whip());
    ui->clawShotChkBox->setChecked(clawshot());
    ui->diggingMittsChkBox->setChecked(diggingMitts());
    ui->moleMittsChkBox->setChecked(moleMitts());
    ui->sailClothChkBox->setChecked(sailCloth());
    ui->dragonScaleChkBox->setChecked(waterDragonScale());
    ui->fireEaringsChkBox->setChecked(fireShieldEarings());
    ui->bombChkBox->setChecked(bombs());
    ui->bombAmmoSpinBox->setValue(ammo(Bombs));
    ui->slingShotChkBox->setChecked(slingshot(Slingshot));
    ui->scatterShotChkBox->setChecked(slingshot(Scattershot));
    ui->seedAmmoSpinBox->setValue(ammo(Seeds));

    // Wallets
    ui->smallWalletChkBox->setChecked(wallet(SmallWallet));
    ui->mediumWalletChkBox->setChecked(wallet(MediumWallet));
    ui->bigWalletChkBox->setChecked(wallet(BigWallet));
    ui->giantWalletChkBox->setChecked(wallet(GiantWallet));
    ui->tycoonWalletChkBox->setChecked(wallet(TycoonWallet));

    // Swords
    ui->practiceSwdChkBox->setChecked(practiceSword());
    ui->goddessSwdChkBox->setChecked(goddessSword());
    ui->longSwdChkBox->setChecked(goddessLongSword());
    ui->whiteSwdChkBox->setChecked(goddessWhiteSword());
    ui->masterSwdChkBox->setChecked(masterSword());
    ui->trueMasterSwdChkBox->setChecked(trueMasterSword());

    // Triforce
    TriforceWidget* tri = qobject_cast<TriforceWidget*>(ui->triforceWidget);
    if (tri)
    {
        tri->setCourageChecked(triforce(TriforceWidget::Courage));
        tri->setWisdomChecked(triforce(TriforceWidget::Wisdom));
        tri->setPowerChecked(triforce(TriforceWidget::Power));
    }

    // Bugs
    updateBugs();

    // Materials
    updateMaterials();

    // Gratitude crystals
    ui->gratitudeCrystalsLabel->setEnabled(gratitudeCrystals() > 0);
    ui->gratitudeCrystalsSpinBox->setValue(gratitudeCrystals());
    this->setUpdatesEnabled(true);
    //this->update();
}

void SkywardSwordEditorForm::updateBugs()
{
    ui->hornetChkBox      ->setChecked(bug(DekuHornet));
    ui->hornetSpinBox     ->setValue(bugAmount(DekuHornet));
    ui->butterflyChkBox   ->setChecked(bug(BlessedButterfly));
    ui->butterflySpinBox  ->setValue(bugAmount(BlessedButterfly));
    ui->dragonflyChkBox   ->setChecked(bug(GerudoDragonfly));
    ui->dragonflySpinBox  ->setValue(bugAmount(GerudoDragonfly));
    ui->fireflyChkBox     ->setChecked(bug(StarryFirefly));
    ui->fireflySpinBox    ->setValue(bugAmount(StarryFirefly));
    ui->rhinoBeetleChkBox ->setChecked(bug(WoodlandRhinoBeetle));
    ui->rhinoBeetleSpinBox->setValue(bugAmount(WoodlandRhinoBeetle));
    ui->ladybugChkBox     ->setChecked(bug(VolcanicLadybug));
    ui->ladybugSpinBox    ->setValue(bugAmount(VolcanicLadybug));
    ui->sandCicadaChkBox  ->setChecked(bug(SandCicada));
    ui->sandCicadaSpinBox ->setValue(bugAmount(SandCicada));
    ui->stagBeetleChkBox  ->setChecked(bug(SkyStagBeetle));
    ui->stagBeetleSpinBox ->setValue(bugAmount(SkyStagBeetle));
    ui->grasshopperChkBox ->setChecked(bug(FaronGrasshopper));
    ui->grasshopperSpinBox->setValue(bugAmount(FaronGrasshopper));
    ui->mantisChkBox      ->setChecked(bug(SkyloftMantis));
    ui->mantisSpinBox     ->setValue(bugAmount(SkyloftMantis));
    ui->antChkBox         ->setChecked(bug(LanayruAnt));
    ui->antSpinBox        ->setValue(bugAmount(LanayruAnt));
    ui->eldinRollerChkBox ->setChecked(bug(EldinRoller));
    ui->eldinRollerSpinBox->setValue(bugAmount(EldinRoller));
}


void SkywardSwordEditorForm::updateMaterials()
{
    ui->hornetLarvaeChkBox->setChecked(material(DekuHornetLarvae));
    ui->hornetLarvaeSpinBox->setValue(materialAmount(DekuHornetLarvae));
    ui->birdFeatherChkBox->setChecked(material(BirdFeather));
    ui->birdFeatherSpinBox->setValue(materialAmount(BirdFeather));
    ui->tumbleWeedChkBox->setChecked(material(TumbleWeed));
    ui->tumbleWeedSpinBox->setValue(materialAmount(TumbleWeed));
    ui->lizardTailChkBox->setChecked(material(LizardTail));
    ui->lizardTailSpinBox->setValue(materialAmount(LizardTail));
    ui->eldinOreChkBox->setChecked(material(EldinOre));
    ui->eldinOreSpinBox->setValue(materialAmount(EldinOre));
    ui->ancientFlowerChkBox->setChecked(material(AncientFlower));
    ui->ancientFlowerSpinBox->setValue(materialAmount(AncientFlower));
    ui->amberRelicChkBox->setChecked(material(AmberRelic));
    ui->amberRelicSpinBox->setValue(materialAmount(AmberRelic));
    ui->duskRelicChkBox->setChecked(material(DuskRelic));
    ui->duskRelicSpinBox->setValue(materialAmount(DuskRelic));
    ui->jellyBlobChkBox->setChecked(material(JellyBlob));
    ui->jellyBlobSpinBox->setValue(materialAmount(JellyBlob));
    ui->monsterClawChkBox->setChecked(material(MonsterClaw));
    ui->monsterClawSpinBox->setValue(materialAmount(MonsterClaw));
    ui->monsterHornChkBox->setChecked(material(MonsterHorn));
    ui->monsterHornSpinBox->setValue(materialAmount(MonsterHorn));
    ui->decoSkullChkBox->setChecked(material(OrnamentalSkull));
    ui->decoSkullSpinBox->setValue(materialAmount(OrnamentalSkull));
    ui->evilCrystalChkBox->setChecked(material(EvilCrystal));
    ui->evilCrystalSpinBox->setValue(materialAmount(EvilCrystal));
    ui->blueBirdFeatherChkBox->setChecked(material(BlueBirdFeather));
    ui->blueBirdFeatherSpinBox->setValue(materialAmount(BlueBirdFeather));
    ui->goldenSkullChkBox->setChecked(material(GoldenSkull));
    ui->goldenSkullSpinBox->setValue(materialAmount(GoldenSkull));
    ui->goddessPlumeChkBox->setChecked(material(GoddessPlume));
    ui->goddessPlumeSpinBox->setValue(materialAmount(GoddessPlume));
}
