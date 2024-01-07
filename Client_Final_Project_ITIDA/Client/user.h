 /******************************************************************************
 * File Name: user.h
 *
 * Description: Header file for the User Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#ifndef USER_H
#define USER_H

#include "client.h"


class User : public Client
{
public:
    //user methods
    explicit User();
    void TransferMoney(QString& to_accNo,quint32& amount);
    void MakeTransaction(qint32& amount);
    void GetAccNo(QString& username);

    //client methods
    void ViewAccountBalance();
    void ViewTransactionHistory(quint16& count);
    void SendRequest(QString& request) override;
    bool Login(QString& username,QString& password) override;
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
    QString m_userName;
    QString m_accountNumber;

};

#endif // USER_H
