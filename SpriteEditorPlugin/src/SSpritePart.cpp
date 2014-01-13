#include "SSpritePart.hpp"
#include "SSpriteFrame.hpp"
#include "SSprite.hpp"


SSpritePart::SSpritePart(SSprite* root)
    : m_root(root),
      m_hasCollision(false),
      m_currentFrame(NULL),
      m_frameIndex(0)
{
}

SSpritePart::SSpritePart(SSprite* root, const std::string& name, bool hasCollision)
    : m_root(root),
      m_name(name),
      m_hasCollision(hasCollision),
      m_currentFrame(NULL),
      m_frameIndex(0)
{
}

SSpritePart::~SSpritePart()
{

}

void SSpritePart::setName(const std::string& name)
{
    m_name = name;
}

std::string SSpritePart::name() const
{
    return m_name;
}

void SSpritePart::setCollision(bool col)
{
    m_hasCollision = col;
}

bool SSpritePart::hasCollision() const
{
    return m_hasCollision;
}

void SSpritePart::addFrame(SSpriteFrame* frame)
{
    m_frames.push_back(frame);
}

SSpriteFrame* SSpritePart::frame(int id)
{
    if (id < 0 || id >= (int)m_frames.size())
        return NULL;

    return m_frames[id];
}

void SSpritePart::setFrames(std::vector<SSpriteFrame*> frames)
{
    if (frames.size() == 0)
        return;

    if (m_frames.size() > 0)
    {
        for (SSpriteFrame* frame : m_frames)
        {
            delete frame;
            frame = NULL;
        }
        m_frames.clear();
    }

    if (!m_currentFrame)
    {
        m_currentFrame = frames[0];
        updateTexture();
    }

    m_frames = frames;
}

std::vector<SSpriteFrame*> SSpritePart::frames() const
{
    return m_frames;
}

Uint32 SSpritePart::frameCount() const
{
    return m_frames.size();
}

//void SSpritePart::update(const sf::Time& dt)
//{
//    if (!m_root)
//        return;

//    m_currentTime += dt;
//    m_partSprite.setPosition(m_root->position() + m_currentFrame->offset());
//    if (m_currentFrame && m_currentTime.asSeconds() > m_currentFrame->frameTime())
//    {
//        m_currentTime = sf::seconds(0);
//        m_frameIndex++;
//        if (m_frameIndex >= frameCount())
//            m_frameIndex = 0;

//        m_currentFrame = frame(m_frameIndex);
//        updateTexture();
//    }
//}

//void SSpritePart::draw(sf::RenderTarget& rt)
//{
//    if (!m_root)
//        return;

//    rt.draw(m_partSprite);
//}

void SSpritePart::updateTexture()
{
    float scaleX = 1.f;
    float scaleY = 1.f;
    QVector2D origin(0, 0);
    if (m_currentFrame->flippedHorizontally())
    {
        origin.setX(m_currentFrame->size().width());
        scaleX = -scaleX;
    }
    if (m_currentFrame->flippedVertically())
    {
        origin.setY(m_currentFrame->size().height());
        scaleY = -scaleY;
    }
}
