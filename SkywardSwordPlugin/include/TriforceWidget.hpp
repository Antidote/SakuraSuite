// This file is part of WiiKing2 Editor.
//
// WiiKing2 Editor is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Wiiking2 Editor is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with WiiKing2 Editor.  If not, see <http://www.gnu.org/licenses/>

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
