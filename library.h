#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdio.h>
#include <string.h>

#define MAX_BOOKS 100
#define MAX_USERS 100
#define MAX_RECORDS 200
#define NAME_LEN 100
#define EMAIL_LEN 100
#define DATE_LEN 20
#define BOOKS_FILE "books.dat"
#define USERS_FILE "users.dat"
#define RECORDS_FILE "records.dat"

typedef struct
{
    int id;
    char title[NAME_LEN];
    char author[NAME_LEN];
    int total_copies;
    int available_copies;
    int is_deleted;
} Book;
typedef struct
{
    int id;
    char name[NAME_LEN];
    char email[EMAIL_LEN];
    int books_borrowed;
    int is_deleted;
} User;
typedef struct
{
    int id;
    int book_id;
    int user_id;
    char borrow_date[DATE_LEN];
    char due_date[DATE_LEN];
    char return_date[DATE_LEN];
    int is_returned;
} BorrowRecord;
typedef struct
{
    Book books[MAX_BOOKS];
    int book_count;
    int next_book_id;
    User users[MAX_USERS];
    int user_count;
    int next_user_id;
    BorrowRecord records[MAX_RECORDS];
    int record_count;
    int next_record_id;
} Library;
// Functions
void read_string(char *prompt, char *buffer, int size)
{
    printf("%s", prompt);
    fgets(buffer, size, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}
int read_int(char *prompt)
{
    int value;
    printf("%s", prompt);
    scanf("%d", &value);
    getchar();
    return value;
}
void save_books(Library *lib)
{
    FILE *f = fopen(BOOKS_FILE, "wb");
    if (f == NULL)
    {
        printf("Error: could not save books.\n");
        return;
    }
    fwrite(&lib->book_count, sizeof(int), 1, f);
    fwrite(&lib->next_book_id, sizeof(int), 1, f);
    fwrite(lib->books, sizeof(Book), lib->book_count, f);
    fclose(f);
}
void save_users(Library *lib)
{
    FILE *f = fopen(USERS_FILE, "wb");
    if (f == NULL)
    {
        printf("Error: could not save users.\n");
        return;
    }
    fwrite(&lib->user_count, sizeof(int), 1, f);
    fwrite(&lib->next_user_id, sizeof(int), 1, f);
    fwrite(lib->users, sizeof(User), lib->user_count, f);
    fclose(f);
}
void save_records(Library *lib)
{
    FILE *f = fopen(RECORDS_FILE, "wb");
    if (f == NULL)
    {
        printf("Error: could not save records.\n");
        return;
    }
    fwrite(&lib->record_count, sizeof(int), 1, f);
    fwrite(&lib->next_record_id, sizeof(int), 1, f);
    fwrite(lib->records, sizeof(BorrowRecord), lib->record_count, f);
    fclose(f);
}
void save_library(Library *lib)
{
    save_books(lib);
    save_users(lib);
    save_records(lib);
    printf("Data saved.\n");
}
void load_books(Library *lib)
{
    FILE *f = fopen(BOOKS_FILE, "rb");
    if (f == NULL)
        return;
    fread(&lib->book_count, sizeof(int), 1, f);
    fread(&lib->next_book_id, sizeof(int), 1, f);
    fread(lib->books, sizeof(Book), lib->book_count, f);
    fclose(f);
}
void load_users(Library *lib)
{
    FILE *f = fopen(USERS_FILE, "rb");
    if (f == NULL)
        return;
    fread(&lib->user_count, sizeof(int), 1, f);
    fread(&lib->next_user_id, sizeof(int), 1, f);
    fread(lib->users, sizeof(User), lib->user_count, f);
    fclose(f);
}
void load_records(Library *lib)
{
    FILE *f = fopen(RECORDS_FILE, "rb");
    if (f == NULL)
        return;
    fread(&lib->record_count, sizeof(int), 1, f);
    fread(&lib->next_record_id, sizeof(int), 1, f);
    fread(lib->records, sizeof(BorrowRecord), lib->record_count, f);
    fclose(f);
}
void load_library(Library *lib)
{
    load_books(lib);
    load_users(lib);
    load_records(lib);
}
Book *find_book(Library *lib, int id)
{
    int i;
    for (i = 0; i < lib->book_count; i++)
    {
        if (lib->books[i].id == id && lib->books[i].is_deleted == 0)
            return &lib->books[i];
    }
    return NULL;
}
User *find_user(Library *lib, int id)
{
    int i;
    for (i = 0; i < lib->user_count; i++)
    {
        if (lib->users[i].id == id && lib->users[i].is_deleted == 0)
            return &lib->users[i];
    }
    return NULL;
}
void add_book(Library *lib, char *title, char *author, int copies)
{
    if (lib->book_count >= MAX_BOOKS)
    {
        printf("Book limit reached.\n");
        return;
    }
    Book *b = &lib->books[lib->book_count];
    b->id = lib->next_book_id;
    b->total_copies = copies;
    b->available_copies = copies;
    b->is_deleted = 0;
    strncpy(b->title, title, NAME_LEN - 1);
    strncpy(b->author, author, NAME_LEN - 1);
    lib->book_count++;
    lib->next_book_id++;
    printf("Book added! ID: %d\n", b->id);
}
void delete_book(Library *lib, int id)
{
    Book *b = find_book(lib, id);
    if (b == NULL)
    {
        printf("Book not found.\n");
        return;
    }
    if (b->available_copies < b->total_copies)
    {
        printf("Cannot delete. Some copies are still borrowed.\n");
        return;
    }
    b->is_deleted = 1;
    printf("Book deleted.\n");
}
void list_books(Library *lib)
{
    int i;
    int found = 0;
    printf("\n--- BOOKS ---\n");
    printf("%-5s %-30s %-20s %s\n", "ID", "Title", "Author", "Copies");
    printf("-------------------------------------------------------------\n");
    for (i = 0; i < lib->book_count; i++)
    {
        if (lib->books[i].is_deleted == 1)
            continue;
        printf("%-5d %-30s %-20s %d/%d\n",
               lib->books[i].id,
               lib->books[i].title,
               lib->books[i].author,
               lib->books[i].available_copies,
               lib->books[i].total_copies);
        found = 1;
    }
    if (found == 0)
        printf("No books yet.\n");
}
void add_user(Library *lib, char *name, char *email)
{
    if (lib->user_count >= MAX_USERS)
    {
        printf("User limit reached.\n");
        return;
    }
    User *u = &lib->users[lib->user_count];
    u->id = lib->next_user_id;
    u->books_borrowed = 0;
    u->is_deleted = 0;
    strncpy(u->name, name, NAME_LEN - 1);
    strncpy(u->email, email, EMAIL_LEN - 1);
    lib->user_count++;
    lib->next_user_id++;
    printf("User added! ID: %d\n", u->id);
}
void delete_user(Library *lib, int id)
{
    User *u = find_user(lib, id);
    if (u == NULL)
    {
        printf("User not found.\n");
        return;
    }
    if (u->books_borrowed > 0)
    {
        printf("Cannot delete. User still has borrowed books.\n");
        return;
    }
    u->is_deleted = 1;
    printf("User deleted.\n");
}
void list_users(Library *lib)
{
    int i;
    int found = 0;
    printf("\n--- USERS ---\n");
    printf("%-5s %-25s %-30s %s\n", "ID", "Name", "Email", "Borrowed");
    printf("-------------------------------------------------------------\n");
    for (i = 0; i < lib->user_count; i++)
    {
        if (lib->users[i].is_deleted == 1)
            continue;
        printf("%-5d %-25s %-30s %d\n",
               lib->users[i].id,
               lib->users[i].name,
               lib->users[i].email,
               lib->users[i].books_borrowed);
        found = 1;
    }
    if (found == 0)
        printf("No users yet.\n");
}
void borrow_book(Library *lib, int book_id, int user_id,
                 char *borrow_date, char *due_date)
{
    if (lib->record_count >= MAX_RECORDS)
    {
        printf("Record limit reached.\n");
        return;
    }
    Book *b = find_book(lib, book_id);
    User *u = find_user(lib, user_id);
    if (b == NULL)
    {
        printf("Book not found.\n");
        return;
    }
    if (u == NULL)
    {
        printf("User not found.\n");
        return;
    }
    if (b->available_copies <= 0)
    {
        printf("No copies available.\n");
        return;
    }
    BorrowRecord *r = &lib->records[lib->record_count];
    r->id = lib->next_record_id;
    r->book_id = book_id;
    r->user_id = user_id;
    r->is_returned = 0;
    strncpy(r->borrow_date, borrow_date, DATE_LEN - 1);
    strncpy(r->due_date, due_date, DATE_LEN - 1);
    strncpy(r->return_date, "N/A", DATE_LEN - 1);
    lib->record_count++;
    lib->next_record_id++;
    b->available_copies--;
    u->books_borrowed++;
    printf("Borrowed! Record ID: %d\n", r->id);
}
void return_book(Library *lib, int record_id, char *return_date)
{
    int i;
    BorrowRecord *r = NULL;
    for (i = 0; i < lib->record_count; i++)
    {
        if (lib->records[i].id == record_id && lib->records[i].is_returned == 0)
        {
            r = &lib->records[i];
            break;
        }
    }
    if (r == NULL)
    {
        printf("Active record not found.\n");
        return;
    }
    r->is_returned = 1;
    strncpy(r->return_date, return_date, DATE_LEN - 1);
    Book *b = find_book(lib, r->book_id);
    User *u = find_user(lib, r->user_id);
    if (b != NULL)
        b->available_copies++;
    if (u != NULL)
        u->books_borrowed--;
    printf("Book returned. Record %d closed.\n", record_id);
}
void show_borrow_log(Library *lib)
{
    int i;
    int found = 0;
    printf("\n--- BORROW LOG ---\n");
    printf("%-5s %-25s %-20s %-12s %-12s %s\n",
           "ID", "Book", "User", "Borrowed", "Due", "Status");
    printf("---------------------------------------------------------------------\n");
    for (i = 0; i < lib->record_count; i++)
    {
        BorrowRecord *r = &lib->records[i];
        Book *b = find_book(lib, r->book_id);
        User *u = find_user(lib, r->user_id);
        printf("%-5d %-25s %-20s %-12s %-12s %s\n",
               r->id,
               b != NULL ? b->title : "Unknown",
               u != NULL ? u->name : "Unknown",
               r->borrow_date,
               r->due_date,
               r->is_returned == 1 ? "Returned" : "Still Out");
        found = 1;
    }
    if (found == 0)
        printf("No records yet.\n");
}
// Menus
void menu_books(Library *lib)
{
    int choice;
    do
    {
        printf("\n--- BOOKS ---\n");
        printf("1. List books\n");
        printf("2. Add book\n");
        printf("3. Delete book\n");
        printf("0. Back\n");
        choice = read_int("> ");
        if (choice == 1)
        {
            list_books(lib);
        }
        else if (choice == 2)
        {
            char title[NAME_LEN];
            char author[NAME_LEN];
            int copies;
            read_string("Title:  ", title, NAME_LEN);
            read_string("Author: ", author, NAME_LEN);
            copies = read_int("Copies: ");
            add_book(lib, title, author, copies);
        }
        else if (choice == 3)
        {
            list_books(lib);
            int id = read_int("Book ID to delete: ");
            delete_book(lib, id);
        }
    } while (choice != 0);
}
void menu_users(Library *lib)
{
    int choice;
    do
    {
        printf("\n--- USERS ---\n");
        printf("1. List users\n");
        printf("2. Add user\n");
        printf("3. Delete user\n");
        printf("0. Back\n");
        choice = read_int("> ");
        if (choice == 1)
        {
            list_users(lib);
        }
        else if (choice == 2)
        {
            char name[NAME_LEN];
            char email[EMAIL_LEN];
            read_string("Name:  ", name, NAME_LEN);
            read_string("Email: ", email, EMAIL_LEN);
            add_user(lib, name, email);
        }
        else if (choice == 3)
        {
            list_users(lib);
            int id = read_int("User ID to delete: ");
            delete_user(lib, id);
        }
    } while (choice != 0);
}
void menu_borrow(Library *lib)
{
    int choice;
    do
    {
        printf("\n--- BORROW / RETURN ---\n");
        printf("1. Borrow a book\n");
        printf("2. Return a book\n");
        printf("3. View borrow log\n");
        printf("0. Back\n");
        choice = read_int("> ");
        if (choice == 1)
        {
            list_books(lib);
            int book_id = read_int("Book ID: ");
            list_users(lib);
            int user_id = read_int("User ID: ");
            char borrow_date[DATE_LEN];
            char due_date[DATE_LEN];
            read_string("Borrow date (YYYY-MM-DD): ", borrow_date, DATE_LEN);
            read_string("Due date   (YYYY-MM-DD): ", due_date, DATE_LEN);
            borrow_book(lib, book_id, user_id, borrow_date, due_date);
        }
        else if (choice == 2)
        {
            show_borrow_log(lib);
            int record_id = read_int("Record ID to return: ");
            char return_date[DATE_LEN];
            read_string("Return date (YYYY-MM-DD): ", return_date, DATE_LEN);
            return_book(lib, record_id, return_date);
        }
        else if (choice == 3)
        {
            show_borrow_log(lib);
        }
    } while (choice != 0);
}
void run_menu(Library *lib)
{
    int choice;
    do
    {
        printf("\n======================\n");
        printf("  LIBRARY SYSTEM\n");
        printf("======================\n");
        printf("1. Books\n");
        printf("2. Users\n");
        printf("3. Borrow / Return\n");
        printf("4. Save\n");
        printf("0. Save & Exit\n");
        choice = read_int("> ");
        if (choice == 1)
            menu_books(lib);
        else if (choice == 2)
            menu_users(lib);
        else if (choice == 3)
            menu_borrow(lib);
        else if (choice == 4)
            save_library(lib);
    } while (choice != 0);
}

#endif