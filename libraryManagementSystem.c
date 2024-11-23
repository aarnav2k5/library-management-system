#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Book {
    char title[100];
    char author[100];
    int isAvailable; 
    struct Book* next;
} Book;

typedef struct Borrow {
    char bookTitle[100];
    char borrowDate[20];
    struct Borrow* next;
} Borrow;

void addBook(Book** head, char* title, char* author);
void displayBooks(Book* head);
Book* searchBook(Book* head, char* key, char* type);
void borrowBook(Book* head, Borrow** stack, char* title, char* date);
void returnBook(Book* head, Borrow** stack, char* title);
void push(Borrow** stack, char* bookTitle, char* borrowDate);
void pop(Borrow** stack);
void displayHistory(Borrow* stack);
void menu();

int main() {
    Book* library = NULL;  
    Borrow* history = NULL; 
    int choice;
    char title[100], author[100], date[20];

    do {
        menu();
        printf("\nEnter your choice: ");
        scanf("%d", &choice);
        getchar(); 

        switch (choice) {
            case 1:
                printf("Enter book title: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; 
                printf("Enter book author: ");
                fgets(author, sizeof(author), stdin);
                author[strcspn(author, "\n")] = 0; 
                addBook(&library, title, author);
                printf("Book added successfully!\n");
                break;

            case 2:
                printf("Books in the library:\n");
                displayBooks(library);
                break;

            case 3:
                printf("Enter book title to borrow: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; 
                printf("Enter today's date (YYYY-MM-DD): ");
                fgets(date, sizeof(date), stdin);
                date[strcspn(date, "\n")] = 0; 
                borrowBook(library, &history, title, date);
                break;

            case 4:
                printf("Enter book title to return: ");
                fgets(title, sizeof(title), stdin);
                title[strcspn(title, "\n")] = 0; 
                returnBook(library, &history, title);
                break;

            case 5:
                printf("Borrowing history:\n");
                displayHistory(history);
                break;

            case 6:
                printf("Exiting the program.\n");
                break;

            default:
                printf("Invalid choice! Please try again.\n");
        }

    } while (choice != 6);

    return 0;
}


void menu() {
    printf("\n==== Library Management System ====\n");
    printf("1. Add a new book\n");
    printf("2. Display all books\n");
    printf("3. Borrow a book\n");
    printf("4. Return a book\n");
    printf("5. View borrowing history\n");
    printf("6. Exit\n");
}

void addBook(Book** head, char* title, char* author) {
    Book* newBook = (Book*)malloc(sizeof(Book));
    strcpy(newBook->title, title);
    strcpy(newBook->author, author);
    newBook->isAvailable = 1; 
    newBook->next = *head;
    *head = newBook;
}

void displayBooks(Book* head) {
    if (head == NULL) {
        printf("No books available in the library.\n");
        return;
    }
    Book* current = head;
    while (current != NULL) {
        printf("Title: %s, Author: %s, Available: %s\n",
               current->title, current->author, current->isAvailable ? "Yes" : "No");
        current = current->next;
    }
}

Book* searchBook(Book* head, char* key, char* type) {
    Book* current = head;
    while (current != NULL) {
        if ((strcmp(type, "title") == 0 && strcmp(current->title, key) == 0) ||
            (strcmp(type, "author") == 0 && strcmp(current->author, key) == 0)) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void borrowBook(Book* head, Borrow** stack, char* title, char* date) {
    Book* book = searchBook(head, title, "title");
    if (book) {
        if (book->isAvailable) {
            book->isAvailable = 0; 
            push(stack, title, date); 
            printf("Book '%s' borrowed successfully!\n", title);
        } else {
            printf("Book '%s' is currently not available.\n", title);
        }
    } else {
        printf("Book '%s' does not exist in the library.\n", title);
    }
}
void returnBook(Book* head, Borrow** stack, char* title) {
    Book* book = searchBook(head, title, "title");
    if (book && !book->isAvailable) {
        book->isAvailable = 1; 
        pop(stack); 
        printf("Book '%s' returned successfully!\n", title);
    } else {
        printf("Book '%s' is already available or does not exist.\n", title);
    }
}

void push(Borrow** stack, char* bookTitle, char* borrowDate) {
    Borrow* newBorrow = (Borrow*)malloc(sizeof(Borrow));
    strcpy(newBorrow->bookTitle, bookTitle);
    strcpy(newBorrow->borrowDate, borrowDate);
    newBorrow->next = *stack;
    *stack = newBorrow;
}

void pop(Borrow** stack) {
    if (*stack == NULL) {
        printf("No borrow history available.\n");
        return;
    }
    Borrow* temp = *stack;
    *stack = (*stack)->next;
    free(temp);
}

void displayHistory(Borrow* stack) {
    if (stack == NULL) {
        printf("No borrowing history available.\n");
        return;
    }
    Borrow* current = stack;
    while (current != NULL) {
        printf("Book: %s, Borrowed on: %s\n", current->bookTitle, current->borrowDate);
        current = current->next;
    }
}
