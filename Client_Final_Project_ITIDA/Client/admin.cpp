 /******************************************************************************
 * File Name: admin.cpp
 *
 * Description: source file for the Admin Class
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include "admin.h"
#include <QVariant>
#include <QVariantMap>
#include <QMap>

QTextStream s_out(stdout);
QTextStream s_in(stdin);

Admin::Admin()
{
    outStream.setDevice(&socket);
    outStream.setVersion(QDataStream::Qt_6_6);
    inStream.setDevice(&socket);
    inStream.setVersion(QDataStream::Qt_6_6);
    m_role="admin";
    connect(&socket,&QTcpSocket::connected,this,&Admin::connected);
    connect(&socket,&QTcpSocket::disconnected,this,&Admin::disconnected);
    connect(&socket,&QTcpSocket::stateChanged,this,&Admin::stateChanged);
    connect(&socket,&QTcpSocket::readyRead,this,&Admin::readyRead);
    connect(&socket,&QTcpSocket::errorOccurred,this,&Admin::error);
}
//void clearScreen()
//{
//    system("cls");
//}

//Admin Class methods
void Admin::AGetAccNo(QString& User_Name)
{
    m_UserName = User_Name;
    outStream << m_UserName;

    socket.waitForBytesWritten();
    //waiting server respond
    socket.waitForReadyRead();
    s_out << "Account number : " << m_serverrespond.toInt()<<Qt::endl;
}
void Admin::ViewBankDB()
{
    socket.waitForReadyRead();
    s_out << "Bank Database : " << m_serverrespond.toString()<<Qt::endl;
}
void Admin::CreateNewUser(QString& username,QString& password,QString& fullname,quint32& balance,quint16& age)
{
    QVariantMap map;
    quint16 attempts=0;
    bool state =false;
    QString flag = "check";
    m_requestflag = "CreateUser" ;

    //checking for user name
    do
    {
        if(attempts>0)
        {
            s_out << "User name already exists!!\nEnter a different user name : " << Qt::endl;
            QString User_name;
            s_in >> User_name;
            m_UserName = User_name;

            outStream<<m_requestflag;
            outStream<<flag;
            outStream << m_UserName;
        }
        else
        {
            s_out << "Username : " << Qt::endl;
            m_UserName = username;
            outStream << flag;
            outStream << m_UserName;
        }

        attempts++;
        socket.waitForReadyRead();
        state = m_serverrespond.toBool();
    }while(!state);
    flag="update";

    m_FullName = fullname;
    m_age = age;
    m_balance = balance;
    m_Password = password;

    map["Full Name"] = m_FullName;
    map["Age"] = m_age;
    map["Balance"] = m_balance;
    map["Username"] = m_UserName;

    outStream<<m_requestflag;
    outStream<<flag;
    outStream << m_UserName << m_Password << map;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();

    m_requestflag="General";
    if(m_serverrespond.toBool() == true)
    {
        s_out << "User created Successfully" << Qt::endl;
    }
    else
    {
        s_out << "User is not created!!" << Qt::endl;
    }

}
void Admin::DeleteUser(QString& AccNumber)
{
    m_AccountNo = AccNumber;
    outStream << m_AccountNo;
    socket.waitForBytesWritten();
    //waiting for respond from the server
    socket.waitForReadyRead();
    if(m_serverrespond.toBool())
    {
        s_out << "User deleted Successfully" << Qt::endl;
    }
    else
    {
        s_out << "User is not deleted!!" << Qt::endl;
    }

}

void Admin::UpdateUser(QString& AccNumber,QString& Fullname,QString& password,quint16& age,quint32& balance)
{
    QString flag="check";
    QVariantMap map;
    quint16 input;
    QString key;
    bool loop = true;
    quint16 attempts =0;
    bool state = false;
    m_requestflag="UpdateUser";

    do
    {
        if(attempts>0)
        {
            s_out <<"Account Number not exists!!\nplease Enter a valid one:" << Qt::endl ;
            m_AccountNo = AccNumber;
            outStream<<m_requestflag;
            outStream<<flag;
            outStream<<m_AccountNo;
        }
        else
        {
            s_out << "Account Number:" << Qt::endl;
            m_AccountNo = AccNumber;
            outStream<<flag;
            outStream<<m_AccountNo;
        }
        attempts++;
        socket.waitForReadyRead();
        state =m_serverrespond.toBool();
    }while(!state);

    flag="update";

    do
    {
        s_out <<"Choose the field you want to update:" << Qt::endl;
        s_out <<"1-Full Name\n2-Age\n3-Balance\n4-Password" << Qt::endl;
        input = s_in.readLine().toInt();

        switch (input)
        {
        case 1:
            m_FullName = Fullname;
            map["Fullname"] = m_FullName;
            qInfo() << m_FullName;
            break;
        case 2:
            m_age = age;
            map["Age"] = m_age;
            break;
        case 3:
            m_balance = balance;
            map["Balance"] = m_balance;
            break;
        case 4:
            m_Password = password;
            map["password"] = m_Password;
            break;
        default:
            qInfo()<<"Invalid choice";
            break;
        }

        s_out << "----------------------------------------------------" << Qt::endl;
        s_out << "to update another field press yes , to exit press no" << Qt::endl;
        key = s_in.readLine();

        if(key != "yes")
        {
            loop =false;
        }
    }
    while(loop);

    outStream << m_requestflag;
    outStream << flag;
    outStream << m_AccountNo << map;

    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    m_requestflag="General";

    if(m_serverrespond.toBool()==true)
    {
        s_out << "User Data updated Successfully" << Qt::endl;
    }
    else
    {
        s_out << "Update failed!!" << Qt::endl;
    }

}

//Client class methods
void Admin::ViewAccountBalance(QString& AccountNo)//done
{
    m_AccountNo = AccountNo;
    outStream << m_AccountNo;

    socket.waitForBytesWritten();
    socket.waitForReadyRead();

    s_out <<" Account balance is : " << m_serverrespond.toInt() << Qt::endl;
}

void Admin::ViewTransactionhistory(QString& AccNo,quint16& count)
{
    m_AccountNo = AccNo;

    outStream << m_AccountNo << count;
    socket.waitForBytesWritten();
    socket.waitForReadyRead();
    qInfo().noquote()<<m_serverrespond.toString();

}

//handling requests along with messages and sends them to the server
void Admin::SendRequest(QString& request)
{
    m_request = request;
    outStream<<m_requestflag;
    outStream<<m_request<<m_role;

    if(m_request=="View Account Balance")
    {
        s_out <<"Enter account number :" <<Qt::endl;
        QString f_accNo = s_in.readLine();
        m_AccountNo = f_accNo;

        ViewAccountBalance(m_AccountNo);
    }
    else if(request=="Get Account Number")
    {
        s_out <<"Enter username :" <<Qt::endl;
        QString username = s_in.readLine();
        m_UserName = username;
        AGetAccNo(m_UserName);
    }
    else if(request=="Create New User")
    {
        s_out <<"Enter user name :" <<Qt::endl;
        QString userName = s_in.readLine();
        m_UserName = userName;
        s_out <<"Enter Password :" <<Qt::endl;
        QString pass = s_in.readLine();
        m_Password = pass;
        s_out <<"Enter Full name :" <<Qt::endl;
        QString FullName = s_in.readLine();
        m_FullName = FullName;
        s_out <<"Enter balance :" <<Qt::endl;
        quint32 balance = s_in.readLine().toInt();
        m_balance = balance;
        s_out <<"Enter age :" <<Qt::endl;
        quint16 age = s_in.readLine().toInt();
        m_age = age;

        CreateNewUser(m_UserName,m_Password,m_FullName,m_balance,m_age);
    }
    else if(request=="View Transaction History")
    {
        s_out <<"Enter account number :" <<Qt::endl;
        QString Acc_No = s_in.readLine();
        s_out <<"Enter number of transactions:" <<Qt::endl;
        quint16 count = s_in.readLine().toInt();
        m_AccountNo = Acc_No;
        ViewTransactionhistory(m_AccountNo, count);
    }
    else if (request=="Update User")
    {
        s_out <<"Enter Account Number :" <<Qt::endl;
        QString AccNo = s_in.readLine();
        m_AccountNo = AccNo;
        s_out <<"Enter Password :" <<Qt::endl;
        QString pass = s_in.readLine();
        m_Password = pass;
        s_out <<"Enter Full name :" <<Qt::endl;
        QString FullName = s_in.readLine();
        m_FullName = FullName;
        s_out <<"Enter balance :" <<Qt::endl;
        quint32 balance = s_in.readLine().toInt();
        m_balance = balance;
        s_out <<"Enter age :" <<Qt::endl;
        quint16 age = s_in.readLine().toInt();
        m_age = age;
        UpdateUser(m_AccountNo,m_FullName,m_Password,m_age,m_balance);
    }
    else if (request=="Delete User")
    {
        s_out <<"Enter Account Number :" <<Qt::endl;
        QString AccNo = s_in.readLine();
        DeleteUser(AccNo);
    }
    else if (request=="View Bank DataBase")
    {
        ViewBankDB();
    }
    else
    {
        s_out << "Invalid request" << Qt::endl;
    }
}

//admin login function
bool Admin::Login(QString& username,QString& password)
{
    qInfo()<<"\t\t\tWELCOME\n\t\t\t!!Admin logging window!!";
    m_request="Login";

    bool state = false;
    quint8 attempts =0;

    outStream << m_requestflag;
    outStream << m_request << m_role;

    m_Password = password;

    //checking for wrong login attempts
    while(attempts<3)
    {
        attempts++;
        if(!username.isEmpty() && !m_Password.isEmpty())
        {
            outStream<<username<<m_Password;
            socket.waitForBytesWritten();
            socket.waitForReadyRead();

            state = m_serverrespond.toBool();
            m_adminname = username;
            break;
        }
    }
    clearScreen();
    return state;
}

//Socket functions
void Admin::connectToHost(QString host, quint16 port)
{
    if(socket.isOpen()) disconnect();
    socket.connectToHost(host,port);
}

void Admin::disconnect()
{
    socket.close();
    socket.waitForDisconnected();
}

void Admin::connected()
{
    QTextStream input(stdin, QIODevice::ReadOnly);
    QString userInput = input.readLine().trimmed();
    socket.write(userInput.toUtf8());
}

void Admin::disconnected()
{
    qInfo()<<"Disconnected";
}

void Admin::error(QAbstractSocket::SocketError socketerror)
{
    qInfo()<<"Error:"<<socketerror<<socket.errorString();
}

void Admin::stateChanged(QAbstractSocket::SocketState socketstate)
{
    QMetaEnum metaenum = QMetaEnum::fromType<QAbstractSocket::SocketState>();
    QString str= metaenum.valueToKey(socketstate);
}

//handling server response to provided requests
void Admin::readyRead()
{
    if(m_request == "View Bank DataBase")
    {
        QString Database;
        inStream >> Database;
        m_serverrespond.setValue(Database);
    }
    else if (m_request=="View Account Balance")
    {
        quint32 balance;
        inStream >> balance;
        m_serverrespond.setValue(balance);
    }
    else if(m_request=="Create User"||(m_request=="Delete User")||(m_request=="Update User")||(m_request=="Login"))
    {
        bool response;
        inStream >> response;
        m_serverrespond.setValue(response);
    }
    else if(m_request == "Get Account Number")
    {
        QString AccNum ;
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
