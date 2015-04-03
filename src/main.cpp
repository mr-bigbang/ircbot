#include <QtCore/QCoreApplication>

#include "Network.IRC.Server.hpp"
#include "myserver.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    //Network::IRC::Server *srv = new Network::IRC::Server("irc.rizon.net", 6667, &a);
    MyServer *srv = new MyServer("irc.rizon.net", 9999, &a);
    // Join channel when connected to server
    // Now with 100% more lambda voodoo
    QObject::connect(srv, &MyServer::connected, [&](){ srv->join("mr-bigbang"); });
    QObject::connect(srv, &MyServer::quitting, &a, &QCoreApplication::quit);

    srv->connect("testos", "Test-App", true);

    return a.exec();
}
