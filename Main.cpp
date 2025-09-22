

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
void loadCourses();
void saveCourses();
void addCourse();
void deleteCourse();
void listCourses();

// Global variable to track login status
bool isLoggedIn = false;
string courseIds[MAX_COURSES];
string titles[MAX_COURSES];
int creditHours[MAX_COURSES];
int courseCount = 0; // Tracks how many courses are currently loaded


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

// Function to load courses from courses.txt into arrays
void loadCourses() {
    ifstream file(COURSE_FILE);
    string line;
    courseCount = 0; // Reset count

    if (file.is_open()) {
        while (getline(file, line) && courseCount < MAX_COURSES) {
            size_t commaPos1 = line.find(',');
            size_t commaPos2 = line.find(',', commaPos1 + 1);

            if (commaPos1 != string::npos && commaPos2 != string::npos) {
                courseIds[courseCount] = line.substr(0, commaPos1);
                titles[courseCount] = line.substr(commaPos1 + 1, commaPos2 - commaPos1 - 1);
                creditHours[courseCount] = stoi(line.substr(commaPos2 + 1)); // Convert string to int
                courseCount++;
            }
        }
        file.close();
        cout << "Loaded " << courseCount << " courses." << endl;
    } else {
        cout << "No existing course file found. A new one will be created." << endl;
    }
}

// Function to save courses from arrays to courses.txt
void saveCourses() {
    ofstream file(COURSE_FILE);
    if (file.is_open()) {
        for (int i = 0; i < courseCount; i++) {
            file << courseIds[i] << "," << titles[i] << "," << creditHours[i] << endl;
        }
        file.close();
        cout << "Courses saved successfully!" << endl;
    } else {
        cout << "Error: Could not open course file for writing!" << endl;
    }
}

// Function to add a new course
void addCourse() {
    if (courseCount >= MAX_COURSES) {
        cout << "Error: Maximum course limit reached!" << endl;
        return;
    }

    string newId, newTitle;
    int newHours;

    cout << "Enter Course ID: ";
    cin.ignore(); // Clears the input buffer
    getline(cin, newId);
    cout << "Enter Course Title: ";
    getline(cin, newTitle);
    cout << "Enter Credit Hours (1-6): ";
    cin >> newHours;

    // Validate credit hours
    if (newHours < 1 || newHours > 6) {
        cout << "Error: Credit hours must be between 1 and 6." << endl;
        return;
    }

    // Check for duplicate course ID
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == newId) {
            cout << "Error: A course with ID '" << newId << "' already exists!" << endl;
            return;
        }
    }

    // If all checks pass, add the new course to arrays
    courseIds[courseCount] = newId;
    titles[courseCount] = newTitle;
    creditHours[courseCount] = newHours;
    courseCount++;

    // Save the updated arrays to file
    saveCourses();
    cout << "Course added successfully!" << endl;
}

// Function to list all courses
void listCourses() {
    if (courseCount == 0) {
        cout << "No courses available." << endl;
        return;
    }
    cout << "\n--- List of All Courses ---" << endl;
    cout << "ID\tTitle\t\tHours" << endl;
    cout << "--------------------------------" << endl;
    for (int i = 0; i < courseCount; i++) {
        cout << courseIds[i] << "\t" << titles[i] << "\t\t" << creditHours[i] << endl;
    }
}

// Function to delete a course by ID
void deleteCourse() {
    if (courseCount == 0) {
        cout << "No courses available to delete." << endl;
        return;
    }

    string targetId;
    cout << "Enter the Course ID to delete: ";
    cin.ignore();
    getline(cin, targetId);

    int foundIndex = -1;
    // Search for the course
    for (int i = 0; i < courseCount; i++) {
        if (courseIds[i] == targetId) {
            foundIndex = i;
            break;
        }
    }

    if (foundIndex == -1) {
        cout << "Error: Course with ID '" << targetId << "' not found." << endl;
        return;
    }

    // Confirm deletion
    cout << "Are you sure you want to delete '" << titles[foundIndex] << "'? (y/n): ";
    char confirm;
    cin >> confirm;
    if (confirm != 'y' && confirm != 'Y') {
        cout << "Deletion cancelled." << endl;
        return;
    }

    // Shift all elements after the found index one position left
    for (int i = foundIndex; i < courseCount - 1; i++) {
        courseIds[i] = courseIds[i + 1];
        titles[i] = titles[i + 1];
        creditHours[i] = creditHours[i + 1];
    }
    courseCount--; // Decrease the count

    // Save the updated arrays to file
    saveCourses();
    cout << "Course deleted successfully!" << endl;
}int main() {
    loadCourses(); // <- ADD THIS LINE to load courses on startup

    int choice;
    cout << "*** Course Management System ***" << endl;
    // ... rest of existing code
}void showMainMenu() {
    int choice;
    do {
        cout << "\n--- MAIN MENU ---" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Delete Course" << endl;
        cout << "3. Search Course" << endl; // Placeholder for Member C
        cout << "4. Update Course" << endl; // Placeholder for Member C
        cout << "5. List All Courses" << endl;
        cout << "6. Logout" << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                addCourse();
                break;
            case 2:
                deleteCourse();
                break;
            case 3:
                cout << "Search function coming soon! (Member C)" << endl;
                break;
            case 4:
                cout << "Update function coming soon! (Member C)" << endl;
                break;
            case 5:
                listCourses();
                break;
            case 6:
                cout << "Logging out..." << endl;
                isLoggedIn = false;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 6 && isLoggedIn);
}
