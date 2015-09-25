#include "pdfgenerator.h"

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

    int pdfHeight = 0;

    // Première requête pour lister les utilisateurs
    QSqlQuery query_utilisateur;
    query_utilisateur.exec("SELECT * FROM nw_utilisateur");
    while(query_utilisateur.next())
    {
       // On vérifie leurs nombres de points relais
       QSqlQuery query_pointrelais_utilisateur;
       query_pointrelais_utilisateur.exec("SELECT prID FROM nw_utilisateurpointvente WHERE userID = '" + query_utilisateur.value(0).toString() + "'");

       while(query_pointrelais_utilisateur.next())
       {
           // S'il y a plus d'un point relais, alors on crée un catalogue pour lui
           if(query_pointrelais_utilisateur.size() != 0)
           {
               // Indiquation de la création du catalogue
               QString valeur = "Generation du formulaire pour: " + query_utilisateur.value(1).toString() + " (" + query_utilisateur.value(5).toString() + ")";
               cout<<valeur.toStdString()<<endl;

               // Création du catalogue en PDF
               QPrinter printer(QPrinter::HighResolution); //create your QPrinter (don't need to be high resolution, anyway)
               printer.setOutputFormat(QPrinter::PdfFormat);
               printer.setFullPage(QPrinter::A4);
               printer.setOutputFileName("catalogues/" + query_utilisateur.value(0).toString() + "_catalogue_bouffier_pierre_sio.pdf");

               // Déclaration du contenue du PDF
               QPainter painter;
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


               QSqlQuery query_pointrelais;
               query_pointrelais.exec("SELECT * FROM nw_pointrelais WHERE prID = '" + query_pointrelais_utilisateur.value(0).toString() + "'");
               query_pointrelais.first();


               painter.drawRect(0, 1800, 5000, 2000);
               painter.setPen(QColor("#046380"));
               painter.setFont(QFont("Tahoma", 10));
               painter.drawText(300, 2200, "Point de vente N°: " + query_pointrelais.value(1).toString());
               painter.drawText(300, 2400, "Nom: " + query_pointrelais.value(2).toString());
               painter.drawText(300, 2600, "Adresse: " + query_pointrelais.value(7).toString());
               painter.drawText(300, 2800, "Email: " + query_pointrelais.value(6).toString());
               painter.drawText(300, 3200, "M. " + query_pointrelais.value(3).toString() + " " + query_pointrelais.value(4).toString());
               painter.drawText(300, 3400, query_pointrelais.value(5).toString());
               painter.drawText(300, 3600, query_pointrelais.value(8).toString() + " " + query_pointrelais.value(9).toString());

               painter.drawText(300, 23600, query_pointrelais.value(8).toString() + " " + query_pointrelais.value(9).toString());

               painter.end();
           }
        }
    }
    cout<<endl<<"La génération des catalogues est maintenant terminé !"<<endl<<endl<<endl;
}
