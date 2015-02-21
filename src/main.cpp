#include <QCoreApplication>
#include "Network.IRC.Server.hpp"

#include <iostream>
#include <QTextStream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Network::IRC::Server srv("irc.rizon.net", 6667);
    srv.connect("Test-App", "test");

    return a.exec();
}
