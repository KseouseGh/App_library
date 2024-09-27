#include "stdlib.h"
#include "stdio.h"
#include "math.h"
#include "iostream"

class Book {
    friend class Hash;
    friend class Library;
private:
    std::string title;
    std::string borrowed_by;
public:
    Book(std::string title) :title(title) {}

    Book() {}

    void set_borrowed_by(std::string user) {
        borrowed_by = user;
    }

    std::string get_title() {
        return title;
    }

    std::string get_user() {
        return borrowed_by;
    }
};

class user {
private:
    std::string name;
public:
    user(std::string name) :name(name) {}

    user() {}

    std::string get_name() {
        return name;
    }
};

struct Node {
    Book info;
    Node* next = nullptr;
};

class Hash {
private:
    Node storage[26];
    int size = 26;
    Node* temp = nullptr;
public:
    friend class Library;
    Hash() {
        for (int i = 0; i < (size); i++) {
            storage[i].info.title = 'A' + i;
        }
    }

    ~Hash() {
        for (int i = 0; i < 26; i++) {
            if (storage[i].next != nullptr) {
                temp = storage[i].next;
                while (storage[i].next != nullptr) {
                    storage[i].next = temp->next;
                    delete temp;
                    temp = storage[i].next;
                }
            }
        }
    }

    int hash(const std::string& title) const {
        return title[0] - 'A';
    }

    Book* find_book(const std::string& title) const {
        int index = hash(title);
        Node* current = storage[index].next;

        while (current != nullptr) {
            if (current->info.title == title) {
                return &current->info;
            }
            current = current->next;
        }

        return nullptr;
    }
};

class Library {
private:
    int MAX_BOOKS;
    int MAX_USERS;
    int books_c;
    int users_c;
    user* users;
    const int MAX_TITLE_LEN = 50;
    Book* books_arr; // Array of books with sort

public:
    Hash book_hash;

    Library() {}

    Library(int maxBooks, int maxUsers) : MAX_BOOKS(maxBooks), MAX_USERS(maxUsers), books_c(0), users_c(0) {
        users = new user[MAX_USERS];
    }

    void add_book(std::string title) {
        if (book_hash.find_book(title) == nullptr) {
            int index = book_hash.hash(title);
            Node* new_book = new Node;
            new_book->info.title = title;
            new_book->next = book_hash.storage[index].next;
            book_hash.storage[index].next = new_book;
            books_c = books_c ++;
        }
        else {
            std::cout << "Book with title '" << title << "' already exists." << std::endl;
        }
    }

    void add_user(std::string name) {
        if (users_c < MAX_USERS) {
            users[users_c++] = user(name);
        }
        else {
            std::cout << "Maximum users already." << std::endl;
        }
    }

    void show_bookshelf() const {
        std::cout << "Library Bookshelf books:" << std::endl;
        for (int i = 0; i < book_hash.size; i++) {
            Node* temp = book_hash.storage[i].next;
            while (temp != nullptr) {
                std::cout << temp->info.get_title() << std::endl;
                temp = temp->next;
            }
        }
        std::cout << "." << std::endl;
    }

    void borrow_book(std::string username, std::string booktitle) const {
        Book* book = book_hash.find_book(booktitle);
        if (book != nullptr && book->get_user().empty()) {
            book->set_borrowed_by(username);
            std::cout << username << " has borrowed " << book->get_title() << "." << std::endl;
        }
        else if (book == nullptr) {
            std::cout << "Book '" << booktitle << "' not found." << std::endl;
        }
        else {
            std::cout << "Book '" << booktitle << "' is already borrowed." << std::endl;
        }
    }

    void show_borrowed() const {
        bool found = false;
        std::cout << "Books borrowed by users:" << std::endl;
        for (int i = 0; i < book_hash.size; i++) {
            Node* temp = book_hash.storage[i].next;
            while (temp != nullptr) {
                if (!temp->info.get_user().empty()) {
                    std::cout << temp->info.get_title() << " - borrowed by: " << temp->info.get_user() << std::endl;
                    found = true;
                }
                temp = temp->next;
            }
        }
        std::cout << "." << std::endl;
        if (!found) {
            std::cout << "No books are currently borrowed" << std::endl;
        }
    }

    void sort_books_by_letter(char letter) const {
        if (letter >= 'a' && letter <= 'z') {
            letter -= 'a' - 'A';
        }
        int index = letter - 'A';
        if (index < 0 || index >= 26) {
            std::cout << "Invalid letter." << std::endl;
            return;
        }
        Node* start = book_hash.storage[index].next;
        int count = 0;
        Node* current = start;
        while (current != nullptr) {
            count++;
            current = current->next;
        }
        if (count == 0) {
            std::cout << "No books starting with letter '" << letter << "'." << std::endl;
            return;
        }
        Book* books = new Book[count];
        current = start;
        for (int i = 0; i < count; i++) {
            books[i] = current->info;
            current = current->next;
        }
        for (int i = 0; i < count - 1; i++) {
            for (int j = 0; j < count - i - 1; j++) {
                if (books[j].get_title() > books[j + 1].get_title()) {
                    Book temp = books[j];
                    books[j] = books[j + 1];
                    books[j + 1] = temp;
                }
            }
        }
        std::cout << "Books starting with letter '" << letter << "':" << std::endl;
        for (int i = 0; i < count; i++) {
            std::cout << books[i].get_title() << std::endl;
        }
        delete[] books;
    }
    
    ~Library() {
        delete[] users;
    }
};

int main() {

    system("chcp 1251");
    Hash hash;
    int c;
    int m1;
    int m2;
    int k;
    std::cout << "Welcome to the Library Bookshelf" << std::endl;
    printf_s("Define max-m count of books in library=");
    scanf_s("%d", &m1);
    printf_s("Define max-m count of users in library=");
    scanf_s("%d", &m2);
    Library obj(m1, m2);
    std::string str;
    std::string strs;
    while (true) {
   printf_s("Enter number to choose function of library\n1-Add book\n2-Add user\n3-Show bookshelf\n4-Borrowing book by user\n5-Show borrowed books list\n6-Sort books by first letter\n");
        scanf_s("%d", &c);
        switch (c) {
        case 1: {
            printf_s("\nEnter title of the book\n");
            std::cin >> str;
            obj.add_book(str);
            continue;
        }
        case 2: {
            printf_s("Enter nickname of the user\n");
            std::cin >> str;
            obj.add_user(str);
            continue;
        }
        case 3: {
            obj.show_bookshelf();
            continue;
        }
        case 4: {
            printf_s("Enter nickname of the user who borrowed concrete book\n");
            std::cin >> str;
            printf_s("Enter book's title\n");
            std::cin >> strs;
            obj.borrow_book(str,strs);
            continue;
        }
        case 5: {
            obj.show_borrowed();
            continue;
        }
        case 6: {
            printf("Enter letter to sort books by\n");
            std::cin >> str;
            obj.sort_books_by_letter(str[0]);
            continue;
        }
        }
    }
    /*
    Library obj(100, 100);
    obj.add_book("Good Omens");
    obj.add_book("The Great Geatsby");
    obj.add_book("The Witcher");
    obj.add_book("Aladin");
    obj.add_user("drochun228");
    obj.borrow_book("drochun228", "The Witcher");
    obj.add_user("milfhunter");
    obj.borrow_book("milfhunter", "The Witcher");
    obj.show_borrowed();
    obj.show_bookshelf();//при запуске консольного приложения через switch реализовать использование функций пользователем с простейшей оболочкой пр-ния
    */
    return 0;
}