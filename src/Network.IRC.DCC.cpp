#include "Network.IRC.DCC.hpp"

DCC::DCC(/*Network::IRC::Server *server2*/QObject *parent) : QObject(parent), file(new QFile()), socket(new QTcpSocket())/*, server(server2)*/ {
    qDebug() << "DCC::DCC()";
    QObject::connect(this->socket, &QTcpSocket::readyRead, this, &DCC::readData);
}

// TODO Make sure no one is using the Object while an operation is running

void DCC::readData() {
    if(!this->file->isOpen()) {
        if(this->file->exists()) {
            this->file->open(QIODevice::Append);
        } else {
            this->file->open(QIODevice::WriteOnly);
        }
    }
    this->file->write(this->socket->readAll());

    if(this->file->size() == this->_filesize) {
        qDebug() << "Same filesize... Download finished!";
        this->file->close();
        emit this->finished();
    }
}

DCC::~DCC() {
    if(this->file->isOpen()) {
        this->file->close();
    }
}

void DCC::send() {
    qDebug() << "DCC::send()";
    /* << DCC SEND <filename> <ip> <port> [<file size>]
     * 1. Connect to <ip>:<port>
     * 2. Ignore
     * 3. Resume
     *   >> DCC RESUME <filename> <port> <position>
     *   << DCC ACCEPT <filename> <port> <position>
     */

    // TODO Check if required Member are populated
    this->file->setFileName(this->filename());
    qDebug() << this->_filesize;
    /*if (this->file->exists()) {
        this->server->rawData(CTCP::rawCTCP(QString("DCC RESUME %1 %2 %3").arg(filename).arg(port).arg(this->file->size()))); /* ??? */
    //}
    //this->socket->connectToHost(ip, port, QTcpSocket::OpenModeFlag::ReadOnly);
    this->socket->connectToHost(this->_ip, this->_port, QTcpSocket::OpenModeFlag::ReadOnly);
}
