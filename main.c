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
    char depIDA[4];
    char arrIDA[4];
    date depDate;
    time depTime;
    time flightTime;
    int maxPax;
} flight;



/* ================================================ Global Variables ================================================= */
airport listAirpts[SizeAirpts] = {0};
flight listFlights[SizeFlights] = {0};

int nAirpts = 0;
int nFlights = 0; 

date currentDate = {.year = 2022, .month = 1, .day = 1};

/* ==================================================== Prototypes =================================================== */

void eatLine();

date makeDate(char inputDate[11]);
void printDate(date date);
date incrementDate(date date);
date getArrDate(date depDate, time arrTime, time fTime);

time makeTime(char inputTime[5]);
time getArrTime(time inputTime, time flightTime);

void addAirport(airport newAirpt);
void printAirports(airport airport);
int findAirport(char IDA[4]);

void addFlight(flight newFlight);
void printFlight(flight flight);

void command_a();
void command_l();
void command_v();
void command_p();
void command_c();
void command_T();


/* ================================================ Main Program ===================================================== */
void eatLine() {
    while(getchar() != '\n') {}
}

int validateIDV(char inputIDV[7]) {
    for(int i = 0; i < strlen(inputIDV); i++) {
        if((i < 2 && islower(inputIDV[i])) ||
            (i >= 2 && !isdigit(inputIDV[i])))
            return 0; 
    }

    return 1;
}

//Its late and I'm lazy, so braindead code it is for all the date stuff xD
date makeDate(char inputDate[11]) {
    char day[3] = {0}, month[3] = {0}, year[5] = {0};

    for(int i = 0; i < strlen(inputDate); i++) {
        if(i < 2)
            day[i] = inputDate[i];
        else if(i > 2 && i < 5)
            month[i % 3] += inputDate[i];
        else if(i > 5)
            year[i % 6] += inputDate[i];
    }

    date newDate = {.day = atoi(day), .month = atoi(month), .year = atoi(year)};

    return newDate;
}

date incrementDate(date inputDate) {
    int days[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if(++inputDate.day > days[inputDate.month - 1]) {
        inputDate.day = 1;
        
        if(++inputDate.month > 12) {
            inputDate.year++;
            inputDate.month = 1;
        }
    }

    return inputDate;
}


date getArrDate(date depDate, time arrTime, time fTime) {
    date arrDate;

    if((arrTime.hour - fTime.hour) < 0 ||
        ((arrTime.hour - fTime.hour) == 0 && (arrTime.minute - fTime.minute) < 0))
        arrDate = incrementDate(depDate);
    else 
        arrDate = depDate;
    
    return arrDate;

}

void printDate(date date) {
    printf("%02d-%02d-%04d\n", date.day, date.month, date.year);
}

int valiDate(date date) {
    if(date.year < currentDate.year ||
        (date.year == currentDate.year && date.month < currentDate.month) ||
        (date.year == currentDate.year && date.month == currentDate.month && date.day < currentDate.day) ||
        (date.year - currentDate.year) > 1 ||
        ((date.year - currentDate.year) == 1 && date.month > currentDate.month) ||
        ((date.year - currentDate.year) == 1 && date.month == currentDate.month && date.day > currentDate.month))
        return 0;

    return 1;
}

time makeTime(char inputTime[6]) {
    char hours[3] = {inputTime[0], inputTime[1], 0};
    char minutes[3] = {inputTime[3], inputTime[4], 0};

    time newTime = {.hour = atoi(hours), .minute = atoi(minutes)};

    return newTime;
}

time getArrTime(time inputTime, time flightTime) {
    time arrTime;

    int aux = inputTime.minute + flightTime.minute;

    arrTime.minute = aux % 60;
    arrTime.hour = (inputTime.hour + flightTime.hour  + aux / 60) % 24;

    return arrTime;
}

void printAirport(airport airport) {
    printf("%s %s %s %d\n", 
        airport.IDA, airport.country, airport.city, airport.nFlights);
}

int findAirport(char IDA[4]) {
    int i = 0;

    for(i = 0; i < nAirpts; i++) 
        if(!strcmp(listAirpts[i].IDA, IDA))
            break;     

    return i;
}

void printFlight(flight flight) {
    printf("%s %s %s ",
        flight.IDV, flight.depIDA, flight.arrIDA);

    printf("%02d-%02d-%04d %02d:%02d %02d:%02d %d\n", 
        flight.depDate.day, flight.depDate.month, flight.depDate.year,
        flight.depTime.hour, flight.depTime.minute, flight.flightTime.hour, flight.flightTime.minute,
        flight.maxPax);  
}

int main() {
    memset(listAirpts, 0, SizeAirpts * sizeof(airport));

    char cmd;

    while (cmd != 'q') {

        cmd = getchar();

        switch(cmd) {
            case 'a':
                command_a();
                eatLine();
                break;
            case 'l':
                command_l();
                break;
            case 'v':
                command_v();
                break;
            case 'p':
                command_p();
                break;
            case 'c':
                command_c();
                break;
            case 't':
                command_T();
                break;

            default:
                eatLine();
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
    else if (findAirport(IDA) != nAirpts)
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
    if(getchar() == '\n') //List all airports
        for(int i = 0; i < nAirpts; i ++)
            printAirport(listAirpts[i]);

    else { //List specific airports
        do {
            char IDA[4] = {0};
            int pos = 0;

            scanf("%s", IDA);
            getchar();

            pos = findAirport(IDA);

            if(pos == nAirpts)
                printf("%s: no such airport ID\n", IDA);
            else
                printAirport(listAirpts[pos]);

        } while (getchar() != '\n');

    }
}


void command_v() {
    if(getchar() == '\n') //List all Flights 
        for(int i = 0; i < nFlights; i++)
            printFlight(listFlights[i]);

    else { //Add a flight
        char IDV[7], depIDA[4] = {0}, arrIDA[4] = {0};
        char inputDate[11], inputTime[6], fTime[6] = {0};
        char pax[4];


        scanf("%s %s %s %s %s %s %s", IDV, depIDA, arrIDA, inputDate, inputTime, fTime, pax);
        getchar();

        date depDate = makeDate(inputDate);
        time depTime = makeTime(inputTime);
        time flightTime = makeTime(fTime);

        int maxPax = atoi(pax);
        
        //validateIDV
        int depAirpt = findAirport(depIDA);
        if(!validateIDV(IDV))
            printf("invalid flight\n");
        else if(depAirpt == nAirpts)
            printf("depIDA: no such airport ID\n");
        else if(findAirport(arrIDA) == nAirpts)
            printf("depIDA: no such airport ID\n");
        else if(nFlights == SizeFlights)
            printf("too many flights\n");
        else if(!valiDate(depDate))
            printf("invalid date\n");
        else if(flightTime.hour > 12 || (flightTime.hour == 12 && flightTime.minute != 0))
            printf("invalid duration\n");
        else if(maxPax < 10 || maxPax > 100)
            printf("invalid capacity\n");
        else { //valid flight!
            flight newFlight;

            strcpy(newFlight.IDV, IDV);
            strcpy(newFlight.depIDA, depIDA);
            strcpy(newFlight.arrIDA, arrIDA);
            newFlight.depDate = depDate;
            newFlight.depTime = depTime;
            newFlight.flightTime = flightTime;

            newFlight.maxPax = maxPax;

            listFlights[nFlights] = newFlight;
            nFlights++;

            listAirpts[depAirpt].nFlights++;
        }
    }
    
}

void command_p() {
    char IDA[4] = {0};

    scanf("%s", IDA);
    getchar();

    if(findAirport(IDA) == nAirpts)
        printf("%s: no such airport ID\n", IDA);
    else {
        for(int i = 0; i < nFlights; i++) {
            if(!strcmp(listFlights[i].depIDA, IDA))
                printf("%s %s %02d-%02d-%04d %02d:%02d\n", 
                    listFlights[i].IDV, listFlights[i].arrIDA,
                    listFlights[i].depDate.day, listFlights[i].depDate.month, listFlights[i].depDate.year,
                    listFlights[i].depTime.hour, listFlights[i].depTime.minute);            
        }
    }
}

void command_c() {
    char IDA[4] = {0};

    scanf("%s", IDA);
    getchar();

    if(findAirport(IDA) == nAirpts)
        printf("%s: no such airport ID\n", IDA);
    else 
        for(int i = 0; i < nFlights; i++)
            if(!strcmp(listFlights[i].depIDA, IDA)) {
                time arrTime = getArrTime(listFlights[i].depTime, listFlights[i].flightTime);
                date arrDate = getArrDate(listFlights[i].depDate, arrTime, listFlights[i].flightTime);

                printf("%s %s %02d-%02d-%04d %02d:%02d\n", 
                    listFlights[i].IDV, listFlights[i].arrIDA,
                    arrDate.day, arrDate.month, arrDate.year,
                    arrTime.hour, arrTime.minute);            
            }

        
}

void command_T() {
    char inputDate[11] = {0};

    getchar();
    scanf("%s", inputDate);

    date newDate = makeDate(inputDate); 
    
    if(valiDate(newDate)) {
        currentDate = newDate;
        printDate(currentDate);
    } else
        printf("invalid date\n");

    getchar();
}