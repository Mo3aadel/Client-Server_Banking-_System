 /******************************************************************************
 * File Name: client.cpp
 *
 * Description: source file for the Client Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include "client.h"
#include <QTextStream>
#include <QDataStream>
#include <QDebug>

Client::Client(QObject *parent)
    : QObject{parent}
{
    m_requestflag="General";
}

void Client::clearScreen()
{
    system("cls");
}
