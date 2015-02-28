#ifndef NETWORK_IRC_REPLIES
#define NETWORK_IRC_REPLIES

namespace Network {
    namespace IRC {
        enum class CommandResponse {
            // RFC2812
            // https://tools.ietf.org/html/rfc2812#section-5.1
            //
            // Range: 001-099: client-server conecction only
            //        200-399: response to commands

            /*
             * The server sends Replies 001 to 004 to a user upon
             * successful registration.
             */
            RPL_WELCOME  = 001, // "Welcome to the Internet Relay Network <nick>!<user>@<host>"
            RPL_YOURHOST = 002, // "Your host is <servername>, running version <ver>"
            RPL_CREATED  = 003, // "This server was created <date>"
            RPL_MYINFO   = 004, // "<servername> <version> <available user modes> <available channel modes>"

            /*
             * Sent by the server to a user to suggest an alternative
             * server.  This is often used when the connection is
             * refused because the server is already full.
             */
            RPL_BOUNCE = 005, // "Try server <server name>, port <port number>"

            /*
             * Reply format used by USERHOST to list replies to
             * the query list.  The reply string is composed as
             * follows:
             *
             * reply = nickname [ "*" ] "=" ( "+" / "-" ) hostname
             *
             * The '*' indicates whether the client has registered
             * as an Operator.  The '-' or '+' characters represent
             * whether the client has set an AWAY message or not
             * respectively.
             */
            RPL_USERHOST = 302, // ":*1<reply> *( " " <reply> )"

            /*
             * Reply format used by ISON to list replies to the
             * query list.
             */
            RPL_ISON = 303, // ":*1<nick> *( " " <nick> )"

            /*
             * These replies are used with the AWAY command (if
             * allowed).  RPL_AWAY is sent to any client sending a
             * PRIVMSG to a client which is away.  RPL_AWAY is only
             * sent by the server to which the client is connected.
             * Replies RPL_UNAWAY and RPL_NOWAWAY are sent when the
             * client removes and sets an AWAY message.
             */
            RPL_AWAY    = 301, // "<nick> :<away message>"
            RPL_UNAWAY  = 305, // ":You are no longer marked as being away"
            RPL_NOWAWAY = 306, // ":You have been marked as being away"

            /*
             * Replies 311 - 313, 317 - 319 are all replies
             * generated in response to a WHOIS message.  Given that
             * there are enough parameters present, the answering
             * server MUST either formulate a reply out of the above
             * numerics (if the query nick is found) or return an
             * error reply.  The '*' in RPL_WHOISUSER is there as
             * the literal character and not as a wild card.  For
             * each reply set, only RPL_WHOISCHANNELS may appear
             * more than once (for long lists of channel names).
             * The '@' and '+' characters next to the channel name
             * indicate whether a client is a channel operator or
             * has been granted permission to speak on a moderated
             * channel.  The RPL_ENDOFWHOIS reply is used to mark
             * the end of processing a WHOIS message.
             */
            RPL_WHOISUSER     = 311, // "<nick> <user> <host> * :<real name>"
            RPL_WHOISSERVER   = 312, // "<nick> <server> :<server info>"
            RPL_WHOISOPERATOR = 313, // "<nick> :is an IRC operator"
            RPL_WHOISIDLE     = 317, // "<nick> <integer> :seconds idle"
            RPL_ENDOFWHOIS    = 318, // "<nick> :End of WHOIS list"
            RPL_WHOISCHANNELS = 319, // "<nick> :*( ( "@" / "+" ) <channel> " " )"

            /*
             * When replying to a WHOWAS message, a server MUST use
             * the replies RPL_WHOWASUSER, RPL_WHOISSERVER or
             * ERR_WASNOSUCHNICK for each nickname in the presented
             * list.  At the end of all reply batches, there MUST
             * be RPL_ENDOFWHOWAS (even if there was only one reply
             * and it was an error).
             */
            RPL_WHOWASUSER  = 314, // "<nick> <user> <host> * :<real name>"
            RPL_ENDOFWHOWAS = 369, // "<nick> :End of WHOWAS"

            RPL_LISTSTART = 321, // Obsolete. Not used.

            /*
             * Replies RPL_LIST, RPL_LISTEND mark the actual replies
             * with data and end of the server's response to a LIST
             * command.  If there are no channels available to return,
             * only the end reply MUST be sent.
             */
            RPL_LIST    = 322, // "<channel> <# visible> :<topic>"
            RPL_LISTEND = 323, // ":End of LIST"

            RPL_UNIQOPIS = 325, // "<channel> <nickname>"

            RPL_CHANNELMODEIS = 324, // "<channel> <mode> <mode params>"

            /*
             * When sending a TOPIC message to determine the
             * channel topic, one of two replies is sent.  If
             * the topic is set, RPL_TOPIC is sent back else
             * RPL_NOTOPIC.
             */
            RPL_NOTOPIC = 331, // "<channel> :No topic is set"
            RPL_TOPIC   = 332, // "<channel> :<topic>"

            /*
             * Returned by the server to indicate that the
             * attempted INVITE message was successful and is
             * being passed onto the end client.
             */
            RPL_INVITING = 341, // "<channel> <nick>"

            /*
             * Returned by a server answering a SUMMON message to
             * indicate that it is summoning that user.
             */
            RPL_SUMMONING = 342, // "<user> :Summoning user to IRC"

            /*
             * When listing the 'invitations masks' for a given channel,
             * a server is required to send the list back using the
             * RPL_INVITELIST and RPL_ENDOFINVITELIST messages.  A
             * separate RPL_INVITELIST is sent for each active mask.
             * After the masks have been listed (or if none present) a
             * RPL_ENDOFINVITELIST MUST be sent.
             */
            RPL_INVITELIST      = 346, // "<channel> <invitemask>"
            RPL_ENDOFINVITELIST = 347, // "<channel> :End of channel invite list"

            /*
             * When listing the 'exception masks' for a given channel,
             * a server is required to send the list back using the
             * RPL_EXCEPTLIST and RPL_ENDOFEXCEPTLIST messages.  A
             * separate RPL_EXCEPTLIST is sent for each active mask.
             * After the masks have been listed (or if none present)
             * a RPL_ENDOFEXCEPTLIST MUST be sent.
             */
            RPL_EXCEPTLIST      = 348, // "<channel> <exceptionmask>"
            RPL_ENDOFEXCEPTLIST = 349, // "<channel> :End of channel exception list"

            /*
             * Reply by the server showing its version details.
             * The <version> is the version of the software being
             * used (including any patchlevel revisions) and the
             * <debuglevel> is used to indicate if the server is
             * running in "debug mode".
             *
             * The "comments" field may contain any comments about
             * the version or further version details.
             */
            RPL_VERSION = 351, // "<version>.<debuglevel> <server> :<comments>"

            /*
             * The RPL_WHOREPLY and RPL_ENDOFWHO pair are used
             * to answer a WHO message.  The RPL_WHOREPLY is only
             * sent if there is an appropriate match to the WHO
             * query.  If there is a list of parameters supplied
             * with a WHO message, a RPL_ENDOFWHO MUST be sent
             * after processing each list item with <name> being
             * the item.
             */
            RPL_WHOREPLY = 352, // "<channel> <user> <host> <server> <nick>
                                // ( "H" / "G" > ["*"] [ ( "@" / "+" ) ]
                                // :<hopcount> <real name>"
            RPL_ENDOFWHO = 315, // "<name> :End of WHO list"

            /*
             * To reply to a NAMES message, a reply pair consisting
             * of RPL_NAMREPLY and RPL_ENDOFNAMES is sent by the
             * server back to the client.  If there is no channel
             * found as in the query, then only RPL_ENDOFNAMES is
             * returned.  The exception to this is when a NAMES
             * message is sent with no parameters and all visible
             * channels and contents are sent back in a series of
             * RPL_NAMEREPLY messages with a RPL_ENDOFNAMES to mark
             * the end.
             */
            RPL_NAMREPLY   = 353, // "( "=" / "*" / "@" ) <channel>
                                  // :[ "@" / "+" ] <nick> *( " " [ "@" / "+" ] <nick> )"
                                  //
                                  // "@" is used for secret channels, "*" for private
                                  // channels, and "=" for others (public channels).
            RPL_ENDOFNAMES = 366, // "<channel> :End of NAMES list"

            /*
             * In replying to the LINKS message, a server MUST send
             * replies back using the RPL_LINKS numeric and mark the
             * end of the list using an RPL_ENDOFLINKS reply.
             */
            RPL_LINKS      = 364, // "<mask> <server> :<hopcount> <server info>"
            RPL_ENDOFLINKS = 365, // "<mask> :End of LINKS list"

            /*
             * When listing the active 'bans' for a given channel,
             * a server is required to send the list back using the
             * RPL_BANLIST and RPL_ENDOFBANLIST messages.  A separate
             * RPL_BANLIST is sent for each active banmask.  After the
             * banmasks have been listed (or if none present) a
             * RPL_ENDOFBANLIST MUST be sent.
             */
            RPL_BANLIST      = 367, // "<channel> <banmask>"
            RPL_ENDOFBANLIST = 368, // "<channel> :End of channel ban list"

            /*
             * A server responding to an INFO message is required to
             * send all its 'info' in a series of RPL_INFO messages
             * with a RPL_ENDOFINFO reply to indicate the end of the
             * replies.
             */
            RPL_INFO      = 371, // ":<string>"
            RPL_ENDOFINFO = 374, // ":End of INFO list"

            /*
             * When responding to the MOTD message and the MOTD file
             * is found, the file is displayed line by line, with
             * each line no longer than 80 characters, using
             * RPL_MOTD format replies.  These MUST be surrounded
             * by a RPL_MOTDSTART (before the RPL_MOTDs) and an
             * RPL_ENDOFMOTD (after).
             */
            RPL_MOTDSTART = 375, // ":- <server> Message of the day - "
            RPL_MOTD      = 372, // ":- <text>"
            RPL_ENDOFMOTD = 376, // ":End of MOTD command"

            /*
             * RPL_YOUREOPER is sent back to a client which has
             * just successfully issued an OPER message and gained
             * operator status.
             */
            RPL_YOUREOPER = 381, // ":You are now an IRC operator"

            /*
             * If the REHASH option is used and an operator sends
             * a REHASH message, an RPL_REHASHING is sent back to
             * the operator.
             */
            RPL_REHASHING = 382, // "<config file> :Rehashing"

            /*
             * Sent by the server to a service upon successful
             * registration.
             */
            RPL_YOURESERVICE = 383, // "You are service <servicename>"

            /*
             * When replying to the TIME message, a server MUST send
             * the reply using the RPL_TIME format above.  The string
             * showing the time need only contain the correct day and
             * time there.  There is no further requirement for the
             * time string.
             */
            RPL_TIME = 391, // "<server> :<string showing server's local time>"

            /*
             * If the USERS message is handled by a server, the
             * replies RPL_USERSTART, RPL_USERS, RPL_ENDOFUSERS and
             * RPL_NOUSERS are used.  RPL_USERSSTART MUST be sent
             * first, following by either a sequence of RPL_USERS
             * or a single RPL_NOUSER.  Following this is
             * RPL_ENDOFUSERS.
             */
            RPL_USERSSTART = 392, // ":UserID   Terminal  Host"
            RPL_USERS      = 393, // ":<username> <ttyline> <hostname>"
            RPL_ENDOFUSERS = 394, // ":End of users"
            RPL_NOUSERS    = 395, // ":Nobody logged in"

            /*
             * The RPL_TRACE* are all returned by the server in
             * response to the TRACE message.  How many are
             * returned is dependent on the TRACE message and
             * whether it was sent by an operator or not.  There
             * is no predefined order for which occurs first.
             * Replies RPL_TRACEUNKNOWN, RPL_TRACECONNECTING and
             * RPL_TRACEHANDSHAKE are all used for connections
             * which have not been fully established and are either
             * unknown, still attempting to connect or in the
             * process of completing the 'server handshake'.
             * RPL_TRACELINK is sent by any server which handles
             * a TRACE message and has to pass it on to another
             * server.  The list of RPL_TRACELINKs sent in
             * response to a TRACE command traversing the IRC
             * network should reflect the actual connectivity of
             * the servers themselves along that path.
             *
             * RPL_TRACENEWTYPE is to be used for any connection
             * which does not fit in the other categories but is
             * being displayed anyway.
             * RPL_TRACEEND is sent to indicate the end of the list.
             */
            RPL_TRACELINK       = 200, // "Link <version & debug level> <destination>
                                       // <next server> V<protocol version>
                                       // <link uptime in seconds> <backstream sendq>
                                       // <upstream sendq>"
            RPL_TRACECONNECTING = 201, // "Try. <class> <server>"
            RPL_TRACEHANDSHAKE  = 202, // "H.S. <class> <server>"
            RPL_TRACEUNKNOWN    = 203, // "???? <class> [<client IP address in dot form>]"
            RPL_TRACEOPERATOR   = 204, // "Oper <class> <nick>"
            RPL_TRACEUSER       = 205, // "User <class> <nick>"
            RPL_TRACESERVER     = 206, // "Serv <class> <int>S <int>C <server>
                                       // <nick!user|*!*>@<host|server> V<protocol version>"
            RPL_TRACESERVICE    = 207, // "Service <class> <name> <type> <active type>"
            RPL_TRACENEWTYPE    = 208, // "<newtype> 0 <client name>"
            RPL_TRACECLASS      = 209, // "Class <class> <count>"
            RPL_TRACERECONNECT  = 210, // Unused.
            RPL_TRACELOG        = 261, // "File <logfile> <debug level>"
            RPL_TRACEEND        = 262, // "<server name> <version & debug level> :End of TRACE"

            /*
             * reports statistics on a connection.  <linkname>
             * identifies the particular connection, <sendq> is
             * the amount of data that is queued and waiting to be
             * sent <sent messages> the number of messages sent,
             * and <sent Kbytes> the amount of data sent, in
             * Kbytes. <received messages> and <received Kbytes>
             * are the equivalent of <sent messages> and <sent
             * Kbytes> for received data, respectively.  <time
             * open> indicates how long ago the connection was
             * opened, in seconds.
             */
            RPL_STATSLINKINFO = 211, // "<linkname> <sendq> <sent messages>
                                     // <sent Kbytes> <received messages>
                                     // <received Kbytes> <time open>"

            /*
             * reports statistics on commands usage.
             */
            RPL_STATSCOMMANDS = 212, // "<command> <count> <byte count> <remote count>"

            RPL_ENDOFSTATS = 219, // "<stats letter> :End of STATS report"

            /*
             * reports the server uptime.
             */
            RPL_STATSUPTIME = 242, // ":Server Up %d days %d:%02d:%02d"

            /*
             * reports the allowed hosts from where user may become IRC
             * operators.
             */
            RPL_STATSOLINE = 243, // "O <hostmask> * <name>"

            /*
             * To answer a query about a client's own mode,
             * RPL_UMODEIS is sent back.
             */
            RPL_UMODEIS = 221, // "<user mode string>"

            /*
             * When listing services in reply to a SERVLIST message,
             * a server is required to send the list back using the
             * RPL_SERVLIST and RPL_SERVLISTEND messages.  A separate
             * RPL_SERVLIST is sent for each service.  After the
             * services have been listed (or if none present) a
             * RPL_SERVLISTEND MUST be sent.
             */
            RPL_SERVLIST    = 234, // "<name> <server> <mask> <type> <hopcount> <info>"
            RPL_SERVLISTEND = 235, // "<mask> <type> :End of service listing"

            /*
             * In processing an LUSERS message, the server
             * sends a set of replies from RPL_LUSERCLIENT,
             * RPL_LUSEROP, RPL_USERUNKNOWN,
             * RPL_LUSERCHANNELS and RPL_LUSERME.  When
             * replying, a server MUST send back
             * RPL_LUSERCLIENT and RPL_LUSERME.  The other
             * replies are only sent back if a non-zero count
             * is found for them.
             */
            RPL_LUSERCLIENT   = 251, // ":There are <integer> users and <integer>
                                     // services on <integer> servers"
            RPL_LUSEROP       = 252, // "<integer> :operator(s) online"
            RPL_LUSERUNKNOWN  = 253, // "<integer> :unknown connection(s)"
            RPL_LUSERCHANNELS = 254, // "<integer> :channels formed"
            RPL_LUSERME       = 255, // ":I have <integer> clients and <integer>
                                     // servers"

            /*
             * When replying to an ADMIN message, a server
             * is expected to use replies RPL_ADMINME
             * through to RPL_ADMINEMAIL and provide a text
             * message with each.  For RPL_ADMINLOC1 a
             * description of what city, state and country
             * the server is in is expected, followed by
             * details of the institution (RPL_ADMINLOC2)
             * and finally the administrative contact for the
             * server (an email address here is REQUIRED)
             * in RPL_ADMINEMAIL.
             */
            RPL_ADMINME    = 256, // "<server> :Administrative info"
            RPL_ADMINLOC1  = 257, // ":<admin info>"
            RPL_ADMINLOC2  = 258, // ":<admin info>"
            RPL_ADMINEMAIL = 259, // ":<admin info>"

            /*
             * When a server drops a command without processing it,
             * it MUST use the reply RPL_TRYAGAIN to inform the
             * originating client.
             */
            RPL_TRYAGAIN = 263, // "<command> :Please wait a while and try again."
        };
    }
}

#endif // NETWORK_IRC_REPLIES

