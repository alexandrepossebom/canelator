#include <QtCore/QCoreApplication>
#include "ircclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    IrcClient irc("chat.freenode.net", 6667,"#canelada","canelator");
    return a.exec();
}
