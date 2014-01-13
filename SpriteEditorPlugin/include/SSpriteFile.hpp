#ifndef SSPRITEFILE_HPP
#define SSPRITEFILE_HPP

#include <vector>
#include <unordered_map>
#include <string>
#include <Types.hpp>
#include <QVector2D>
#include <QSize>

struct STexture
{
    std::string Filepath;
    bool        Preload;
};

class SSprite;
class SSpriteFile
{
public:
    /*!
     * \brief Major
     */
    static const Uint32 Major;

    /*!
     * \brief Minor
     */
    static const Uint32 Minor;

    /*!
     * \brief Revision
     */
    static const Uint32 Revision;

    /*!
     * \brief Patch
     */
    static const Uint32 Build;

    /*!
     * \brief Version
     */
    static const Uint32 Version;

    /*!
     * \brief Magic
     */
    static const Uint32 Magic;

    /*!
     * \brief SSprite
     */
    SSpriteFile();

    /*!
     * \brief SSpriteFile
     * \param width
     * \param height
     * \param originX
     * \param originY
     */
    SSpriteFile(Uint32 width, Uint32 height, float originX, float originY);

    /*!
     * \brief SSpriteFile
     * \param size
     * \param origin
     */
    SSpriteFile(const QSize& size, const QVector2D& origin);

    /*!
     * \brief setSize
     * \param width
     * \param height
     */
    void setSize(Uint32 width, Uint32 height);

    /*!
     * \brief setSize
     * \param size
     */
    void setSize(const QSize& size);

    /*!
     * \brief size
     * \return
     */
    QSize size() const;

    /*!
     * \brief width
     * \return
     */
    Uint32 width() const;

    /*!
     * \brief height
     * \return
     */
    Uint32 height() const;

    /*!
     * \brief setOrigin
     * \param x
     * \param y
     */
    void setOrigin(const float x, const float y);

    /*!
     * \brief setOrigin
     * \param origin
     */
    void setOrigin(const QVector2D& origin);

    /*!
     * \brief origin
     * \return
     */
    QVector2D origin() const;

    /*!
     * \brief originX
     * \return
     */
    float originX() const;

    /*!
     * \brief originY
     * \return
     */
    float originY() const;

    /*!
     * \brief addTexture
     * \param texture
     */
    void addTexture(STexture* texture);

    /*!
     * \brief removeTexture
     * \param id
     */
    void removeTexture(int id);

    /*!
     * \brief texture
     * \param id
     * \return
     */
    STexture* texture(Uint32 id);

    std::vector<STexture*> textures() const;
    Uint32 textureCount() const;
    /*!
     * \brief setTextures
     * \param textures
     */
    void setTextures(std::vector<STexture*> textures);

    void addSprite(SSprite* sprite);

    void setSprites(std::unordered_map<std::string, SSprite*> sprites);

    SSprite* sprite(const std::string& name);

    std::unordered_map<std::string, SSprite*> sprites() const;
    Uint32 spriteCount() const;

private:
    std::vector<STexture*> m_textures;
    QSize                  m_size;
    QVector2D              m_origin;
    std::unordered_map<std::string, SSprite*> m_sprites;
};

#endif // SSPRITE_HPP
