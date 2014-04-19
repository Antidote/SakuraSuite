#ifndef OUTPUTSTREAMMONITOR_HPP
#define OUTPUTSTREAMMONITOR_HPP

#include <iostream>
#include <streambuf>
#include <string>
#include <QObject>

class OutputStreamMonitor : public QObject, public std::basic_streambuf<char>
{
    Q_OBJECT
public:
    OutputStreamMonitor(std::ostream &stream);
    ~OutputStreamMonitor();

signals:
    void messageRecieved(QString);
protected:
    virtual int_type overflow(int_type v);

    virtual std::streamsize xsputn(const char *p, std::streamsize n);

private:
    std::ostream &m_stream;
    std::streambuf *m_oldBuf;
    std::string m_string;
};

#endif // OUTPUTSTREAMMONITOR_HPP
