#ifndef TRIFORCEWIDGET_HPP
#define TRIFORCEWIDGET_HPP

#include <QWidget>


class TriforceWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TriforceWidget(QWidget *parent = 0);

    enum TriforcePiece
    {
        Power,
        Wisdom,
        Courage
    };

    bool isCourageChecked() const;
    bool isWisdomChecked() const;
    bool isPowerChecked() const;

signals:
    void courageClicked();
    void wisdomClicked();
    void powerClicked();

public slots:
    void setCourageChecked(bool checked);
    void setWisdomChecked(bool checked);
    void setPowerChecked(bool checked);
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    QImage m_bg;
    QImage m_piece;
    QImage m_glow;

    bool m_courage;
    bool m_courageHovered;
    bool m_power;
    bool m_powerHovered;
    bool m_wisdom;
    bool m_wisdomHovered;
};

#endif // TRIFORCEWIDGET_HPP
