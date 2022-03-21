/* =================================================== Libraries ===================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/* ==================================================== Constants ==================================================== */

#define SizeCountry 31
#define SizeCity 51

#define SizeAirpts 10
#define SizeFlights 30000

/* ================================================ Data Structures ================================================== */

typedef struct {
    int year;
    int month;
    int day;
} date;


typedef struct {
    int hour;
    int minute;
} time;

typedef struct {
    char IDA[4];
    char country[SizeCountry];
    char city[SizeCity];
    int nFlights;
} airport;

typedef struct {
    char IDV[7];
    airport depAirpt;
    airport arrAirpt;
    date depDate;
    time depTime;
    time flightTime;
    int maxPax;
} flight;



/* ================================================ Global Variables ================================================= */
airport listAirpts[SizeAirpts] = {0};

int nAirpts = 0; 

/* ==================================================== Prototypes =================================================== */

void eatLine();

date makeDate(char inputDate[10]);
time makeTime(char inputTime[4]);

void addAirport(airport newAirpt);
void printAirports(airport airport);
int findAirport(char IDA[3]);

void addFlight(flight newFlight);


void command_a();
void command_l();
void command_v();
void command_p();
void command_c();
void command_t();


/* ================================================ Main Program ===================================================== */
void eatLine() {
    while(getchar() != '\n') {}
}

void printAirport(airport airport) {
    printf("%s %s %s %d\n", 
        airport.IDA, airport.country, airport.city, airport.nFlights);
}

int findAirport(char IDA[3]) {
    int i = 0;

    for(i = 0; i < nAirpts; i++)
        if(!strcmp(listAirpts[i].IDA, IDA))
            break;        

    return i;
}

//So gross to make the exact same function twice. Easily solvable with pointers

int equalAirport(char IDA[3]) {
    for(int i = 0; i < nAirpts; i++)
        if(!strcmp(listAirpts[i].IDA, IDA))
            return 1;

    return 0;

}




int main() {
    memset(listAirpts, 0, SizeAirpts * sizeof(airport));

    char cmd;

    while (cmd != 'q') {

        cmd = getchar();

        switch(cmd) {
            case 'a':
                printf("Read: %c\n", cmd);
                command_a();
                eatLine();
                break;
            case 'l':
                printf("Read: %c\n", cmd);
                command_l();
                break;
            case 'v':
                printf("Read: %c\n", cmd);
                break;
            case 'p':
                printf("Read: %c\n", cmd);
                break;
            case 'c':
                printf("Read: %c\n", cmd);
                break;
            case 't':
                printf("Read: %c\n", cmd);
                break;

            default:
                printf("Read: %c\n", cmd);
                //erase read line
        }

    }

    return 0;
}



void command_a() {
    char IDA[4] = {0};
    char city[SizeCity] = {0};
    char country[SizeCountry] = {0};

    getchar(); //Eats initial space
    scanf("%s %s %s", IDA, country, city);


    //Check for Errors
    if(islower(IDA[0]) || islower(IDA[1]) || islower(IDA[2]))
        printf("invalid airport ID\n");
    else if (nAirpts == SizeAirpts)
        printf("too many airports\n");    
    else if (equalAirport(IDA))
        printf("duplicate airport\n");

    else { //Passes all validations

        //Create Event
        airport newAirport;

        strcpy(newAirport.IDA, IDA);
        strcpy(newAirport.country, country); 
        strcpy(newAirport.city, city); 
        newAirport.nFlights = 0;

        //Add Event
        nAirpts++;

        for(int i = 0; i < nAirpts - 1; i++) {

            if(strcmp(listAirpts[i].IDA, newAirport.IDA) > 0) {
                //Move everything up one spot from where the new one will be inserted
                for(int j = nAirpts - 1; j > i; j--)
                    listAirpts[j] = listAirpts[j - 1];

                listAirpts[i] = newAirport;

                printf("airport %s\n", newAirport.IDA);
                return;
            }
        }

        listAirpts[nAirpts - 1] = newAirport;
        printf("airport %s\n", newAirport.IDA);
    }
}

void command_l() {
    if(getchar() == '\n') 
        for(int i = 0; i < nAirpts; i ++)
            printAirport(listAirpts[i]);

    //TODO Receive list of IDAs and only print those. Error message
}