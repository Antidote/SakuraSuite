#include "PlaytimeWidget.hpp"
#include "ui_PlaytimeWidget.h"
#include <QDebug>

PlaytimeWidget::PlaytimeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::PlaytimeWidget),
    m_blink(false)
{
    ui->setupUi(this);

    m_blinkTimer.setInterval(1000); // Blink every 2 seconds (needs to be tweaked)
    m_blinkTimer.start();
    connect(&m_blinkTimer, SIGNAL(timeout()), this, SLOT(blinkText()));
    connect(ui->daysSpinBox,  SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
    connect(ui->hoursSpinBox, SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
    connect(ui->minsSpinBox,  SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
    connect(ui->secsSpinBox,  SIGNAL(valueChanged(int)), this, SLOT(valueChanged()));
}

PlaytimeWidget::~PlaytimeWidget()
{
    delete ui;
}

void PlaytimeWidget::setPlaytime(Playtime val)
{
    ui->daysSpinBox ->setValue(val.Days);
    ui->hoursSpinBox->setValue(val.Hours);
    ui->minsSpinBox ->setValue(val.Minutes);
    ui->secsSpinBox ->setValue(val.Seconds);
}

void PlaytimeWidget::blinkText()
{
    m_blink ^= 1; // Toggle blink

    if (this->isEnabled())
    {
        ui->hourSepLbl->setProperty("blink", m_blink);
        ui->minSepLbl->setProperty("blink", m_blink);
        ui->secSepLbl->setProperty("blink", m_blink);
        style()->unpolish(ui->hourSepLbl);
        style()->polish(ui->hourSepLbl);
        style()->unpolish(ui->minSepLbl);
        style()->polish(ui->minSepLbl);
        style()->unpolish(ui->secSepLbl);
        style()->polish(ui->secSepLbl);
    }

    m_blinkTimer.start((m_blink ? 500 : 1000));
}

void PlaytimeWidget::valueChanged()
{
    emit playtimeChanged((Playtime){ui->daysSpinBox->value(), ui->hoursSpinBox->value(),
                                    ui->minsSpinBox->value(), ui->secsSpinBox->value()});
}
