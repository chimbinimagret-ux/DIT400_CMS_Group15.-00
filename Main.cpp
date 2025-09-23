
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm> // For transform() function used in case-insensitive search

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
void showMainMenu();
void loadCourses();
void saveCourses();
void addCourse();
void deleteCourse();
void listCourses();
void searchCourse();    
void updateCourse();    

// Global variable to track login status
bool isLoggedIn = false;
string courseIds[MAX_COURSES];
string titles[MAX_COURSES];
int creditHours[MAX_COURSES];
int courseCount = 0; // Tracks how many courses are currently loaded

int main() {
    loadCourses(); // Load courses when program starts
    
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
                    showMainMenu();
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
}


// Function to convert string to lowercase for case-insensitive search
string toLower(string str) {
    transform(str.begin(), str.end(), str.begin(), ::tolower);
    return str;
}

// Function to search for courses
void searchCourse() {
    if (courseCount == 0) {
        cout << "No courses available to search." << endl;
        return;
    }

    int searchChoice;
    cout << "\n--- Search Courses ---" << endl;
    cout << "1. Search by Course ID" << endl;
    cout << "2. Search by Title (partial match)" << endl;
    cout << "Enter your choice: ";
    cin >> searchChoice;
    cin.ignore(); // Clear the input buffer

    if (searchChoice == 1) {
        // Search by Course ID
        string searchId;
        cout << "Enter Course ID to search: ";
        getline(cin, searchId);

        bool found = false;
        cout << "\nSearch Results:" << endl;
        cout << "ID\tTitle\t\tHours" << endl;
        cout << "--------------------------------" << endl;
        
        for (int i = 0; i < courseCount; i++) {
            if (courseIds[i] == searchId) {
                cout << courseIds[i] << "\t" << titles[i] << "\t\t" << creditHours[i] << endl;
                found = true;
                break; // IDs are unique, so we can break after finding one
            }
        }
        
        if (!found) {
            cout << "No course found with ID: " << searchId << endl;
        }

    } else if (searchChoice == 2) {
        // Search by Title (partial match, case-insensitive)
        string searchTitle;
        cout << "Enter title or part of title to search: ";
        getline(cin, searchTitle);

        string searchTitleLower = toLower(searchTitle);
        bool found = false;
        
        cout << "\nSearch Results:" << endl;
        cout << "ID\tTitle\t\tHours" << endl;
        cout << "--------------------------------" << endl;
        
        for (int i = 0; i < courseCount; i++) {
            string titleLower = toLower(titles[i]);
            if (titleLower.find(searchTitleLower) != string::npos) {
                cout << courseIds[i] << "\t" << titles[i] << "\t\t" << creditHours[i] << endl;
                found = true;
            }
        }
        
        if (!found) {
            cout << "No courses found containing: " << searchTitle << endl;
        }

    } else {
        cout << "Invalid search choice!" << endl;
    }
}

// Function to update a course
void updateCourse() {
    if (courseCount == 0) {
        cout << "No courses available to update." << endl;
        return;
    }

    string targetId;
    cout << "Enter the Course ID to update: ";
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

    // Display current course information
    cout << "\nCurrent Course Details:" << endl;
    cout << "ID: " << courseIds[foundIndex] << endl;
    cout << "Title: " << titles[foundIndex] << endl;
    cout << "Credit Hours: " << creditHours[foundIndex] << endl;

    // Get new details
    string newTitle;
    int newHours;

    cout << "\nEnter new title (or press Enter to keep current): ";
    getline(cin, newTitle);
    
    cout << "Enter new credit hours (1-6, or 0 to keep current): ";
    cin >> newHours;

    // Update fields if new values provided
    if (!newTitle.empty()) {
        titles[foundIndex] = newTitle;
    }
    
    if (newHours != 0) {
        if (newHours < 1 || newHours > 6) {
            cout << "Error: Credit hours must be between 1 and 6. Update cancelled." << endl;
            return;
        }
        creditHours[foundIndex] = newHours;
    }

    // Save changes to file
    saveCourses();
    cout << "Course updated successfully!" << endl;
}

// Main menu function
void showMainMenu() {
    int choice;
    do {
        cout << "\n--- MAIN MENU ---" << endl;
        cout << "1. Add Course" << endl;
        cout << "2. Delete Course" << endl;
        cout << "3. Search Course" << endl; 
        cout << "4. Update Course" << endl; 
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
                searchCourse(); 
                break;
            case 4:
                updateCourse();
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