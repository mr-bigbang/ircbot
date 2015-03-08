#include <QtCore/QCoreApplication>

#include "Network.IRC.Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Network::IRC::Server *srv = new Network::IRC::Server("irc.rizon.net", 9999);
    // Join channel when connected to server
    // Now with 100% more lambda voodoo
    QObject::connect(srv, &Network::IRC::Server::connected, [&](){ srv->join("mr-bigbang"); });
    QObject::connect(srv, &Network::IRC::Server::quitting, &a, &QCoreApplication::quit);

    srv->connect("testos", "Test-App", true);

    return a.exec();
}
