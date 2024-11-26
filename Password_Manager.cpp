#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

// PasswordEntry Class
class PasswordEntry {
private:
    string website;
    string username;
    string password;
    string verificationQuestion;
    string verificationAnswer;

public:
    // Constructor
    PasswordEntry(const string& site = "", const string& user = "", const string& pass = "",
                  const string& question = "", const string& answer = "")
        : website(site), username(user), password(pass),
          verificationQuestion(question), verificationAnswer(answer) {}

    // Getters
    string getWebsite() const { return website; }
    string getUsername() const { return username; }
    string getPassword() const { return password; }
    string getVerificationQuestion() const { return verificationQuestion; }
    string getVerificationAnswer() const { return verificationAnswer; }

    // Setters
    void setUsername(const string& user) { username = user; }
    void setPassword(const string& pass) { password = pass; }

    // Display function
    void display() const {
        cout << "Website: " << website << "\nUsername: " << username << "\n";
    }
};

// PasswordManager Class
class PasswordManager {
private:
    vector<PasswordEntry> entries;
    const string filename = "passwords.txt";

    // Load entries from file
    void loadFromFile() {
        ifstream file(filename);
        if (file.is_open()) {
            entries.clear();
            string website, username, password, question, answer;
            while (file >> website >> username >> password) {
                file.ignore(); // Ignore leftover newline
                getline(file, question);
                getline(file, answer);
                entries.emplace_back(website, username, password, question, answer);
            }
            file.close();
        }
    }

    // Save entries to file
    void saveToFile() const {
        ofstream file(filename, ios::trunc);
        if (file.is_open()) {
            for (const auto& entry : entries) {
                file << entry.getWebsite() << " "
                     << entry.getUsername() << " "
                     << entry.getPassword() << "\n"
                     << entry.getVerificationQuestion() << "\n"
                     << entry.getVerificationAnswer() << "\n";
            }
            file.close();
        }
    }

public:
    PasswordManager() {
        loadFromFile();
    }

    ~PasswordManager() {
        saveToFile();
    }

    void addEntry() {
        string site, user, pass, question, answer;

        cout << "Enter website: ";
        cin >> site;
        cout << "Enter username: ";
        cin >> user;

        // Check for duplicate entry
        for (const auto& existingEntry : entries) {
            if (existingEntry.getWebsite() == site && existingEntry.getUsername() == user) {
                cout << "An entry for this website and username already exists.\n";
                return;
            }
        }

        cout << "Enter password: ";
        cin >> pass;
        cin.ignore(); // Ignore leftover newline

        cout << "Enter a security question: ";
        getline(cin, question);

        cout << "Enter the answer to the security question: ";
        getline(cin, answer);

        PasswordEntry entry(site, user, pass, question, answer);
        entries.push_back(entry);
        saveToFile();
        cout << "Entry added successfully.\n";
    }

    void displayEntries() const {
        if (entries.empty()) {
            cout << "No entries found.\n";
            return;
        }
        cout << "Stored Passwords:\n";
        for (const auto& entry : entries) {
            entry.display();
            cout << "\n";
        }
    }

    void searchEntry(const string& website) const {
        vector<const PasswordEntry*> matchingEntries;
        for (const auto& entry : entries) {
            if (entry.getWebsite() == website) {
                matchingEntries.push_back(&entry);
            }
        }

        if (matchingEntries.empty()) {
            cout << "No entry found for the website: " << website << endl;
            return;
        }

        cout << "Entries for website: " << website << "\n";
        for (size_t i = 0; i < matchingEntries.size(); ++i) {
            cout << i + 1 << ". Username: " << matchingEntries[i]->getUsername() << "\n";
        }

        // Ask if the user wants to view a password
        cout << "Would you like to view a password? (yes/no): ";
        string choice;
        cin >> choice;
        if (choice == "yes" || choice == "Yes") {
            cout << "Enter the number of the entry: ";
            int index;
            cin >> index;

            if (index < 1 || index > static_cast<int>(matchingEntries.size())) {
                cout << "Invalid choice.\n";
                return;
            }

            const auto& selectedEntry = *matchingEntries[index - 1];
            cout << selectedEntry.getVerificationQuestion() << ": ";
            string answer;
            cin.ignore();
            getline(cin, answer);

            if (answer == selectedEntry.getVerificationAnswer()) {
                cout << "Password: " << selectedEntry.getPassword() << "\n";
            } else {
                cout << "Verification failed. Cannot display password.\n";
            }
        }
    }

    void modifyEntry(const string& website) {
        string username, answer;

        cout << "Enter the username to modify: ";
        cin >> username;

        for (auto& entry : entries) {
            if (entry.getWebsite() == website && entry.getUsername() == username) {
                cout << entry.getVerificationQuestion() << ": ";
                getline(cin >> ws, answer); // Read input with spaces if needed

                if (answer == entry.getVerificationAnswer()) {
                    string newUser, newPass;
                    cout << "Verification successful.\n";
                    cout << "Enter new username: ";
                    cin >> newUser;
                    cout << "Enter new password: ";
                    cin >> newPass;

                    entry.setUsername(newUser);
                    entry.setPassword(newPass);
                    saveToFile();
                    cout << "Entry updated successfully.\n";
                } else {
                    cout << "Verification failed. Entry not modified.\n";
                }
                return;
            }
        }
        cout << "No entry found for the website and username combination.\n";
    }

    void deleteEntry(const string& website, const string& username) {
        for (auto it = entries.begin(); it != entries.end(); ++it) {
            if (it->getWebsite() == website && it->getUsername() == username) {
                string pass;
                cout << "Enter the password to confirm deletion: ";
                cin >> pass;

                if (it->getPassword() == pass) {
                    entries.erase(it);
                    saveToFile();
                    cout << "Entry deleted successfully.\n";
                } else {
                    cout << "Password verification failed. Entry not deleted.\n";
                }
                return;
            }
        }
        cout << "No entry found for the website and username provided.\n";
    }
};

// Main Function
int main() {
    PasswordManager manager;
    int choice;
    string website, username;

    do {
        cout << "\nPassword Manager\n";
        cout << "1. Add Entry\n";
        cout << "2. Display Entries\n";
        cout << "3. Search Entry\n";
        cout << "4. Modify Entry\n";
        cout << "5. Delete Entry\n";
        cout << "6. Exit\n";
        cout << "Choose an option: ";
        cin >> choice;

        switch (choice) {
            case 1:
                manager.addEntry();
                break;
            case 2:
                manager.displayEntries();
                break;
            case 3:
                cout << "Enter website to search: ";
                cin >> website;
                manager.searchEntry(website);
                break;
            case 4:
                cout << "Enter website to modify: ";
                cin >> website;
                manager.modifyEntry(website);
                break;
            case 5:
                cout << "Enter website to delete: ";
                cin >> website;
                cout << "Enter username to delete: ";
                cin >> username;
                manager.deleteEntry(website, username);
                break;
            case 6:
                cout << "Exiting Password Manager.\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                break;
        }
    } while (choice != 6);

    return 0;
}