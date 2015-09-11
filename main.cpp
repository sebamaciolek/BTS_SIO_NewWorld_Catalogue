/**
 *@author Pierre Bouffier
 *@version 1.0.0
 *@date 11/09/2015
 *@file main.cpp
 */


#include <QCoreApplication>
#include <QSqlDatabase>
#include <iostream>

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
    baseNW->setHostName("localhost");
    baseNW->setUserName("root");
    baseNW->setDatabaseName("qt");
    baseNW->setPassword("passf005");
    baseNW->open();

    cout<<"Bienvenue dans le générateur de catalogue !"<<endl;

    return a.exec();
}
