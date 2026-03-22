#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HISTORY 100
#define MAX_PAYMENTS 100
#define MAX_APPOINTMENTS 100
#define MAX_EMERGENCY 100
#define WARD_ROWS 3
#define WARD_COLS 4



struct HistoryAction {
    int patientId;
    char patientName[50];
    char action[100];
};

struct Patient {
    int id;
    char name[50];
    int age;
    struct Patient *next;
};

struct Appointment {
    int appointmentId;
    char patientName[50];
    char doctorName[50];
};

struct Payment {
    int billId;
    float amount;
    char description[80];
};

struct Doctor {
    int id;
    char name[50];
    char specialty[50];
    struct Doctor *prev;
    struct Doctor *next;
};

struct Ward {
    int beds[WARD_ROWS][WARD_COLS];
};

struct EmergencyCase {
    int caseId;
    char patientName[50];
    int priority;
};


struct HistoryAction historyStack[MAX_HISTORY];
int historyTop = -1;

struct Patient *patientHead = NULL;

struct Appointment appointmentQueue[MAX_APPOINTMENTS];
int frontA = 0, rearA = -1, countA = 0;

struct Payment paymentStack[MAX_PAYMENTS];
int paymentTop = -1;

struct Doctor *doctorHead = NULL;
struct Doctor *doctorTail = NULL;

struct Ward hospitalWard = {0};

struct EmergencyCase emergencyQueue[MAX_EMERGENCY];
int emergencyCount = 0;


void clearInputBuffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

void readLine(char *buffer, int size) {
    if (fgets(buffer, size, stdin) != NULL) {
        buffer[strcspn(buffer, "\n")] = '\0';
    }
}


void pushHistory(void) {
    struct HistoryAction item;

    if (historyTop == MAX_HISTORY - 1) {
        printf("History stack is full.\n");
        return;
    }

    printf("Enter Patient ID: ");
    scanf("%d", &item.patientId);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readLine(item.patientName, sizeof(item.patientName));

    printf("Enter history action: ");
    readLine(item.action, sizeof(item.action));

    historyStack[++historyTop] = item;
    printf("History action pushed successfully.\n");
}

void undoHistory(void) {
    struct HistoryAction item;

    if (historyTop == -1) {
        printf("No history action to undo.\n");
        return;
    }

    item = historyStack[historyTop--];
    printf("Undone history -> Patient ID: %d | Name: %s | Action: %s\n",
           item.patientId, item.patientName, item.action);
}

void displayHistory(void) {
    int i;

    if (historyTop == -1) {
        printf("No patient history actions available.\n");
        return;
    }

    printf("\nPatient History Stack (latest first):\n");
    for (i = historyTop; i >= 0; i--) {
        printf("Patient ID: %d | Name: %s | Action: %s\n",
               historyStack[i].patientId,
               historyStack[i].patientName,
               historyStack[i].action);
    }
}

void showLatestHistory(void) {
    if (historyTop == -1) {
        printf("No history action available.\n");
        return;
    }

    printf("Latest history -> Patient ID: %d | Name: %s | Action: %s\n",
           historyStack[historyTop].patientId,
           historyStack[historyTop].patientName,
           historyStack[historyTop].action);
}

void countHistoryForPatient(void) {
    int patientId, i, count = 0;

    if (historyTop == -1) {
        printf("No patient history actions available.\n");
        return;
    }

    printf("Enter Patient ID to count history actions: ");
    scanf("%d", &patientId);
    clearInputBuffer();

    for (i = 0; i <= historyTop; i++) {
        if (historyStack[i].patientId == patientId) {
            count++;
        }
    }

    printf("Patient ID %d has %d history action(s).\n", patientId, count);
}

void patientHistoryMenu(void) {
    int choice;

    do {
        printf("\n--- Patient History (Stack) ---\n");
        printf("1. Push History Action\n");
        printf("2. Undo Last History Action\n");
        printf("3. Display History Stack\n");
        printf("4. Show Latest History Action\n");
        printf("5. Count History Actions for a Patient\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: pushHistory(); break;
            case 2: undoHistory(); break;
            case 3: displayHistory(); break;
            case 4: showLatestHistory(); break;
            case 5: countHistoryForPatient(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void swapPatientData(struct Patient *a, struct Patient *b) {
    int tempId, tempAge;
    char tempName[50];

    tempId = a->id;
    a->id = b->id;
    b->id = tempId;

    tempAge = a->age;
    a->age = b->age;
    b->age = tempAge;

    strcpy(tempName, a->name);
    strcpy(a->name, b->name);
    strcpy(b->name, tempName);
}

void bubbleSortPatients(void) {
    struct Patient *ptr1;
    struct Patient *ptr2;
    int swapped;

    if (patientHead == NULL || patientHead->next == NULL) {
        printf("Not enough patients to sort.\n");
        return;
    }

    do {
        swapped = 0;
        ptr1 = patientHead;

        while (ptr1->next != NULL) {
            ptr2 = ptr1->next;

            if (ptr1->id > ptr2->id) {
                swapPatientData(ptr1, ptr2);
                swapped = 1;
            }

            ptr1 = ptr1->next;
        }
    } while (swapped);

    printf("Patients sorted by ID using Bubble Sort.\n");
}

void addPatient(void) {
    struct Patient *newNode = (struct Patient *)malloc(sizeof(struct Patient));
    struct Patient *temp;

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter Patient ID: ");
    scanf("%d", &newNode->id);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readLine(newNode->name, sizeof(newNode->name));

    printf("Enter Patient Age: ");
    scanf("%d", &newNode->age);
    clearInputBuffer();

    newNode->next = NULL;

    if (patientHead == NULL) {
        patientHead = newNode;
    } else {
        temp = patientHead;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    if (historyTop < MAX_HISTORY - 1) {
        historyTop++;
        historyStack[historyTop].patientId = newNode->id;
        strcpy(historyStack[historyTop].patientName, newNode->name);
        strcpy(historyStack[historyTop].action, "Patient registered");
    }

    printf("Patient registered successfully.\n");
}

void displayPatients(void) {
    struct Patient *temp = patientHead;

    if (temp == NULL) {
        printf("No patients registered.\n");
        return;
    }

    printf("\nRegistered Patients:\n");
    while (temp != NULL) {
        printf("ID: %d | Name: %s | Age: %d\n", temp->id, temp->name, temp->age);
        temp = temp->next;
    }
}

void searchPatient(void) {
    int id;
    struct Patient *temp = patientHead;

    if (temp == NULL) {
        printf("No patients registered.\n");
        return;
    }

    printf("Enter Patient ID to search: ");
    scanf("%d", &id);
    clearInputBuffer();

    while (temp != NULL) {
        if (temp->id == id) {
            printf("Patient found -> ID: %d | Name: %s | Age: %d\n",
                   temp->id, temp->name, temp->age);
            return;
        }
        temp = temp->next;
    }

    printf("Patient not found.\n");
}

void patientAgeStatistics(void) {
    struct Patient *temp = patientHead;
    int minAge, maxAge, totalAge = 0, count = 0;
    char minName[50], maxName[50];

    if (temp == NULL) {
        printf("No patients registered.\n");
        return;
    }

    minAge = temp->age;
    maxAge = temp->age;
    strcpy(minName, temp->name);
    strcpy(maxName, temp->name);

    while (temp != NULL) {
        if (temp->age < minAge) {
            minAge = temp->age;
            strcpy(minName, temp->name);
        }
        if (temp->age > maxAge) {
            maxAge = temp->age;
            strcpy(maxName, temp->name);
        }
        totalAge += temp->age;
        count++;
        temp = temp->next;
    }

    printf("Minimum age: %d (%s)\n", minAge, minName);
    printf("Maximum age: %d (%s)\n", maxAge, maxName);
    printf("Average age: %.2f\n", (float)totalAge / count);
}

void patientRegistrationMenu(void) {
    int choice;

    do {
        printf("\n--- Patient Registration (Singly Linked List) ---\n");
        printf("1. Add Patient\n");
        printf("2. Display Patients\n");
        printf("3. Search Patient by ID\n");
        printf("4. Sort Patients by ID (Bubble Sort)\n");
        printf("5. Show Min / Max / Average Age\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addPatient(); break;
            case 2: displayPatients(); break;
            case 3: searchPatient(); break;
            case 4: bubbleSortPatients(); break;
            case 5: patientAgeStatistics(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void enqueueAppointment(void) {
    struct Appointment item;

    if (countA == MAX_APPOINTMENTS) {
        printf("Appointment queue is full.\n");
        return;
    }

    printf("Enter Appointment ID: ");
    scanf("%d", &item.appointmentId);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readLine(item.patientName, sizeof(item.patientName));

    printf("Enter Doctor Name: ");
    readLine(item.doctorName, sizeof(item.doctorName));

    rearA = (rearA + 1) % MAX_APPOINTMENTS;
    appointmentQueue[rearA] = item;
    countA++;

    printf("Appointment added to queue.\n");
}

void dequeueAppointment(void) {
    struct Appointment item;

    if (countA == 0) {
        printf("No appointments in queue.\n");
        return;
    }

    item = appointmentQueue[frontA];
    frontA = (frontA + 1) % MAX_APPOINTMENTS;
    countA--;

    printf("Serving appointment -> ID: %d | Patient: %s | Doctor: %s\n",
           item.appointmentId, item.patientName, item.doctorName);
}

void displayAppointments(void) {
    int i, index;

    if (countA == 0) {
        printf("No appointments in queue.\n");
        return;
    }

    printf("\nAppointment Queue (front to rear):\n");
    for (i = 0; i < countA; i++) {
        index = (frontA + i) % MAX_APPOINTMENTS;
        printf("ID: %d | Patient: %s | Doctor: %s\n",
               appointmentQueue[index].appointmentId,
               appointmentQueue[index].patientName,
               appointmentQueue[index].doctorName);
    }
}

void showNextAppointment(void) {
    if (countA == 0) {
        printf("No appointments in queue.\n");
        return;
    }

    printf("Next appointment -> ID: %d | Patient: %s | Doctor: %s\n",
           appointmentQueue[frontA].appointmentId,
           appointmentQueue[frontA].patientName,
           appointmentQueue[frontA].doctorName);
}

void countWaitingAppointments(void) {
    printf("Total waiting appointments: %d\n", countA);
}

void appointmentMenu(void) {
    int choice;

    do {
        printf("\n--- Appointment Management (Queue) ---\n");
        printf("1. Add Appointment\n");
        printf("2. Serve Next Appointment\n");
        printf("3. Display Appointment Queue\n");
        printf("4. Show Next Appointment\n");
        printf("5. Count Waiting Appointments\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: enqueueAppointment(); break;
            case 2: dequeueAppointment(); break;
            case 3: displayAppointments(); break;
            case 4: showNextAppointment(); break;
            case 5: countWaitingAppointments(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void pushPayment(void) {
    struct Payment item;

    if (paymentTop == MAX_PAYMENTS - 1) {
        printf("Payment stack is full.\n");
        return;
    }

    printf("Enter Bill ID: ");
    scanf("%d", &item.billId);
    clearInputBuffer();

    printf("Enter Payment Description: ");
    readLine(item.description, sizeof(item.description));

    printf("Enter Amount: ");
    scanf("%f", &item.amount);
    clearInputBuffer();

    paymentStack[++paymentTop] = item;
    printf("Payment entry pushed successfully.\n");
}

void undoPayment(void) {
    struct Payment item;

    if (paymentTop == -1) {
        printf("No payment entry to undo.\n");
        return;
    }

    item = paymentStack[paymentTop--];
    printf("Undone payment -> Bill ID: %d | Description: %s | Amount: %.2f\n",
           item.billId, item.description, item.amount);
}

void displayPayments(void) {
    int i;

    if (paymentTop == -1) {
        printf("No payment entries available.\n");
        return;
    }

    printf("\nPayment Stack (latest first):\n");
    for (i = paymentTop; i >= 0; i--) {
        printf("Bill ID: %d | Description: %s | Amount: %.2f\n",
               paymentStack[i].billId,
               paymentStack[i].description,
               paymentStack[i].amount);
    }
}

void paymentStatistics(void) {
    int i;
    float total = 0.0f, minAmount, maxAmount;
    int minBillId, maxBillId;

    if (paymentTop == -1) {
        printf("No payment entries available.\n");
        return;
    }

    minAmount = paymentStack[0].amount;
    maxAmount = paymentStack[0].amount;
    minBillId = paymentStack[0].billId;
    maxBillId = paymentStack[0].billId;

    for (i = 0; i <= paymentTop; i++) {
        total += paymentStack[i].amount;

        if (paymentStack[i].amount < minAmount) {
            minAmount = paymentStack[i].amount;
            minBillId = paymentStack[i].billId;
        }

        if (paymentStack[i].amount > maxAmount) {
            maxAmount = paymentStack[i].amount;
            maxBillId = paymentStack[i].billId;
        }
    }

    printf("Total payment amount: %.2f\n", total);
    printf("Minimum payment: %.2f (Bill ID %d)\n", minAmount, minBillId);
    printf("Maximum payment: %.2f (Bill ID %d)\n", maxAmount, maxBillId);
    printf("Average payment: %.2f\n", total / (paymentTop + 1));
}

void paymentMenu(void) {
    int choice;

    do {
        printf("\n--- Payment System (Stack) ---\n");
        printf("1. Add Payment Entry\n");
        printf("2. Undo Last Payment Entry\n");
        printf("3. Display Payment Stack\n");
        printf("4. Show Total / Min / Max / Average Payment\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: pushPayment(); break;
            case 2: undoPayment(); break;
            case 3: displayPayments(); break;
            case 4: paymentStatistics(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void addDoctor(void) {
    struct Doctor *newNode = (struct Doctor *)malloc(sizeof(struct Doctor));

    if (newNode == NULL) {
        printf("Memory allocation failed.\n");
        return;
    }

    printf("Enter Doctor ID: ");
    scanf("%d", &newNode->id);
    clearInputBuffer();

    printf("Enter Doctor Name: ");
    readLine(newNode->name, sizeof(newNode->name));

    printf("Enter Specialty: ");
    readLine(newNode->specialty, sizeof(newNode->specialty));

    newNode->next = NULL;
    newNode->prev = doctorTail;

    if (doctorHead == NULL) {
        doctorHead = newNode;
        doctorTail = newNode;
    } else {
        doctorTail->next = newNode;
        doctorTail = newNode;
    }

    printf("Doctor added successfully.\n");
}

void displayDoctorsForward(void) {
    struct Doctor *temp = doctorHead;

    if (temp == NULL) {
        printf("No doctors available.\n");
        return;
    }

    printf("\nDoctors (forward traversal):\n");
    while (temp != NULL) {
        printf("ID: %d | Name: %s | Specialty: %s\n",
               temp->id, temp->name, temp->specialty);
        temp = temp->next;
    }
}

void displayDoctorsBackward(void) {
    struct Doctor *temp = doctorTail;

    if (temp == NULL) {
        printf("No doctors available.\n");
        return;
    }

    printf("\nDoctors (backward traversal):\n");
    while (temp != NULL) {
        printf("ID: %d | Name: %s | Specialty: %s\n",
               temp->id, temp->name, temp->specialty);
        temp = temp->prev;
    }
}

void deleteDoctor(void) {
    int id;
    struct Doctor *temp = doctorHead;

    if (temp == NULL) {
        printf("No doctors available.\n");
        return;
    }

    printf("Enter Doctor ID to delete: ");
    scanf("%d", &id);
    clearInputBuffer();

    while (temp != NULL && temp->id != id) {
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Doctor not found.\n");
        return;
    }

    if (temp->prev != NULL) {
        temp->prev->next = temp->next;
    } else {
        doctorHead = temp->next;
    }

    if (temp->next != NULL) {
        temp->next->prev = temp->prev;
    } else {
        doctorTail = temp->prev;
    }

    free(temp);
    printf("Doctor deleted successfully.\n");
}

void countDoctors(void) {
    struct Doctor *temp = doctorHead;
    int count = 0;

    while (temp != NULL) {
        count++;
        temp = temp->next;
    }

    printf("Total doctors: %d\n", count);
}

void searchDoctorsBySpecialty(void) {
    char specialty[50];
    struct Doctor *temp = doctorHead;
    int found = 0;

    if (temp == NULL) {
        printf("No doctors available.\n");
        return;
    }

    printf("Enter specialty to search: ");
    readLine(specialty, sizeof(specialty));

    while (temp != NULL) {
        if (strcmp(temp->specialty, specialty) == 0) {
            printf("ID: %d | Name: %s | Specialty: %s\n",
                   temp->id, temp->name, temp->specialty);
            found = 1;
        }
        temp = temp->next;
    }

    if (!found) {
        printf("No doctors found with specialty '%s'.\n", specialty);
    }
}

void doctorMenu(void) {
    int choice;

    do {
        printf("\n--- Doctor Management (Doubly Linked List) ---\n");
        printf("1. Add Doctor\n");
        printf("2. Display Doctors Forward\n");
        printf("3. Display Doctors Backward\n");
        printf("4. Delete Doctor\n");
        printf("5. Count Doctors\n");
        printf("6. Search Doctors by Specialty\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addDoctor(); break;
            case 2: displayDoctorsForward(); break;
            case 3: displayDoctorsBackward(); break;
            case 4: deleteDoctor(); break;
            case 5: countDoctors(); break;
            case 6: searchDoctorsBySpecialty(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void displayWardBeds(void) {
    int i, j;

    printf("\nWard Bed Allocation (0 = Empty):\n");
    for (i = 0; i < WARD_ROWS; i++) {
        printf("Ward %d: ", i + 1);
        for (j = 0; j < WARD_COLS; j++) {
            printf("[%d] ", hospitalWard.beds[i][j]);
        }
        printf("\n");
    }
}

void allocateBed(void) {
    int ward, bed, patientId;

    printf("Enter Ward Number (1-%d): ", WARD_ROWS);
    scanf("%d", &ward);
    printf("Enter Bed Number (1-%d): ", WARD_COLS);
    scanf("%d", &bed);
    printf("Enter Patient ID: ");
    scanf("%d", &patientId);
    clearInputBuffer();

    if (ward < 1 || ward > WARD_ROWS || bed < 1 || bed > WARD_COLS) {
        printf("Invalid ward/bed number.\n");
        return;
    }

    if (hospitalWard.beds[ward - 1][bed - 1] != 0) {
        printf("Bed is already occupied by Patient ID %d.\n",
               hospitalWard.beds[ward - 1][bed - 1]);
        return;
    }

    hospitalWard.beds[ward - 1][bed - 1] = patientId;
    printf("Bed allocated successfully.\n");
}

void dischargeBed(void) {
    int ward, bed;

    printf("Enter Ward Number (1-%d): ", WARD_ROWS);
    scanf("%d", &ward);
    printf("Enter Bed Number (1-%d): ", WARD_COLS);
    scanf("%d", &bed);
    clearInputBuffer();

    if (ward < 1 || ward > WARD_ROWS || bed < 1 || bed > WARD_COLS) {
        printf("Invalid ward/bed number.\n");
        return;
    }

    if (hospitalWard.beds[ward - 1][bed - 1] == 0) {
        printf("Bed is already empty.\n");
        return;
    }

    hospitalWard.beds[ward - 1][bed - 1] = 0;
    printf("Bed discharged successfully.\n");
}

void wardStatistics(void) {
    int i, j;
    int occupied = 0, empty = 0;
    int busiestWard = 0, busiestCount = 0;

    for (i = 0; i < WARD_ROWS; i++) {
        int currentWardCount = 0;
        for (j = 0; j < WARD_COLS; j++) {
            if (hospitalWard.beds[i][j] == 0) {
                empty++;
            } else {
                occupied++;
                currentWardCount++;
            }
        }
        if (currentWardCount > busiestCount) {
            busiestCount = currentWardCount;
            busiestWard = i + 1;
        }
    }

    printf("Occupied beds: %d\n", occupied);
    printf("Empty beds: %d\n", empty);
    if (busiestCount > 0) {
        printf("Busiest ward: Ward %d with %d occupied bed(s)\n", busiestWard, busiestCount);
    } else {
        printf("No ward is occupied currently.\n");
    }
    printf("Occupancy rate: %.2f%%\n", (occupied * 100.0f) / (WARD_ROWS * WARD_COLS));
}

void wardMenu(void) {
    int choice;

    do {
        printf("\n--- Ward Allocate (Struct with 2D Array) ---\n");
        printf("1. Allocate Bed\n");
        printf("2. Discharge Bed\n");
        printf("3. Display Ward Beds\n");
        printf("4. Show Occupied / Empty Beds and Busiest Ward\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: allocateBed(); break;
            case 2: dischargeBed(); break;
            case 3: displayWardBeds(); break;
            case 4: wardStatistics(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


void addEmergencyCase(void) {
    struct EmergencyCase item;
    int i;

    if (emergencyCount == MAX_EMERGENCY) {
        printf("Emergency priority queue is full.\n");
        return;
    }

    printf("Enter Emergency Case ID: ");
    scanf("%d", &item.caseId);
    clearInputBuffer();

    printf("Enter Patient Name: ");
    readLine(item.patientName, sizeof(item.patientName));

    printf("Enter Priority (1 = Critical, 2 = Serious, 3 = Normal): ");
    scanf("%d", &item.priority);
    clearInputBuffer();

    if (item.priority < 1 || item.priority > 3) {
        printf("Invalid priority.\n");
        return;
    }

    i = emergencyCount - 1;
    while (i >= 0 && emergencyQueue[i].priority > item.priority) {
        emergencyQueue[i + 1] = emergencyQueue[i];
        i--;
    }

    emergencyQueue[i + 1] = item;
    emergencyCount++;

    printf("Emergency case added successfully.\n");
}

void treatEmergencyCase(void) {
    int i;
    struct EmergencyCase item;

    if (emergencyCount == 0) {
        printf("No emergency cases in queue.\n");
        return;
    }

    item = emergencyQueue[0];
    for (i = 1; i < emergencyCount; i++) {
        emergencyQueue[i - 1] = emergencyQueue[i];
    }
    emergencyCount--;

    printf("Treating case -> ID: %d | Patient: %s | Priority: %d\n",
           item.caseId, item.patientName, item.priority);
}

void displayEmergencyCases(void) {
    int i;

    if (emergencyCount == 0) {
        printf("No emergency cases in queue.\n");
        return;
    }

    printf("\nEmergency Priority Queue:\n");
    for (i = 0; i < emergencyCount; i++) {
        printf("ID: %d | Patient: %s | Priority: %d\n",
               emergencyQueue[i].caseId,
               emergencyQueue[i].patientName,
               emergencyQueue[i].priority);
    }
}

void showHighestPriorityCase(void) {
    if (emergencyCount == 0) {
        printf("No emergency cases in queue.\n");
        return;
    }

    printf("Highest priority case -> ID: %d | Patient: %s | Priority: %d\n",
           emergencyQueue[0].caseId,
           emergencyQueue[0].patientName,
           emergencyQueue[0].priority);
}

void countEmergencyByPriority(void) {
    int i;
    int critical = 0, serious = 0, normal = 0;

    for (i = 0; i < emergencyCount; i++) {
        if (emergencyQueue[i].priority == 1) {
            critical++;
        } else if (emergencyQueue[i].priority == 2) {
            serious++;
        } else {
            normal++;
        }
    }

    printf("Critical cases: %d\n", critical);
    printf("Serious cases: %d\n", serious);
    printf("Normal cases: %d\n", normal);
}

void emergencyMenu(void) {
    int choice;

    do {
        printf("\n--- Emergency (Priority Queue) ---\n");
        printf("1. Add Emergency Case\n");
        printf("2. Treat Highest Priority Case\n");
        printf("3. Display Emergency Queue\n");
        printf("4. Show Highest Priority Case\n");
        printf("5. Count Cases by Priority\n");
        printf("0. Back to Main Menu\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        clearInputBuffer();

        switch (choice) {
            case 1: addEmergencyCase(); break;
            case 2: treatEmergencyCase(); break;
            case 3: displayEmergencyCases(); break;
            case 4: showHighestPriorityCase(); break;
            case 5: countEmergencyByPriority(); break;
            case 0: break;
            default: printf("Invalid choice.\n");
        }
    } while (choice != 0);
}


int main(void) {
    int choice;

    do {
        printf("\n Hospital Management using Data Structures");
        printf("\n\n1. Patient History");
        printf("\n2. Patient Registration");
        printf("\n3. Appointment Management");
        printf("\n4. Payment System");
        printf("\n5. Doctor Management");
        printf("\n6. Ward Allocat");
        printf("\n7. Emergency");
        printf("\n0. Exit");
        printf("\n\nEnter choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Exiting program.\n");
            break;
        }
        clearInputBuffer();

        switch (choice) {
            case 1: patientHistoryMenu(); break;
            case 2: patientRegistrationMenu(); break;
            case 3: appointmentMenu(); break;
            case 4: paymentMenu(); break;
            case 5: doctorMenu(); break;
            case 6: wardMenu(); break;
            case 7: emergencyMenu(); break;
            case 0: printf("Exiting program...\n"); break;
            default: printf("Invalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}
