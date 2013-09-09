#include "SkywardSwordEditorForm.hpp"
#include "ui_SkywardSwordEditorForm.h"
#include <QtEndian>
#include <QDebug>

SkywardSwordEditorForm::SkywardSwordEditorForm(const char *data, QWidget *parent)
    : QWidget(parent),
      ui(new Ui::SkywardSwordEditorForm),
      m_gameData((char*)data)
{
      ui->setupUi(this);
      // Make sure the selected tab is "Player Info"
      ui->tabWidget->setCurrentIndex(0);
      connect(this, SIGNAL(modified()), this, SLOT(onModified()));

      onModified();
}

SkywardSwordEditorForm::~SkywardSwordEditorForm()
{
    delete ui;
}

char* SkywardSwordEditorForm::gameData()
{
    return m_gameData;
}

QString SkywardSwordEditorForm::playerName() const
{
    if (!m_gameData)
        return QString("");

    ushort tmpName[8];
    for (int i = 0, j=0; i < 8; ++i, j+= 2)
    {
        tmpName[i] = *(ushort*)(m_gameData + (0x08D4 + j));
        tmpName[i] = qFromBigEndian<quint16>(tmpName[i]);
    }

    return QString(QString::fromUtf16(tmpName));
}

void SkywardSwordEditorForm::setPlayerName(const QString &name)
{
    qDebug() << name;
    if (!m_gameData)
        return;

    if (name == playerName())
        return;

    for (int i = 0, j = 0; i < 8; ++i, ++j)
    {
        if (i > name.length())
        {
            *(ushort*)(m_gameData + (0x08D4 + j++)) = 0;
            continue;
        }
        *(ushort*)(m_gameData + (0x08D4 + j++)) = qToBigEndian<quint16>(name.utf16()[i]);
    }

    //this->updateChecksum();
    emit modified();
}

void SkywardSwordEditorForm::onModified()
{
    this->setUpdatesEnabled(false);
    updateData();
    this->setUpdatesEnabled(true);
}

bool SkywardSwordEditorForm::isNew() const
{
    if (!m_gameData)
        return true;

    return (*(char*)(m_gameData + 0x53AD)) != 0;
}

void SkywardSwordEditorForm::updateData()
{
    ui->isNewCheckBox->setChecked(isNew());
    ui->nameLineEdit->setText(playerName());
    ui->rupeeSpinBox->setValue(qFromBigEndian(*(quint16*)(m_gameData + 0x0A5E)));
}
