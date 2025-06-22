#include <iostream>
#include <fstream>
#include <string>

using namespace std;

// Function to write (overwrite) data to a file
void writeFile(const string& filename) {
        ofstream file(filename); // overwrite mode
    if (!file) {
        cerr << "Error opening file for writing." << endl;
        return;
    }

    string data;
    cout << "Enter text to write (end with a single line containing only 'END'):\n";
    while (getline(cin, data)) {
        if (data == "END") break;
        file << data << endl;
    }

    file.close();
    cout << "Data written successfully.\n";
}

// Function to append data to a file
void appendToFile(const string& filename) {
    ofstream file(filename, ios::app); // append mode
    if (!file) {
        cerr << "Error opening file for appending." << endl;
        return;
    }

    string data;
    cout << "Enter text to append (end with a single line containing only 'END'):\n";
    while (getline(cin, data)) {
        if (data == "END") break;
        file << data << endl;
    }

    file.close();
    cout << "Data appended successfully.\n";
}

// Function to read and display data from a file
void readFile(const string& filename) {
    ifstream file(filename); // read mode
    if (!file) {
        cerr << "Error opening file for reading." << endl;
        return;
    }

    string line;
    cout << "\nContents of file:\n------------------\n";
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
    cout << "------------------\n";
}

int main() {
    string filename;
    int choice;
    cout << "Enter file name (e.g., data.txt): ";
    getline(cin, filename);

    do {
        cout << "\nFile Operation Menu:\n";
        cout << "1. Write to file (overwrite)\n";
        cout << "2. Append to file\n";
        cout << "3. Read file\n";
        cout << "4. Exit\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;
        cin.ignore(); // To handle newline left in input buffer

        switch (choice) {
            case 1: writeFile(filename); break;
            case 2: appendToFile(filename); break;
            case 3: readFile(filename); break;
            case 4: cout << "Exiting...\n"; break;
            default: cout << "Invalid choice. Please try again.\n"; break;
        }

    } while (choice != 4);

    return 0;
}
