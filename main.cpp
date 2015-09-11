/**
 *@author Pierre Bouffier
 *@version 1.0.0
 *@date 11/09/2015
 *@file main.cpp
 */


#include <QCoreApplication>
#include <QSqlDatabase>
#include <iostream>
#include <QPrinter>

using namespace std;

/**
 * @brief main Fonction principale d'où le programme découlera
 * @param argc Nombre d'arguments
 * @param argv Valeur des arguments
 * @return return l'execution du QCoreApplication
 */

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    
    QSqlDatabase * baseNW;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    baseNW = new QSqlDatabase(db);
    baseNW->setHostName("172.16.63.111");
    baseNW->setUserName("pbouffier");
    baseNW->setDatabaseName("pbouffier");
    baseNW->setPassword("4KdBi14qsP");
    baseNW->open();


    cout<<"Bienvenue dans le générateur de catalogue !"<<endl;

    return a.exec();
}
