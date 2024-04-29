#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#define MAX_AIRPORTS 100
#define MAX_FLIGHTS 1000
#define CUSTOM_INT_MAX 24 * 60

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

typedef struct Time
{
    int hours;
    int minutes;
} Time;

typedef struct Flight
{
    char flightNumber[10];
    Airport departureAirport;
    Airport arrivalAirport;
    Time departureTime; // Departure time in HH:MM format
    Time arrivalTime;   // Arrival time in HH:MM format
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
void findFastestFlight(Graph *graph);
void dijkstra(Graph *graph, const char *source, const char *destination);
int isAlphaNumeric(const char *str);
int timeToMinutes(Time time);

void MaddAirport(Graph *graph, Airport airport);
void MaddFlight(Graph *graph, Flight flight);

void findFastestFlight(Graph *graph)
{
    char source[4];
    char destination[4];
    printf("Enter source airport code (3 characters): ");
    scanf("%3s", source);
    printf("Enter destination airport code (3 characters): ");
    scanf("%3s", destination);

    // Find the index of the source and destination airports
    int sourceIndex = -1, destIndex = -1;
    for (int i = 0; i < graph->numAirports; ++i)
    {
        if (strcmp(graph->airports[i].code, source) == 0)
        {
            sourceIndex = i;
        }
        if (strcmp(graph->airports[i].code, destination) == 0)
        {
            destIndex = i;
        }
        if (sourceIndex != -1 && destIndex != -1)
        {
            break;
        }
    }

    if (sourceIndex == -1 || destIndex == -1)
    {
        printf("Source or destination airport not found.\n");
        return;
    }

    dijkstra(graph, graph->airports[sourceIndex].code, graph->airports[destIndex].code);
}

void searchFlights(Graph *graph, const char *departure, const char *arrival)
{
    printf("Flights from %s to %s:\n", departure, arrival);
    bool found = false;
    for (int i = 0; i < graph->numFlights; ++i)
    {
        Flight flight = graph->flights[i];
        if (strcmp(flight.departureAirport.code, departure) == 0 &&
            strcmp(flight.arrivalAirport.code, arrival) == 0)
        {
            printf("%s: Departure %02d:%02d, Arrival %02d:%02d, Cost $%d\n",
                   flight.flightNumber, flight.departureTime.hours, flight.departureTime.minutes,
                   flight.arrivalTime.hours, flight.arrivalTime.minutes, flight.cost);
            found = true;
        }
    }
    if (!found)
        printf("No flights found for the specified route.\n");
}

void removeFlight(Graph *graph, const char *flightNumber)
{
    int index = -1;
    for (int i = 0; i < graph->numFlights; ++i)
    {
        if (strcmp(graph->flights[i].flightNumber, flightNumber) == 0)
        {
            index = i;
            break;
        }
    }
    if (index != -1)
    {
        for (int i = index; i < graph->numFlights - 1; ++i)
        {
            graph->flights[i] = graph->flights[i + 1];
        }
        graph->numFlights--;
        printf("Flight %s removed successfully.\n", flightNumber);
    }
    else
    {
        printf("Flight %s not found.\n", flightNumber);
    }
}

void displayAirportSchedule(Graph *graph, const char *airportCode)
{
    printf("Flight Schedule for Airport %s:\n", airportCode);
    bool found = false;
    for (int i = 0; i < graph->numFlights; ++i)
    {
        Flight flight = graph->flights[i];
        if (strcmp(flight.departureAirport.code, airportCode) == 0 ||
            strcmp(flight.arrivalAirport.code, airportCode) == 0)
        {
            char departureAirportName[50];
            char arrivalAirportName[50];
            for (int j = 0; j < graph->numAirports; ++j)
            {
                if (strcmp(flight.departureAirport.code, graph->airports[j].code) == 0)
                {
                    strcpy(departureAirportName, graph->airports[j].name);
                }
                if (strcmp(flight.arrivalAirport.code, graph->airports[j].code) == 0)
                {
                    strcpy(arrivalAirportName, graph->airports[j].name);
                }
            }
            printf("%s: %s (%02d:%02d) -> %s (%02d:%02d), Cost: $%d\n", flight.flightNumber,
                   departureAirportName, flight.departureTime.hours, flight.departureTime.minutes,
                   arrivalAirportName, flight.arrivalTime.hours, flight.arrivalTime.minutes,
                   flight.cost);
            found = true;
        }
    }
    if (!found)
        printf("No flights found for the specified airport.\n");
}

void MaddAirport(Graph *graph, Airport airport)
{
    if (graph->numAirports >= MAX_AIRPORTS)
    {
        printf("Maximum number of airports reached.\n");
        return;
    }

    graph->airports[graph->numAirports++] = airport;
    printf("Airport %s added successfully.\n", airport.code);
}

void MaddFlight(Graph *graph, Flight flight)
{
    if (graph->numFlights >= MAX_FLIGHTS)
    {
        printf("Maximum number of flights reached.\n");
        return;
    }

    graph->flights[graph->numFlights++] = flight;
    printf("Flight %s added successfully.\n", flight.flightNumber);
}

int main()
{

    printf("*********************************************\n");
    printf("*          FLIGHT ROUTE PLANNER             *\n");
    printf("*        SY CS-A BATCH 2 GROUP 11           *\n");
    printf("*******************************************\n\n");
    printf("Welcome to the Flight Route Planner!\n");
    Graph *graph = createGraph();
    Airport airport1 = {"ABC", "Airport 1"};
    Airport airport2 = {"DEF", "Airport 2"};
    Airport airport3 = {"GHI", "Airport 3"};
    Airport airport4 = {"JKL", "Airport 4"};
    MaddAirport(graph, airport1);
    MaddAirport(graph, airport2);
    MaddAirport(graph, airport3);
    MaddAirport(graph, airport4);

    // Add flights
    Flight flight1 = {"F1", airport1, airport2, {8, 0}, {10, 30}, 100};
    Flight flight2 = {"F2", airport1, airport2, {9, 0}, {11, 0}, 120};
    Flight flight3 = {"F3", airport2, airport3, {10, 0}, {12, 0}, 150};
    Flight flight4 = {"F4", airport3, airport1, {12, 0}, {13, 30}, 80};
    Flight flight5 = {"F5", airport1, airport2, {9, 0}, {10, 30}, 100};
    Flight flight6 = {"F6", airport3, airport4, {15, 0}, {18, 30}, 100};
    MaddFlight(graph, flight1);
    MaddFlight(graph, flight2);
    MaddFlight(graph, flight3);
    MaddFlight(graph, flight4);
    MaddFlight(graph, flight5);
    MaddFlight(graph, flight6);

    int choice;
    do
    {
        printf("\n---------------------\n");
        printf("Flight Route Planner Menu:\n");
        printf("---------------------\n");
        printf("1. Add Airport\n");
        printf("2. Add Flight\n");
        printf("3. Display Routes\n");
        printf("4. Remove Flight\n");
        printf("5. Search Flight\n");
        printf("6. Display Airport Schedule\n");
        printf("7. Find Fastest Flight\n");
        printf("8. Find Connecting Flight\n");
        printf("9. Exit\n");
        printf("---------------------\n");
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
        {
            char flightNumber[10];
            printf("Enter flight number to remove: ");
            scanf("%9s", flightNumber);
            removeFlight(graph, flightNumber);
        }
        break;
        case 5:
        {
            char departure[4], arrival[4];
            printf("Enter departure airport code (3 characters): ");
            scanf("%3s", departure);
            printf("Enter arrival airport code (3 characters): ");
            scanf("%3s", arrival);
            searchFlights(graph, departure, arrival);
        }
        break;
        case 6:
        {
            char airportCode[4];
            printf("Enter airport code (3 characters): ");
            scanf("%3s", airportCode);
            displayAirportSchedule(graph, airportCode);
        }
        break;
        case 7:
        {
            findFastestFlight(graph);
        }
        break;
        case 8:
        {
            char source[4];
            char destination[4];
            printf("Enter source airport code (3 characters): ");
            scanf("%3s", source);
            printf("Enter destination airport code (3 characters): ");
            scanf("%3s", destination);
            dijkstra(graph, source, destination);
        }
        break;
        case 9:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 9);

    // Free allocated memory
    free(graph);

    return 0;
}

void dijkstra(Graph *graph, const char *source, const char *destination)
{
    // Array to track visited airports
    bool visited[MAX_AIRPORTS] = {false};

    // Array to track shortest flight duration to each airport
    int dist[MAX_AIRPORTS];
    for (int i = 0; i < MAX_AIRPORTS; ++i)
    {
        dist[i] = CUSTOM_INT_MAX; // Initialize all distances to a large value
    }

    // Array to track previous flight for each airport
    int prevFlight[MAX_AIRPORTS] = {-1};

    // Find the index of source airport
    int sourceIndex = -1;
    for (int i = 0; i < graph->numAirports; ++i)
    {
        if (strcmp(graph->airports[i].code, source) == 0)
        {
            sourceIndex = i;
            break;
        }
    }

    // Handle source airport not found
    if (sourceIndex == -1)
    {
        printf("Source airport not found.\n");
        return;
    }

    // Set distance to source airport as 0
    dist[sourceIndex] = 0;

    // Dijkstra's algorithm
    for (int count = 0; count < graph->numAirports - 1; ++count)
    {
        // Find the airport with the minimum distance that has not been visited
        int minDist = CUSTOM_INT_MAX;
        int minIndex = -1;
        for (int i = 0; i < graph->numAirports; ++i)
        {
            if (!visited[i] && dist[i] < minDist)
            {
                minDist = dist[i];
                minIndex = i;
            }
        }

        // Mark the selected airport as visited
        visited[minIndex] = true;

        // Update distances to adjacent airports through the current airport
        for (int i = 0; i < graph->numFlights; ++i)
        {
            Flight flight = graph->flights[i];
            if (strcmp(flight.departureAirport.code, graph->airports[minIndex].code) == 0)
            {
                int destIndex = -1;
                for (int j = 0; j < graph->numAirports; ++j)
                {
                    if (strcmp(graph->airports[j].code, flight.arrivalAirport.code) == 0)
                    {
                        destIndex = j;
                        break;
                    }
                }
                if (destIndex != -1)
                {
                    int duration = timeToMinutes(flight.arrivalTime) - timeToMinutes(flight.departureTime);
                    if (duration < 0)
                    {
                        duration += 24 * 60;
                    }
                    if (dist[minIndex] != CUSTOM_INT_MAX && dist[minIndex] + duration < dist[destIndex])
                    {
                        dist[destIndex] = dist[minIndex] + duration;
                        prevFlight[destIndex] = i; // Update previous flight for destination airport
                    }
                }
            }
        }
    }

    // Find the index of destination airport
    int destIndex = -1;
    for (int i = 0; i < graph->numAirports; ++i)
    {
        if (strcmp(graph->airports[i].code, destination) == 0)
        {
            destIndex = i;
            break;
        }
    }

    // Handle destination airport not found
    if (destIndex == -1)
    {
        printf("Destination airport not found.\n");
        return;
    }

    // Output the shortest flight duration to the destination airport
    printf("Shortest flight duration from %s to %s: %d minutes\n", source, destination, dist[destIndex]);

    // Print the flight details for the shortest path
    int flightIndex = prevFlight[destIndex];
    int flightPath[MAX_FLIGHTS], pathLength = 0;
    while (flightIndex != -1)
    {
        flightPath[pathLength++] = flightIndex;
        Flight flight = graph->flights[flightIndex];
        int departureIndex = -1;
        for (int i = 0; i < graph->numAirports; ++i)
        {
            if (strcmp(graph->airports[i].code, flight.departureAirport.code) == 0)
            {
                departureIndex = i;
                break;
            }
        }
        flightIndex = prevFlight[departureIndex];
    }

    // Print the flight details in reverse order
    printf("Flight details from %s to %s:\n", source, destination);
    for (int i = pathLength - 1; i >= 0; --i)
    {
        Flight flight = graph->flights[flightPath[i]];
        printf("Flight: %s, Departure: %02d:%02d, Arrival: %02d:%02d, Cost: $%d\n",
               flight.flightNumber, flight.departureTime.hours, flight.departureTime.minutes,
               flight.arrivalTime.hours, flight.arrivalTime.minutes, flight.cost);
    }
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

int timeToMinutes(Time time)
{
    return time.hours * 60 + time.minutes;
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

    printf("Enter departure time (HH:MM): ");
    if (scanf("%d:%d", &newFlight.departureTime.hours, &newFlight.departureTime.minutes) != 2 ||
        newFlight.departureTime.hours < 0 || newFlight.departureTime.hours > 23 ||
        newFlight.departureTime.minutes < 0 || newFlight.departureTime.minutes > 59)
    {
        printf("Invalid input for departure time. Please enter in HH:MM format.\n");
        while (getchar() != '\n')
            ;
        return;
    }

    printf("Enter arrival time (HH:MM): ");
    if (scanf("%d:%d", &newFlight.arrivalTime.hours, &newFlight.arrivalTime.minutes) != 2 ||
        newFlight.arrivalTime.hours < 0 || newFlight.arrivalTime.hours > 23 ||
        newFlight.arrivalTime.minutes < 0 || newFlight.arrivalTime.minutes > 59)
    {
        printf("Invalid input for arrival time. Please enter in HH:MM format.\n");
        while (getchar() != '\n')
            ;
        return;
    }

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
        char arrivalAirportName[50];
        bool departureFound = false;
        bool arrivalFound = false;

        for (int j = 0; j < graph->numAirports; ++j)
        {
            if (!departureFound && strcmp(flight.departureAirport.code, graph->airports[j].code) == 0)
            {
                strcpy(departureAirportName, graph->airports[j].name);
                departureFound = true;
            }
            if (!arrivalFound && strcmp(flight.arrivalAirport.code, graph->airports[j].code) == 0)
            {
                strcpy(arrivalAirportName, graph->airports[j].name);
                arrivalFound = true;
            }
            if (departureFound && arrivalFound)
                break; // Break the loop if both departure and arrival airports are found
        }

        printf("%s: %s (%02d:%02d) -> %s (%02d:%02d), Cost: $%d\n", flight.flightNumber,
               departureAirportName, flight.departureTime.hours, flight.departureTime.minutes,
               arrivalAirportName, flight.arrivalTime.hours, flight.arrivalTime.minutes,
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
