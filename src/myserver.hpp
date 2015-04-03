#ifndef MYSERVER_HPP
#define MYSERVER_HPP

#include <QtCore/QObject>
#include <QNetworkReply>
#include "Network.IRC.Server.hpp"

using namespace Network::IRC;
class MyServer : public Server
{
    Q_OBJECT
public:
    explicit MyServer(const QString &hostname, const int &port=6667, QObject *parent = 0);
signals:
    void nyaa(const QString &nyaaUrl);
    void xdcc(const QString &xdccBot, const QString &xdccCommand);
    void searchCrc32(const QString &crc32);
private slots:
    void ircCommand(const IrcCommand &command) override;
    void downloadNyaa(const QString &nyaaUrl) const;
    void parseNyaaHTML(QNetworkReply *reply);
    void parseXdccHTML(QNetworkReply *reply);
    void getBotData(const QString &searchString);

    void getXdcc(QString botname, int pack);
    //void downloadXDCC(const QString &xdccBot, const QString &xdccCommand) const;

};

#endif // MYSERVER_HPP
