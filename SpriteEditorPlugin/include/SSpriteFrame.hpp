#ifndef SSpRITEFRAME_HPP
#define SSpRITEFRAME_HPP


#include <QVector2D>
#include <QSize>
#include <Types.hpp>

#include <vector>

class SSpriteFrame
{
public:
    /*!
     * \brief SSpriteFrame
     */
    SSpriteFrame();

    /*!
     * \brief SSpriteFrame
     * \param offX
     * \param offY
     * \param texX
     * \param texY
     * \param width
     * \param height
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SSpriteFrame(float offX, float offY, float texX, float texY, Uint32 width, Uint32 height, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief SSpriteFrame
     * \param frameOff
     * \param texOff
     * \param size
     * \param frameTime
     * \param flippedH
     * \param flippedV
     */
    SSpriteFrame(const QVector2D& frameOff, const QVector2D& texOff, const QSize& size, float frameTime, bool flippedH = false, bool flippedV = false);

    /*!
     * \brief setOffset
     * \param x
     * \param y
     */
    void setOffset(float x, float y);

    /*!
     * \brief setOffset
     * \param offset
     */
    void setOffset(const QVector2D& offset);

    /*!
     * \brief offset
     * \return
     */
    QVector2D offset() const;

    /*!
     * \brief setTextureOffset
     * \param x
     * \param y
     */
    void setTextureOffset(float x, float y);

    /*!
     * \brief setTextureOffset
     * \param texOff
     */
    void setTextureOffset(const QVector2D& texOff);

    /*!
     * \brief textureOffset
     * \return
     */
    QVector2D textureOffset() const;

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
     * \brief setFlippedHorizontally
     * \param val
     */
    void setFlippedHorizontally(const bool val);

    /*!
     * \brief flippedHorizontally
     * \return
     */
    bool flippedHorizontally() const;

    /*!
     * \brief setFlippedVertically
     * \param val
     */
    void setFlippedVertically(const bool val);

    /*!
     * \brief flippedVertically
     * \return
     */
    bool flippedVertically() const;

    /*!
     * \brief setFrameTime
     * \param frameTime
     */
    void setFrameTime(float frameTime);

    /*!
     * \brief frameTime
     * \return
     */
    float frameTime() const;

private:
    QVector2D    m_offset;
    QVector2D    m_textureOffset;
    QSize        m_size;
    float        m_frameTime;
    bool         m_flippedH;
    bool         m_flippedV;
};

#endif // SSpRITEFRAME_HPP
