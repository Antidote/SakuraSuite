#ifndef PLAYTIMEWIDGET_H
#define PLAYTIMEWIDGET_H

#include <QWidget>

#include <QTimer>

namespace Ui {
class PlaytimeWidget;
}

#include "Common.hpp"

class PlaytimeWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit PlaytimeWidget(QWidget *parent = 0);
    ~PlaytimeWidget();

    void setPlaytime(Playtime);

signals:
    void playtimeChanged(Playtime);

private slots:
    void blinkText();
    void valueChanged();

private:
    Ui::PlaytimeWidget *ui;
    QTimer              m_blinkTimer;
    bool                m_blink;
};

#endif // PLAYTIMEWIDGET_H
