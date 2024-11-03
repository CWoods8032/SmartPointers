#include <iostream>
#include <memory>
#include <vector>
#include <string>

using namespace std;

// Book class to represent a single book in the library
class Book {
public:
	string title;

	// Constructor that takes the title of the book as an argument
	Book(const string& bookTitle) : title(bookTitle) {
		cout << "Book \"" << title << "\" is deleted.\n";
	}
};

// Library class to manage books using smart pointers
class Library {
private:
	// Vector to hold unique_ptrs to Book objects. Only the library owns these book intances.
	vector<unique_ptr<Book>> books;

public:
	// Function to add a new book to the library
	void addBook(const string& title) {
		cout << "Adding book: " << title << "\n";
		// Use unique_ptr to create new Book, since each book is only owned by the library
		books.push_back(make_unique<Book>(title));
		cout << "Book \"" << title << "\" successfully added to the library.\n";
	}

	void removeBook(const string& title) {
		cout << "Attempting to remove book: " << title << "\n";
		// Find and remove the book with the specified title
		auto it = remove_if(books.begin(), books.end(),
			[&title](const unique_ptr<Book>& book) {
				return book->title == title;
			});

		if (it != books.end()) {
			// Book will be deleted when unique_ptr goes out of scope here
			books.erase(it, books.end());
			cout << "Book \"" << title << "\" removed from the library.\n";
		}
		else {
			cout << "Book \"" << title << "\" not found in the library.\n";
		}
	}

	// Function to show all books currently in the library
	void listBooks() const {
		if (books.empty()) {
			cout << "The library is currently empty.\n";
		}
		else {
			cout << "Listing all books in the library:\n";
			for (const auto& book : books) {
				cout << "- " << book->title << "\n";
			}
		}
	}
};

// User class to demonstrate shared_ptr behavior with book borrowing
class User {
private:
	string name;
	shared_ptr<Book> borrowedBook;

public:
	// Constructor that sets the user's name
	User(const string& userName) : name(userName) {
		cout << "User \"" << name << "\" created.\n";
	}

	//Function to borrow a book (takes a shared_ptr to simulate shared ownership)
	void borrowBook(shared_ptr<Book> book) {
		borrowedBook = book;
		cout << name << " borrowed \"" << book->title << "\".\n";
	}

	// Destructor to notify when user is deleted
	~User() {
		cout << "User \"" << name << "\" is deleted.\n";
	}
};

int main() {
	cout << "Starting Library Program...\n";

	// Create the library object
	Library library;

	// Add some books to the library
	library.addBook("The Catcher in the Rye");
	library.addBook("To Kill a Mockingbird");
	library.addBook("The Swamp Thing");
	library.addBook("Superman");
	library.addBook("Batman");
	library.addBook("Thor");
	library.addBook("Odin");

	// List all books in the library
	library.listBooks();

	// Simulate users borrowing books using shared_ptr
	{
		shared_ptr<Book> sharedBook = make_shared<Book>("The Great Gatsby");

		// Create two users who borrow the same book
		User user1("Chris");
		User user2("Jeff");

		// Both users share the ownership of the same book
		user1.borrowBook(sharedBook);
		user2.borrowBook(sharedBook);

		// sharedBook will go out of scope here after both users are deleted
		// The book will be deleted only after both shared_ptr references are out of scope
	} // End of scope for sharedBook, triggering book deletion

	// Removing a book from the library
	library.removeBook("Odin");

	// List remaining books in the library
	library.listBooks();

	cout << "Ending Library Program...\n";
	return 0;
}