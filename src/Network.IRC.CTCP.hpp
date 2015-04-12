#ifndef CTCP_HPP
#define CTCP_HPP

#include <QtCore/QObject>
#include <QtCore/QDebug> //TODO

// See also http://www.irchelp.org/irchelp/rfc/ctcpspec.html
class CTCP : public QObject
{
    /*
     * << \001FINGER\001
     * >> :from NOTICE to :\001FINGER :#\001
     */

    /*
     * << \001SOURCE\001
     * >> \001SOURCE #:#:#\001
     * >> \001SOURCE\001
     */

    /*
     * << \001USERINFO\001
     * >> \001USERINFO :#\001
     */

    /*
     * << \001USERINFO\001
     * >> \001USERINFO :#\001
     */

    Q_OBJECT
public:
    /** //TODO
     * \brief isCTCP
     * \param testString
     * \return
     */
    static bool isCTCP(const QString &testString) {
        return (testString.startsWith('\001') && testString.endsWith('\001')) ? true : false;
    }
    /** //TODO
     * \brief isUserinfoCommand
     * \param ctcpMessage
     * \return
     */
    static bool isUserinfoCommand(const QString &ctcpMessage) {
        /*
         * << \001USERINFO\001
         * >> \001USERINFO :#\001
         */
        return ctcpMessage.toLower() == "\001userinfo\001" ? true : false;
    }
    /** //TODO
     * \brief isVersionCommand
     * \param ctcpMessage
     * \return
     */
    static bool isVersionCommand(const QString &ctcpMessage) {
        // :from PRIVMSG to :\001VERSION\001
        return ctcpMessage.toLower() == "\001version\001" ? true : false;
    }
    /** //TODO
     * \brief version
     * \param clientName
     * \param clientVersion
     * \param clientEnvironment
     * \return
     */
    static QString version(const QString &clientName, const QString &clientVersion, const QString &clientEnvironment) {
        qDebug() << "CTCP::version()";
        /* << :from PRIVMSG to :\001VERSION\001
         * >> :from NOTICE to :\001VERSION #:#:#\001
         */
        QString versionCommand = QString("VERSION %1:%2:%3").arg(clientName).arg(clientVersion).arg(clientEnvironment);
        return CTCP::rawCTCP(versionCommand);
    }
    /** //TODO
     * \brief rawCTCP
     * \param data
     * \return
     */
    static QString rawCTCP(QString data) { //TODO Private?
        qDebug() << "CTCP::rawCTCP()";
        return QString("\001%1\001\r\n").arg(data);
    }
};

#endif // CTCP_HPP
