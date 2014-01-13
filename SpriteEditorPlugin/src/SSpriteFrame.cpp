#include "SSpriteFrame.hpp"
#include "SSpritePart.hpp"

SSpriteFrame::SSpriteFrame()
{
}

SSpriteFrame::SSpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH, bool flippedV)
    : m_offset(QVector2D(offX, offY)),
      m_textureOffset(QVector2D(texX, texY)),
      m_size(QSize(width, height)),
      m_frameTime(frameTime),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

SSpriteFrame::SSpriteFrame(const QVector2D& frameOff, const QVector2D& texOff, const QSize& size, float frameTime, bool flippedH, bool flippedV)
    : m_offset(frameOff),
      m_textureOffset(texOff),
      m_size(size),
      m_frameTime(frameTime),
      m_flippedH(flippedH),
      m_flippedV(flippedV)
{
}

void SSpriteFrame::setOffset(float x, float y)
{
    setOffset(QVector2D(x, y));
}

void SSpriteFrame::setOffset(const QVector2D& offset)
{
    m_offset = offset;
}

QVector2D SSpriteFrame::offset() const
{
    return m_offset;
}

void SSpriteFrame::setTextureOffset(float x, float y)
{
    setTextureOffset(QVector2D(x, y));
}

void SSpriteFrame::setTextureOffset(const QVector2D& texOff)
{
    m_textureOffset = texOff;
}

QVector2D SSpriteFrame::textureOffset() const
{
    return m_textureOffset;
}

void SSpriteFrame::setSize(Uint32 width, Uint32 height)
{
    setSize(QSize(width, height));
}

void SSpriteFrame::setSize(const QSize& size)
{
    m_size = size;
}

QSize SSpriteFrame::size() const
{
    return m_size;
}

void SSpriteFrame::setFlippedHorizontally(const bool val)
{
    m_flippedH = val;
}

bool SSpriteFrame::flippedHorizontally() const
{
    return m_flippedH;
}

void SSpriteFrame::setFlippedVertically(const bool val)
{
    m_flippedV = val;
}

bool SSpriteFrame::flippedVertically() const
{
    return m_flippedV;
}

void SSpriteFrame::setFrameTime(float frameTime)
{
    m_frameTime = frameTime;
}

float SSpriteFrame::frameTime() const
{
    return m_frameTime;
}
