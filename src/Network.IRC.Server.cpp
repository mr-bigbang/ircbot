#include "Network.IRC.Server.hpp"

#include <iostream>
#include <QDebug>

namespace Network {
    namespace IRC {
        Server::Server(QString hostname, int port) :
            socket(new QSslSocket(this)),
            hostname(hostname),
            port(port)
        {
            QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readData()));
        }

        Server::~Server() {
            qDebug() << "Sending QUIT command...";
            this->socket->write("QUIT :Goodbye");
            this->socket->waitForBytesWritten();
            delete this->socket;
        }

        // public functions
        void Server::connect(QString realname, QString nickname) {
            qDebug() << "Connecting to host" << this->hostname << "on port" << this->port << "...";
            this->socket->connectToHost(this->hostname, this->port);
            this->socket->waitForConnected();

            qDebug() << "Sending NICK command...";
            QString nickCommand = QString("NICK %1\r\n").arg(nickname);
            this->socket->write(nickCommand.toStdString().c_str());
            this->socket->waitForBytesWritten();

            qDebug() << "Sending USER command...";
            QString userCommand = QString("USER %1 0 * :%2\r\n").arg(nickname).arg(realname);
            this->socket->write(userCommand.toStdString().c_str());
            this->socket->waitForBytesWritten();
        }

        void Server::readData() {
            qDebug() << "New data to read!";
            // Get all incoming data and split it by line
            QList<QByteArray> input = this->socket->readAll().replace("\n", "").split('\r');

            // Parse each line of the incoming data
            for(auto command : input) {
                if(command.isEmpty()) {
                    continue;
                }
                qDebug() << command;

                if(command.toLower().startsWith("ping")) {
                    qDebug() << "PING recived! Sending PONG...";

                    QString pongCommand = QString("PONG :%1\r\n").arg(QString::fromStdString(command.mid(6).toStdString()));
                    this->socket->write(pongCommand.toStdString().c_str());
                    this->socket->waitForBytesWritten();
                }
            }
        }
    }
}
