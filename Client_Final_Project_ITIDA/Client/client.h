#ifndef CLIENT_H
#define CLIENT_H

/******************************************************************************
 * File Name: client.h
 *
 * Description: Header file for the Client Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include <QObject>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMetaEnum>
#include <QString>
#include <QDebug>
#include <QTextStream>
#include <QDataStream>
#include <windows.h>


class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);
    //virtual void ViewAccountBalance()=0;
    //virtual void ViewTransactionHistory(/*QString& AccNo,*/quint16& count)=0;
    virtual void SendRequest(QString& request)=0;
    virtual bool Login(QString& username,QString& password)=0;
    void clearScreen();

protected:
    QString m_role;
    QString m_request;
    QVariant m_serverrespond;
    QString m_requestflag;
    QString m_AccountNo;
    QString m_UserName;
    quint16 m_age;
    QString m_FullName;
    QString m_Password;
    quint32 m_balance;
};

#endif // CLIENT_H
