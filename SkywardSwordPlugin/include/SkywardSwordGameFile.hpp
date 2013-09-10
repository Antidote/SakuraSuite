#ifndef SKYWARDSWORDGAMEFILE_HPP
#define SKYWARDSWORDGAMEFILE_HPP

#include <GameFile.hpp>
#include <QObject>

class SkywardSwordGameFile : public GameFile
{
    Q_OBJECT
public:
    SkywardSwordGameFile(const PluginInterface* loader, const QString& file = QString());

    QString game() const;
    bool save(const QString &filename = QString());

private slots:
    void onModified();
private:
    char* m_skipData;
    char m_region;
};

#endif // SKYWARDSWORDGAMEFILE_HPP
