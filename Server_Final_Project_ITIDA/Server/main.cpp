 /******************************************************************************
 * File Name: main.cpp
 *
 * Description: source file for the main(server)
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include <QCoreApplication>
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server server;
    server.start();
    return a.exec();
}

