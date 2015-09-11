/**
 *@author Pierre Bouffier
 *@version 1.0.0
 *@date 11/09/2015
 *@file main.cpp
 */


#include <QApplication>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <iostream>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <vector>
using namespace std;

/**
 * @brief main Fonction principale d'où le programme découlera
 * @param argc Nombre d'arguments
 * @param argv Valeur des arguments
 * @return return l'execution du QCoreApplication
 */

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QSqlDatabase * baseNW;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    baseNW = new QSqlDatabase(db);
    baseNW->setHostName("172.16.63.111");
    baseNW->setUserName("pbouffier");
    baseNW->setDatabaseName("pbouffier");
    baseNW->setPassword("4KdBi14qsP");
    baseNW->open();

    QSqlQuery query;
    query.exec("SELECT * FROM nw_utilisateur");
    while(query.next())
    {

    }

    cout<<"Bienvenue dans le générateur de catalogue !"<<endl;

    /*
    QPrinter printer(QPrinter::HighResolution); //create your QPrinter (don't need to be high resolution, anyway)
    printer.setFullPage(QPrinter::A4);
    printer.setOutputFormat(QPrinter::NativeFormat);
    printer.setOutputFileName("catalogue_bouffier_pierre_sio.pdf");
    QPainter painter;
    painter.begin(&printer);
    painter.setFont(QFont("Tahoma",8));
    painter.drawText(200,200,"HELOOOO");
    painter.end();
    */

    return 0;
}
