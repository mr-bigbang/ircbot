#include "Network.IRC.Server.hpp"
#include "Network.IRC.Replies.hpp"

#include <QtCore/QDebug>
#include <QtCore/QCoreApplication>
#include <QtCore/QRegularExpression>

namespace Network {
    namespace IRC {
        Server::Server(QString hostname, uint port, QObject *parent) :
            QObject(parent),
            socket(new QSslSocket(this)),
            hostname(hostname),
            port(port)
        {
            QObject::connect(this->socket, &QSslSocket::readyRead, this, &Server::readData);
            // Cast because signal is overloaded
            void (QSslSocket:: *sslError)(const QList<QSslError> &error) = &QSslSocket::sslErrors;
            QObject::connect(this->socket, sslError, this, &Server::sslError);
            QObject::connect(this, &Server::ping, &Server::pong);
            QObject::connect(this, &Server::newCommand, &Server::ircCommand);
        }

        Server::~Server() {
            delete this->socket;
        }

        // public functions
        void Server::connect(QString nickname, QString realname, bool encrypted) {
            this->nickname = nickname;
            this->realname = realname;

            qDebug() << QString("Connecting to host %1 on port %2...").arg(this->hostname).arg(this->port);
            if (encrypted) {
                this->socket->connectToHostEncrypted(this->hostname, this->port);
                QObject::connect(this->socket, &QSslSocket::encrypted, this, &Server::registerConnection);
            } else {
                this->socket->connectToHost(this->hostname, this->port);
                QObject::connect(this->socket, &QSslSocket::connected, this, &Server::registerConnection);
            }
        }

        void Server::registerConnection() {
            this->pass("bazinga"); //TODO Make configurable
            this->nick(this->nickname);
            this->user(this->nickname, this->realname);
        }

        void Server::pass(QString password) {
            // PASS <password>
            qDebug() << "Sending PASS command...";
            QString passCommand = QString("PASS %1\r\n").arg(password);
            this->rawData(passCommand);
            //this->socket->write(passCommand.toStdString().c_str());
        }

        void Server::user(QString nickname, QString realname) {
            // USER <user> <mode> <unused> <realname>
            qDebug() << "Sending USER command...";
            QString userCommand = QString("USER %1 8 * :%2\r\n").arg(nickname).arg(realname);
            this->rawData(userCommand);
            //this->socket->write(userCommand.toStdString().c_str());
        }

        void Server::quit(QString quitMessage) {
            // See also https://stackoverflow.com/questions/13486050
            // QUIT [<quit message>]
            qDebug() << "Sending QUIT command...";
            QString quitCommand = QString("QUIT :%1\r\n").arg(quitMessage);
            this->rawData(quitCommand);
            qDebug() << "Closing socket...";
            this->socket->close();
            emit quitting();
        }

        void Server::privmsg(QString recipient, QString message) {
            // PRIVMSG <msgtarget> <text to be sent>
            qDebug() << "Sending PRIVMSG command...";
            QString privmsgCommand = QString("PRIVMSG %1 :%2\r\n").arg(recipient).arg(message);
            this->rawData(privmsgCommand);
        }

        // private slots/functions
        void Server::sslError(QList<QSslError> listOfErrors) {
            for(QSslError error : listOfErrors) {
                qWarning() << "SSL error!" << error.errorString();
            }
            qWarning() << "Proceeding anyway...";
            this->socket->ignoreSslErrors();
        }

        void Server::pong(QString id) {
            qDebug() << "PING recived! Sending PONG...";
            QString pongCommand = QString("PONG :%1\r\n").arg(id);
            this->rawData(pongCommand);
        }

        void Server::join(QString channel) {
            qDebug() << "Sending JOIN command...";
            qDebug() << "Joining channel" << channel << "...";
            if(!channel.startsWith('#')) {
                channel = channel.prepend('#');
            }

            QString joinCommand = QString("JOIN :%1\r\n").arg(channel);
            this->rawData(joinCommand);
        }

        void Server::nick(QString nickname) {
            qDebug() << "Sending NICK command...";
            qDebug() << "New nickname will be" << nickname;
            QString nickCommand = QString("NICK %1\r\n").arg(nickname);
            this->rawData(nickCommand);
        }

        void Server::notice(QString recipient, QString message) {
            // NOTICE <msgtarget> <text>
            qDebug() << "Sending NOTICE command...";
            QString noticeCommand = QString("NOTICE %1 :%2\r\n").arg(recipient).arg(message);
            this->rawData(noticeCommand);
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
                    recivedCommand.from.parseHostmask(matchedInput.captured("from"));
                    recivedCommand.code_command = matchedInput.captured("command");
                    recivedCommand.to = matchedInput.captured("recipient");
                    recivedCommand.message = matchedInput.captured("message");
                } else {
                    qWarning() << "Not a valid IRC Command!";
                    continue;
                }

                qDebug() << "Parsing command" << command;
                /*qDebug() << "Sender:" << matchedInput.captured("from");
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

        void User::parseHostmask(QString hostmask) {
            QRegularExpression exp(R"((?<nick>.+)\!(?<user>\~?.+)\@(?<host>.+))");
            QRegularExpressionMatch matchedInput = exp.match(hostmask);

            if(matchedInput.hasMatch()) {
                this->nick_ = matchedInput.captured("nick");
                this->user_ = matchedInput.captured("user");
                this->host_ = matchedInput.captured("host");
                this->hostmask_ = hostmask;
            }
        }
    }
}
