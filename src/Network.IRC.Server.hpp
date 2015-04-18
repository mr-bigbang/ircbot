#ifndef NETWORK_IRC_SERVER_HPP
#define NETWORK_IRC_SERVER_HPP

#include <QtNetwork/QSslSocket>

namespace Network {
    namespace IRC {
        class User
        {
        public:
            User() { }
            void parseHostmask(QString hostmask);
            inline QString nick() const { return this->nick_; }
            inline QString user() const { return this->user_; }
            inline QString host() const { return this->host_; }
            inline QString hostmask() const { return this->hostmask_; }
        private:
            QString hostmask_;
            QString nick_;
            QString user_;
            QString host_;
        };


        /**
         * \brief Store disassembled IRC-Command
         */
        struct IrcCommand {
            User from;
            QString code_command;
            QString to; //Special case #366: "to channel"
            QString message;
        };

        struct Configuration {
            QString connectionPassword;
            QString quitMessage;
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
            explicit Server(QString hostname, uint port = 6667, QObject *parent = 0);
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
            void privmsg(QString recipient, QString message);
            /**
             * \brief Send a NOTICE command
             *
             * Send a private message to 'recipient', but don't expect a reply.
             * See also PRIVMSG.
             *
             * \param recipient The recipient of the message
             * \param message The message to be send
             */
            void notice(QString recipient, QString message);
            inline void rawData(QString data) { this->socket->write(data.toStdString().c_str()); }
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
            virtual void ircCommand(const IrcCommand &command) = 0;
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
        /*protected:
            inline void rawData(QString data) { this->socket->write(data.toStdString().c_str()); }*/
        };
    }
}

#endif // NETWORK_IRC_SERVER_HPP
