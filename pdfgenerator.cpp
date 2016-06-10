/**
  * @author Pierre Bouffier
  * @date 30/10/2015
  * @file pdfgenerator.cpp
  * @version 1.0
*/

#include "pdfgenerator.h"
#include <QDebug>

/**
 * @brief PDFGenerator::PDFGenerator
 * Constructeur de la génération du PDF. Il se connectera à la base de donnée.
 */

PDFGenerator::PDFGenerator()
{
    // Connexion à la base de donnée
    QSqlDatabase * baseNW;
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
    baseNW = new QSqlDatabase(db);
    baseNW->setHostName("localhost");
    baseNW->setUserName("sebastianm");
    baseNW->setDatabaseName("catalogue");
    baseNW->setPassword("smaciolek");
    baseNW->open();
}

/**
 * @brief PDFGenerator::boucleUtilisateur
 * Procédure parcourant chaque utilisateur pour déterminer quel sont leurs point de vente.
 */

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
        // Nombre de point relais et mise en place de la "première hauteure"
        numPointRelais = 0;
        pdfHeight = 1800;
        // Indiquation de la création du catalogue
        QString valeur = "Generation du formulaire pour: " + query_utilisateur.value(1).toString() + " (" + query_utilisateur.value(5).toString() + ")";
        cout<<valeur.toStdString()<<endl;

        // Création du catalogue en PDF
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setFullPage(QPrinter::A4);
        printer.setOutputFileName("catalogues/" + query_utilisateur.value(0).toString() + "_catalogue_bouffier_pierre_sio.pdf");
        printerGlobal = &printer;
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

        // S'il y a plus d'un point relais, alors on crée un catalogue pour lui
        if(query_pointrelais_utilisateur.size() > 0)
        {
            // On parcourt les points relais
            while(query_pointrelais_utilisateur.next())
            {
                // Demande de mise en place des points relais
                setPointRelais(query_pointrelais_utilisateur.value(0).toString());
            }
        }
        // On met fin au painter afin de fermer le PDF
        painter.end();
    }
    cout<<endl<<"La génération des catalogues est maintenant terminé !"<<endl<<endl<<endl;
}

/**
 * @brief PDFGenerator::setPointRelais
 * @param prID Identifiant du point relais
 * Récupération des informations du point relais (Nom, date, adresse,...)
 * avec affichage du cadre. Si le point relais est le premier, on ne crée pas de nouvelles pages,
 * sinon, nouvelle page.
 */

void PDFGenerator::setPointRelais(QString prID)
{
    // Si le nombre de point relais est supérieur à un, alors on crée une nouvelle page
    if(numPointRelais > 0)
    {
        printerGlobal->newPage();
        pdfHeight = 0;
    }
    // Requête de récupération des informations du point relais
    QSqlQuery query_pointrelais;
    query_pointrelais.exec("SELECT * FROM nw_pointrelais WHERE prID = '" + prID + "'");
    query_pointrelais.first();

    // Création du cadre d'information du point relais
    painter.drawRect(0, pdfHeight, 5000, 1800);
    painter.setPen(QColor("#046380"));
    painter.setFont(QFont("Tahoma", 10));
    painter.drawText(300, pdfHeight + 200, "Point de vente N°: " + query_pointrelais.value(1).toString());
    painter.drawText(300, pdfHeight + 400, "Nom: " + query_pointrelais.value(2).toString());
    painter.drawText(300, pdfHeight + 600, "Adresse: " + query_pointrelais.value(7).toString());
    painter.drawText(300, pdfHeight + 800, "Email: " + query_pointrelais.value(6).toString());
    painter.drawText(300, pdfHeight + 1200, "M. " + query_pointrelais.value(3).toString() + " " + query_pointrelais.value(4).toString());
    painter.drawText(300, pdfHeight + 1400, query_pointrelais.value(5).toString());
    painter.drawText(300, pdfHeight + 1600, query_pointrelais.value(8).toString() + " " + query_pointrelais.value(9).toString());
    pdfHeight += 2000;
    numPointRelais++;
    // Générer les rayons en fonction du point relais
    genererRayon(prID);
}

/**
 * @brief PDFGenerator::genererRayon
 * @param prID Identifiant du point relais
 * Procédure générant les rayons en rapport avec les points relais qui possédent des lots.
 */

void PDFGenerator::genererRayon(QString prID)
{
    // Requête permettant de lister les lots en fonction du rayon
    QSqlQuery lot_point_relais;
    lot_point_relais.exec("SELECT rayID, rayLib from nw_rayon inner join nw_categorie on rayID = catRayon inner join nw_produit on catID = prodCategorie inner join nw_lot on prodID = lotProduit natural join nw_lotrelais WHERE relaisID = '" + prID + "' GROUP BY rayID;");
    while(lot_point_relais.next())
    {
        ajoutNouvellePage();
        // Affichage du nom du rayon
        painter.setPen(QColor("#046380"));
        painter.setFont(QFont("Tahoma", 17));
        painter.drawText(300, pdfHeight + 200, lot_point_relais.value(1).toString());
        painter.drawLine(100, pdfHeight + 400, 8917, pdfHeight + 400);
        pdfHeight += 600;
        // Générer les catégorie en fonction du rayon et du point relais
        genererCategorie(prID, lot_point_relais.value(0).toString());
    }
}

/**
 * @brief PDFGenerator::genererCategorie
 * @param prID Identifiant du point relais
 * @param rayID Identifiant du rayon
 * Procédure générant les catégories en rapport avec les rayons et points relais qui possédent des lots.
 */

void PDFGenerator::genererCategorie(QString prID, QString rayID)
{
    // Requête de récupération
    QSqlQuery lot_point_relais;
    lot_point_relais.exec("SELECT catID, catLib, catRayon from nw_categorie inner join nw_produit on catID = prodCategorie inner join nw_lot on prodID = lotProduit natural join nw_lotrelais WHERE relaisID = '" + prID + "' AND catRayon = '" + rayID + "' GROUP BY catID;");
    while(lot_point_relais.next())
    {
        ajoutNouvellePage();
        // Affichage du rayon
        painter.setPen(QColor("#047080"));
        QFont font("Tahoma", 11);
        font.setItalic(true);
        painter.setFont(font);
        painter.drawText(300, pdfHeight + 100, "-> " + lot_point_relais.value(1).toString());
        pdfHeight += 400;
        // Générer les lots en fonction du point relais et de la catégorie
        genererLot(prID, lot_point_relais.value(0).toString());
    }
    pdfHeight += 300;
}

/**
 * @brief PDFGenerator::genererLot
 * @param prID Identifiant du point relais
 * @param catID Identifiant de la catégorie
 * Procédure générant les lots en rapport avec les catégories et le point relais qui possédent des lots.
 */

void PDFGenerator::genererLot(QString prID, QString catID)
{
    ajoutNouvellePage();
    // Affichage des colonnes (Nom, prix,...)
    painter.setPen(QColor("#487080"));
    QFont font("Tahoma", 9);
    font.setBold(true);
    font.setItalic(true);
    painter.setFont(font);
    painter.drawText(700, pdfHeight + 200, "Nom produit");
    painter.drawText(2700, pdfHeight + 200, "Prix/Unité");
    painter.drawText(3900, pdfHeight + 200, "Mode de prod.");
    painter.drawText(5400, pdfHeight + 200, "Date / Conservation");
    painter.drawText(7400, pdfHeight + 200, "Lieu de culture");
    font.setBold(false);
    font.setItalic(false);
    painter.setFont(font);
    pdfHeight += 800;

    // Requête affichant les lots enregistré
    QSqlQuery lot_point_relais;
    lot_point_relais.exec("SELECT nw_lot.*, nw_produit.* from nw_lot inner join nw_produit on prodID = lotProduit natural join nw_lotrelais WHERE relaisID = '" + prID + "' AND prodCategorie = '" + catID + "' ORDER BY prodID;");
    while(lot_point_relais.next())
    {
        ajoutNouvellePage();

        // Définition du cadre de l'image ainsi que de son image
        QRectF rectangleImage(-250, pdfHeight - 280, 580, 580);
        QRectF rectangleBorder(rectangleImage.x() - 30, rectangleImage.y() - 30, rectangleImage.width() + 60, rectangleImage.height() + 60);
        painter.drawRoundRect(rectangleBorder, 4, 4);
        QImage image;
        image.load("image/defaut.jpg");
        painter.drawImage(rectangleImage, image);
        // Affichage des détails du lot
        painter.drawText(600, pdfHeight + 100, lot_point_relais.value(12).toString());
        painter.drawText(2800, pdfHeight + 100, lot_point_relais.value(2).toString() + "/" + lot_point_relais.value(3).toString());
        painter.drawText(4000, pdfHeight + 100, lot_point_relais.value(4).toString());
        painter.drawText(5500, pdfHeight + 100, lot_point_relais.value(5).toString() + " / " + lot_point_relais.value(6).toString() + " J.");
        painter.drawText(7500, pdfHeight + 100, lot_point_relais.value(8).toString());
        pdfHeight += 800;
    }
}

/**
 * @brief PDFGenerator::ajoutNouvellePage
 * Procédure créant une nouvelle page si la hauteur de la page dépasse une certaine taille.
 * Celà permet de ne pas perdre d'information et d'avoir un rendu beaucoup plus propre.
 */

void PDFGenerator::ajoutNouvellePage()
{
    if(pdfHeight > 12500)
    {
        printerGlobal->newPage();
        pdfHeight = 200;
    }
}
