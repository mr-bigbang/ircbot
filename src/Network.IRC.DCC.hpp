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
    explicit DCC(Network::IRC::Server *server, QObject *parent = 0);
    ~DCC();

    /** //TODO
     * \brief isSendCommand
     * \param command
     * \return
     */
    static bool isSendCommand(const QString &command); //TODO

    /* Getter */
    inline QString filename() const { return this->m_filename; }
    inline QHostAddress ip() const { return this->m_ip; }
    inline uint port() const { return this->m_port; }
    inline int filesize() const { return this->m_filesize; }

    /* Setter */
    inline void setFilename(const QString &filename) { this->m_filename = filename; }
    inline void setIP(const QHostAddress &ip) { this->m_ip = ip; }
    inline void setPort(const uint &port) { this->m_port = port; }
    inline void setFilesize(const int &filesize = -1) { this->m_filesize = filesize; }
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
    QFile *m_file;
    QTcpSocket *m_socket;

    QString m_filename;
    QHostAddress m_ip;
    uint m_port;
    int m_filesize = -1;

    Network::IRC::Server *m_server;
};

#endif // DCC_H
