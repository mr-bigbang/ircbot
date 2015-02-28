#include "Network.IRC.Server.hpp"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>

namespace Network {
    namespace IRC {
        Server::Server(QString hostname, int port, QObject *parent) :
            QObject(parent),
            socket(new QSslSocket(this)),
            hostname(hostname),
            port(port)
        {
            QObject::connect(this->socket, SIGNAL(readyRead()), this, SLOT(readData()));
            QObject::connect(this->socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslError(QList<QSslError>)));
            QObject::connect(this, SIGNAL(ping(QString)), SLOT(pong(QString)));
            QObject::connect(this, SIGNAL(connected()), this, SLOT(join()));
        }

        Server::~Server() {
            qDebug() << "Closing socket...";
            this->socket->close();
            delete this->socket;
        }

        // public functions
        void Server::connect(QString nickname, QString realname, bool encrypted) {
            qDebug() << "Connecting to host" << this->hostname << "on port" << this->port << "...";
            if (encrypted) {
                this->socket->connectToHostEncrypted(this->hostname, this->port);
            } else {
                this->socket->connectToHost(this->hostname, this->port);
            }

            // Send NICK command
            this->nick(nickname);

            qDebug() << "Sending USER command...";
            QString userCommand = QString("USER %1 0 * :%2\r\n").arg(nickname).arg(realname);
            this->socket->write(userCommand.toStdString().c_str());
        }

        // private slots/functions
        void Server::sslError(QList<QSslError> listOfErrors) {
            for(QSslError error : listOfErrors) {
                if(error.error() == QSslError::SelfSignedCertificate) {
                    qWarning() << "Self-signed certificate. Proceeding anyway...";
                    this->socket->ignoreSslErrors();
                    return;
                }
                qCritical() << "Unknown SSL error!" << error.errorString();
            }
        }

        void Server::pong(QString id) {
            qDebug() << "PING recived! Sending PONG...";
            QString pongCommand = QString("PONG :%1\r\n").arg(id);
            this->socket->write(pongCommand.toStdString().c_str());
        }

        void Server::join(QString channel) {
            qDebug() << "Sending JOIN command. Joining channel" << channel << "...";
            QString joinCommand = QString("JOIN :%1\r\n").arg(channel);
            this->socket->write(joinCommand.toStdString().c_str());
        }

        void Server::nick(QString nickname) {
            qDebug() << "Sending NICK command. New nickname will be " << nickname;
            QString nickCommand = QString("NICK %1\r\n").arg(nickname);
            this->socket->write(nickCommand.toStdString().c_str());
        }

        void Server::readData() {
            qDebug() << "New data to read!";
            // Get all incoming data and split it by line
            QList<QByteArray> input = this->socket->readAll().replace("\r", "").split('\n');

            // Parse each line of the incoming data
            for(QByteArray command : input) {
                if(command.isEmpty()) {
                    continue;
                }
                qDebug() << command;

                // Reply to PING requests
                if(command.toLower().startsWith("ping")) {
                    emit ping(QString(command.mid(6)));
                    continue;
                }

                if(command.toLower().contains(" 376 ")) {
                    qDebug() << "End of MOTD";
                    emit connected();
                    continue;
                }
            }
        }
    }
}
