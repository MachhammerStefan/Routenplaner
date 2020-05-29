#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

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

void print(struct Citys* head)  //�bergabe der head = Anfang der Liste
{
    struct Citys *p;  //Zus�tzliches Struct f�r die Ausgabe
    if(head ==NULL)   //Wenn nur eine leere Liste vorhanden ist
    {
        printf("list empty\n");
        return;
    }

    for(p = head; p != NULL; p = p->next)  //Abarbeitung der einzelnen structs von head bis zum Ende => p==NULL
    {
        printf("city: %s\nascii: %s\nlat: %f\nlng: %f\ncountry: %s\ncapital: %s\npopulation: %f\nbefore: %p current: %p next: %p\n", p->city, p->city_ascii, p->lat, p->lng, p->country, p->capital,p->popolation, p->before, p, p->next);
        printf("\n");
    }
    printf("\n");
}

double distance(double lon1, double lon2, double lat1, double lat2) //Berechnung der Distanz (Luftlinie) �ber den Pytagoras
{
    //Einfachste Berechnung => leider etwas ungenau!!!!!
    //latitude => Breitengrad in Grad
    //longitude => L�ngengrad in Grad
    double dx;
    double dy;
    double distance;

    dx = 71.5 * (lon1 - lon2);  //Distanz zwischen den L�ngengraden mit Umrechnung in Kilometer
    dy = 111.3 * (lat1 - lat2); //Distanz zwischen den Breitengraden mit Umrechnung in Kilometer
    distance = sqrt(dx * dx + dy * dy); //Anwendung Satz dens Pydagoras => Errechnung der Hypothenuse

    return distance;

}

void printOneCity(struct Citys* p) //Ausgabe einer Stadt mit ihren Daten
{
    if(p != NULL)
    {
        printf("city: %s\nascii: %s\nlat: %f\nlng: %f\ncountry: %s\ncapital: %s\npopulation: %f\nbefore: %p current: %p next: %p\n", p->city, p->city_ascii, p->lat, p->lng, p->country, p->capital,p->popolation, p->before, p, p->next);
        printf("\n");
    }

}


double changeStringtoDouble(char *ptr)  //Verwandelt einen String mit Zahlen in einen double Wert
{
    int i = 0;      //counter
    int counterAfterPoint = 0;  //f�r die Berechnung der Kommerstelle
    double sign = 1;            //Vorzeichen 1 pos, -1 neg
    double stelle = 10;         //zum Verschieben der 10er-Stellen der Zahl
    int returnValue = 0;        //Ergebnis und R�ckgabewert

    int lenghtString = strlen(ptr); //L�nge des Strings = Anzahl der Stellen der Zahl
    char in[lenghtString];          //zu bearbeitender String
    strncpy(in, ptr, lenghtString); //Kopieren des Inhaltes des �bergegebene Strings

    if(in[0] == '-')    //�berpr�fen ob die Zahl negativ oder Positiv ist!
    {
        sign = -1;
        i++;
    }

    while(i < lenghtString)     //Durchgehen des Arrays + Umwandlung der Werte
    {
        if(in[i] == '.')        //KOMMA!!!
        {
            counterAfterPoint = 1; //Kommastelle z�hler wird auf eins erh�ht
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
            printf("%c\n", in[i]);
            printf("Fehler - Zahl konnte nicht eingelesen werden!\n");
        }

        if(counterAfterPoint > 0)  //erh�hen der Kommastelle, wenn es bereits eine gibt
        {
            counterAfterPoint++;
        }
        i++;        //erh�hen des Z�hlers der Zehnerstelle
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
                       char* country, double population, char* capital)    //Am Anfang der Liste einf�gen + �bergeben der verwendeten Parameter der einzelnen St�dte
{
    struct Citys* newCitys = malloc(sizeof(struct Citys)); //freien Speicher anfordern
    if(newCitys == NULL)
        return NULL;   //Bei Fehler wird Null zur�ckgegeben

    newCitys->city = myStrDup(city);    //�bergeben der einzelnen Paramter an die Struct
    newCitys->city_ascii = myStrDup(city_ascii);
    newCitys->lat = lat;
    newCitys->lng = lng;
    newCitys->country = myStrDup(country);
    newCitys->popolation = population;
    newCitys->capital = myStrDup(capital);

    newCitys->next = head;  //die head-Strcut wird auf die zweite Position in der Liste verschoben (Adresse wird �brgeben)
    newCitys->before = NULL;    //vor der ersten Struct in Liste ist keine Struct mehr => deswegen NULL

    if(head != NULL)
        head->before = newCitys; //Wenn head nicht NULL ist, dann wird der Head-Struct die Adresse der jetzt ersten Struct �bergeben


    head = newCitys;    //die neue Struct City wird zu head...erste Stelle der Liste
    return head;        //Zur�ckgegeben wird die head!!!
}

struct Citys* addBack(struct Citys* head, char *city, char* city_ascii, double lat, double lng,
                      char* country,double population, char* capital)    //Am Ende der Liste einf�gen + �bergeben der verwendeten Parameter der einzelnen St�dte
{
    struct Citys* newCitys = malloc(sizeof(struct Citys)); //freien Speicher anfordern
    if(newCitys == NULL)
        return NULL;   //Bei Fehler wird Null zur�ckgegeben

    struct Citys *p;
    newCitys->city = myStrDup(city);    //�bergeben der einzelnen Paramter an die Struct
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
    p->next= newCitys;                                  //Struct wird hinten angeh�ngt (mittels Adresse)
    p->next->before = p;                                //zur�ck verlinkung der Struct

    return head;        //Zur�ckgegeben wird die head!!!
}


struct Citys* openFile(struct Citys* head)
{
    char *ptr;
    char delimiter[] = ",\"";

    FILE *Myfile= NULL;
    Myfile = fopen("worldcities.csv","r"); //�ffnen nur mit Leserechten

    if (NULL==Myfile)
    {
        printf("ERROR: Could not open the.csv file!");
        exit(-1);
    }
    printf("Die Daten werden geladen....\n");

    int i = 0;
    int count = 0;
    char temp [1024];
    do
    {
        struct Citys one;
        fgets(temp, 1024, (FILE*)Myfile);
        count++;
        if(count != 1)
        {
            ptr = strtok(temp, delimiter);

            while(ptr != NULL)      //Durchz�hlen und zuordnen der einzelnen Werte aus der Datei
            {
                if(i == 0)
                {
                    one.city = ptr;
                }
                else if(i == 1)
                {
                    one.city_ascii = ptr;
                }
                else if(i == 2)
                {
                    one.lat = changeStringtoDouble(ptr);
                }
                else if(i == 3)
                {
                    one.lng = changeStringtoDouble(ptr);
                }
                else if(i == 4)
                {
                    one.country = ptr;
                }
                else if(i == 8)
                {
                    one.capital = ptr;          //Funkt no ned ganz!!!!!!!!!!!!!!!!!!!!!!!!
                }
                else if(i == 9)
                {
                    one.popolation = changeStringtoDouble(ptr);
                }
                i++;
                ptr = strtok(NULL, delimiter);
            }
        }
        if(count != 1)
        {
            head = addFront(head, one.city, one.city_ascii, one.lat, one.lng, one.country,one.popolation ,one.capital);

            i = 0;
        }

    }
    while((getc(Myfile))!=EOF);

    fclose(Myfile);
    return head;
}

struct Citys* searchCity(struct Citys* head, char cityName[100])
{
    struct Citys* p;
    for(p=head; p != NULL; p=p->next)
    {
        if(strcmp(p->city_ascii, myStrDup(cityName)) == 0)
        {
            return p;
        }
    }
    //printf("\nKeine Stadt mit diesem Namen gefunden!\n");
    return NULL;
}

int existCity(struct Citys* head, char cityName[100])
{
    struct Citys* p;
    for(p=head; p != NULL; p=p->next)
    {
        if(strcmp(p->city_ascii, myStrDup(cityName)) == 0)
        {
            return 1;
        }
    }
    return 0;
}


char* inStr()
{
    char str[50];
    //fflush(stdout);
    gets(str);
    printf("%s\n", str);
    return myStrDup(str);
}


struct Citys* newList(struct Citys* head, struct Citys* headNewList)
{
    printf("\nNeue St�dteliste: \nGeben Sie bitte die Namen ein. Zum Beenden - eingeben!\n");
    int counter = 0;
    char* inputName;
    struct Citys* p = NULL;

    while(1)
    {
        printf("Name %d. Stadt: ", counter+1);
        inputName = inStr();
        if(strcmp(inputName, "-") == 0)
            break;
        p = searchCity(head, inputName);
        if(p == NULL)
        {
            printf("Keine Stadt mit diesem Namen gefunden!\n");
            printf("Bitte ueberpruefen Sie den Namen der Stadt!\n");
        }
        else
        {
            //doppelte Eingabe muss noch �berpr�ft werden!!!!!!!!!!!!!
            if(existCity(headNewList, inputName) == 1)
            {
                printf("Die Stadt %s ist bereits in der Liste vorhanden!\n");
                printf("Bitte geben Sie den Namen einer anderen Stadt ein!\n");
            }
            else
            {
                headNewList = addFront(headNewList, p->city, p->city_ascii, p->lat, p->lng, p->country,p->popolation, p->capital);
                counter++;
            }

        }
    }
    return headNewList;
}

struct Citys* addCityConsol(struct Citys* head)
{
    char in[50];
    struct Citys one;
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

    if(existCity(head, one.city_ascii) == 1)
    {
        printf("Die Stadt %s ist bereits in der Liste vorhanden!\n");
    }
    else
    {
        head = addFront(head, one.city, one.city_ascii, one.lat, one.lng, one.country,one.popolation ,one.capital);
    }

    return head;

}

void printReverse(struct Citys* head)
{
    struct Citys *p;
    struct Citys *last;
    if(head ==NULL)
    {
        printf("list empty\n");
        return;
    }

    for(p = head; p != NULL; p = p->next)
    {
        last = p;
    }
    for(p = last; p != NULL; p = p->before)
    {
        printf("City: %s\n", p->city_ascii);
        if(p->before != NULL)
        {
            printf("Distance Between: %.2f km\n",distance(p->lng,p->before->lng, p->lat, p->before->lat));
        }
        else
        {
            printf("Distance Between: %.2f km\nCity: %s\n",distance(p->lng,last->lng, p->lat, last->lat), last->city_ascii);
        }
    }
    printf("\n");

}

struct Citys* freeOne(struct Citys* head, struct Citys* target)
{
    if(head == target)
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
    else
    {
        if(target->next == NULL)
        {
            target->before->next = NULL;
        }
        else
        {
            target->next->before = target->before;
            target->before->next = target->next;
        }
    }

    free(target);

    return head;
}

struct Citys* nearestNeigborsAlgorithm(struct Citys* head)
{
    struct Citys* p_head = NULL;
    p_head = addFront(p_head, "Vienna", "Vienna", 48.2, 16.3666, "Austria", 1897491 , "primary"); //Neue List
    struct Citys* one = NULL; //Suchen
    struct Citys* nearest = NULL; //N�hestes
    double distance1;
    double nearestDistance = 99999999999999;  // auf dobuble max wert umschreiben
    double dx;
    double dy;
    double lon1;
    double lon2;
    double lat1;
    double lat2;

    while(head != NULL)
    {
        for (one=head; one !=NULL; one = one->next)
        {
            //printOneCity(p_head);
            //printOneCity(one);
            distance1 = distance(p_head->lng, lon2 = one->lng, lat1 = p_head->lat,lat2 = one->lat);
            //printf("\nDistanc: %f <> Nearest: %f\n", distance1, nearestDistance);

            if(distance1 < nearestDistance)
            {
                nearest = one;
                nearestDistance = distance1;
            }
        }
        p_head = addFront(p_head, nearest->city, nearest->city_ascii, nearest->lat, nearest->lng, nearest->country,nearest->popolation, nearest->capital);
        head = freeOne(head, nearest);
        nearestDistance = 99999999999999;
    }
    return p_head;
}

//felix code beginn

void printallsorted(struct Citys * data)
{
    for (struct Citys *i = data; i != NULL; i = i->next)
    {
        printf("%20s    %15.f\n", i->city_ascii, i->popolation);
    }

}

/*void sortbyname(struct Citys *data, struct Citys *newdata)
{

}
*/
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

int main()
{
    int select;  //Auswahl Variable f�r das Men�
    int endProg = 0;        // Das Program wird solange Wiederholt bis diese Variable auf 1 gesetzt wird
    struct Citys* head = NULL;  //Erste Struct/Stadt der Liste => aus CSV-Datei
    struct Citys* headNewList = NULL;       // head einer Neu erstellten Liste (Zum Suchen, Ausw�hlen und Berechnen)
    //head = addBack(head, "test", "test2", 22.2, 11.1, "test3", "test4");
    head = openFile(head);  //�ffnen und einlesen der  CSV-Datei
    printf("Ihre Daten wurden geladen.\n");
    while(endProg != 1)         //Men�
    {

        printf("Bitte waehlen Sie eine Zahl fuer Ihre Aktion aus:\n");
        printf(" + Programm beenden................0\n");
        printf(" + Ausgeben der Daten..............1\n");
        printf(" + Suchen nach einer Stadt.........2\n");
        printf(" + Sortieren nach Namen............3\n");
        printf(" + Sortieren nach Einwohnerzahl....4\n");
        printf(" + Hinzufuegen einer neuen Stadt...5\n");
        printf(" + Kuerzeste Sterecke berechnen....6\n");
        scanf("%d", &select); //Eingabe auf der Konsole zur Auswahl
        getchar(); //Damit fegts funktionert, ansonsten wird ein leere String eingelesen

        switch(select)      //Auswahl der zuverf�gung stehendne Programme
        {
        case 0:             // Das Program wird beendet
            printf("Programm wird geschlossen. ShutDown!\n");
            endProg = 1;     // setzten der Variable, zum Unterbrechen der while-Schleife
            break;
        case 1:
            print(head);     //Asugabe aller in der Liste vorhanden Daten/St�dte
            break;
        case 2:
            printf("Bitte geben Sie den Namen der Stadt ein: ");
            printOneCity(searchCity(head,inStr())); //Asugabe einer Stadt + Suchen eine Stadt + Namen einlesen aus der Konsole
            break;
        case 3:
            //sortbyname(head, headNewList);
            break;
        case 4:
            sortbypopulation(head, headNewList);
            break;
        case 5:
            head = addCityConsol(head); //Einf�gen einer neuen Stadt, am anfang der Liste
            break;
        case 6:
            headNewList = newList(head, headNewList);  //Neue Liste mit ausgew�hlten St�dten wird erstellt
            print(headNewList);                         //Ausgabe aller in der Liste vorhanden Daten/St�dte
            headNewList = nearestNeigborsAlgorithm(headNewList);    //gibt eine List zur�ck mit den Eingegebenen St�dten und dem AusgangsPunkt, hier Vienna
            printReverse(headNewList);  //Die zur�ckgegebne List ist nach dem LAST-IN-FIRST-OUT Prinzip, deswegen muss die Liste umgedreht werden
            free(headNewList); //Die neu erstellte Liste wird nach deren Verwendung wieder freigegeben
            headNewList = NULL;
            break;
        default:
            printf("Ein Fehler ist aufgetreten, bitte starten Sie das Programm neu!\n");
            break;
        }

    }
    return 0;
}
