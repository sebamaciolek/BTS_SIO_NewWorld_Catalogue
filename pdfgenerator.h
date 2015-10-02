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
    void setPointRelais(QString prID, int height);
    void genererRayon(QString prID);

private:
    int pdfHeight;
    QPainter painter;
};

#endif // PDFGENERATOR_H
