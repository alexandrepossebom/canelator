#include <QtCore/QCoreApplication>
#include "ircclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    IrcClient irc("irc.oftc.net", 6667,"#masmorras","canelator");
    return a.exec();
}
