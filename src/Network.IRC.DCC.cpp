#include "Network.IRC.DCC.hpp"

DCC::DCC(Network::IRC::Server *server, QObject *parent) : QObject(parent), m_file(new QFile()), m_socket(new QTcpSocket()), m_server(server) {
    qDebug() << "DCC::DCC()";
    QObject::connect(this->m_socket, &QTcpSocket::readyRead, this, &DCC::readData);
}

// TODO Make sure no one is using the Object while an operation is running

void DCC::readData() {
    if(!this->m_file->isOpen()) {
        if(this->m_file->exists()) {
            this->m_file->open(QIODevice::Append);
        } else {
            this->m_file->open(QIODevice::WriteOnly);
        }
    }
    this->m_file->write(this->m_socket->readAll());

    if(this->m_file->size() == this->m_filesize) {
        qDebug() << "Same filesize... Download finished!";
        this->m_file->close();
        emit this->finished();
    }
}

DCC::~DCC() {
    if(this->m_file->isOpen()) {
        this->m_file->close();
    }
    if(this->m_socket->isOpen()) {
        this->m_socket->close();
    }
    this->m_file->deleteLater();
    this->m_socket->deleteLater();
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
    this->m_file->setFileName(this->filename());
    if (this->m_file->exists()) {
        qDebug() << "File already exists. Checking if file is complete...";
        if(this->m_file->size() == this->m_filesize) {
            qDebug() << "File seems to be complete. Aborting...";
            emit this->finished(); // return ???
        } else {
            qDebug() << "File seems to be incomplete. Trying to resume...";
            //this->server->rawData(CTCP::rawCTCP(QString("DCC RESUME %1 %2 %3").arg(filename).arg(port).arg(this->file->size()))); /* ??? */
            emit this->finished(); // Until I figured out how DCC RESUME works...
        }
    }
    this->m_socket->connectToHost(this->m_ip, this->m_port, QTcpSocket::OpenModeFlag::ReadOnly);
}
