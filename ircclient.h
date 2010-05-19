#ifndef IRCCLIENT_H
#define IRCCLIENT_H

#include <QTcpSocket>
#include <QStringList>
#include "canelada.h"

class IrcClient : public QTcpSocket
{
	Q_OBJECT
public:
        IrcClient(const QString &host, int port, const QString &channel = "#canelada", const QString &nick = "canelator");
	~IrcClient();
	void sendCommand(const QString &cmd);
	void sendPrivMessage(const QString to, const QString &msg);
	void parseLine(const QString &cmd);
        void printHelp(const QString &nick);
        void updateTopic();
        void quit();

public slots:
	void slotLogin();
	void slotHandleOutput();
        void slotError(QAbstractSocket::SocketError);

private:
	QString m_host;
	int m_port;
	QString m_channel;
	QString m_nick;
        Canelada canelada;
};

#endif // IRCCLIENT_H
