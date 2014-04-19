#ifndef APPLICATIONLOG_HPP
#define APPLICATIONLOG_HPP

#include <QDialog>
#include <QDateTime>
#include "OutputStreamMonitor.hpp"

namespace Ui {
class ApplicationLog;
}

QT_BEGIN_NAMESPACE
class QAbstractButton;
QT_END_NAMESPACE

class ApplicationLog : public QDialog
{
    Q_OBJECT

public:
    static ApplicationLog* instance();
    ~ApplicationLog();
public slots:
    void debug(const QString& debug);
    void warning(const QString& warning);
    void error(const QString& error);
    void fatal(const QString& fatal);
    void saveLog(const QString& logFilename);
private slots:
    void onButtonPressed();
    void onStdOut(QString msg);
    void onStdLog(QString msg);
    void onStdErr(QString msg);
private:
    explicit ApplicationLog();
    enum Level
    {
        Debug,
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

    void addEntry(Level level, const QString& debug);
    Ui::ApplicationLog *ui;
    QList<Entry> m_messages;
    static ApplicationLog *m_instance;
    OutputStreamMonitor    m_stdoutMonitor;
    OutputStreamMonitor    m_stdlogMonitor;
    OutputStreamMonitor    m_stderrMonitor;
};

#endif // APPLICATIONLOG_HPP
