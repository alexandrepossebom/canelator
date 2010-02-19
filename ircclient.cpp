#include "ircclient.h"
#include <QDebug>
#include <QStringList>
#include <QProcess>
#include <QTimerEvent>
#include <QDir>
#include <QTime>
#include <QRegExp>


IrcClient::IrcClient(const QString &host, int port, const QString &channel, const QString &nick)
    : m_host(host), m_port(port), m_channel(channel), m_nick(nick)
{
    connect(this, SIGNAL(connected()), this, SLOT(slotLogin()));
    connect(this, SIGNAL(readyRead()), this, SLOT(slotHandleOutput()));
    connectToHost(m_host, m_port);
}

IrcClient::~IrcClient()
{
}

void IrcClient::slotLogin()
{
    sendCommand(QString("NICK %1").arg(m_nick));
    sendCommand(QString("USER canelada %1 canelator :Canelator Bot").arg(m_host));
    sendCommand(QString("JOIN %1").arg(m_channel));
}

void IrcClient::slotHandleOutput()
{
    while (!atEnd())
    {
        parseLine(QString::fromUtf8(readLine()));
    }
}

void IrcClient::sendCommand(const QString &cmd)
{
    write(QString("%1\r\n").arg(cmd).toUtf8(),QString("%1\r\n").arg(cmd).toUtf8().size());
}

void IrcClient::sendPrivMessage(const QString to, const QString &msg)
{    
    QString cmd = QString("PRIVMSG %1 : %2").arg(to).arg(msg);
    sendCommand(cmd);
}

void IrcClient::updateTopic()
{
    QString cmd = QString("TOPIC %1 : %2").arg(m_channel).arg(canelada.getTopic());
    sendCommand(cmd);
}

void IrcClient::parseLine(const QString &cmd)
{
    QString line = cmd;
    line.remove('\n');
    line.remove('\r');
    QStringList list = line.split(" ");
    if (list[0] == "PING")
    {
        qDebug() << "*** PING replying";
        sendCommand(QString("PONG %1").arg(list[1]));
    }
    else if (list.count() > 3 && list[1] == "PRIVMSG")
    {
        QString nick = list[0].split("!")[0].remove(":");
        QRegExp regexPlayerAdd("([a-z][0-9a-z_\\-\\.]+)\\+\\+");
        QRegExp regexPlayerRem("([a-z][0-9a-z_\\-\\.]+)\\-\\-");
        QRegExp regexDay("!date: (.*)$");
        if (line.contains(regexPlayerAdd))
        {
            canelada.addPlayer(regexPlayerAdd.cap(1));
            updateTopic();
        }
        else if (line.contains(regexPlayerRem))
        {
            canelada.remPlayer(regexPlayerRem.cap(1));
            updateTopic();
        }
        else if (line.contains(regexDay))
        {
            canelada.setDate(regexDay.cap(1));
            updateTopic();
        }
        else if (line.contains(":!help"))
        {
            printHelp(nick);
        }
        else if (line.contains(":!clear"))
        {
            canelada.clear();
            updateTopic();
        }
        else if (line.contains(":!topic"))
        {
            updateTopic();
        }
        else if (line.contains(":!update"))
        {
            updateTopic();
        }
        else if (line.contains(":!list"))
        {
            sendPrivMessage(m_channel,canelada.getTopic());
        }
    }
}


void IrcClient::printHelp(const QString &nick)
{
    sendPrivMessage(m_channel, "!clear - remove all players");
    sendPrivMessage(m_channel, "!date: - set a day ex: !date: Today at 10:00");
    sendPrivMessage(m_channel, "!list: - show players");
    sendPrivMessage(m_channel, "!topic - update topic");
}
