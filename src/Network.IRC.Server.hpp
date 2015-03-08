#ifndef NETWORK_IRC_SERVER_HPP
#define NETWORK_IRC_SERVER_HPP

#include <QtNetwork/QSslSocket>

namespace Network {
    namespace IRC {
        /**
         * \brief Store disassembled IRC-Command
         */
        struct IrcCommand {
            QString from;
            QString code_command;
            QString to; //Special case #366: "to channel"
            QString message;
        };

        /**
         * \brief Connect to an IRC Server
         */
        class Server : public QObject
        {
            Q_OBJECT
        public:
            /**
             * \brief Constructor
             *
             * \param hostname Name of the IRC-Server
             * \param port Port to connect to
             * \param parent Parent which will supervise the instance
             */
            explicit Server(QString hostname, int port = 6667, QObject *parent = 0);
            /**
             * \brief Connect to the IRC-Server
             *
             * \param nickname Username to be used
             * \param realname Your real name
             * \param encrypted True if an encrypted connection should be used
             */
            void connect(QString nickname, QString realname, bool encrypted = false);
            /**
             * \brief Destructor
             */
            ~Server();
        public slots:
            /**
             * \brief Join a channel
             *
             * Join a given channel by issuing the JOIN command.
             *
             * \param channelname The channel to join
             */
            void join(QString channelname);
            /**
             * \brief Change nickname
             *
             * Change the nickname by issuing the NICK command.
             *
             * \param nickname The new nickname
             */
            void nick(QString nickname);
            void quit(QString quitMessage);
        private slots:
            /**
             * \brief Send a PONG response
             *
             * Reply to a PING request with a PONG response to stay connected.
             *
             * \param id The heartbeat identifier
             */
            void pong(QString id);
            /**
             * \brief Parse all incomming network data
             */
            void readData();
            void ircCommand(const IrcCommand &command);
            void sslError(QList<QSslError> listOfErrors);
            void registerConnection();
            void pass(QString password);
            void user(QString nickname, QString realname);
        signals:
            void ping(QString id);
            void connected();
            void quitting();
            void newCommand(const IrcCommand &command);
        private:
            QSslSocket *socket;
            QString hostname;
            int port;
            QString nickname;
            QString realname;
        };
    }
}

#endif // NETWORK_IRC_SERVER_HPP
