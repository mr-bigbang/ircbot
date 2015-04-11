#include "Network.IRC.CTCP.hpp"

DCC::DCC(Network::IRC::Server *server2) : QObject(this), file(new QFile(this)), socket(new QTcpSocket(this)), server(server2) {
    qDebug() << "DCC::DCC()";
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &DCC::readData);
}

void DCC::readData() {
    qDebug() << "DCC::readData()";
    this->file->open(QFile::OpenModeFlag::Append);
    this->file->write(this->socket->readAll());
    this->file->close();
}

void DCC::send(QString filename, QHostAddress ip, uint port, int filesize) {
    qDebug() << "DCC::send()";
    /* << DCC SEND <filename> <ip> <port> [<file size>]
     * 1. Connect to <ip>:<port>
     * 2. Ignore
     * 3. Resume
     *   >> DCC RESUME <filename> <port> <position>
     *   << DCC ACCEPT <filename> <port> <position>
     */
    this->file->setFileName(filename);
    if (this->file->exists()) {
        this->server->rawData(CTCP::rawCTCP(QString("DCC RESUME %1 %2 %3").arg(filename).arg(port).arg(this->file->size()))); /* ??? */
    }
    this->socket->connectToHost(ip, port, QTcpSocket::OpenModeFlag::ReadOnly);
}
