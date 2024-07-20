#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include <vector>
#include <algorithm>

using namespace std;

struct User {
    string username;
    string password;
};

// Function to clear the screen
void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Function to pause the screen
void pauseScreen() {
    cout << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

// Function to print the main menu
void printMenu() {
    clearScreen();
    cout << "=============================\n";
    cout << "       Login System\n";
    cout << "=============================\n";
    cout << "1. Register\n";
    cout << "2. Login\n";
    cout << "3. Change Password\n";
    cout << "4. Delete Account\n";
    cout << "5. View All Users (Admin)\n";
    cout << "6. Exit\n";
    cout << "=============================\n";
    cout << "Enter your choice: ";
}

// Function to load users from the file
vector<User> loadUsers() {
    vector<User> users;
    ifstream inFile("users.txt");
    if (inFile.is_open()) {
        User user;
        while (inFile >> user.username >> user.password) {
            users.push_back(user);
        }
        inFile.close();
    }
    return users;
}

// Function to save users to the file
void saveUsers(const vector<User>& users) {
    ofstream outFile("users.txt", ios::trunc);
    if (outFile.is_open()) {
        for (const auto& user : users) {
            outFile << user.username << " " << user.password << endl;
        }
        outFile.close();
    }
}

// Function to register a new user
void registerUser() {
    clearScreen();
    string username, password;
    cout << "=============================\n";
    cout << "       User Registration\n";
    cout << "=============================\n";
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;

    vector<User> users = loadUsers();
    users.push_back({username, password});
    saveUsers(users);

    cout << "Registration successful!\n";
    pauseScreen();
}

// Function to login an existing user
bool loginUser(string& loggedInUser) {
    clearScreen();
    string username, password;
    cout << "=============================\n";
    cout << "          User Login\n";
    cout << "=============================\n";
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    vector<User> users = loadUsers();
    for (const auto& user : users) {
        if (user.username == username && user.password == password) {
            loggedInUser = username;
            return true;
        }
    }

    return false;
}

// Function to change user password
void changePassword(const string& loggedInUser) {
    clearScreen();
    string oldPassword, newPassword;
    cout << "=============================\n";
    cout << "       Change Password\n";
    cout << "=============================\n";
    cout << "Enter your old password: ";
    cin >> oldPassword;
    cout << "Enter your new password: ";
    cin >> newPassword;

    vector<User> users = loadUsers();
    for (auto& user : users) {
        if (user.username == loggedInUser && user.password == oldPassword) {
            user.password = newPassword;
            saveUsers(users);
            cout << "Password changed successfully!\n";
            pauseScreen();
            return;
        }
    }

    cout << "Incorrect old password!\n";
    pauseScreen();
}

// Function to delete user account
void deleteUser(const string& loggedInUser) {
    clearScreen();
    string password;
    cout << "=============================\n";
    cout << "       Delete Account\n";
    cout << "=============================\n";
    cout << "Enter your password to confirm: ";
    cin >> password;

    vector<User> users = loadUsers();
    auto it = remove_if(users.begin(), users.end(), [&](const User& user) {
        return user.username == loggedInUser && user.password == password;
    });

    if (it != users.end()) {
        users.erase(it, users.end());
        saveUsers(users);
        cout << "Account deleted successfully!\n";
        pauseScreen();
        exit(0);
    } else {
        cout << "Incorrect password!\n";
    }

    pauseScreen();
}

// Function to view all users (admin only)
void viewAllUsers() {
    clearScreen();
    string adminPassword;
    cout << "=============================\n";
    cout << "        View All Users\n";
    cout << "=============================\n";
    cout << "Enter admin password: ";
    cin >> adminPassword;

    if (adminPassword == "admin") {
        vector<User> users = loadUsers();
        cout << "Registered Users:\n";
        for (const auto& user : users) {
            cout << "Username: " << user.username << "\n";
        }
    } else {
        cout << "Incorrect admin password!\n";
    }

    pauseScreen();
}

// Main function to display the menu and handle user choices
int main() {
    int choice;
    string loggedInUser;
    bool isRunning = true;

    while (isRunning) {
        printMenu();
        cin >> choice;

        // Input validation
        while (cin.fail()) {
            cin.clear(); // Clear the error flags
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Ignore invalid input
            cout << "Invalid input. Please enter a number: ";
            cin >> choice;
        }

        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (loginUser(loggedInUser)) {
                    cout << "Login successful!\n";
                    pauseScreen();
                } else {
                    cout << "Invalid username or password.\n";
                    pauseScreen();
                }
                break;
            case 3:
                if (!loggedInUser.empty()) {
                    changePassword(loggedInUser);
                } else {
                    cout << "Please login first.\n";
                    pauseScreen();
                }
                break;
            case 4:
                if (!loggedInUser.empty()) {
                    deleteUser(loggedInUser);
                } else {
                    cout << "Please login first.\n";
                    pauseScreen();
                }
                break;
            case 5:
                viewAllUsers();
                break;
            case 6:
                cout << "Are you sure you want to exit? (y/n): ";
                char exitChoice;
                cin >> exitChoice;
                if (exitChoice == 'y' || exitChoice == 'Y') {
                    isRunning = false;
                }
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                pauseScreen();
                break;
        }
    }

    return 0;
}
