

#include <iostream>
#include <fstream>
#include <string> 
#include <wchar.h>

using namespace std;

// Constants
const int MAX_USERS = 100;
const int MAX_COURSES = 1000;
const string USER_FILE = "users.txt";
const string COURSE_FILE = "courses.txt";

// Function Prototypes
bool loginUser();
void registerUser();
int readUserFile(string usernames[], string passwords[]);
void writeUserFile(string usernames[], string passwords[], int userCount);
void showMainMenu(); // Placeholder for now

// Global variable to track login status
bool isLoggedIn = false;

int main() {
    int choice;

    cout << "*** Course Management System ***" << endl;

    do {
        cout << "\n1. Login" << endl;
        cout << "2. Register" << endl;
        cout << "3. Exit" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                isLoggedIn = loginUser();
                if (isLoggedIn) {
                    showMainMenu(); // This will be built by others
                }
                break;
            case 2:
                registerUser();
                break;
            case 3:
                cout << "Goodbye!" << endl;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 3 && !isLoggedIn);

    return 0;
}

// Function to read users from the file
int readUserFile(string usernames[], string passwords[]) {
    ifstream file(USER_FILE);
    int count = 0;
    string line;

    if (file.is_open()) {
        while (getline(file, line) && count < MAX_USERS) {
            size_t commaPos = line.find(',');
            if (commaPos != string::npos) {
                usernames[count] = line.substr(0, commaPos);
                passwords[count] = line.substr(commaPos + 1);
                count++;
            }
        }
        file.close();
    }
    return count;
}

// Function to save users to the file
void writeUserFile(string usernames[], string passwords[], int userCount) {
    ofstream file(USER_FILE);
    if (file.is_open()) {
        for (int i = 0; i < userCount; i++) {
            file << usernames[i] << "," << passwords[i] << endl;
        }
        file.close();
    } else {
        cout << "Error: Could not open user file for writing!" << endl;
    }
}

// User Registration function
void registerUser() {
    string usernames[MAX_USERS];
    string passwords[MAX_USERS];
    int userCount = readUserFile(usernames, passwords);

    string username, password;
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    // Check for duplicate username
    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username) {
            cout << "Username already exists! Registration failed." << endl;
            return;
        }
    }

    // Add new user to arrays
    if (userCount < MAX_USERS) {
        usernames[userCount] = username;
        passwords[userCount] = password;
        userCount++;
        writeUserFile(usernames, passwords, userCount);
        cout << "Registration successful!" << endl;
    } else {
        cout << "Maximum user limit reached. Cannot register." << endl;
    }
}

// User Login function
bool loginUser() {
    string usernames[MAX_USERS];
    string passwords[MAX_USERS];
    int userCount = readUserFile(usernames, passwords);

    string username, password;
    cout << "Enter username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    for (int i = 0; i < userCount; i++) {
        if (usernames[i] == username && passwords[i] == password) {
            cout << "Login successful! Welcome, " << username << "!" << endl;
            return true;
        }
    }
    cout << "Login failed! Invalid username or password." << endl;
    return false;
}

// Placeholder for the main menu (to be filled by Members B and C)
void showMainMenu() {
    cout << "\n--- MAIN MENU ---" << endl;
    cout << "This is where the course management features will be." << endl;
    cout << "For now, this is the end of the demo. Logging out." << endl;
    isLoggedIn = false; // Simulate a logout
}