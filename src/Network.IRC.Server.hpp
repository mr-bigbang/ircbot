#ifndef SERVER_H
#define SERVER_H

#include <QtNetwork/QSslSocket>

namespace Network {
    namespace IRC {
        class Server : public QObject
        {
            Q_OBJECT
        public:
            explicit Server(QString hostname, int port=6667, QObject *parent = 0);
            void connect(QString nickname, QString realname);
            ~Server();
        private slots:
            void pong(QString id);
            void readData();
            void join(QString channelname = "#mr-bigbang");
            void nick(QString nickname);
        signals:
            void changeNickname(QString nickname);
            void ping(QString id);
            void connected();
        private:
            QSslSocket *socket;
            QString hostname;
            int port;
        };
    }
}

#endif // SERVER_H
