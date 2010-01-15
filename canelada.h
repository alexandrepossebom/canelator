#ifndef CANELADA_H
#define CANELADA_H

#include <QObject>
#include <QStringList>

class Canelada : public QObject
{
Q_OBJECT
public:
    explicit Canelada(QObject *parent = 0);
    QString getTopic();
    void setDate(QString date);
    void addPlayer(QString name);
    void remPlayer(QString name);
    void clear();
private:
    QString date;
    QStringList players;
    void persist();
    void read();
};

#endif // CANELADA_H
