#include <QtCore/QRegularExpression>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonValue>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>

#include <QtNetwork/QHostAddress> //DCC SEND IP-Adress

#include "myserver.hpp"
#include "Network.IRC.CTCP.hpp"

MyServer::MyServer(const QString &hostname, const int &port, QObject *parent) :
     Server(hostname, port, parent)
{
    QObject::connect(this, &MyServer::nyaa, &MyServer::downloadNyaa);
    QObject::connect(this, &MyServer::searchCrc32, this, &MyServer::getBotData);
}

/*
 * html downloadNyaa()
 *   cr32 parseNyaaHTML()
 * json getBotData()
 *   xdcc_string parseBotHTML()
 *
 *
 */

void MyServer::downloadNyaa(const QString &nyaaUrl) const {
    qDebug() << "downloadNyaa()";
    QNetworkAccessManager *man = new QNetworkAccessManager(this->parent());
    QObject::connect(man, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseNyaaHTML(QNetworkReply*)));
    man->get(QNetworkRequest(QUrl(nyaaUrl)));
}

void MyServer::parseNyaaHTML(QNetworkReply *reply) {
    qDebug() << "parseNyaaHTML()";
    QString html = reply->readAll();
    reply->deleteLater();
    // Search HTML-Code for typical filename
    QRegularExpression torrentName(R"(<td class=\"viewtorrentname\">([a-zA-Z0-9 \]\[\-\.\_\(\)\&\#\;\!\@]*)<\/td>)");
    QRegularExpressionMatch matchedTorrentName = torrentName.match(html);
    // Extract CRC from filename
    QRegularExpression crc(R"(\[([a-zA-Z0-9]{8})\])");
    QRegularExpressionMatch matchedCrc = crc.match(matchedTorrentName.captured());
    QString crc32 = matchedCrc.captured().replace('[', "").replace(']', "");

    emit this->searchCrc32(crc32);
}

void MyServer::getBotData(const QString &searchString) {
    qDebug() << "getBotData()";
    QNetworkAccessManager *man = new QNetworkAccessManager(this->parent());
    QObject::connect(man, SIGNAL(finished(QNetworkReply*)), this, SLOT(parseXdccHTML(QNetworkReply*)));
    man->get(QNetworkRequest(QUrl(QString("https://news.kae.re/api/0.1/search/%1.json").arg(searchString))));
}

void MyServer::parseXdccHTML(QNetworkReply *reply) {
     qDebug() << "parseXdccHTML()";
     QString json = reply->readAll();
     reply->deleteLater();
     QJsonDocument response = QJsonDocument::fromJson(json.toUtf8());
     QJsonObject obj1 = response.object();
     QJsonArray packs = ((obj1["response"].toObject())["data"].toObject())["packs"].toArray();
     if(packs.isEmpty()) {
         qWarning() << "Couldn't find packs!";
         return;
     }

     bool suitableBotFound = false;
     for(QJsonValue pack : packs) {
         QJsonObject botEntry = pack.toObject();
         if(botEntry["botname"] == "KareRaisu" ||
            botEntry["botname"] == "Ginpachi-Sensei" ||
            botEntry["botname"] == "Kerbster") {
             qDebug() << QString("Downloading %1 which is %2...").arg(botEntry["name"].toString()).arg(botEntry["size"].toString());
             this->getXdcc(botEntry["botname"].toString(), botEntry["id"].toInt());
             suitableBotFound = true;
             break;
         }
     }

     // Send warning if no bot for download has been found
     if(!suitableBotFound) {
        qWarning() << "No suitable Bot found";
        this->privmsg("Jinnai", "No suitable XDCC Bot found.");
     }
}

void MyServer::getXdcc(QString botname, int pack) {
    // QString("/msg %1 xdcc send #%2").arg(botname).arg(pack)
    this->privmsg(botname, QString("xdcc send %1").arg(pack));
}

#include "Network.IRC.CTCP.hpp"

void MyServer::ircCommand(const IrcCommand &command) {
    if(CTCP::isCTCP(command.message)) {
        qDebug() << "CTCP!";
    }

    if(command.message == "\001VERSION\001") {
        this->notice(command.from.nick(), CTCP::version("IRCBot", "0.1-alpha", "QT 5.4"));
    }

    if(command.message.startsWith("\001DCC SEND")) {
        QRegularExpression dccSend(R"((?<dcc>DCC)\ (?<command>SEND|CHAT)\ (?<filename>\".+\")\ (?<ip>\d+)\ (?<port>\d+)\ (?<filesize>\d+))");
        QRegularExpressionMatch matchedDccSend = dccSend.match(command.message);
        DCC dcc(this);
        dcc.send(matchedDccSend.captured("filename"), QHostAddress(matchedDccSend.captured("ip").toInt()), matchedDccSend.captured("port").toInt(), matchedDccSend.captured("filesize").toInt());
        return;
    }

    if(command.from.hostmask() == "Jinnai!~Jinnai@Certified.Lolicon") {
        qDebug() << "My master called?!";
        QRegularExpression botCommand(R"(^(?<command>xdcc|nyaa|fuck)\ (?<param1>\S+)\ ?(?<param2>.+)?$)");
        QRegularExpressionMatch matchedBotCommand = botCommand.match(command.message);

        QString order = matchedBotCommand.captured("command").toLower();
        if(command.code_command.toLower() == "privmsg") {
            if(order == "nyaa") {
                emit this->nyaa(matchedBotCommand.captured("param1"));
            } else if(order == "xdcc") {
                emit this->xdcc(matchedBotCommand.captured("param1"), matchedBotCommand.captured("param2"));
            } else if(order == "fuck") {
                if(matchedBotCommand.captured("param1").toLower() == "you!") {
                    this->quit("fuck off");
                }
            } else if(order == "nick") {
                this->nick(matchedBotCommand.captured("param1"));
            } else {
                //What du you want?
            }
        }
    }
}

