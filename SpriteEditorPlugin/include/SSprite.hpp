#ifndef SSPRITE_HPP
#define SSPRITE_HPP

#include <unordered_map>
#include <vector>
#include <string>
#include <Types.hpp>
#include <QVector2D>

class SSpriteFile;
class SSpritePart;

class SSprite
{
public:
    SSprite(SSpriteFile* root);
    SSprite(SSpriteFile* root, const std::string& name);
    virtual ~SSprite();

    virtual void setPosition(const float x, const float y);
    virtual void setPosition(const QVector2D& pos);
    QVector2D position() const;
    void setName(const std::string& name);
    std::string name() const;

    void addStateId(int id);

    /*!
     * \brief Returns the texture id of a given state
     * \param index The index of the id.
     * \return return the state id if it exists, -1 otherwise
     */
    int stateId(int index) const;
    void setStateIds(std::vector<int> ids);
    Uint32 stateCount() const;
    void setCurrentState(const Uint32 id);

    void addPart(SSpritePart* part);
    SSpritePart* part(const std::string& name);
    void setParts(std::vector<SSpritePart*> parts);
    Uint32 partCount() const;

    std::vector<SSpritePart*> parts() const;

private:
    SSpriteFile* m_root;
    std::string  m_name;
    QVector2D    m_position;

    std::vector<int> m_stateIds; //!< Stores the texture id's for each state.
    std::vector<SSpritePart*> m_parts;
    Uint32 m_currentState;
};

#endif // SSPRITE_HPP
