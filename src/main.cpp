#include <QApplication>
#include <QHostAddress>

#include <iostream>

#include "client/tclient.h"
#include "server/tserver.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    FortuneServer server;
    constexpr quint16 serverPort = 4242;
    if (!server.listen(QHostAddress::LocalHost, serverPort)) {
        std::cout << "Server can't run" << std::endl;
    }
    std::cout << "The server is running on\n\nIP: "
              << QHostAddress(QHostAddress::LocalHost).toString().toStdString()
              << "\nport: " << server.serverPort() << "\n\n";


    BlockingClient client;
    client.resize(400, 300); // Set the window size
    client.show();

    return a.exec();
}
