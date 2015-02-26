#include <QCoreApplication>
#include "Network.IRC.Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Network::IRC::Server srv("irc.rizon.net", 6667, &a);
    srv.connect("testos", "Test-App");

    return a.exec();
}
