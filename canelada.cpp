#include "canelada.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

Canelada::Canelada(QObject *parent) :
        QObject(parent)
{
    read();
}

void Canelada::setDate(QString tmp)
{
    date = tmp;
    persist();
}

void Canelada::addPlayer(QString name)
{

    if(!players.contains(name))
        players.append(name);
    players.sort();
    persist();
}

void Canelada::remPlayer(QString name)
{
    players.removeAt(players.indexOf(name));
    persist();
}

void Canelada::clear()
{
    players.clear();
    persist();
}

QString Canelada::getTopic()
{
    QString topic;
    topic.append(date);
    topic.append(" (");
    topic.append(QString::number(players.size()));
    topic.append(") ");
    for(int i=0;i<players.size();i++)
    {
        topic.append(players.at(i));
        topic.append(", ");
    }
    topic.remove(QRegExp(", $"));
    return topic;
}

void Canelada::persist()
{
    QFile file("canelada.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
    out << "#" << date << "\n";
    for(int i=0;i<players.size();i++)
    {
        out << "@" << players.at(i) << "\n";
    }
    file.close();
}

void Canelada::read()
{
    QFile file("canelada.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line;
        line = in.readLine();
        if(line.startsWith("#"))
        {
            line.remove("#").remove("\n");
            date = line;
        }
        else if(line.startsWith("@"))
        {
            line.remove("@").remove("\n");
            players.append(line);
        }
    }
    players.sort();
}
