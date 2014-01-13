#include "SSpriteFile.hpp"
#include "SSprite.hpp"
#include <iostream>

const Uint32 SSpriteFile::Major = 1;
const Uint32 SSpriteFile::Minor = 0;
const Uint32 SSpriteFile::Revision = 1;
const Uint32 SSpriteFile::Build = 0;
const Uint32 SSpriteFile::Version = Major | (Minor << 8) | (Revision << 16) | (Build << 24);

const Uint32 SSpriteFile::Magic = 'S' | ('P' << 8) | ('R' << 16) | ('S' << 24);


SSpriteFile::SSpriteFile()
    : m_size(QSize(1, 1))
{
}

SSpriteFile::SSpriteFile(Uint32 width, Uint32 height, float originX, float originY)
    : m_size(QSize(width, height)),
      m_origin(QVector2D(originX, originY))
{
}

SSpriteFile::SSpriteFile(const QSize& size, const QVector2D& origin)
    : m_size(size),
      m_origin(origin)
{
}

void SSpriteFile::setSize(Uint32 width, Uint32 height)
{
    setSize(QSize(width, height));
}

void SSpriteFile::setSize(const QSize& size)
{
    m_size = size;
}

QSize SSpriteFile::size() const
{
    return m_size;
}

Uint32 SSpriteFile::width() const
{
    return m_size.width();
}

Uint32 SSpriteFile::height() const
{
    return m_size.height();
}

void SSpriteFile::setOrigin(const float x, const float y)
{
    setOrigin(QVector2D(x, y));
}

void SSpriteFile::setOrigin(const QVector2D& origin)
{
    m_origin = origin;
}

QVector2D SSpriteFile::origin() const
{
    return m_origin;
}

float SSpriteFile::originX() const
{
    return m_origin.x();
}

float SSpriteFile::originY() const
{
    return m_origin.y();
}

void SSpriteFile::addTexture(STexture* texture)
{
    m_textures.push_back(texture);
}

void SSpriteFile::removeTexture(int id)
{
    if (id > (int)m_textures.size() || id < 0)
        return;

    STexture* tex = m_textures[id];
    m_textures.erase(m_textures.begin() + id);
    delete tex;
}

STexture* SSpriteFile::texture(Uint32 id)
{
    if (id >= m_textures.size())
        return NULL;

    return m_textures[id];
}

std::vector<STexture*> SSpriteFile::textures() const
{
    return m_textures;
}

Uint32 SSpriteFile::textureCount() const
{
    return m_textures.size();
}

void SSpriteFile::addSprite(SSprite* sprite)
{
    if (m_sprites.find(sprite->name()) != m_sprites.end())
        return;

    m_sprites[sprite->name()] = sprite;
}

void SSpriteFile::setSprites(std::unordered_map<std::string, SSprite*> sprites)
{
    if (sprites.size() == 0)
        return;
    if (m_sprites.size() > 0)
    {
        for (std::pair<std::string, SSprite*> sprite : m_sprites)
        {
            delete sprite.second;
            sprite.second = NULL;
        }
        m_sprites.clear();
    }

    m_sprites = sprites;
}

SSprite* SSpriteFile::sprite(const std::string& name)
{
    if (m_sprites.find(name) == m_sprites.end())
        return NULL;

    return m_sprites[name];
}

std::unordered_map<std::string, SSprite*> SSpriteFile::sprites() const
{
    return m_sprites;
}

Uint32 SSpriteFile::spriteCount() const
{
    return m_sprites.size();
}

void SSpriteFile::setTextures(std::vector<STexture*> textures)
{
    if (textures.size() == 0)
        return;

    if (m_textures.size() > 0)
    {
        for(STexture* tex : m_textures)
        {
            delete tex;
            tex = NULL;
        }
        m_textures.clear();
    }

    m_textures = textures;
}
