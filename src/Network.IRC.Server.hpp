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
        private:
            QSslSocket *socket;
            QString hostname;
            int port;
        private slots:
            void readData();
        };
    }
}

#endif // SERVER_H
