#include "PreferencesDialog.hpp"
#include "ui_PreferencesDialog.h"
#include "Constants.hpp"

#include <QSettings>
#include <QFile>
#include <QDateTime>
#include <QStyleFactory>
#include <QApplication>
#include <QMessageBox>
#include <QUrl>

PreferencesDialog::PreferencesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDialog),
    m_currentChanged(false),
    m_defaultChanged(false),
    m_singleInstance(false)
{
    ui->setupUi(this);
}

PreferencesDialog::~PreferencesDialog()
{
    delete ui;
}

void PreferencesDialog::showEvent(QShowEvent* se)
{
    QDialog::showEvent(se);
    QSettings settings;
    m_currentStyle = settings.value(Constants::Settings::WIIKING2_CURRENT_STYLE).toString();
    m_defaultStyle = settings.value(Constants::Settings::WIIKING2_DEFAULT_STYLE).toString();
    m_singleInstance = settings.value("singleInstance").toBool();

    ui->checkOnStart->setChecked(settings.value(Constants::Settings::WIIKING2_CHECK_ON_START, false).toBool());

    int index = 0;
    this->setUpdatesEnabled(false);
    foreach(QString style, QStyleFactory::keys())
    {
        ui->currentStyleCombo->addItem(style);
        ui->defaultStyleCombo->addItem(style);
        if (!QString::compare(style, m_currentStyle, Qt::CaseInsensitive))
            ui->currentStyleCombo->setCurrentIndex(index);
        if (!QString::compare(style, m_defaultStyle, Qt::CaseInsensitive))
            ui->defaultStyleCombo->setCurrentIndex(index);
        index++;
    }

    ui->updateUrlLineEdit->setText(settings.value(Constants::Settings::WIIKING2_UPDATE_URL, Constants::Settings::WIIKING2_UPDATE_URL_DEFAULT).toString());
    //ui->updateUrlLineEdit->setModified(false);
    ui->singleInstanceCheckBox->setChecked(QSettings().value("singleInstance", false).toBool());
    this->setUpdatesEnabled(true);
}

void PreferencesDialog::accept()
{
    QSettings settings;

    // Check the update url first, so we can bail out if it's invalid
    if (ui->updateUrlLineEdit->isModified() && !ui->updateUrlLineEdit->text().isEmpty())
    {
        if (ui->updateUrlLineEdit->property("valid").toBool())
            settings.setValue(Constants::Settings::WIIKING2_UPDATE_URL, ui->updateUrlLineEdit->text());
        else
        {
            QMessageBox mbox(this);
            mbox.setWindowTitle("Invalid update url...");
            mbox.setText("The url you specified is not valid.<br />"
                         "Please check it and make the necessary corrections.<br />"
                         "All urls must start with http:// or https://");
            mbox.setStandardButtons(QMessageBox::Ok);
            mbox.exec();
            return;
        }
    }

    if (m_currentChanged)
        settings.setValue(Constants::Settings::WIIKING2_CURRENT_STYLE, ui->currentStyleCombo->currentText());
    if (m_defaultChanged)
        settings.setValue(Constants::Settings::WIIKING2_DEFAULT_STYLE, ui->defaultStyleCombo->currentText());

    qApp->setStyle(ui->currentStyleCombo->currentText());

    settings.setValue(Constants::Settings::WIIKING2_CHECK_ON_START, ui->checkOnStart->isChecked());
    settings.setValue("singleInstance", m_singleInstance);

    if (m_singleInstance && !QFile::exists(Constants::WIIKING2_LOCK_FILE))
    {
        QFile file(Constants::WIIKING2_LOCK_FILE);
        if (file.open(QFile::WriteOnly))
        {
            file.seek(0);
            file.write(QString(QDateTime::currentDateTime().toString() + "\n").toAscii());
            file.resize(file.pos());
        }
    }

    QDialog::accept();
}

void PreferencesDialog::reject()
{
    qApp->setStyle(m_currentStyle);

    QDialog::reject();
}

void PreferencesDialog::onCurrentIndexChanged(QString style)
{
    if (!this->updatesEnabled())
        return;

    if (sender() == ui->currentStyleCombo)
    {
        m_currentChanged = true;
        qApp->setStyle(style);
    }
    else if (sender() == ui->defaultStyleCombo)
    {
        m_defaultChanged = true;
    }
}

void PreferencesDialog::onTextChanged(QString text)
{
    QRegExp http("^(http|https)://", Qt::CaseInsensitive);
    if (sender() == ui->updateUrlLineEdit)
    {
        if (text.isEmpty() || !text.contains(http))
        {
            ui->updateUrlLineEdit->setProperty("valid", false);
            ui->statusLabel->setText("Invalid url");
        }
        else
        {
            ui->updateUrlLineEdit->setProperty("valid", true);
            ui->statusLabel->clear();
        }
        style()->unpolish(ui->updateUrlLineEdit);
        style()->polish(ui->updateUrlLineEdit);

        // If the text matches what is currently stored
        // Set the line edit is unmodified
        if (text == QSettings().value(Constants::Settings::WIIKING2_UPDATE_URL, Constants::Settings::WIIKING2_UPDATE_URL).toString())
            ui->updateUrlLineEdit->setModified(false);
    }
}

void PreferencesDialog::onSingleInstanceToggled(bool checked)
{
    m_singleInstance = checked;
}
