#include "Network.IRC.Server.hpp"
#include "Network.IRC.Replies.hpp"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegularExpression>

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
            QObject::connect(this, SIGNAL(newCommand(IrcCommand)), this, SLOT(ircCommand(IrcCommand)));
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

        void Server::ircCommand(const IrcCommand &command) {

        }

        void Server::readData() {
            // Get all incoming data and split it by line
            QList<QByteArray> input = this->socket->readAll().replace("\r", "").split('\n');

            // Parse each line of the incoming data
            for(QByteArray command : input) {
                if(command.isEmpty()) {
                    continue;
                }

                // Source http://www.mybuddymichael.com/writings/a-regular-expression-for-irc-messages.html
                // Modified slightly so we have named capture groups
                QRegularExpression input(R"(^(?:[:](?<from>\S+) )?(?<command>\S+)(?: (?!:)(?<recipient>.+?))?(?: [:](?<message>.+))?$)");
                QRegularExpressionMatch matchedInput = input.match(command);

                IrcCommand recivedCommand;
                if(matchedInput.hasMatch()) {
                    recivedCommand.from = matchedInput.captured("from");
                    recivedCommand.code_command = matchedInput.captured("command");
                    recivedCommand.to = matchedInput.captured("recipient");
                    recivedCommand.message = matchedInput.captured("message");
                } else {
                    qWarning() << "Not a valid IRC Command!";
                    continue;
                }

                /*
                qDebug() << "Parsing command" << command;
                qDebug() << "Sender:" << matchedInput.captured("from");
                qDebug() << "Command/Code:" << matchedInput.captured("command");
                qDebug() << "Recipient:" << matchedInput.captured("recipient");
                qDebug() << "Message:" << matchedInput.captured("message");
                */

                // Reply to PING requests
                if(recivedCommand.code_command.toLower() == "ping") {
                    emit ping(QString(command.mid(6)));
                    continue;
                }

                // Check if we are connected successfully
                if(recivedCommand.code_command.toInt() == CommandResponse::RPL_ENDOFMOTD) {
                    qDebug() << "Recived response 376 (End of MOTD)";
                    emit connected();
                    continue;
                }

                emit newCommand(recivedCommand);
            }
        }
    }
}
