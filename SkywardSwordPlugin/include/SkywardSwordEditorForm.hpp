#ifndef SKYWARDSWORDEDITORFORM_HPP
#define SKYWARDSWORDEDITORFORM_HPP

#include <QWidget>
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

    explicit SkywardSwordEditorForm(SkywardSwordGameFile* file, const char* data, QWidget *parent = 0);
    ~SkywardSwordEditorForm();

    char* gameData();
public slots:
    void onDelete();
    void onCreate();
    QString playerName() const;
    void setPlayerName(const QString& name);
    void onModified();
    bool isNew() const;
    void setNew(bool isNew);
    uint checksum();
    void updateChecksum();

signals:
    void modified();

private:
    void updateData();
    Ui::SkywardSwordEditorForm *ui;
    SkywardSwordGameFile* m_gameFile;
    char* m_gameData;
};

#endif // SKYWARDSWORDEDITORFORM_HPP
