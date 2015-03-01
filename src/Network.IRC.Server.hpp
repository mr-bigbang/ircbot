#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QSslSocket>

namespace Network {
    namespace IRC {
        struct IrcCommand {
            QString from;
            QString code_command;
            QString to; //Special case #366: "to channel"
            QString message;
        };

        class Server : public QObject
        {
            Q_OBJECT
        public:
            explicit Server(QString hostname, int port=6667, QObject *parent = 0);
            void connect(QString nickname, QString realname, bool encrypted = false);
            ~Server();
        private slots:
            void pong(QString id);
            void readData();
            void join(QString channelname = "#mr-bigbang");
            void nick(QString nickname);
            void ircCommand(const IrcCommand &command);
            void sslError(QList<QSslError> listOfErrors);
        signals:
            void changeNickname(QString nickname);
            void ping(QString id);
            void connected();
            void newCommand(const IrcCommand &command);
        private:
            QSslSocket *socket;
            QString hostname;
            int port;
        };
    }
}

#endif // SERVER_H
