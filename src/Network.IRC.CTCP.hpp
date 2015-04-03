#ifndef CTCP_HPP
#define CTCP_HPP

#include <QObject>

// See also http://www.irchelp.org/irchelp/rfc/ctcpspec.html
class CTCP
{
public:
    static bool isCTCP(const QString &testString) {
        return (testString.startsWith('\001') && testString.endsWith('\001')) ? true : false;
    }
    static QString version(const QString &clientName, const QString &clientVersion, const QString &clientEnvironment) {
        QString versionCommand = QString("VERSION %1:%2:%3").arg(clientName).arg(clientVersion).arg(clientEnvironment);
        return CTCP::rawCTCP(versionCommand);
    }
private:
    static QString rawCTCP(QString data) {
        return QString("\001%1\001\r\n").arg(data);
    }
};

/*
class DCC : public QObject
{
    Q_OBJECT
public:
    explicit DCC(QObject *parent = 0);
    ~DCC();
signals:
public slots:
    void send(QString ip, int port, int filesize);
    QFile *file;
};

class XDCC : public DCC
{
    Q_OBJECT
public:
    explicit XDCC(QObject *parent = 0);
    ~XDCC();
signals:
public slots:
};



*/
#endif // CTCP_HPP
