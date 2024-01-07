 /******************************************************************************
 * File Name: user.cpp
 *
 * Description: source file for the user Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include "user.h"
#include<QDataStream>
#include<QDebug>

QTextStream qin(stdin);
QTextStream qout(stdout);

User::User()
{
    outStream.setDevice(&socket);
    outStream.setVersion(QDataStream::Qt_6_6);
    inStream.setDevice(&socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    m_role="user";
    m_request="Login";
    m_requestflag="General";
    connect(&socket,&QTcpSocket::connected,this,&User::connected);
    connect(&socket,&QTcpSocket::disconnected,this,&User::disconnected);
    connect(&socket,&QTcpSocket::stateChanged,this,&User::stateChanged);
    connect(&socket,&QTcpSocket::readyRead,this,&User::readyRead);
    connect(&socket,&QTcpSocket::errorOccurred,this,&User::error);
}
void User::GetAccNo(QString& Username)
{
    m_userName = Username;
    outStream << m_userName;
    socket.waitForReadyRead();
    qInfo()<<"Account Number :"<<m_serverrespond.toString();
}

void User::TransferMoney(QString& to_accNo,quint32& amount)
{
    outStream<<to_accNo<<amount;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    if(m_serverrespond.toBool())
    {
        qout << "Money transfered successfully" << Qt::endl;
    }
    else{
        qout << "Money transfer Failed" << Qt::endl;
    }
}
void User::MakeTransaction(qint32& amount)
{
    outStream<<amount;
    socket.waitForReadyRead();
    if(m_serverrespond.toBool())
    {
        qout << "Transaction done successfully" << Qt::endl;
    }
    else{
        qout << "Transaction Failed" << Qt::endl;
    }
}

void User::ViewAccountBalance()
{
    //wait for respond from the server
    socket.waitForReadyRead();
    if(m_serverrespond.toBool())
    {
        qout << "Account balance : " << m_serverrespond.toInt()<<Qt::endl;
    }
    else
    {
        qout << "failed to show account balance" << Qt::endl;
    }
}


void User::ViewTransactionHistory(quint16& count)
{
    outStream<<count;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qInfo().noquote()<<m_serverrespond.toString();
}

//handling requests along with messages and sends them to the server
void User::SendRequest(QString& request)
{
    m_request=request;
    outStream<<m_requestflag;
    outStream<<m_request<<m_role;

    if(m_request=="Transfer Money")
    {
        qout <<"Enter account number to transfer to :" <<Qt::endl;
        QString accto = qin.readLine();

        qout <<"Enter amount :" <<Qt::endl;
        quint32 amountto = qin.readLine().toInt();
        TransferMoney(accto,amountto);
    }
    else if(request=="View Account Balance")
    {
        ViewAccountBalance();
    }
    else if(request=="Get Account Number")
    {
        qout <<"Enter username :" <<Qt::endl;
        QString username = qin.readLine();
        m_userName = username;
        GetAccNo(m_userName);
    }
    else if(request=="Make Transaction")
    {
        qout <<"Enter amount :" <<Qt::endl;
        qint32 amount = qin.readLine().toInt();
        MakeTransaction(amount);
    }
    else if(request=="View Transaction History")
    {
        qout <<"Enter number of transactions:" <<Qt::endl;
        quint16 count = qin.readLine().toInt();
        ViewTransactionHistory(count);
    }
    else
    {
        qout << "Invalid request" << Qt::endl;
    }
}
//User login function
bool User::Login(QString& username,QString& password)
{
    qInfo()<<"WELCOME , logging window!!";
    m_request="Login";
    bool state = false;
    quint8 attempts =0;

    outStream<<m_requestflag;
    outStream<<m_request<<m_role;

    m_UserName = username;
    m_Password = password;

    //checking for wrong login attempts
    while(attempts<3)
    {
        attempts++;
        if(!m_UserName.isEmpty() && !m_Password.isEmpty())
        {
            outStream<<m_UserName<<m_Password;
            socket.waitForBytesWritten();
            socket.waitForReadyRead();
            state = m_serverrespond.toBool();
            break;
        }
    }
    clearScreen();
    return state;
}

//Socket functions
void User::connectToHost(QString host, quint16 port)
{
    if(socket.isOpen()) disconnect();
    socket.connectToHost(host,port);
}
void User::disconnect()
{
    socket.close();
    socket.disconnectFromHost();
    socket.waitForDisconnected();
}
void User::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    socket.write(userInput.toUtf8());
}
void User::disconnected()
{
    qInfo()<<"!!Closing System!!";
    qInfo()<<"Disconnected";
}
void User::error(QAbstractSocket::SocketError socketerror)
{
    qInfo()<<"Error:"<<socketerror<<socket.errorString();
}

void User::stateChanged(QAbstractSocket::SocketState socketstate)
{
    QMetaEnum metaenum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str= metaenum.valueToKey(socketstate);
}

//handling server response to provided requests
void User::readyRead()
{
    if (m_request=="View Account Balance")
    {
        quint32 balance;
        inStream >> balance;
        m_serverrespond.setValue(balance);
    }
    else if(m_request=="Transfer Money"||m_request=="Make Transaction"||m_request=="Login")
    {
        bool respond;

        inStream >> respond;
        m_serverrespond.setValue(respond);
    }
    else if(m_request == "Get Account Number")
    {
        QString AccNum;
        inStream >> AccNum;
        m_serverrespond.setValue(AccNum);
    }
    else if(m_request == "View Transaction History")
    {
        QString history;
        inStream >> history;
        m_serverrespond.setValue(history);
    }
}
