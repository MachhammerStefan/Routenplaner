#ifndef ROUTENPLANER_H_INCLUDED
#define ROUTENPLANER_H_INCLUDED

typedef struct Citys
{
    char* city;             //0
    char* city_ascii;       //1
    double lat;             //2
    double lng;             //3
    char* country;          //4
    //char iso2[80];        //5
    //char iso3[80];        //6
    //char admin_name[80];  //7
    char* capital;          //8
    double popolation;      //9
    //int id;               //10
    struct Citys* next;     //11
    struct Citys* before;   //12
} Citys;

typedef struct Bucket
{
    char letter;
    struct Bucket* next;
    struct Bucket* before;
    struct Citys* head;
} Bucket;

void print(struct Citys* head);
double distance(double lon1, double lon2, double lat1, double lat2);
double betterDistance(double lon1, double lon2, double lat1, double lat2);
double exactDistance(double lon1, double lon2, double lat1, double lat2);
void printOneCity(struct Citys* p);
double changeStringtoDouble(char *ptr, char *ascii);
char* myStrDup(const char* str);
struct Citys* addFront(struct Citys* head, char *city, char* city_ascii, double lat, double lng,
                       char* country, double population, char* capital);
struct Citys* addBack(struct Citys* head, char *city, char* city_ascii, double lat, double lng,
                      char* country,double population, char* capital);
struct Citys* openFile(struct Citys* head);
struct Citys* searchCity(struct Citys* head, char cityName[100]);
int existCity(struct Citys* head, char cityName[100]);
int existCity(struct Citys* head, char cityName[100]);
char* inStr();
struct Citys* newList(struct Citys* head, struct Citys* headNewList);
struct Citys* addCityConsol(struct Citys* head);
void printReverse(struct Citys* head);
struct Citys* freeOne(struct Citys* head, struct Citys* target);
struct Citys* nearestNeigborsAlgorithm(struct Citys* head);
void printallsorted(struct Citys * data);
void printsortedbyname(struct Citys * data);
void sortbyname(struct Citys *data, struct Citys *newdata);
void sortbypopulation(struct Citys *data, struct Citys *newdata);
void createtxt(struct Citys* head);
void createcsv(struct Citys* head);
struct Bucket* addBackBucket(struct Bucket* head, char letter);
struct Bucket* BucketSort(struct Citys* head);
struct Citys* searchInBucket(struct Bucket* head, char cityName[100]);

void print(struct Citys* head)  //Übergabe der head = Anfang der Liste
{
    struct Citys *p;  //Zusätzliches Struct für die Ausgabe
    if(head ==NULL)   //Wenn nur eine leere Liste vorhanden ist
    {
        printf("list empty\n");
        return;
    }

    for(p = head; p != NULL; p = p->next)  //Abarbeitung der einzelnen structs von head bis zum Ende => p==NULL
    {
        printf("city: %s\nlat: %f\nlng: %f\ncountry: %s\ncapital: %s\npopulation: %.2f\n", p->city_ascii, p->lat, p->lng, p->country, p->capital,p->popolation);
        printf("\n");
    }
    printf("\n");
}

double distance(double lon1, double lon2, double lat1, double lat2) //Berechnung der Distanz (Luftlinie) über den Pytagoras
{
    //Einfachste Berechnung => leider etwas ungenau!!!!!
    //latitude => Breitengrad in Grad
    //longitude => Längengrad in Grad
    double dx;
    double dy;
    double distance;

    dx = 71.5 * (lon1 - lon2);  //Distanz zwischen den Längengraden mit Umrechnung in Kilometer
    dy = 111.3 * (lat1 - lat2); //Distanz zwischen den Breitengraden mit Umrechnung in Kilometer
    distance = sqrt(dx * dx + dy * dy); //Anwendung Satz dens Pydagoras => Errechnung der Hypothenuse

    return distance;

}

double betterDistance(double lon1, double lon2, double lat1, double lat2) //Berechnung der Distanz (Luftlinie) über den Pytagoras und cos()
{
    //https://www.kompf.de/gps/distcalc.html

    double dx;  //Länge
    double dy; //Breite
    double distance;
    double lat; //Breitengrad
   //mit distance: Entfernung in km
    lat = (lat1 + lat2) / 2 * 0.01745;   //1° = π/180 rad ≈ 0.01745 => Umrechnung rad
    dx = 111.3 * cos(lat) * (lon1 - lon2); //Abstand zwischen zwei Längenkreisen in Abhängigkeit von der geografischen Breite => cos()
    dy = 111.3 * (lat1 - lat2); //abstand zwischen zwei Breitenkreisen immer konstant 111.3 km
    //lat1, lat2, lon1, lon2: Breite, Länge
    distance = sqrt(dx * dx + dy * dy); //Pythagoras
    return distance;
}

double exactDistance(double lon1, double lon2, double lat1, double lat2) //Exakte Entfernungsberechnung für die Kugeloberfläche
{
    //Seitenkosinussatz
    double distance;
    lon1 = lon1 * 3.14 /180; //Umrechnung in rad!
    lon2 = lon2 * 3.14 /180;
    lat1 = lat1 * 3.14 /180;
    lat2 = lat2 * 3.14 /180;

    distance = 6378.388 * (acos(sin(lat1) * sin(lat2) + cos(lat1) * cos(lat2) * cos(lon2 - lon1)));
    // 6378.388 = Erdradius => damit km
    //mit Entfernung in km
    return distance;
}

void printOneCity(struct Citys* p) //Ausgabe einer Stadt mit ihren Daten
{
    if(p != NULL)
    {
        //Ausgabe der Daten
        printf("city: %s\nlat: %f\nlng: %f\ncountry: %s\ncapital: %s\npopulation: %.2f\n", p->city_ascii, p->lat, p->lng, p->country, p->capital,p->popolation);
        printf("\n");
    }
}

double changeStringtoDouble(char *ptr, char* ascii)  //Verwandelt einen String mit Zahlen in einen double Wert
{
    int i = 0;      //counter
    int counterAfterPoint = 0;  //für die Berechnung der Kommerstelle
    double sign = 1;            //Vorzeichen 1 pos, -1 neg
    double stelle = 10;         //zum Verschieben der 10er-Stellen der Zahl
    int returnValue = 0;        //Ergebnis und Rückgabewert

    int lenghtString = strlen(ptr); //Länge des Strings = Anzahl der Stellen der Zahl
    char in[lenghtString];          //zu bearbeitender String
    strncpy(in, ptr, lenghtString); //Kopieren des Inhaltes des Übergegebene Strings

    if(in[0] == '-')    //Überprüfen ob die Zahl negativ oder Positiv ist!
    {
        sign = -1;
        i++;
    }

    while(i < lenghtString)     //Durchgehen des Arrays + Umwandlung der Werte
    {
        if(in[i] == '.')        //KOMMA!!!
        {
            counterAfterPoint = 1; //Kommastelle zähler wird auf eins erhöht
        }
        else if(in[i] == '0')
        {
            returnValue = returnValue * stelle + 0; // Der vorhandene Wert wird mit Zehn multipliziert und danach
        }                                           // mit dem neuen Wert der letzten stelle addiert z.B 1 * 10 = 10 => 10 +2 = 12
        else if(in[i] == '1')
        {
            returnValue = returnValue * (stelle)+1;
        }
        else if(in[i] == '2')
        {
            returnValue = returnValue * (stelle) + 2 ;
        }
        else if(in[i] == '3')
        {
            returnValue = returnValue * (stelle)+3;
        }
        else if(in[i] == '4')
        {
            returnValue = returnValue * (stelle)+4;
        }
        else if(in[i] == '5')
        {
            returnValue = returnValue * (stelle)+5;
        }
        else if(in[i]== '6')
        {
            returnValue = returnValue * (stelle)+6;
        }
        else if(in[i] == '7')
        {
            returnValue = returnValue * (stelle)+7;
        }
        else if(in[i] == '8')
        {
            returnValue = returnValue * (stelle)+8;
        }
        else if(in[i] == '9')
        {
            returnValue = returnValue * (stelle)+9;
        }
        else
        {
            printf("%s\n", ptr);
            printf("Fehler - Uwandlung der Zahl nicht m%cglich! Stadt: %s\n",132, ascii);
            return -1;
        }

        if(counterAfterPoint > 0)  //erhöhen der Kommastelle, wenn es bereits eine gibt
        {
            counterAfterPoint++;
        }
        i++;        //erhöhen des Zählers der Zehnerstelle
    }
    return returnValue * sign / (pow(10.0, counterAfterPoint-2));  //erechnen des Werte, Vorzeichen, Kommastellen
}


char* myStrDup(const char* str)
{
    size_t count = strlen(str);
    char* newStr = malloc(sizeof(char)* (count+1));
    if(newStr == NULL)
        return NULL;

    strcpy(newStr, str);
    return newStr;
}

struct Citys* addFront(struct Citys* head, char *city, char* city_ascii, double lat, double lng,
                       char* country, double population, char* capital)    //Am Anfang der Liste einfügen + Übergeben der verwendeten Parameter der einzelnen Städte
{
    struct Citys* newCitys = malloc(sizeof(struct Citys)); //freien Speicher anfordern
    if(newCitys == NULL)
        return NULL;   //Bei Fehler wird Null zurückgegeben

    newCitys->city = myStrDup(city);    //Übergeben der einzelnen Paramter an die Struct
    newCitys->city_ascii = myStrDup(city_ascii);
    newCitys->lat = lat;
    newCitys->lng = lng;
    newCitys->country = myStrDup(country);
    newCitys->popolation = population;
    newCitys->capital = myStrDup(capital);

    newCitys->next = head;  //die head-Strcut wird auf die zweite Position in der Liste verschoben (Adresse wird übrgeben)
    newCitys->before = NULL;    //vor der ersten Struct in Liste ist keine Struct mehr => deswegen NULL

    if(head != NULL)
        head->before = newCitys; //Wenn head nicht NULL ist, dann wird der Head-Struct die Adresse der jetzt ersten Struct übergeben

    head = newCitys;    //die neue Struct City wird zu head...erste Stelle der Liste
    return head;        //Zurückgegeben wird die head!!!
}

struct Citys* addBack(struct Citys* head, char *city, char* city_ascii, double lat, double lng,
                      char* country,double population, char* capital)    //Am Ende der Liste einfügen + Übergeben der verwendeten Parameter der einzelnen Städte
{
    struct Citys* newCitys = malloc(sizeof(struct Citys)); //freien Speicher anfordern
    if(newCitys == NULL)
        return NULL;   //Bei Fehler wird Null zurückgegeben

    struct Citys *p;
    newCitys->city = myStrDup(city);    //Übergeben der einzelnen Paramter an die Struct
    newCitys->city_ascii = myStrDup(city_ascii);
    newCitys->lat = lat;
    newCitys->lng = lng;
    newCitys->country = myStrDup(country);
    newCitys->popolation = population;
    newCitys->capital = myStrDup(capital);

    newCitys->next = NULL;      //nach der letzten Struct in Liste ist keine Struct mehr => deswegen NULL

    if (head == NULL)           // wenn head leer ist => newCity autmatisch an erste Stelle der Liste = neuer head
    {
        return newCitys;
    }

    for (p=head; p->next !=NULL; p = p->next) ;         //durch gehen der Liste bis zum letzten Struct
    p->next= newCitys;                                  //Struct wird hinten angehängt (mittels Adresse)
    p->next->before = p;                                //zurück verlinkung der Struct

    return head;        //Zurückgegeben wird die head!!!
}

struct Citys* openFile(struct Citys* head)
{
    char *ptr;
    char delimiter[] = "\""; // Zeichen festelegen, nach denen der eingelesene String aus der csv datei zerlegt werden soll

    FILE *Myfile= NULL;
    Myfile = fopen("worldcities.csv","r"); //öffnen nur mit Leserechten

    if (NULL==Myfile) //Fehlermeldung, falls csv nicht geöffnet werden kann
    {
        printf("ERROR: Could not open the.csv file!");
        exit(-1);
    }
    printf("Die Daten werden geladen....\n"); // ausgebe an den user, dass die Daten geladen werden
    int i = 0;
    int count = 0;
    char temp [1024];
    do
    {
        struct Citys one;
        fgets(temp, 1024, (FILE*)Myfile); //einlesen der Daten aus der csv datei
        count++;

        if(count != 1)
        {
            ptr = strtok(temp, delimiter); // zerlegen des Strings in einzelstrings, duch selektieren nach zuvor festgelegten zeichen (delimeter)


            while(ptr != NULL)      //Durchzählen und zuordnen der einzelnen Werte aus der Datei und in struct speichern
            {
                if(i == 0)
                {
                    one.city = ptr; //stadt speichern
                }
                else if(i == 1)
                {
                    one.city_ascii = ptr;//stadt in ascii speichern
                }
                else if(i == 3)
                {
                    one.lat = changeStringtoDouble(ptr, one.city_ascii); //längengrad speichern
                }
                else if(i == 5)
                {
                    one.lng = changeStringtoDouble(ptr, one.city_ascii);//breitengrad Speichern
                }
                else if(i == 7)
                {
                    one.country = ptr; //land speichern
                }
                else if(i == 15)
                {
                    one.capital = ptr;   //hauptstadt speichern
                }
                else if(i == 17)
                {
                    one.popolation = changeStringtoDouble(ptr, one.city_ascii); //einwohner speichern
                }
                i++;
                ptr = strtok(NULL, delimiter);//prt zurücksetzen
            }
        }
        if(count != 1)
        {
            head = addFront(head, one.city, one.city_ascii, one.lat, one.lng, one.country,one.popolation,one.capital); //Speichern der Daten

            i = 0;
        }

    }while((getc(Myfile))!=EOF); //ausführen solnage nicht das ende des Files erreicht wurde.

    fclose(Myfile);//csv datei nach erfolgreichem Einlesen wieder schließen
    return head;//head zurückgeben
}

struct Citys* searchCity(struct Citys* head, char cityName[100])  //Suchen nach einer Stadt - mithilfe ASCII
{
    struct Citys* p;
    for(p=head; p != NULL; p=p->next)       //Durchgehne aller Städte
    {
        if(strcmp(p->city_ascii, myStrDup(cityName)) == 0)
        {
            return p;
        }
    }
    //printf("\nKeine Stadt mit diesem Namen gefunden!\n");
    return NULL;
}

int existCity(struct Citys* head, char cityName[100]) //Überprüfen ob eine Stadt bereits in der Liste vorkommt/exestiert
{
    struct Citys* p;
    for(p=head; p != NULL; p=p->next)
    {
        if(strcmp(p->city_ascii, myStrDup(cityName)) == 0)  //strcmp => 0 wenn gleich
        {
            return 1;       //1 = is gibt bereits eine Stadt mit dem gleichen Namen
        }
    }
    return 0; //0 = is keine Stadt mit dem gleichen Namen in der Liste
}


char* inStr()   //einlesen eines Strings
{
    char str[50];
    //fflush(stdout);
    gets(str);
    printf("%s\n", str);
    return myStrDup(str);
}


struct Citys* newList(struct Citys* head, struct Citys* headNewList)        //erstellen einer neuen Liste mit den einlesen der Städtenamen über die Konsole
{
    printf("\nNeue St%cdteliste: \nGeben Sie bitte die Namen ein. Zum Beenden - eingeben!\n",132);
    int counter = 0; //Counter fürs zählen der Städte - nur für die Konsole
    char* inputName;
    struct Citys* p = NULL;

    while(1)
    {
        printf("Name %d. Stadt: ", counter+1);
        inputName = inStr(); //einlesen der Stadt, mit Leerzeichen
        if(strcmp(inputName, "-") == 0) //Abbruchbedingung mit Char => '-'
            break;
        p = searchCity(head, inputName); //überprüfen ob die eingegebene Stadt bereits exestiert
        if(p == NULL)
        {
            printf("Keine Stadt mit diesem Namen gefunden!\n");
            printf("Bitte %cberpruefen Sie den Namen der Stadt!\n",129);
        }
        else
        {
            //Überprüfen ob die Stadt bereits in der Liste ist
            if(existCity(headNewList, inputName) == 1)
            {
                printf("Die Stadt %s ist bereits in der Liste vorhanden!\n");
                printf("Bitte geben Sie den Namen einer anderen Stadt ein!\n");
            }
            else
            {
                //Stadt wird zur Liste hinzugefügt
                headNewList = addFront(headNewList, p->city, p->city_ascii, p->lat, p->lng, p->country,p->popolation, p->capital);
                counter++;
            }

        }
    }
    return headNewList;
}

struct Citys* addCityConsol(struct Citys* head)     //zusätzliche Eingabe einer Stadt, mit den Daten, über die Konsole
{
    char in[50];
    struct Citys one;
    //Einlesen der Daten!
    printf("Bitte geben Sie folgenden Daten der neuen Stadt ein:\n");
    printf("City: ");
    gets(in);
    one.city = myStrDup(in);
    printf("City in ASCII: ");
    gets(in);
    one.city_ascii = myStrDup(in);
    printf("lat: ");
    scanf("%lf", &one.lat);
    printf("lng: ");
    scanf("%lf", &one.lng);
    getchar();
    printf("Country: ");
    gets(in);
    one.country = myStrDup(in);
    printf("Population: ");
    scanf("%lf", &one.popolation);
    getchar();
    printf("Capital: ");
    gets(in);
    one.capital = myStrDup(in);

    if(existCity(head, one.city_ascii) == 1) //Überprüfen ob die Stadt schon in der Liste vorhanden ist
    {
        printf("Die Stadt %s ist bereits in der Liste vorhanden!\n");
    }
    else
    {
        //Hinzufügen der Stadt zu Liste
        head = addFront(head, one.city, one.city_ascii, one.lat, one.lng, one.country,one.popolation,one.capital);
    }

    return head;

}

void printReverse(struct Citys* head) //Ausgabe auf der Konsole => Daten des Algorithmus, in verkehrter Reihenfolge, da die Ausgangstadt am ende der Liste liegt
{
    struct Citys *p;
    struct Citys *last;
    if(head ==NULL)
    {
        printf("list is empty!\n");
        return;
    }

    for(p = head; p != NULL; p = p->next)
    {
        last = p;   //Suchen der Ausgangsstadt - an letzter Stelle der Liste
    }
    for(p = last; p != NULL; p = p->before) //Ausgabe beginnten von hinten
    {
        printf("City: %s\n", p->city_ascii);
        if(p->before != NULL)
        {
            //printf("Distance Between:        %.2f km\n",distance(p->lng,p->before->lng, p->lat, p->before->lat));
            //printf("Better Distance Between: %.2f km\n",betterDistance(p->lng,p->before->lng, p->lat, p->before->lat));
            printf("Exact Distance Between:  %.2f km\n",exactDistance(p->lng,p->before->lng, p->lat, p->before->lat));
        }
        else
        {
            //printf("Distance Between:        %.2f km\n",distance(p->lng,last->lng, p->lat, last->lat));
            //printf("Better Distance Between: %.2f km\n",betterDistance(p->lng,last->lng, p->lat, last->lat));
            printf("Exact Distance Between:  %.2f km\nCity: %s\n", exactDistance(p->lng,last->lng, p->lat, last->lat), last->city_ascii);
        }
    }
    printf("\n");

}

struct Citys* freeOne(struct Citys* head, struct Citys* target) //Freigabe eine Stadt aus der Liste
{
    if(head == target)  //Wenn das Ziel die head-Stadt ist => Sonderbedingungen
    {
        if(head->next == NULL)
        {
            free(head);
            return NULL;
        }
        else
        {
            head = head->next;
            head->before = NULL;
        }

    }
    else //Stadt liegt nach der head
    {
        if(target->next == NULL) //Stadt ist das letzte Glied der Liste
        {
            target->before->next = NULL;
        }
        else    //Stadt liegt zwischen drinen
        {
            target->next->before = target->before;
            target->before->next = target->next;
        }
    }

    free(target);

    return head;
}

struct Citys* nearestNeigborsAlgorithm(struct Citys* head)      //Nearest-Neighbor-Heuristik-Algorithmus
{
    //Beginn: Ausgangsstadt => nähesten nächste Stadt = nächstes Ziel => dieses Zeil wird zum Ausgangspunkt
    //Wenn alle Städte asugewählt wurden => zurück zur Ausgangsstadt
    struct Citys* p_head = NULL; //Neu Liste zum Sortiern
    p_head = addFront(p_head, "Vienna", "Vienna", 48.2, 16.3666, "Austria", 1897491, "primary");  //Erster Eintrag in der neuen List = Ausgangsstadt
    struct Citys* one = NULL; //Suchen/Durchgehen der Städte
    struct Citys* nearest = NULL; //Nähestes - zum zwischen Speichern
    double distance1; //Entfernug zwischen zwei Städten
    double nearestDistance = 99999999999999;  // auf dobuble max wert umschreiben //Nähestes Distanz - zum zwischen Speichern


    while(head != NULL) //Die Ergebnise werden in eine neue Liste gespeichert, solang bis die alte Liste leer ist
    {
        for (one=head; one !=NULL; one = one->next)
        {
            //printOneCity(p_head);
            //printOneCity(one);
            distance1 = exactDistance(p_head->lng, one->lng, p_head->lat, one->lat); //Berechnung der Distanz

            //printf("\nDistanc: %f <> Nearest: %f\n", distance1, nearestDistance);

            if(distance1 < nearestDistance) //Vergeleichen der Distanzen
            {
                nearest = one;      //Überschreiben
                nearestDistance = distance1; //Überschreiben
            }
        }
        p_head = addFront(p_head, nearest->city, nearest->city_ascii, nearest->lat, nearest->lng, nearest->country,nearest->popolation, nearest->capital); //hinzufügen zur neuen Liste an erster Stelle
        head = freeOne(head, nearest); //Entfernen der Stadt aus der alten Liste => damit sie nicht doppelt verwendet wid
        nearestDistance = 99999999999999;
    }
    return p_head;
}

//felix code beginn

void printallsorted(struct Citys * data)
{
    printf("city:                    lat:      lng:        country:              capital:    population: \n");
    for (struct Citys *i = data; i != NULL; i = i->next)
    {
        //printf("%20s    %15.f\n", i->city_ascii, i->popolation);
        printf("%20s  %10.4f  %10.4f  %20s  %10s  %15.2f\n", i->city_ascii, i->lat, i->lng, i->country, i->capital,i->popolation);
    }

}

void printsortedbyname(struct Citys * data)
{
    printf("city:                    lat:      lng:        country:              capital:    population: \n");
    for (struct Citys *i = data; i != NULL; i = i->next)
    {
        //printf("%20s\n", i->city_ascii);
        printf("%20s  %10.4f  %10.4f  %20s  %10s  %15.2f\n", i->city_ascii, i->lat, i->lng, i->country, i->capital,i->popolation);
    }

}

void sortbyname(struct Citys *data, struct Citys *newdata)
{
    for(struct Citys *i = data; i != NULL; i = i->next)
    {
        struct Citys *newentry = malloc(sizeof(struct Citys)); //Arbeitskopie erstellen
        newentry->before = NULL;
        newentry->next = NULL;
        newentry->capital = myStrDup(i->capital);
        newentry->city = myStrDup(i->city);
        newentry->city_ascii = myStrDup(i->city_ascii);
        newentry->country = myStrDup(i->country);
        newentry->lat = i->lat;
        newentry->lng = i->lng;
        newentry->popolation = i->popolation;


        if (i->before == NULL) //Liste leer: 1. Element und setzen
        {
            newdata = newentry;
        }
        else //Liste nicht leer, Position für aktuelles Element finden
        {
            if (strcmp(i->city_ascii, newdata->city_ascii) < 0) //aktuelles Element ist kleiner -> Tausch
            {
                struct Citys *temp = newdata;
                newdata = newentry;
                newdata->next = temp;
                newdata->next->before = newdata;
            }
            else //aktuelles Element ist größer
            {
                for (struct Citys *j = newdata; j != NULL; j = j->next) //Element wird hinten angehängt
                {
                    if (j->next == NULL) //Element ist größer als das letzte der Liste
                    {
                        j->next = newentry;
                        newentry->before = j;
                        newentry->next = NULL;
                        break;
                    }

                    else if (strcmp(j->next->city_ascii, i->city_ascii) > 0) //Element gehört zwischen 2 Einträge
                    {
                        newentry->next = j->next;
                        newentry->before = j;
                        j->next->before = newentry;
                        j->next = newentry;
                        break;

                    }

                }
            }
        }
    }

    printsortedbyname(newdata);
}

void sortbypopulation(struct Citys *data, struct Citys *newdata)
{

    for(struct Citys *i = data; i != NULL; i = i->next)
    {
        struct Citys *newentry = malloc(sizeof(struct Citys));
        newentry->before = NULL;
        newentry->next = NULL;
        newentry->capital = myStrDup(i->capital);
        newentry->city = myStrDup(i->city);
        newentry->city_ascii = myStrDup(i->city_ascii);
        newentry->country = myStrDup(i->country);
        newentry->lat = i->lat;
        newentry->lng = i->lng;
        newentry->popolation = i->popolation;


        if (i->before == NULL)
        {
            newdata = newentry;
        }
        else
        {
            if (i->popolation < newdata->popolation)
            {
                struct Citys *temp = newdata;
                newdata = newentry;
                newdata->next = temp;
                newdata->next->before = newdata;
            }
            else
            {
                for (struct Citys *j = newdata; j != NULL; j = j->next)
                {
                    if (j->next == NULL)
                    {
                        j->next = newentry;
                        newentry->before = j;
                        newentry->next = NULL;
                        break;
                    }

                    else if (j->next->popolation > i->popolation)
                    {
                        newentry->next = j->next;
                        newentry->before = j;
                        j->next->before = newentry;
                        j->next = newentry;
                        break;

                    }


                }
            }
        }
    }
    printallsorted(newdata);

}

//felix code end


void createtxt(struct Citys* head)//Übergabe des head = Anfang der Liste
{
    FILE* outputfile;
    outputfile = fopen("Route.txt","w"); // Datei neu erzeugen bzw. ueberschreiben, wenn es sie schon gibt

    struct Citys *p;
    struct Citys *last;
    if(head ==NULL) //wenn Liste leer ist fehler ausgeben
    {
        printf("Fehler, .txt konnte nicht erstellt werden, da die Liste leer ist!\n");
        return;
    }

    for(p = head; p != NULL; p = p->next)
    {
        last = p;
    }
    for(p = last; p != NULL; p = p->before) //Liste der berechneten Route der Reihe nach von hinten durchgehen
    {
        fprintf(outputfile,"City: %s\n", p->city_ascii); //ausgabe der Stadt
        if(p->before != NULL)
        {
            fprintf(outputfile,"Distance Between: %.2f km\n",distance(p->lng,p->before->lng, p->lat, p->before->lat)); //Ausgabe der Distanz der ersten Stadt
        }
        else
        {
            fprintf(outputfile,"Distance Between: %.2f km\nCity: %s\n",distance(p->lng,last->lng, p->lat, last->lat), last->city_ascii); //ausgabe der berechneten distanz und stadt von hinten nach vorne
        }
    }
    fclose(outputfile); // Datei schließen
    printf(" \n//////////////////////////\n"); //bestätigung für user das .txt erstellt wurde
    printf(".txt erfolgreich erstellt!\n");
    printf("//////////////////////////\n\n");

}

void createcsv(struct Citys* head)  //Übergabe des head = Anfang der Liste
{
    FILE* outputfile;
    outputfile = fopen("worldcitiesnew.csv","w"); // Datei neu erzeugen bzw. ueberschreiben, wenn es sie schon gibt

    struct Citys *p;  //Zusätzliches Struct für die Ausgabe
    if(head == NULL)   //Wenn nur eine leere Liste vorhanden ist
    {
        fprintf(outputfile,"list empty\n"); //Fehlermeldung, dass Liste leer ist
        return;
    }
    fprintf(outputfile,"city,\"city_ascii\",\"lat\",\"lng\",\"country\",\"iso2\",\"iso3\",\"admin_name\",\"capital\",\"population\",\"id\"\n"); //Legende in erste Zeile der csv Datei schreiben
    for(p = head; p != NULL; p = p->next)  //Abarbeitung der einzelnen structs von head bis zum Ende => p==NULL
    {
        fprintf(outputfile,"\"%s\",\"%s\",\"%f\",\"%f\",\"%s\",\"\",\"\",\"\",\"%s\",\"%f\",\"\"\n", p->city, p->city_ascii, p->lat, p->lng, p->country, p->capital,p->popolation);//erstellen der csv Datei, Stadt für Statd
    }
    fclose(outputfile);
    printf(" \n////////////////////////////////////\n");//bestätigung für user das .csv erstellt wurde
    printf("Neue csv Datei erfolgreich erstellt!\n");
    printf("////////////////////////////////////\n\n");
}

struct Bucket* addBackBucket(struct Bucket* head, char letter)    //Am Ende der Liste einfügen + Übergeben der verwendeten Parameter der einzelnen Städte
{
    struct Bucket* newBucket = malloc(sizeof(struct Bucket)); //freien Speicher anfordern
    if(newBucket == NULL)
        return NULL;   //Bei Fehler wird Null zurückgegeben

    struct Bucket *p;
    newBucket->letter = letter;    //Übergeben der einzelnen Paramter an die Struct

    newBucket->next = NULL;      //nach der letzten Struct in Liste ist keine Struct mehr => deswegen NULL
    newBucket->head = NULL;

    if (head == NULL)           // wenn head leer ist => newCity autmatisch an erste Stelle der Liste = neuer head
    {
        return newBucket;
    }

    for (p=head; p->next !=NULL; p = p->next) ;         //durch gehen der Liste bis zum letzten Struct
    p->next= newBucket;                                  //Struct wird hinten angehängt (mittels Adresse)
    p->next->before = p;                                //zurück verlinkung der Struct

    return head;        //Zurückgegeben wird die head!!!
}

struct Bucket* BucketSort(struct Citys* head) //Einzelne Buckets nach Alphapet A-Z
{
    struct Bucket* headBucket = NULL;
    struct Bucket* p = NULL;
    struct Citys* x = NULL;
    struct Citys* newX = NULL;
    char abc[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"; //Szring für die Bennenung der einzelnen Buckets => first letter
    char first = ' ';

    for(int i = 0; i < 26; i++) //Bennenung der einzelnen Buckets von A-Z
    {
        headBucket = addBackBucket(headBucket, abc[i]); //Hinzufügen eines neuen Buckets mit letter/Buchstabe
    }
    /*
        for(p = headBucket; p != NULL; p = p->next)  //Abarbeitung der einzelnen structs von head bis zum Ende => p==NULL
        {
            printf("%c\n", p->letter);
        }
        */
    for(x = head; x != NULL; x = x->next)  //Einordnen der Städte in die Buckets => Abarbeitung der einzelnen structs von head bis zum Ende => x==NULL
    {
        first = x->city_ascii[0];   //erster Buchstabe des Städtenamens
        for(p = headBucket; p != NULL; p = p->next)
        {
            if(first == p->letter) //Vergleich zwischen ersten Buchstaben den Städtenamens und dem Bucket-Letter
            {
                //printf("%c\n", first);
                //printf("%s\n", x->city_ascii);
                p->head = addBack(p->head, x->city, x->city_ascii, x->lat, x->lng, x->country, x->popolation, x->capital );
                //einfügen in die Städte Liste des Buckets
            }
        }
        //printf("%c\n", p->letter);
    }

    for(p = headBucket; p != NULL; p = p->next)     //Sortieren der Listen in den einzelnen Buckets
    {
        printf("\nBuchstabe %c:\n", p->letter);
        sortbyname(p->head, newX);
        newX = NULL;
    }

    return headBucket; //Rückgabe der sortierten Buckets
}

struct Citys* searchInBucket(struct Bucket* head, char cityName[100])  //Suchen nach einer Stadt in den Buckets - mithilfe ASCII
{
    struct Bucket* p = NULL;
    struct Citys* x = NULL;
    for(p=head; p != NULL; p=p->next)       //Durchgehne aller Buckets
    {
        if(cityName[0]==p->letter)          //Vergleichen der Letter des Buckets mit dem ersten Buchstaben der Stadt
        {
            for(x = p->head; x != NULL; x = x->next) //Durchgehen der Städte-Liste im Bucket
            {
                if(strcmp(x->city_ascii, myStrDup(cityName)) == 0) //Vergleichen des ASCII Codes
                {
                    return x; //Rückgabe der gefundenen Stadt
                }
            }
        }

    }
    printf("\nKeine Stadt mit diesem Namen gefunden!\n");
    return NULL;
}

#endif // ROUTENPLANER_H_INCLUDED
