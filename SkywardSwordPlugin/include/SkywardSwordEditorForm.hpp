#ifndef SKYWARDSWORDEDITORFORM_HPP
#define SKYWARDSWORDEDITORFORM_HPP

#include <QWidget>
#include <QDateTime>
#include "Common.hpp"
class SkywardSwordGameFile;
class SettingsManager;

namespace Ui {
class SkywardSwordEditorForm;
}

class SkywardSwordEditorForm : public QWidget
{
    Q_OBJECT
public:
    // Default coords for new file
    static const float DEFAULT_POS_X;
    static const float DEFAULT_POS_Y;
    static const float DEFAULT_POS_Z;

    enum Region
    {
        REGION_NTSCU = 0x45,
        REGION_NTSCJ = 0x4A,
        REGION_PAL   = 0x50
    };

    explicit SkywardSwordEditorForm(SkywardSwordGameFile* file, const char* data, QWidget *parent = 0);
    ~SkywardSwordEditorForm();

    char* gameData();
public slots:
    // Actions
    void onDelete();
    void onCreate();
    void onModified();

    // Data
    // Play Stats
    Playtime playtime() const;
    void setPlaytime(Playtime val);
    QDateTime savetime() const;
    void setSavetime(const QDateTime& time);
    QString currentMap() const;
    void setCurrentMap(const QString& map);
    QString currentArea() const;
    void setCurrentArea(const QString& area);
    QString currentRoom() const;
    void setCurrentRoom(const QString& room);
    int roomID() const;
    void setRoomID(int id);
    bool watchedIntro();
    void setIntroWatched(bool val);
    bool isNight();
    void setNight(bool val);
    bool isHeroMode();
    void setHeroMode(bool val);

    // Player
    QString playerName() const;
    void setPlayerName(const QString& name);
    quint32 rupees() const;
    void setRupees(int val);
    int totalHP();
    void setTotalHP(int val);
    quint32 unkHP();
    void setUnkHP(int val);
    quint32 currentHP();
    void setCurrentHP(int val);

    // Player Location
    float playerX();
    void setPlayerX(double x);
    float playerY();
    void setPlayerY(double y);
    float playerZ();
    void setPlayerZ(double z);
    float playerRoll();
    void setPlayerRoll(double roll);
    float playerPitch();
    void setPlayerPitch(double pitch);
    float playerYaw();
    void setPlayerYaw(double yaw);

    // Camera Location
    float cameraX();
    void setCameraX(double x);
    float cameraY();
    void setCameraY(double y);
    float cameraZ();
    void setCameraZ(double z);
    float cameraRoll();
    void setCameraRoll(double roll);
    float cameraPitch();
    void setCameraPitch(double pitch);
    float cameraYaw();
    void setCameraYaw(double yaw);

    // Equipment
    bool beetle();
    void setBeetle(bool val);
    bool hookBeetle();
    void setHookBeetle(bool val);
    bool quickBeetle();
    bool toughBeetle();
    bool bow();
    bool ironBow();
    bool sacredBow();
    bool bugnet();
    bool bigBugnet();
    bool gustBellows();
    bool harp();
    bool whip();
    bool clawshot();
    bool diggingMitts();
    bool moleMitts();
    bool sailCloth();
    bool waterDragonScale();
    bool fireShieldEarings();
    bool bomb();
    bool slingshot();
    bool scattershot();

    // Swords
    bool practiceSword();
    bool goddessSword();
    bool longSword();
    bool whiteSword();
    bool masterSword();
    bool trueMasterSword();

    bool isNew() const;
    void setNew(bool isNew);
    int checksum();
    void updateChecksum();

signals:
    void modified();

private:
    void setQuantity(bool isRight, int offset, quint32 val);
    quint32 quantity(bool isRight, int offset) const;
    bool flag(quint32 offset, quint32 flag);
    void setFlag(quint32 offset, quint32 flag, bool val);
    void updateData();
    Ui::SkywardSwordEditorForm *ui;
    SkywardSwordGameFile* m_gameFile;
    char* m_gameData;
};

#endif // SKYWARDSWORDEDITORFORM_HPP
