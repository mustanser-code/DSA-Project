# ✈️ Aircraft Seat Booking and Management System

A console-based Aircraft Seat Booking and Management System developed in **C++** for the Data Structures and Algorithms (DSA) Lab at SZABIST.

The project demonstrates the practical implementation of fundamental data structures, including 2D arrays, linked lists, stacks, and queues, through an airline seat booking simulation.

## 📌 Project Overview

The system allows users to view aircraft seats, select and book seats, manage passenger bookings, cancel reservations, maintain a waiting list, and view flight statistics through a menu-driven console interface.

It is designed for educational purposes to demonstrate how data structures can be used to solve real-world management problems.

## ✨ Features

- **Aircraft Seat Map:** Displays all 180 seats in a 30-row, 6-column layout.
- **Seat Details:** Displays seat number, type, price, and booking status.
- **Seat Selection and Booking:** Select multiple seats and confirm reservations.
- **Undo Seat Selection:** Undo the most recent seat selection using a stack.
- **Seat Search:** Search for a seat and view associated booking information.
- **Seat Filtering:** Filter available seats by window, middle, aisle, bulkhead, and exit-row categories.
- **Booking Records:** Store and display passenger details and booking IDs.
- **Booking Cancellation:** Cancel a reservation and release its seat.
- **Waiting List:** Add passengers to a FIFO waiting list.
- **Automatic Waiting List Processing:** Assign an available seat to the next waiting passenger when a booking is cancelled.
- **Flight Statistics:** Display booked and available seats, seat categories, revenue, and waiting passengers.

## 🧰 Technologies Used

- **Programming Language:** C++
- **IDE:** Dev-C++ 5.11
- **Programming Paradigm:** Procedural programming
- **Interface:** Console-based application

## 🧠 Data Structures Implemented

| Data Structure | Purpose |
|---|---|
| 2D Array | Stores the aircraft seat layout and seat information. |
| Linked List | Stores passenger booking records dynamically. |
| Stack | Tracks seat selections and supports undo operations using LIFO. |
| Queue | Maintains the passenger waiting list using FIFO. |

## 💺 Seat Layout and Pricing

The aircraft contains 30 rows and 6 seats per row, for a total of 180 seats.

| Seat Category | Columns | Base Fare |
|---|---|---:|
| Window | A, F | $120 |
| Middle | B, E | $100 |
| Aisle | C, D | $115 |

Additional charges apply to special rows:

- **Row 1:** Additional $50 for bulkhead seats.
- **Rows 12 and 13:** Additional $60 for exit-row seats.

The fare is calculated according to the seat's column and row.

## 🚀 Getting Started

### Prerequisites

- A Windows computer or compatible C++ environment.
- Dev-C++ 5.11 or another compiler that supports the C++ features used in the source code.

### Installation

1. Clone the repository:

   ```bash
   git clone https://github.com/mustanser-code/aircraft-seat-booking-system.git
   ```

2. Open the project folder.

3. Open the C++ source file in Dev-C++ or your preferred C++ IDE.

4. Compile and run the program.

### Compile Using GCC

If GCC is installed, compile the source file with:

```bash
g++ main.cpp -o aircraft_booking
```

Run the executable on Windows:

```bash
aircraft_booking.exe
```

Replace `main.cpp` with your actual source filename if it is different.

## 🖥️ Main Menu

The program provides the following options:

```text
============================================
   AIRCRAFT SEAT BOOKING AND MANAGEMENT
============================================
 1. View Aircraft Seat Map
 2. View Seat Details
 3. Select / Book Seats
 4. Undo Last Seat Selection
 5. Search Seat
 6. Filter Seats
 7. View Booking Records
 8. Cancel Booking
 9. Add Passenger to Waiting List
10. View Waiting List
11. Flight Statistics
12. Exit
============================================
```

## 📂 Project Structure

```text
aircraft-seat-booking-system/
│
├── main.cpp
├── README.md
└── .gitignore
```

- `main.cpp` — Contains the application source code.
- `README.md` — Documents the project, features, and setup instructions.
- `.gitignore` — Specifies files that should not be tracked by Git.

## 📚 Learning Objectives

This project helps students practise:

- Working with arrays and structures.
- Designing and using functions.
- Implementing linked lists with pointers.
- Performing stack operations such as push and pop.
- Implementing queue operations such as enqueue and dequeue.
- Using dynamic memory allocation with `new` and `delete`.
- Searching, traversing, inserting, and deleting linked-list nodes.
- Applying data structures to a practical programming problem.

## ⚠️ Limitations

- Booking information is stored in memory and is lost when the program exits.
- The application does not use a database or file-based persistence.
- It is a simulation and does not connect to a real airline reservation service.
- Input validation and error handling can be improved.
- The undo feature is intended for seat selections before booking confirmation.

## 🔮 Future Improvements

- Save and load booking records using files.
- Add passenger and booking ID search features.
- Improve input validation and error handling.
- Add a graphical user interface.
- Implement persistent storage using a database.
- Improve booking management and cancellation workflows.

## 👨‍🎓 Academic Project

**Course:** Data Structures and Algorithms (DSA) Lab  
**Institution:** SZABIST  
**Language:** C++

Developed as an educational project to practise fundamental data structures and their application in a seat booking management system.

## 📄 License

This project is intended for educational and learning purposes. Add a license if you intend to distribute or reuse the project under specific terms.
