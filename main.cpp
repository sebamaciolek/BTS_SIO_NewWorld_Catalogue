/**
 *@author Pierre Bouffier
 *@version 1.0.0
 *@date 11/09/2015
 *@file main.cpp
 */


#include <QCoreApplication>
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
    
    cout<<"Bienvenue dans le générateur de catalogue !"<<endl;

    return a.exec();
}
