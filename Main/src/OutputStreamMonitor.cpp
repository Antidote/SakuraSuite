#include "OutputStreamMonitor.hpp"

OutputStreamMonitor::OutputStreamMonitor(std::ostream& stream)
    : m_stream(stream)
{
    m_oldBuf = stream.rdbuf();
    stream.rdbuf(this);
}

OutputStreamMonitor::~OutputStreamMonitor()
{
    // output anything that is left
    if (!m_string.empty())
        emit messageRecieved(QString::fromStdString(m_string));
    m_stream.rdbuf(m_oldBuf);
}

std::basic_streambuf<char>::int_type OutputStreamMonitor::overflow(std::basic_streambuf<char>::int_type v)
{
    if (v == '\n')
    {
        emit messageRecieved(QString::fromStdString(m_string));
        m_string = "";
        m_string.clear();
    }
    else
        m_string += v;

    return v;
}

std::streamsize OutputStreamMonitor::xsputn(const char* p, std::streamsize n)
{
    m_string.append(p, p + n);

    std::string::size_type pos = 0;
    while (pos != std::string::npos)
    {
        pos = m_string.find('\n');
        if (pos != std::string::npos)
        {
            std::string tmp(m_string.begin(), m_string.begin() + pos);
            emit messageRecieved(QString::fromStdString(tmp));
            m_string = "";
            m_string.clear();
        }
    }

    return n;
}
