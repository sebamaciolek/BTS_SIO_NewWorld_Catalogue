#include "pdfgenerator.h"
#include <QDebug>

PDFGenerator::PDFGenerator()
{
    // Connexion à la base de donnée
    QSqlDatabase * baseNW;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    baseNW = new QSqlDatabase(db);
    baseNW->setHostName("172.16.63.111");
    baseNW->setUserName("pbouffier");
    baseNW->setDatabaseName("pbouffier");
    baseNW->setPassword("4KdBi14qsP");
    baseNW->open();


}

void PDFGenerator::boucleUtilisateur()
{
    // Affichage du message de bienvenue
    cout<<"############################################"<<endl;
    cout<<"Bienvenue dans le générateur de catalogue !"<<endl;
    cout<<"############################################"<<endl<<endl;

    // Première requête pour lister les utilisateurs
    QSqlQuery query_utilisateur;
    query_utilisateur.exec("SELECT * FROM nw_utilisateur natural join nw_utilisateurpointvente group by userID");
    while(query_utilisateur.next())
    {

        pdfHeight = 1800;

        // Indiquation de la création du catalogue
        QString valeur = "Generation du formulaire pour: " + query_utilisateur.value(1).toString() + " (" + query_utilisateur.value(5).toString() + ")";
        cout<<valeur.toStdString()<<endl;

        // Création du catalogue en PDF
        QPrinter printer(QPrinter::HighResolution); //create your QPrinter (don't need to be high resolution, anyway)
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setFullPage(QPrinter::A4);
        printer.setOutputFileName("catalogues/" + query_utilisateur.value(0).toString() + "_catalogue_bouffier_pierre_sio.pdf");
            qDebug()<<printer.width();
        // Déclaration du contenue du PDF
        painter.begin(&printer);
        painter.setPen(QColor("#002F2F"));
        painter.setFont(QFont("Tahoma",65));
        painter.drawText(0, 700, "NewWorld");
        painter.setFont(QFont("Tahoma",12));
        painter.drawText(100, 950, "Bonjour " + query_utilisateur.value(4).toString() + " ! Voici votre catalogue...");

        // Dessin du cadre
        QRectF rectangle(5400, -100, 4000, 4000);
        QImage image;
        image.load("image/arbre.png");
        painter.drawImage(rectangle, image);

        // On vérifie leurs nombres de points relais
        QSqlQuery query_pointrelais_utilisateur;
        query_pointrelais_utilisateur.exec("SELECT prID FROM nw_utilisateurpointvente inner join nw_lotrelais on prID = relaisID WHERE userID = '" + query_utilisateur.value(0).toString() + "' GROUP BY prID");

        cout<<query_pointrelais_utilisateur.size()<<endl;

        // S'il y a plus d'un point relais, alors on crée un catalogue pour lui
        if(query_pointrelais_utilisateur.size() > 0)
        {
            // On parcourt les points relais
            while(query_pointrelais_utilisateur.next())
            {
                setPointRelais(query_pointrelais_utilisateur.value(0).toString());
            }
        }

        painter.end();

    }
    cout<<endl<<"La génération des catalogues est maintenant terminé !"<<endl<<endl<<endl;
}

void PDFGenerator::setPointRelais(QString prID)
{
    int height = pdfHeight;
    QSqlQuery query_pointrelais;
    query_pointrelais.exec("SELECT * FROM nw_pointrelais WHERE prID = '" + prID + "'");
    query_pointrelais.first();

    painter.drawRect(0, height, 5000, 1800);
    painter.setPen(QColor("#046380"));
    painter.setFont(QFont("Tahoma", 10));
    painter.drawText(300, height + 200, "Point de vente N°: " + query_pointrelais.value(1).toString());
    painter.drawText(300, height + 400, "Nom: " + query_pointrelais.value(2).toString());
    painter.drawText(300, height + 600, "Adresse: " + query_pointrelais.value(7).toString());
    painter.drawText(300, height + 800, "Email: " + query_pointrelais.value(6).toString());
    painter.drawText(300, height + 1200, "M. " + query_pointrelais.value(3).toString() + " " + query_pointrelais.value(4).toString());
    painter.drawText(300, height + 1400, query_pointrelais.value(5).toString());
    painter.drawText(300, height + 1600, query_pointrelais.value(8).toString() + " " + query_pointrelais.value(9).toString());
    pdfHeight += 2000;
    genererRayon(prID);
}

void PDFGenerator::genererRayon(QString prID)
{
    pdfHeight += 200;
    QSqlQuery lot_point_relais;
    lot_point_relais.exec("SELECT rayID, rayLib from nw_rayon inner join nw_categorie on rayID = catRayon inner join nw_produit on catID = prodCategorie inner join nw_lot on prodID = lotProduit natural join nw_lotrelais WHERE relaisID = '" + prID + "' GROUP BY rayID;");
    while(lot_point_relais.next())
    {

        painter.setFont(QFont("Tahoma", 17));
        painter.drawText(500, pdfHeight + 200, lot_point_relais.value(1).toString());
        painter.drawLine(300, pdfHeight + 400, 8517, pdfHeight + 400);
        pdfHeight += 600;
    }
}
