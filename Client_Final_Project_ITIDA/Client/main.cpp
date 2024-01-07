 /******************************************************************************
 * File Name: main.cpp
 *
 * Description: source file for the main
 *
 * Author: Mohamed Adel
 *******************************************************************************/
#include <QCoreApplication>
#include <QDebug>
#include "user.h"
#include "admin.h"

QString request{};


int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);

    QTextStream qin(stdin);
    QTextStream qout(stdout);
    bool loggedin = false;

    while(1)
    {
        qout << "\t\t\tWelcome to client-Server Application\nchoose"
                " from the above options :\n\t\tuser\t\tadmin\t\texit" <<Qt::endl;
        qout << "--------------------------------------------------"
                "------------------------------" <<Qt::endl;

        QString role = qin.readLine();

        if(role == "admin")
        {
            Admin admin;
            admin.connectToHost("127.0.0.1", 1234);
            //logging in admin
            while(!loggedin)
            {
                qout << "enter Admin login details : " <<Qt::endl;
                qout << "username : " <<Qt::endl;
                QString username = qin.readLine();
                qout << "password : " <<Qt::endl;
                QString password = qin.readLine();

                loggedin = admin.Login(username,password);
                if(!loggedin)
                {
                    qout << "Username or password is wrong" << Qt::endl;
                }
            }
            //can't make any process if admin is logged out
            while(loggedin)
            {
                qout << "Choose from the list:\n1-View Account Balance\n2-View Transaction History\n"
                        "3-Get Account Number\n4-Create New User\n"
                        "5-Update User\n6-Delete User\n7-View Bank DataBase\n8-exit" << Qt::endl;

                quint32 input = qin.readLine().toInt();
                QString choice{};
                admin.clearScreen();

                switch(input)
                {
                case 1:
                    request="View Account Balance";
                    admin.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 2:
                    request="View Transaction History";
                    admin.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 3:
                    request="Get Account Number";
                    admin.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 4:
                    request="Create New User";
                    admin.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 5:
                    request="Update User";
                    admin.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 6:
                    request="Delete User";
                    admin.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 7:
                    request="View Bank DataBase";
                    admin.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 8:
                    loggedin = false;
                    break;
                default:
                    qout << "Invalid choice" << Qt::endl;
                    break;
                }
                admin.clearScreen();
            }
        }
        else if(role=="user")
        {
            User o_user;
            o_user.connectToHost("127.0.0.1", 1234);

            while(!loggedin)
            {
                qout << "enter user login details : " <<Qt::endl;
                qout << "username : " <<Qt::endl;
                QString username = qin.readLine();
                qout << "password : " <<Qt::endl;
                QString password = qin.readLine();

                loggedin = o_user.Login(username,password);
                if(!loggedin)
                {
                    qout << "Username or password is wrong" << Qt::endl;
                }
            }
            while(loggedin)
            {
                qout << "\nChoose from the list above :\n1-View Account Balance\n"
                        "2-View Transaction History\n3-Get Account Number\n"
                        "4-Transfer Money\n5-Make Transaction\n6-exit" << Qt::endl;

                quint32 input = qin.readLine().toInt();
                QString choice{};
                o_user.clearScreen();

                switch(input)
                {
                case 1:
                    request="View Account Balance";
                    o_user.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 2:
                    request="View Transaction History";
                    o_user.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 3:
                    request="Get Account Number";
                    o_user.SendRequest(request);
                    qout << "\nif you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 4:
                    request="Transfer Money";
                    o_user.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 5:
                    request="Make Transaction";
                    o_user.SendRequest(request);
                    qout << "if you want to proceed click yes or no to quit" <<Qt::endl;
                    choice = qin.readLine();
                    if(choice == "yes")
                    {
                        break;
                    }
                    else
                    {
                        loggedin = false;
                        break;
                    }
                case 6:
                    loggedin = false;
                    break;
                default:
                    qout << "Invalid choice" << Qt::endl;
                    break;
                }
                o_user.clearScreen();
            }
        }
        else if(role == "exit")
        {
            qout << "exiting the system" << Qt::endl;
            exit(0);
        }
        else
        {
            qout << "Invalid Input" << Qt::endl;
        }
    }
    return a.exec();
}
