/*
    ============================================================
    AIRCRAFT SEAT BOOKING AND MANAGEMENT SYSTEM
    Data Structure and Algorithm (Lab) - SZABIST
    Console application in C++ (Dev-C++ 5.11 compatible)

    Data structures used:
      - Two-dimensional array : aircraft seat layout (30 x 6)
      - Linked list           : booking records
      - Stack (linked nodes)  : undo last seat selection (LIFO)
      - Queue (linked nodes)  : waiting list (FIFO)

    NOTE: The project report shows only parts of some long
    functions. The parts shown in the report are used as they
    are. The remaining parts (marked "rebuilt") were written in
    the same style, following the report's description and its
    sample outputs.
    ============================================================
*/

#include <iostream>
#include <iomanip>
#include <string>
#include <sstream>
#include <cstdlib>
#include <cctype>

using namespace std;

/* =========================
   CONSTANTS
   ========================= */
const int ROWS = 30;
const int COLS = 6;
const int MAX_SELECTIONS = 20;

/* =========================
   STRUCTURES
   ========================= */
struct Seat
{
    string seatNumber;
    string type;

    bool booked;
    bool selected;
    bool bulkhead;
    bool exitRow;

    double price;
};

struct Booking
{
    int bookingID;
    string passengerName;
    string phone;
    string seatNumber;
    double fare;

    Booking* next;
};

struct StackNode
{
    string seatNumber;
    StackNode* next;
};

/* =========================
   QUEUE NODE
   ========================= */
struct QueueNode
{
    string passengerName;
    string phone;

    QueueNode* next;
};

/* =========================
   GLOBAL DATA
   ========================= */
Seat aircraft[ROWS][COLS];

Booking* bookingHead = NULL;

StackNode* stackTop = NULL;

QueueNode* queueFront = NULL;
QueueNode* queueRear = NULL;

int nextBookingID = 1001;

/* =========================
   FUNCTION PROTOTYPES
   ========================= */
void clearScreen();
void pauseScreen();

string getSeatType(int col);
double getSeatPrice(int row, int col);
void initializeAircraft();

int getRowFromSeat(string seatNumber);
int getColFromSeat(string seatNumber);
bool validSeatNumber(string seatNumber);

void displayAircraft();
void showSeatInfo(int row, int col);
void displaySeatDetails();
void searchSeat();
void filterSeats();

void pushStack(string seatNumber);
string popStack();
bool isStackEmpty();
void undoSelection();
void cancelAllSelections();

void selectSeats();
void bookSelectedSeats();
void addBooking(string passengerName, string phone,
                string seatNumber, double fare);
void displayBookings();
void cancelBooking();

void addToWaitingList();
void displayWaitingList();
void processWaitingList();

void showStatistics();
void mainMenu();

/* =========================
   HELPER FUNCTIONS (rebuilt)
   clearScreen() uses cls on Windows and clear on other systems.
   ========================= */
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseScreen()
{
    cout << "\nPress Enter to continue...";
    cin.ignore(10000, '\n');
    cin.get();
}

/* =========================
   AIRCRAFT INITIALISATION
   ========================= */

/* columns 0 and 5 = Window, 2 and 3 = Aisle, the rest = Middle (rebuilt) */
string getSeatType(int col)
{
    if (col == 0 || col == 5)
        return "Window";

    if (col == 2 || col == 3)
        return "Aisle";

    return "Middle";
}

double getSeatPrice(int row, int col)
{
    double price = 100.0;

    if (col == 0 || col == 5)
        price += 20.0;

    if (col == 2 || col == 3)
        price += 15.0;

    if (row == 1)
        price += 50.0;

    if (row == 12 || row == 13)
        price += 60.0;

    return price;
}

void initializeAircraft()
{
    char seatLetters[COLS] = {'A', 'B', 'C', 'D', 'E', 'F'};

    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            int rowNumber = i + 1;

            /*
               Dev-C++ 5.11 may not support to_string().
               stringstream is used instead.
            */
            stringstream ss;
            ss << rowNumber << seatLetters[j];

            aircraft[i][j].seatNumber = ss.str();

            aircraft[i][j].type = getSeatType(j);

            aircraft[i][j].booked = false;
            aircraft[i][j].selected = false;

            /* Row 1 = Bulkhead */
            if (rowNumber == 1)
                aircraft[i][j].bulkhead = true;
            else
                aircraft[i][j].bulkhead = false;

            /* Rows 12 and 13 = Exit Rows */
            if (rowNumber == 12 || rowNumber == 13)
                aircraft[i][j].exitRow = true;
            else
                aircraft[i][j].exitRow = false;

            aircraft[i][j].price = getSeatPrice(rowNumber, j);
        }
    }
}

/* =========================
   READING AND CHECKING SEAT NUMBERS
   ========================= */
int getRowFromSeat(string seatNumber)
{
    if (seatNumber.length() < 2)
        return -1;

    string rowPart =
        seatNumber.substr(0, seatNumber.length() - 1);

    unsigned int i;

    for (i = 0; i < rowPart.length(); i++)
    {
        if (!isdigit((unsigned char)rowPart[i]))
            return -1;
    }

    int row = atoi(rowPart.c_str());

    if (row < 1 || row > ROWS)
        return -1;

    return row - 1;
}

/* reads the last character, converts it to upper case, A-F -> 0-5 (rebuilt) */
int getColFromSeat(string seatNumber)
{
    if (seatNumber.length() < 2)
        return -1;

    char letter = (char)toupper((unsigned char)seatNumber[seatNumber.length() - 1]);

    if (letter >= 'A' && letter <= 'F')
        return letter - 'A';

    return -1;
}

bool validSeatNumber(string seatNumber)
{
    if (getRowFromSeat(seatNumber) != -1 &&
        getColFromSeat(seatNumber) != -1)
        return true;

    return false;
}

/* =========================
   DISPLAYING THE SEAT MAP
   ========================= */
void displayAircraft()
{
    int i, j;

    clearScreen();

    cout << "============================================================\n";
    cout << "                     AIRCRAFT SEAT MAP\n";
    cout << "============================================================\n\n";
    cout << "                       FRONT / COCKPIT\n\n";
    cout << "           A      B      C         D      E      F\n\n";

    for (i = 0; i < ROWS; i++)
    {
        cout << "Row " << setw(2) << i + 1 << "  ";

        for (j = 0; j < COLS; j++)
        {
            if (aircraft[i][j].booked)
            {
                cout << "[XX]   ";
            }
            else if (aircraft[i][j].selected)
            {
                cout << "[SS]   ";
            }
            else
            {
                cout << "[" << aircraft[i][j].seatNumber << "]   ";
            }

            /* Aircraft aisle */
            if (j == 2)
                cout << "  ";
        }

        if (aircraft[i][0].bulkhead)
            cout << " <-- BULKHEAD";

        if (aircraft[i][0].exitRow)
            cout << " <-- EXIT ROW";

        cout << "\n";
    }

    cout << "\n============================================================\n";
    cout << "XX = BOOKED | SS = SELECTED | [12A] = AVAILABLE\n";
    cout << "============================================================\n";
}

/* =========================
   SEAT DETAILS AND SEARCH (rebuilt)
   Direct array access, no loop needed.
   ========================= */
void showSeatInfo(int row, int col)
{
    cout << "\n========================================\n";
    cout << "            SEAT INFORMATION\n";
    cout << "========================================\n";
    cout << "Seat Number : " << aircraft[row][col].seatNumber << "\n";
    cout << "Row         : " << row + 1 << "\n";
    cout << "Seat Type   : " << aircraft[row][col].type << "\n";
    cout << "Price       : $" << fixed << setprecision(2)
         << aircraft[row][col].price << "\n";

    if (aircraft[row][col].bulkhead)
        cout << "Special     : Bulkhead\n";

    if (aircraft[row][col].exitRow)
        cout << "Special     : Exit Row\n";

    cout << "Status      : ";

    if (aircraft[row][col].booked)
        cout << "BOOKED\n";
    else if (aircraft[row][col].selected)
        cout << "SELECTED\n";
    else
        cout << "AVAILABLE\n";

    cout << "========================================\n";
}

void displaySeatDetails()
{
    string seatNumber;

    cout << "\nEnter seat number (example 12A): ";
    cin >> seatNumber;

    if (!validSeatNumber(seatNumber))
    {
        cout << "Invalid seat number!\n";
        pauseScreen();
        return;
    }

    int row = getRowFromSeat(seatNumber);
    int col = getColFromSeat(seatNumber);

    showSeatInfo(row, col);

    pauseScreen();
}

void searchSeat()
{
    string seatNumber;

    cout << "\nEnter seat number to search (example 12A): ";
    cin >> seatNumber;

    if (!validSeatNumber(seatNumber))
    {
        cout << "Invalid seat number!\n";
        pauseScreen();
        return;
    }

    int row = getRowFromSeat(seatNumber);
    int col = getColFromSeat(seatNumber);

    showSeatInfo(row, col);

    /* if the seat is booked, show who booked it */
    if (aircraft[row][col].booked)
    {
        Booking* temp = bookingHead;

        while (temp != NULL)
        {
            if (temp->seatNumber == aircraft[row][col].seatNumber)
            {
                cout << "Booking ID  : " << temp->bookingID << "\n";
                cout << "Passenger   : " << temp->passengerName << "\n";
                cout << "Phone       : " << temp->phone << "\n";
                cout << "========================================\n";
                break;
            }

            temp = temp->next;
        }
    }

    pauseScreen();
}

/* =========================
   FILTER SEATS
   Traverses the whole array and prints every seat that is
   not booked and matches the chosen category.
   ========================= */
void filterSeats()
{
    int choice;
    int i, j;
    int found = 0;

    cout << "\n========================================\n";
    cout << "              FILTER SEATS\n";
    cout << "========================================\n";
    cout << "1. Window\n";
    cout << "2. Middle\n";
    cout << "3. Aisle\n";
    cout << "4. Bulkhead\n";
    cout << "5. Exit Row\n";
    cout << "\nEnter choice: ";
    cin >> choice;

    if (choice < 1 || choice > 5)
    {
        cout << "\nInvalid choice!\n";
        pauseScreen();
        return;
    }

    cout << "\nAvailable seats:\n";

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            Seat &seat = aircraft[i][j];

            if (seat.booked)
                continue;

            bool match = false;

            if (choice == 1 && seat.type == "Window")
                match = true;

            if (choice == 2 && seat.type == "Middle")
                match = true;

            if (choice == 3 && seat.type == "Aisle")
                match = true;

            if (choice == 4 && seat.bulkhead)
                match = true;

            if (choice == 5 && seat.exitRow)
                match = true;

            if (match)
            {
                cout << seat.seatNumber << " - " << seat.type
                     << " - $" << fixed << setprecision(2)
                     << seat.price << "\n";
                found++;
            }
        }
    }

    if (found == 0)
        cout << "No seats match this category.\n";
    else
        cout << "\nTotal seats found: " << found << "\n";

    pauseScreen();
}

/* =========================
   STACK (linked nodes)
   ========================= */
void pushStack(string seatNumber)
{
    StackNode* newNode = new StackNode;

    newNode->seatNumber = seatNumber;
    newNode->next = stackTop;

    stackTop = newNode;
}

/* ============================================================
   STACK POP
   ============================================================ */

string popStack()
{
    if (stackTop == NULL)
        return "";

    StackNode* temp = stackTop;

    string seatNumber = temp->seatNumber;

    stackTop = stackTop->next;

    delete temp;

    return seatNumber;
}

bool isStackEmpty()
{
    return (stackTop == NULL);
}

void undoSelection()
{
    if (isStackEmpty())
    {
        cout << "\nThere is no selected seat to undo.\n";
        pauseScreen();
        return;
    }

    string seatNumber = popStack();

    int row = getRowFromSeat(seatNumber);
    int col = getColFromSeat(seatNumber);

    if (row != -1 && col != -1)
        aircraft[row][col].selected = false;

    cout << "\nUndo successful!\n";
    cout << "Seat " << seatNumber
         << " is available again.\n";

    pauseScreen();
}

/* pops the stack until it is empty and clears every selected flag (rebuilt) */
void cancelAllSelections()
{
    while (!isStackEmpty())
    {
        string seatNumber = popStack();

        int row = getRowFromSeat(seatNumber);
        int col = getColFromSeat(seatNumber);

        if (row != -1 && col != -1)
            aircraft[row][col].selected = false;
    }
}

/* =========================
   SEAT SELECTION
   ========================= */
void selectSeats()
{
    int numberOfSeats = 0;
    int i;
    int choice;
    double total = 0.0;

    displayAircraft();

    cout << "\nHow many seats do you want to select (1-"
         << MAX_SELECTIONS << ")? ";
    cin >> numberOfSeats;

    if (numberOfSeats < 1 || numberOfSeats > MAX_SELECTIONS)
    {
        cout << "\nInvalid number of seats!\n";
        pauseScreen();
        return;
    }

    for (i = 0; i < numberOfSeats; i++)
    {
        string seatNumber;

        cout << "\nEnter seat " << i + 1 << ": ";
        cin >> seatNumber;

        if (!validSeatNumber(seatNumber))
        {
            cout << "Invalid seat number!\n";
            i--;
            continue;
        }

        int row = getRowFromSeat(seatNumber);
        int col = getColFromSeat(seatNumber);

        if (aircraft[row][col].booked)
        {
            cout << "This seat is already BOOKED!\n";
            i--;
            continue;
        }

        if (aircraft[row][col].selected)
        {
            cout << "This seat is already SELECTED!\n";
            i--;
            continue;
        }

        aircraft[row][col].selected = true;

        /* store the standard seat number (for example 12a becomes 12A) */
        seatNumber = aircraft[row][col].seatNumber;

        pushStack(seatNumber);

        total += aircraft[row][col].price;

        cout << "Seat " << seatNumber
             << " selected successfully.\n";
    }

    /* confirm / undo / cancel all (rebuilt) */
    while (true)
    {
        cout << "\n========================================\n";
        cout << "Total Selected Fare: $" << fixed << setprecision(2)
             << total << "\n";
        cout << "========================================\n";

        cout << "\n1. Confirm Booking\n";
        cout << "2. Undo Last Selection\n";
        cout << "3. Cancel All Selections\n";
        cout << "\nEnter choice: ";
        cin >> choice;

        if (choice == 1)
        {
            bookSelectedSeats();
            return;
        }
        else if (choice == 2)
        {
            string seatNumber = popStack();

            int row = getRowFromSeat(seatNumber);
            int col = getColFromSeat(seatNumber);

            if (row != -1 && col != -1)
            {
                aircraft[row][col].selected = false;
                total -= aircraft[row][col].price;
            }

            cout << "\nUndo successful!\n";
            cout << "Seat " << seatNumber
                 << " is available again.\n";

            if (isStackEmpty())
            {
                cout << "\nNo seats are selected now.\n";
                pauseScreen();
                return;
            }
        }
        else if (choice == 3)
        {
            cancelAllSelections();

            cout << "\nAll selections cancelled.\n";
            pauseScreen();
            return;
        }
        else
        {
            cout << "\nInvalid choice!\n";
        }
    }
}

/* =========================
   BOOKING RECORDS (linked list)
   ========================= */
void bookSelectedSeats()
{
    string passengerName;
    string phone;
    string selectedSeats[MAX_SELECTIONS];

    int count = 0;
    int i;
    double totalFare = 0.0;

    /* remove the newline left in the buffer by cin >> */
    cin.ignore();

    cout << "\nEnter passenger name: ";
    getline(cin, passengerName);

    cout << "Enter phone number: ";
    cin >> phone;

    while (!isStackEmpty() && count < MAX_SELECTIONS)
    {
        string seatNumber = popStack();

        selectedSeats[count] = seatNumber;

        int row = getRowFromSeat(seatNumber);
        int col = getColFromSeat(seatNumber);

        totalFare += aircraft[row][col].price;

        count++;
    }

    cout << "\n============================================\n";
    cout << "               BOOKING SUMMARY\n";
    cout << "============================================\n";
    cout << "Passenger : " << passengerName << "\n";
    cout << "Phone     : " << phone << "\n";
    cout << "\nSelected Seats:\n";

    for (i = 0; i < count; i++)
    {
        int row = getRowFromSeat(selectedSeats[i]);
        int col = getColFromSeat(selectedSeats[i]);

        aircraft[row][col].selected = false;
        aircraft[row][col].booked = true;

        cout << selectedSeats[i]
             << " - "
             << aircraft[row][col].type
             << " - $"
             << fixed << setprecision(2)
             << aircraft[row][col].price
             << "\n";

        addBooking(
            passengerName,
            phone,
            selectedSeats[i],
            aircraft[row][col].price
        );
    }

    cout << "\nTOTAL FARE: $" << fixed << setprecision(2)
         << totalFare << "\n";
    cout << "============================================\n";
    cout << "Booking completed successfully!\n";

    pauseScreen();
}

void addBooking(string passengerName,
                string phone,
                string seatNumber,
                double fare)
{
    Booking* newBooking = new Booking;

    newBooking->bookingID = nextBookingID++;
    newBooking->passengerName = passengerName;
    newBooking->phone = phone;
    newBooking->seatNumber = seatNumber;
    newBooking->fare = fare;
    newBooking->next = NULL;

    if (bookingHead == NULL)
    {
        bookingHead = newBooking;
    }
    else
    {
        Booking* temp = bookingHead;

        while (temp->next != NULL)
        {
            temp = temp->next;
        }

        temp->next = newBooking;
    }
}

/* traverses the list from head to tail and prints every record (rebuilt) */
void displayBookings()
{
    if (bookingHead == NULL)
    {
        cout << "\nNo bookings available.\n";
        pauseScreen();
        return;
    }

    Booking* temp = bookingHead;

    cout << "\n============================================================\n";
    cout << "                      BOOKING RECORDS\n";
    cout << "============================================================\n\n";

    while (temp != NULL)
    {
        cout << "Booking ID : " << temp->bookingID << "\n";
        cout << "Passenger  : " << temp->passengerName << "\n";
        cout << "Phone      : " << temp->phone << "\n";
        cout << "Seat       : " << temp->seatNumber << "\n";
        cout << "Fare       : $" << fixed << setprecision(2)
             << temp->fare << "\n";
        cout << "--------------------------------------------------\n";

        temp = temp->next;
    }

    pauseScreen();
}

/* =========================
   BOOKING CANCELLATION
   ========================= */
void cancelBooking()
{
    int id;

    if (bookingHead == NULL)
    {
        cout << "\nNo bookings available.\n";
        pauseScreen();
        return;
    }

    cout << "\nEnter booking ID to cancel: ";
    cin >> id;

    Booking* current = bookingHead;
    Booking* previous = NULL;

    while (current != NULL &&
           current->bookingID != id)
    {
        previous = current;
        current = current->next;
    }

    if (current == NULL)
    {
        cout << "\nBooking not found.\n";
        pauseScreen();
        return;
    }

    string seatNumber = current->seatNumber;

    int row = getRowFromSeat(seatNumber);
    int col = getColFromSeat(seatNumber);

    if (row != -1 && col != -1)
        aircraft[row][col].booked = false;

    if (previous == NULL)
    {
        bookingHead = current->next;
    }
    else
    {
        previous->next = current->next;
    }

    cout << "\n============================================\n";
    cout << "BOOKING CANCELLED\n";
    cout << "============================================\n";
    cout << "Passenger : " << current->passengerName << "\n";
    cout << "Seat      : " << seatNumber << "\n";

    delete current;

    /* a waiting passenger can take a free seat immediately */
    processWaitingList();

    pauseScreen();
}

/* =========================
   WAITING LIST (queue)
   ========================= */
void addToWaitingList()
{
    string name;
    string phone;

    /* remove the newline left in the buffer by cin >> */
    cin.ignore();

    cout << "\nEnter passenger name: ";
    getline(cin, name);

    cout << "Enter phone number: ";
    cin >> phone;

    QueueNode* newNode = new QueueNode;

    newNode->passengerName = name;
    newNode->phone = phone;
    newNode->next = NULL;

    if (queueRear == NULL)
    {
        queueFront = newNode;
        queueRear = newNode;
    }
    else
    {
        queueRear->next = newNode;
        queueRear = newNode;
    }

    cout << "\nPassenger added to waiting list.\n";

    pauseScreen();
}

void displayWaitingList()
{
    if (queueFront == NULL)
    {
        cout << "\nThe waiting list is empty.\n";
        pauseScreen();
        return;
    }

    QueueNode* temp = queueFront;
    int number = 1;

    cout << "\n============================================\n";
    cout << "                WAITING LIST\n";
    cout << "============================================\n\n";

    while (temp != NULL)
    {
        cout << number << ". " << temp->passengerName
             << " | " << temp->phone << "\n";

        number++;
        temp = temp->next;
    }

    pauseScreen();
}

void processWaitingList()
{
    if (queueFront == NULL)
        return;

    int i, j;
    int freeRow = -1;
    int freeCol = -1;

    /* look for the first free seat, scanning from row 1 */
    for (i = 0; i < ROWS && freeRow == -1; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            if (!aircraft[i][j].booked && !aircraft[i][j].selected)
            {
                freeRow = i;
                freeCol = j;
                break;
            }
        }
    }

    if (freeRow == -1)
        return;

    /* dequeue the passenger at the front */
    QueueNode* passenger = queueFront;

    queueFront = queueFront->next;

    if (queueFront == NULL)
        queueRear = NULL;

    string seatNumber =
        aircraft[freeRow][freeCol].seatNumber;

    double fare =
        aircraft[freeRow][freeCol].price;

    aircraft[freeRow][freeCol].booked = true;

    cout << "\n============================================\n";
    cout << "WAITING LIST PROCESSING\n";
    cout << "============================================\n";
    cout << "Passenger : " << passenger->passengerName << "\n";
    cout << "Seat      : " << seatNumber << "\n";
    cout << "Fare      : $" << fixed << setprecision(2) << fare << "\n";

    addBooking(
        passenger->passengerName,
        passenger->phone,
        seatNumber,
        fare
    );

    cout << "Passenger has been assigned the available seat.\n";

    delete passenger;
}

/* =========================
   FLIGHT STATISTICS
   ========================= */
void showStatistics()
{
    int bookedSeats = 0;
    int availableSeats = 0;
    int windowSeats = 0;
    int middleSeats = 0;
    int aisleSeats = 0;
    int bulkheadSeats = 0;
    int exitSeats = 0;
    int waitingPassengers = 0;
    double revenue = 0.0;

    int i, j;

    for (i = 0; i < ROWS; i++)
    {
        for (j = 0; j < COLS; j++)
        {
            Seat &seat = aircraft[i][j];

            if (seat.booked)
            {
                bookedSeats++;
                revenue += seat.price;
            }
            else
            {
                availableSeats++;
            }

            if (seat.type == "Window")
                windowSeats++;
            else if (seat.type == "Middle")
                middleSeats++;
            else
                aisleSeats++;

            if (seat.bulkhead)
                bulkheadSeats++;

            if (seat.exitRow)
                exitSeats++;
        }
    }

    /* traverse the queue to count waiting passengers */
    QueueNode* temp = queueFront;

    while (temp != NULL)
    {
        waitingPassengers++;
        temp = temp->next;
    }

    cout << "\n============================================\n";
    cout << "              FLIGHT STATISTICS\n";
    cout << "============================================\n";
    cout << "Total Seats        : " << ROWS * COLS << "\n";
    cout << "Booked Seats       : " << bookedSeats << "\n";
    cout << "Available Seats    : " << availableSeats << "\n";
    cout << "\nSeat Categories:\n";
    cout << "Window Seats       : " << windowSeats << "\n";
    cout << "Middle Seats       : " << middleSeats << "\n";
    cout << "Aisle Seats        : " << aisleSeats << "\n";
    cout << "Bulkhead Seats     : " << bulkheadSeats << "\n";
    cout << "Exit Row Seats     : " << exitSeats << "\n";
    cout << "\nTotal Revenue      : $" << fixed << setprecision(2)
         << revenue << "\n";
    cout << "Waiting Passengers : " << waitingPassengers << "\n";
    cout << "============================================\n";

    pauseScreen();
}

/* =========================
   MAIN MENU
   A do ... while loop with a switch statement.
   The loop stops when the user enters 12.
   ========================= */
void mainMenu()
{
    int choice;

    do
    {
        clearScreen();

        cout << "============================================\n";
        cout << "   AIRCRAFT SEAT BOOKING AND MANAGEMENT\n";
        cout << "============================================\n";
        cout << " 1. View Aircraft Seat Map\n";
        cout << " 2. View Seat Details\n";
        cout << " 3. Select / Book Seats\n";
        cout << " 4. Undo Last Seat Selection\n";
        cout << " 5. Search Seat\n";
        cout << " 6. Filter Seats\n";
        cout << " 7. View Booking Records\n";
        cout << " 8. Cancel Booking\n";
        cout << " 9. Add Passenger to Waiting List\n";
        cout << "10. View Waiting List\n";
        cout << "11. Flight Statistics\n";
        cout << "12. Exit\n";
        cout << "============================================\n";
        cout << "Enter your choice: ";

        cin >> choice;

        switch (choice)
        {
            case 1:
                displayAircraft();
                pauseScreen();
                break;

            case 2:
                displaySeatDetails();
                break;

            case 3:
                selectSeats();
                break;

            case 4:
                undoSelection();
                break;

            case 5:
                searchSeat();
                break;

            case 6:
                filterSeats();
                break;

            case 7:
                displayBookings();
                break;

            case 8:
                cancelBooking();
                break;

            case 9:
                addToWaitingList();
                break;

            case 10:
                displayWaitingList();
                break;

            case 11:
                showStatistics();
                break;

            case 12:
                cout << "\nThank you for using the system. Goodbye!\n";
                break;

            default:
                cout << "\nInvalid choice! Please enter 1 to 12.\n";
                pauseScreen();
                break;
        }

    } while (choice != 12);
}

/* =========================
   MAIN
   ========================= */
int main()
{
    initializeAircraft();

    mainMenu();

    return 0;
}
