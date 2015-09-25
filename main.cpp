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
#include <QtGui/QTextEdit>
#include <vector>
#include <QDebug>
#include <QTextCodec>
#include <string>
#include <QTextDocument>
#include <pdfgenerator.h>



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
    QTextCodec::setCodecForCStrings(QTextCodec::codecForName("utf8"));

    PDFGenerator pdf;
    pdf.boucleUtilisateur();

    return 0;
}
