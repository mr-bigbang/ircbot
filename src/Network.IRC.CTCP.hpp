#ifndef CTCP_HPP
#define CTCP_HPP

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "Network.IRC.Server.hpp"

// See also http://www.irchelp.org/irchelp/rfc/ctcpspec.html
class CTCP : public QObject
{
    /*
     * << :from PRIVMSG to :\001VERSION\001
     * >> :from NOTICE to :\001VERSION #:#:#\001
     */

    /*
     * << \001FINGER\001
     * >> :from NOTICE to :\001FINGER :#\001
     */

    /*
     * << \001SOURCE\001
     * >> \001SOURCE #:#:#\001
     * >> \001SOURCE\001
     */

    /*
     * << \001USERINFO\001
     * >> \001USERINFO :#\001
     */

    /*
     * << \001USERINFO\001
     * >> \001USERINFO :#\001
     */

    /*
     * << \001USERINFO\001
     * >> \001USERINFO :#\001
     */

    Q_OBJECT
public:
//    explicit CTCP(QString host, uint port, QObject *parent = 0);
    static bool isCTCP(const QString &testString) {
        return (testString.startsWith('\001') && testString.endsWith('\001')) ? true : false;
    }
    static QString version(const QString &clientName, const QString &clientVersion, const QString &clientEnvironment) {
        qDebug() << "CTCP::version()";
        QString versionCommand = QString("VERSION %1:%2:%3").arg(clientName).arg(clientVersion).arg(clientEnvironment);
        return CTCP::rawCTCP(versionCommand);
    }
    static QString rawCTCP(QString data) {
        qDebug() << "CTCP::rawCTCP()";
        return QString("\001%1\001\r\n").arg(data);
    }
};

class DCC : public QObject
{
    Q_OBJECT
public:
    explicit DCC(Network::IRC::Server *server);
    //~DCC();
public slots:
    void send(QString filename, QHostAddress ip, uint port, int filesize = -1);
private slots:
    void readData();
private:
    QFile *file;
    QTcpSocket *socket;
    Network::IRC::Server *server;
};

#endif // CTCP_HPP
