#ifndef APPLICATIONLOG_HPP
#define APPLICATIONLOG_HPP

#include <QDialog>
#include <QDateTime>

namespace Ui {
class ApplicationLog;
}

class ApplicationLog : public QDialog
{
    Q_OBJECT

public:
    explicit ApplicationLog(QWidget *parent = 0);
    ~ApplicationLog();

public slots:
    void message(const QString& message);
    void warning(const QString& warning);
    void error(const QString& error);
    void fatal(const QString& fatal);

private:
    enum Level
    {
        Message,
        Warning,
        Error,
        Fatal
    };

    struct Entry
    {
        Level level;
        QDateTime dateTime;
        QString message;
    };

    void addEntry(Level level, const QString& message);
    Ui::ApplicationLog *ui;
    QList<Entry> m_messages;
};

#endif // APPLICATIONLOG_HPP
