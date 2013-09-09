#ifndef SKYWARDSWORDEDITORFORM_HPP
#define SKYWARDSWORDEDITORFORM_HPP

#include <QWidget>
class GameFile;

namespace Ui {
class SkywardSwordEditorForm;
}

class SkywardSwordEditorForm : public QWidget
{
    Q_OBJECT
public:
    explicit SkywardSwordEditorForm(const char* data, QWidget *parent = 0);
    ~SkywardSwordEditorForm();

    char* gameData();
signals:
    void modified();
public slots:
    QString playerName() const;
    void setPlayerName(const QString& name);
    void onModified();
    bool isNew() const;
private:
    void updateData();
    Ui::SkywardSwordEditorForm *ui;
    char* m_gameData;
};

#endif // SKYWARDSWORDEDITORFORM_HPP
