 /******************************************************************************
 * File Name: admin.h
 *
 * Description: Header file for the Admin Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#ifndef ADMIN_H
#define ADMIN_H

#include "client.h"


class Admin : public Client
{
public:
    explicit Admin();

    //Admin class methods
    void AGetAccNo(QString& User_Name);
    void ViewBankDB();
    void CreateNewUser(QString& username,QString& password,QString& fullname,quint32& balance,quint16& age);
    void DeleteUser(QString& AccNumber);
    void UpdateUser(QString& AccNumber,QString& Fullname,QString& password,quint16& age,quint32& balance);
    void ViewAccountBalance(QString& AccountNo);
    void ViewTransactionhistory(QString& AccNo,quint16& count);

    //Client class methods
    void SendRequest(QString& request)override;
    bool Login(QString& username,QString& password)override;

signals:
public slots:
    void connectToHost(QString host,quint16 port);
    void disconnect();
protected slots:
    void connected();
    void disconnected();
    void error(QAbstractSocket::SocketError socketerror);
    void stateChanged(QAbstractSocket::SocketState socketstate);
    void readyRead();
private:
    QTcpSocket socket;
    QDataStream outStream;
    QDataStream inStream;
    QString m_adminname;
};

#endif // ADMIN_H
