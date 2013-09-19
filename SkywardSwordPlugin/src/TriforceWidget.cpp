#include "TriforceWidget.hpp"
#include <QPainter>
#include <QMouseEvent>
#include <QDebug>
#include <QStatusBar>
#include <QMainWindow>

TriforceWidget::TriforceWidget(QWidget *parent) :
    QWidget(parent),
    m_bg(QImage(":/Triforce/Background")),
    m_piece(QImage(":/Triforce/Piece")),
    m_glow(QImage(":/Triforce/Glow")),
    m_courage(false),
    m_courageHovered(false),
    m_power(false),
    m_powerHovered(false),
    m_wisdom(false),
    m_wisdomHovered(false)
{
    setAttribute(Qt::WA_ShowWithoutActivating);
    setMouseTracking(true);
    setBaseSize(m_bg.width(), m_bg.height());
    setMinimumSize(m_bg.width(), m_bg.height());
    setMaximumSize(m_bg.width(), m_bg.height());
}

bool TriforceWidget::isCourageChecked() const
{
    return m_courage;
}

bool TriforceWidget::isWisdomChecked() const
{
    return m_wisdom;
}

bool TriforceWidget::isPowerChecked() const
{
    return m_power;
}

void TriforceWidget::setCourageChecked(bool checked)
{
    m_courage = checked;
}

void TriforceWidget::setWisdomChecked(bool checked)
{
    m_wisdom = checked;
}

void TriforceWidget::setPowerChecked(bool checked)
{
    m_power = checked;
}

void TriforceWidget::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton)
    {
        if (m_courageHovered)
        {
            m_courage ^= 1;
            emit courageClicked();
        }
        else if (m_wisdomHovered)
        {
            m_wisdom ^= 1;
            emit wisdomClicked();
        }
        else if (m_powerHovered)
        {
            m_power ^= 1;
            emit powerClicked();
        }
    }
    update();
}

void TriforceWidget::mouseMoveEvent(QMouseEvent* event)
{
    // reset hover state
    m_courageHovered = m_wisdomHovered = m_powerHovered = false;

    setStatusTip(tr("Triforce Pieces"));
    if ((event->pos().x()    > (m_bg.width()/2)
        && event->pos().x()  < (m_bg.width()/2 + m_piece.width()))
        && (event->pos().y() > (m_bg.height()/2)
        && event->pos().y()  < (m_bg.height()/2 + m_piece.height() - 4)))
    {
        setToolTip(tr("Triforce of Courage"));
        m_courageHovered = true;
    }
    else
    {
        m_courageHovered = false;
    }
    if ((event->pos().x()    > (m_bg.width()/2 - m_piece.width())
        && event->pos().x()  < (m_bg.width()/2))
        && (event->pos().y() > (m_bg.height()/2)
        && event->pos().y()  < (m_bg.height()/2 + m_piece.height() - 4)))
    {
        setToolTip(tr("Triforce of Wisdom"));
        m_wisdomHovered = true;
    }
    else
    {
        m_wisdomHovered = false;
    }

    if ((event->pos().x()    > (m_bg.width()/2 - m_piece.width()/2)
        && event->pos().x()  < (m_bg.width()/2 + m_piece.width()/2))
        && (event->pos().y() > (m_piece.height()/2 + 4)
        && event->pos().y()  < ((m_piece.height()/2 + 4) + (m_piece.height() - 8))))
    {
        setToolTip(tr("Triforce of Power"));
        m_powerHovered = true;
    }
    else
    {
        m_powerHovered = false;
    }

    update();
}

void TriforceWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    // Draw the background first
    painter.drawImage(0, 0, m_bg);

    // Courage goes on the lower left
    if (m_courage)
        painter.drawImage(m_bg.width()/2 - 2, m_bg.height()/2 - 4, m_piece);

    // wisdom goes on the lower right
    if (m_wisdom)
        painter.drawImage(m_piece.width()/2, m_bg.height()/2 - 4, m_piece);


    // power goes at the top
    if (m_power)
        painter.drawImage((m_bg.width() / 2) - (m_piece.width()/2), m_piece.height()/2 + 4, m_piece);

    // Now draw the glow effect
    if (m_courageHovered)
        painter.drawImage(m_bg.width()/2 - 2, m_bg.height()/2 - 4, m_glow);
    if (m_wisdomHovered)
        painter.drawImage(m_piece.width()/2, m_bg.height()/2 - 4, m_glow);
    if (m_powerHovered)
        painter.drawImage((m_bg.width() / 2) - (m_piece.width()/2), m_piece.height()/2 + 4, m_glow);

#ifdef DEBUG
    painter.drawRect(0, 0, width()-1, height()-1);
#endif
    // This broke QFileDialog, why?
    //this->update();
    QWidget::paintEvent(event);
}
