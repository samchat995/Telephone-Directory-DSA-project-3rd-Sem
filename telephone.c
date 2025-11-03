#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define FILENAME "contacts.txt"

// Structure for contact
struct Contact {
    char name[50];
    char phone[15];
    struct Contact *next;
};

// Function prototypes
void loadContacts(struct Contact **head);
void saveContacts(struct Contact *head);
void addContact(struct Contact **head);
void displayContacts(struct Contact *head);
void searchContact(struct Contact *head);
void searchByFirstLetter(struct Contact *head);
void deleteContact(struct Contact **head);
void sortContacts(struct Contact **head);
int isValidPhone(char *phone);
int isDuplicate(struct Contact *head, char *name);

// Main menu
int main() {
    struct Contact *head = NULL;
    int choice;

    loadContacts(&head); // Load existing contacts at start
    sortContacts(&head); // Sort after loading

    while (1) {
        printf("\n===== TELEPHONE DIRECTORY =====\n");
        printf("1. Add Contact\n");
        printf("2. Display All Contacts (Sorted)\n");
        printf("3. Search Contact by Name\n");
        printf("4. Search Contact by First Letter\n");
        printf("5. Delete Contact\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear buffer

        switch (choice) {
            case 1:
                addContact(&head);
                break;
            case 2:
                sortContacts(&head);
                displayContacts(head);
                break;
            case 3:
                searchContact(head);
                break;
            case 4:
                searchByFirstLetter(head);
                break;
            case 5:
                deleteContact(&head);
                break;
            case 6:
                saveContacts(head);
                printf("Contacts saved successfully. Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice! Try again.\n");
        }
    }
    return 0;
}

// Validate phone number (must be 10 digits)
int isValidPhone(char *phone) {
    if (strlen(phone) != 10)
        return 0;
    for (int i = 0; i < 10; i++) {
        if (!isdigit(phone[i]))
            return 0;
    }
    return 1;
}

// Check for duplicate name
int isDuplicate(struct Contact *head, char *name) {
    while (head != NULL) {
        if (strcasecmp(head->name, name) == 0)
            return 1;
        head = head->next;
    }
    return 0;
}

//  Load contacts from file
void loadContacts(struct Contact **head) {
    FILE *fp = fopen(FILENAME, "r");
    if (fp == NULL)
        return;

    struct Contact *newContact;
    char name[50], phone[15];

    while (fscanf(fp, "%49[^,],%14[^\n]\n", name, phone) == 2) {
        newContact = (struct Contact *)malloc(sizeof(struct Contact));
        strcpy(newContact->name, name);
        strcpy(newContact->phone, phone);
        newContact->next = *head;
        *head = newContact;
    }

    fclose(fp);
}

//  Save contacts to file
void saveContacts(struct Contact *head) {
    FILE *fp = fopen(FILENAME, "w");
    if (fp == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    struct Contact *temp = head;
    while (temp != NULL) {
        fprintf(fp, "%s,%s\n", temp->name, temp->phone);
        temp = temp->next;
    }

    fclose(fp);
}

// Add new contact (with validation & no duplicates)
void addContact(struct Contact **head) {
    struct Contact *newContact = (struct Contact *)malloc(sizeof(struct Contact));
    if (!newContact) {
        printf("Memory allocation failed!\n");
        return;
    }

    printf("Enter name: ");
    fgets(newContact->name, sizeof(newContact->name), stdin);
    newContact->name[strcspn(newContact->name, "\n")] = '\0';

    if (isDuplicate(*head, newContact->name)) {
        printf(" Duplicate contact! This name already exists.\n");
        free(newContact);
        return;
    }

    char tempPhone[20];
    while (1) {
        printf("Enter 10-digit phone number: ");
        fgets(tempPhone, sizeof(tempPhone), stdin);
        tempPhone[strcspn(tempPhone, "\n")] = '\0';
        if (isValidPhone(tempPhone)) {
            strcpy(newContact->phone, tempPhone);
            break;
        } else {
            printf(" Invalid phone number! Must be 10 digits and numeric.\n");
        }
    }

    newContact->next = *head;
    *head = newContact;

    sortContacts(head);
    saveContacts(*head);
    printf(" Contact added and saved successfully!\n");
}

//  Display all contacts
void displayContacts(struct Contact *head) {
    if (head == NULL) {
        printf("No contacts available.\n");
        return;
    }

    printf("\n----- Contact List (Sorted A–Z) -----\n");
    struct Contact *temp = head;
    while (temp != NULL) {
        printf("Name : %s\n", temp->name);
        printf("Phone: %s\n", temp->phone);
        printf("------------------------------------\n");
        temp = temp->next;
    }
}

// Search contact by exact name
void searchContact(struct Contact *head) {
    char searchName[50];
    printf("Enter name to search: ");
    fgets(searchName, sizeof(searchName), stdin);
    searchName[strcspn(searchName, "\n")] = '\0';

    while (head != NULL) {
        if (strcasecmp(head->name, searchName) == 0) {
            printf("\n Contact Found!\n");
            printf("Name : %s\n", head->name);
            printf("Phone: %s\n", head->phone);
            return;
        }
        head = head->next;
    }

    printf(" Contact not found!\n");
}

//  Search contact by first letter
void searchByFirstLetter(struct Contact *head) {
    if (head == NULL) {
        printf("No contacts to search.\n");
        return;
    }

    char ch;
    printf("Enter the first letter of the name to search: ");
    scanf(" %c", &ch);
    getchar();
    ch = tolower(ch);

    int found = 0;
    printf("\n--- Contacts starting with '%c' ---\n", toupper(ch));

    while (head != NULL) {
        if (tolower(head->name[0]) == ch) {
            printf("Name : %s\n", head->name);
            printf("Phone: %s\n", head->phone);
            printf("------------------------\n");
            found = 1;
        }
        head = head->next;
    }

    if (!found)
        printf("No contacts found starting with '%c'.\n", toupper(ch));
}

//  Delete a contact
void deleteContact(struct Contact **head) {
    char deleteName[50];
    printf("Enter name to delete: ");
    fgets(deleteName, sizeof(deleteName), stdin);
    deleteName[strcspn(deleteName, "\n")] = '\0';

    struct Contact *temp = *head, *prev = NULL;

    if (temp != NULL && strcasecmp(temp->name, deleteName) == 0) {
        *head = temp->next;
        free(temp);
        saveContacts(*head);
        printf("Contact deleted successfully!\n");
        return;
    }

    while (temp != NULL && strcasecmp(temp->name, deleteName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf(" Contact not found!\n");
        return;
    }

    prev->next = temp->next;
    free(temp);
    saveContacts(*head);
    printf(" Contact deleted successfully!\n");
}

//  Sort contacts alphabetically (Bubble Sort for linked list)
void sortContacts(struct Contact **head) {
    if (*head == NULL)
        return;

    struct Contact *i, *j;
    char tempName[50], tempPhone[15];
    int swapped;

    do {
        swapped = 0;
        for (i = *head; i->next != NULL; i = i->next) {
            if (strcasecmp(i->name, i->next->name) > 0) {
                strcpy(tempName, i->name);
                strcpy(tempPhone, i->phone);

                strcpy(i->name, i->next->name);
                strcpy(i->phone, i->next->phone);

                strcpy(i->next->name, tempName);
                strcpy(i->next->phone, tempPhone);

                swapped = 1;
            }
        }
    } while (swapped);
}