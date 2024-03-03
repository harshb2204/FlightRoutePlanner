#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_AIRPORTS 100
#define MAX_FLIGHTS 1000

bool isNumeric(const char *str)
{
    while (*str)
    {
        if (*str < '0' || *str > '9')
            return false;
        str++;
    }
    return true;
}

bool containsDigits(const char *str)
{
    while (*str)
    {
        if (*str >= '0' && *str <= '9')
            return true;
        str++;
    }
    return false;
}

typedef struct Airport
{
    char code[4];
    char name[50];
} Airport;

typedef struct Flight
{
    char flightNumber[10];
    Airport departureAirport;
    Airport arrivalAirport;
    char departureTime[10];
    char arrivalTime[10];
    int cost;
} Flight;

typedef struct Graph
{
    int numAirports;
    Airport airports[MAX_AIRPORTS];
    int numFlights;
    Flight flights[MAX_FLIGHTS];
} Graph;

Graph *createGraph();
void addAirport(Graph *graph);
void addFlight(Graph *graph);
void displayRoutes(Graph *graph);
int isAlphaNumeric(const char *str);

int main()
{
    Graph *graph = createGraph();

    int choice;
    do
    {
        printf("\nFlight Route Planner Menu:\n");
        printf("1. Add Airport\n");
        printf("2. Add Flight\n");
        printf("3. Display Routes\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            addAirport(graph);
            break;
        case 2:
            addFlight(graph);
            break;
        case 3:
            displayRoutes(graph);
            break;
        case 4:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 4);

    // Free allocated memory
    free(graph);

    return 0;
}

Graph *createGraph()
{
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    if (graph == NULL)
    {
        printf("Memory allocation failed.\n");
        exit(1);
    }
    graph->numAirports = 0;
    graph->numFlights = 0;
    return graph;
}

void addAirport(Graph *graph)
{
    if (graph->numAirports >= MAX_AIRPORTS)
    {
        printf("Maximum number of airports reached.\n");
        return;
    }

    Airport newAirport;
    printf("Enter airport code (3 characters): ");
    scanf("%3s", newAirport.code);

    if (strlen(newAirport.code) != 3)
    {
        printf("Airport code must be exactly 3 characters long. Please try again.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    if (!isNumeric(newAirport.code))
    {
        printf("Airport code cannot contain non-numeric characters. Please try again.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    printf("Enter airport name: ");
    scanf(" %[^\n]", newAirport.name);

    if (containsDigits(newAirport.name))
    {
        printf("Airport name cannot contain numbers. Please try again.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    graph->airports[graph->numAirports++] = newAirport;
    printf("Airport added successfully.\n");
}

void addFlight(Graph *graph)
{
    if (graph->numAirports < 2)
    {
        printf("There must be at least two airports to add a flight.\n");
        return;
    }

    Flight newFlight;
    printf("Enter flight number: ");
    scanf("%9s", newFlight.flightNumber);

    printf("Enter departure airport code (3 characters): ");
    scanf("%3s", newFlight.departureAirport.code);

    printf("Enter arrival airport code (3 characters): ");
    scanf("%3s", newFlight.arrivalAirport.code);

    printf("Enter departure time: ");
    scanf("%9s", newFlight.departureTime);
    printf("Enter arrival time: ");
    scanf("%9s", newFlight.arrivalTime);

    printf("Enter cost: ");
    if (scanf("%d", &newFlight.cost) != 1)
    {
        printf("Invalid input for cost. Please enter a numeric value.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    graph->flights[graph->numFlights++] = newFlight;
    printf("Flight added successfully.\n");
}

void displayRoutes(Graph *graph)
{
    if (graph->numFlights == 0)
    {
        printf("No flights available.\n");
        return;
    }

    printf("Available flights:\n");
    for (int i = 0; i < graph->numFlights; ++i)
    {
        Flight flight = graph->flights[i];

        char departureAirportName[50];
        for (int j = 0; j < graph->numAirports; ++j)
        {
            if (strcmp(flight.departureAirport.code, graph->airports[j].code) == 0)
            {
                strcpy(departureAirportName, graph->airports[j].name);
                break;
            }
        }

        char arrivalAirportName[50];
        for (int j = 0; j < graph->numAirports; ++j)
        {
            if (strcmp(flight.arrivalAirport.code, graph->airports[j].code) == 0)
            {
                strcpy(arrivalAirportName, graph->airports[j].name);
                break;
            }
        }

        printf("%s: %s (%s) -> %s (%s), Cost: $%d\n", flight.flightNumber,
               departureAirportName, flight.departureTime,
               arrivalAirportName, flight.arrivalTime,
               flight.cost);
    }
}

int isAlphaNumeric(const char *str)
{
    while (*str)
    {
        if (!isalpha(*str))
            return 1;
        str++;
    }
    return 0;
}
