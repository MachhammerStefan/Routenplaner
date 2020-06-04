#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <Routenplaner.h>


int main()
{

    char answer;
    int select;  //Auswahl Variable für das Menü
    int endProg = 0;        // Das Program wird solange Wiederholt bis diese Variable auf 1 gesetzt wird
    struct Citys* head = NULL;  //Erste Struct/Stadt der Liste => aus CSV-Datei
    struct Citys* headNewList = NULL;       // head einer Neu erstellten Liste (Zum Suchen, Auswählen und Berechnen)
    struct Bucket* headBucketSort = NULL;
    //head = addBack(head, "test", "test2", 22.2, 11.1, "test3", "test4");
    head = openFile(head);  //öffnen und einlesen der  CSV-Datei
    printf("Ihre Daten wurden geladen.\n");
    while(endProg != 1)         //Menü
    {
        printf("Bitte waehlen Sie eine Zahl fuer Ihre Aktion aus:\n");
        printf(" + Programm beenden................0\n");
        printf(" + Ausgeben der Daten..............1\n");
        printf(" + Suchen nach einer Stadt.........2\n");
        printf(" + Sortieren nach Namen............3\n");
        printf(" + Sortieren nach Einwohnerzahl....4\n");
        printf(" + Hinzufuegen einer neuen Stadt...5\n");
        printf(" + Kuerzeste Sterecke berechnen....6\n");
        printf(" + Bucket Sort.....................7\n");
        printf(" + Suchen in Bucket .............  8\n");
        printf("\n");
        scanf("%d", &select); //Eingabe auf der Konsole zur Auswahl
        getchar(); //Damit fegts funktionert, ansonsten wird ein leere String eingelesen

        switch(select)      //Auswahl der zuverfügung stehendne Programme
        {
        case 0:             // Das Program wird beendet
            printf("Sollen die Staedte als neue .csv Datei gespeichert werden? (y/n) ");
            scanf("%c",&answer);
            if(answer == 'y') //user Abfrage ob neue csv Datei erstellt werden soll
            {
                createcsv(head); // head zum erstelen einer neuen csv Datei übergeben
                answer = 'x';
            }
            printf("Programm wird geschlossen. ShutDown!\n");

            endProg = 1;     // setzten der Variable, zum Unterbrechen der while-Schleife
            break;
        case 1:
            print(head);     //Asugabe aller in der Liste vorhanden Daten/Städte
            break;
        case 2:
            printf("Bitte geben Sie den Namen der Stadt ein: ");
            printOneCity(searchCity(head,inStr())); //Asugabe einer Stadt + Suchen eine Stadt + Namen einlesen aus der Konsole
            break;
        case 3:
            sortbyname(head, headNewList);
            break;
        case 4:
            sortbypopulation(head, headNewList);
            break;
        case 5:
            head = addCityConsol(head); //Einfügen einer neuen Stadt, am anfang der Liste
            break;
        case 6:
            headNewList = newList(head, headNewList);  //Neue Liste mit ausgewählten Städten wird erstellt
            //print(headNewList);                         //Ausgabe aller in der Liste vorhanden Daten/Städte
            headNewList = nearestNeigborsAlgorithm(headNewList);    //gibt eine List zurück mit den Eingegebenen Städten und dem AusgangsPunkt, hier Vienna
            printReverse(headNewList);  //Die zurückgegebne List ist nach dem LAST-IN-FIRST-OUT Prinzip, deswegen muss die Liste umgedreht werden
            printf("Soll die Route als .txt gespeichert werden? (y/n) ");
            scanf("%c",&answer);
            if(answer == 'y') // abfarge ob berechnete Route als .txt File gepeichert werden soll
            {
                createtxt(headNewList); // head der Liste zum erstellen einer .txt Datei übergeben
                answer = 'x';
            }

            free(headNewList); //Die neu erstellte Liste wird nach deren Verwendung wieder freigegeben
            headNewList = NULL;
            break;
        case 7:
            headBucketSort = BucketSort(head);
            break;
        case 8:
            if(headBucketSort == NULL)
            {
                printf("Bitte fuehren Sie zuerst den Bucket Sort aus! - Nr 7\n");
            }
            else{
            printf("Bitte geben Sie den Namen der Stadt ein: ");
            printOneCity(searchInBucket(headBucketSort, inStr()));
            }
            break;
        default:
            printf("Ein Fehler ist aufgetreten, bitte starten Sie das Programm neu!\n");
            break;
        }

    }
    free(headBucketSort);
    free(head); // head freigeben
    return 0;
}
