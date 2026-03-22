# Hospital Management System (C Console Application)

A robust, console-based Hospital Management System developed in C. This project demonstrates the practical implementation of fundamental Data Structures and Algorithms (DSA) to solve real-world administrative and medical workflows.

## 🛠️ Data Structures & Implementation
The system is built using a modular approach, where each hospital function is powered by a specific data structure optimized for its logic:

* **Patient Registration (Singly Linked List):** Uses dynamic memory allocation to store patient records (`struct Patient`), allowing the hospital to scale the registry without fixed limits.
* **Doctor Management (Doubly Linked List):** Implements `struct Doctor` with `prev` and `next` pointers, enabling bidirectional navigation and easy deletion of staff records.
* **Emergency Management (Priority Queue):** Handles life-critical cases by sorting entries based on a severity `priority` integer. Higher priority cases are treated first.
* **Ward Allocation (2D Array within Struct):** Uses a `struct Ward` containing a matrix `beds[3][4]` to map physical bed locations across floors and columns.
* **Patient History (Stack):** Implements a Last-In-First-Out (LIFO) `historyStack` to track and display the most recent actions performed in the system.
* **Appointment Management (Queue):** Manages patient consultations using a First-In-First-Out (FIFO) logic to ensure administrative fairness.
* **Payment System (Stack):** Tracks billing records using a stack to allow for quick access to the latest financial transactions.

## 🚀 Features
- **Search & Filter:** Search for doctors by specialty or patients by ID.
- **Statistics:** Automated age demographics for patients and occupancy tracking for hospital wards.
- **Input Validation:** Includes an input buffer clearing mechanism to prevent menu loops and crashes on invalid data.
- **Memory Management:** Proper use of `malloc()` and `free()` for dynamic structures to prevent memory leaks.

## 💻 How to Run
1. **Clone the Repository:**
   ```bash
   git clone [https://github.com/your-username/hospital-management-system.git](https://github.com/your-username/hospital-management-system.git)
2. **Compile the Source Code:**
   ```bash
   gcc hospital2.c -o hospital_system
3. **Run the Application:**
   ```bash
   gcc hospital2.c -o hospital_system

## 🎓 Acknowledgements
Developed as part of the Data Structures and Algorithms module at the University of Moratuwa.
