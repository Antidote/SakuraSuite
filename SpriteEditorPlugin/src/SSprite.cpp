#include "SSprite.hpp"
#include "SSpritePart.hpp"
#include "SSpriteFile.hpp"

SSprite::SSprite(SSpriteFile* root)
    : m_root(root),
      m_currentState(0)
{
}

SSprite::SSprite(SSpriteFile* root, const std::string& name)
    : m_root(root),
      m_name(name),
      m_currentState(0)
{
}

SSprite::~SSprite()
{

}

void SSprite::setPosition(const float x, const float y)
{
    setPosition(QVector2D(x, y));
}

void SSprite::setPosition(const QVector2D& pos)
{
    m_position = pos;
}

QVector2D SSprite::position() const
{
    return m_position;
}

void SSprite::setName(const std::string& name)
{
    m_name = name;
}

std::string SSprite::name() const
{
    return m_name;
}

void SSprite::addStateId(int id)
{
    if (m_stateIds.size() >= 65536)
        return;

    if (std::find(m_stateIds.begin(), m_stateIds.end(), id) == m_stateIds.end())
        m_stateIds.push_back(id);
}

int SSprite::stateId(int index) const
{
    if (index >= (int)m_stateIds.size())
        return -1;

    return m_stateIds[index];
}

void SSprite::setStateIds(std::vector<int> ids)
{
    if (ids.size() == 0)
        return;

    m_stateIds = ids;
}

Uint32 SSprite::stateCount() const
{
    return m_stateIds.size();
}

void SSprite::setCurrentState(const Uint32 id)
{
    m_currentState = id;
}

void SSprite::addPart(SSpritePart* part)
{
    for (SSpritePart* tmp : m_parts)
    {
        if (tmp == part)
            return;
    }
    m_parts.push_back(part);
}

void SSprite::setParts(std::vector<SSpritePart*> parts)
{
    if (parts.size() == 0)
        return;

    if (m_parts.size() > 0)
    {
        for (SSpritePart* part : m_parts)
        {
            delete part;
            part = NULL;
        }
        m_parts.clear();
    }

    m_parts = parts;
}

Uint32 SSprite::partCount() const
{
    return m_parts.size();
}

std::vector<SSpritePart*> SSprite::parts() const
{
    return m_parts;
}
