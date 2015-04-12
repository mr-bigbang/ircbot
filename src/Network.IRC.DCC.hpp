#ifndef DCC_H
#define DCC_H

#include <QtCore/QObject>
#include <QtCore/QFile>
#include <QtNetwork/QHostAddress>
#include <QtNetwork/QTcpSocket>

#include "Network.IRC.Server.hpp" //TODO

class DCC : public QObject
{
    Q_OBJECT
public:
    explicit DCC(/*Network::IRC::Server *server*/QObject *parent = 0);
    ~DCC();

    /** //TODO
     * \brief isSendCommand
     * \param command
     * \return
     */
    static bool isSendCommand(const QString &command); //TODO

    /* Getter */
    inline QString filename() const { return this->_filename; }
    inline QHostAddress ip() const { return this->_ip; }
    inline uint port() const { return this->_port; }
    inline int filesize() const { return this->_filesize; }

    /* Setter */
    inline void setFilename(const QString &filename) { this->_filename = filename; }
    inline void setIP(const QHostAddress &ip) { this->_ip = ip; }
    inline void setPort(const uint &port) { this->_port = port; }
    inline void setFilesize(const int &filesize = -1) { this->_filesize = filesize; }
signals:
    void finished();
public slots:
    /** //TODO
     * @brief send
     */
    void send();
private slots:
    void readData();
private:
    QFile *file;
    QTcpSocket *socket;

    QString _filename;
    QHostAddress _ip;
    uint _port;
    int _filesize = -1;

    Network::IRC::Server *server;
};

#endif // DCC_H
