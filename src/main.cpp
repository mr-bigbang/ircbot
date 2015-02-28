#include <QCoreApplication>
#include "Network.IRC.Server.hpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Network::IRC::Server srv("irc.rizon.net", 9999, &a);
    srv.connect("testos", "Test-App", true);

    return a.exec();
}
