// This file is part of Sakura Suite.
//
// Sakura Suite is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Sakura Suite is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Sakura Suite.  If not, see <http://www.gnu.org/licenses/>

#include "include/CopyWidget.hpp"
#include "ui_CopyWidget.h"
#include <QMouseEvent>
#include <QDebug>

CopyWidget::CopyWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CopyWidget)
{
    ui->setupUi(this);
    setWindowTitle(tr("Copy Quest Data to..."));
}

CopyWidget::~CopyWidget()
{
    delete ui;
}

void CopyWidget::setQuestEnabled(Quest quest, bool enabled)
{
    switch(quest)
    {
        case Quest1: ui->quest1CheckBox->setEnabled(enabled); break;
        case Quest2: ui->quest2CheckBox->setEnabled(enabled); break;
        case Quest3: ui->quest3CheckBox->setEnabled(enabled); break;
    }
}

bool CopyWidget::questChecked(Quest quest)
{
    switch(quest)
    {
        case Quest1: return ui->quest1CheckBox->isChecked();
        case Quest2: return ui->quest2CheckBox->isChecked();
        case Quest3: return ui->quest3CheckBox->isChecked();
        default: return false;
    }
}

void CopyWidget::showEvent(QShowEvent* e)
{
    QDialog::showEvent(e);

    ui->quest1CheckBox->setChecked(false);
    ui->quest2CheckBox->setChecked(false);
    ui->quest3CheckBox->setChecked(false);
}

void CopyWidget::focusOutEvent(QFocusEvent* fe)
{
    this->close();
    QWidget::focusOutEvent(fe);
}
