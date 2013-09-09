#ifndef SKYWARDSWORDGAMEFILE_HPP
#define SKYWARDSWORDGAMEFILE_HPP

#include <GameFile.hpp>

class SkywardSwordGameFile : public GameFile
{
public:
    SkywardSwordGameFile(const QString& file = QString());

    QString game() const;
    bool save(const QString &filename = QString());
private:
    char* m_skipData;
    char m_region;
};

#endif // SKYWARDSWORDGAMEFILE_HPP
