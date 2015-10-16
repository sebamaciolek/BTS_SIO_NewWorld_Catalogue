#ifndef PDFGENERATOR_H
#define PDFGENERATOR_H
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QtGui/QPrinter>
#include <QtGui/QPainter>
#include <QtGui/QTextEdit>
#include <iostream>

using namespace std;


class PDFGenerator
{
public:
    PDFGenerator();
    void boucleUtilisateur();
    void setPointRelais(QString prID);
    void genererRayon(QString prID);
    void genererCategorie(QString prID);
    void ajoutNouvellePage();

private:
    int pdfHeight;
    int numPointRelais;
    QPainter painter;
    QString pointVenteActuel;
    QPrinter* printerGlobal;
};

#endif // PDFGENERATOR_H
