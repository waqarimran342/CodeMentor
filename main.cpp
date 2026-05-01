/* ----------------------------------------------- */
/*               CodeMentor                        */
/*    "Hot, spicy basics of C++ served daily!"     */
/* ----------------------------------------------- */

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <cctype>
using namespace std;

const int CONCEPT_COUNT = 13;
const int MAX_FACTS = 40;
const int MAX_QUESTIONS = 50;

// Global arrays
string conceptList[CONCEPT_COUNT] = {
    "Variables", "Data_types", "operators", "Input_Output", "If_Else",
    "Loops", "Functions", "Arrays", "Strings", "Pointers", "Structures", "File_Handling", "Case_Switch"
};
string flashcards[MAX_FACTS];
int flashcardCount = 0;

struct User {
    string username;
    string gmail;
    string password;
};

struct MCQ {
    string question;
    string options[4];
    char answer; // 'A', 'B', 'C', or 'D'
};

// Function Prototypes
bool signup();
bool login(User &useraccount);
bool userExists(const string &username);
bool isValidGmail(const string &gmail);
bool isPremium(const string &username);
bool validatePremiumKey(const string &key);
void savePremiumUser(const string &username);
void accessVersion(const User &user);
void clearScreen();
char toLowerChar(char c);
void displayFileContent(const string& filename);
void addBookmark(const string& entry);
void showBookmarks();
void showConceptMenu(const string& folder, const string& typeLabel);
void loadFlashcards();
void showFlashcardsViewer();
void showFreeMenu(const User &user);
void showPremiumMenu(const User &user);
bool isValidChoice(char ch);
char normalizeChoice(char ch);
int loadMCQs(const string& filename, MCQ mcqs[], int maxQuestions);
void saveScore(const string& filename, const string& name, int score, int total);
void showScores(const string& filename);
void takeQuiz(const User &user);
void showQuizMenu();
void showQueryFile(const string& fileName, const string& title);
void askAQueryMenu(const User &user);
string trim(const string &str);

int main() {
    srand(static_cast<unsigned int>(time(0)));
    int choice;
    User activeuser;

    do {
        clearScreen();
        cout << "\n";
        cout << "/* ----------------------------------------------- */\n";
        cout << "/*               CodeMentor++                      */\n";
        cout << "/*    \"Hot, spicy basics of C++ served daily!\"     */\n";
        cout << "/* ----------------------------------------------- */\n\n";

        cout << "-|{ Main Menu }|-: " << endl;
        cout << "1. Sign Up\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                clearScreen();
                if (!signup()) {
                    cout << "Please try logging in instead.\n";
                    system("pause");
                }
                break;
            case 2:
                clearScreen();
                if (login(activeuser)) {
                    accessVersion(activeuser);
                    system("pause");
                }
                break;
            case 3:
                cout << "\nThanks for using CodeMentor++!\n";
                cout << "Keep learning. Keep coding. See you soon!\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
        }
    } while (choice != 3);

    return 0;
}

void clearScreen() {
    system("cls");
}

bool signup() {
    User newuser;
    string confirmpassword;

    cout << "Enter username: ";
    getline(cin, newuser.username);

    if (userExists(newuser.username)) {
        cout << "Username already exists. Please login instead.\n";
        return false;
    }

    cout << "Enter Gmail: ";
    getline(cin, newuser.gmail);
    if (!isValidGmail(newuser.gmail)) {
        cout << "Invalid Gmail. Must contain '@'.\n";
        return false;
    }

    cout << "Enter password: ";
    getline(cin, newuser.password);
    cout << "Confirm password: ";
    getline(cin, confirmpassword);

    if (newuser.password != confirmpassword) {
        cout << "Passwords do not match!\n";
        return false;
    }

    ofstream userfile("user_data/login.txt", ios::app);
    if (!userfile) {
        cout << "Error: Unable to open user_data/login.txt.\n";
        return false;
    }

    userfile << quoted(newuser.username) << " " << newuser.gmail << " " << newuser.password << "\n";
    userfile.close();

    cout << "Sign up successful!\n";
    return true;
}

bool login(User &useraccount) {
    string inputUsername, inputPassword;

    cout << "Enter username (can include spaces): ";
    getline(cin, inputUsername);

    cout << "Password: ";
    getline(cin, inputPassword);

    ifstream infile("user_data/login.txt");
    if (!infile) {
        cout << "Error: Cannot open user_data/login.txt.\n";
        return false;
    }

    string line;
    while (getline(infile, line)) {
        istringstream iss(line);
        string usernameQuoted, gmail, password;

        if (iss >> quoted(usernameQuoted) >> gmail >> password) {
            if (usernameQuoted == inputUsername && password == inputPassword) {
                useraccount.username = usernameQuoted;
                useraccount.gmail = gmail;
                useraccount.password = password;

                cout << "\nLogin successful!\n";
                if (isPremium(usernameQuoted)) {
                    cout << "You are a Premium User!\n";
                } else {
                    cout << "You are currently a Free User.\n";
                }
                return true;
            }
        }
    }

    cout << "Invalid credentials.\n";
    return false;
}

void accessVersion(const User &user) {
    int choice;
    do {
        clearScreen();
        cout << "\n";
        cout << "/* ----------------------------------------------- */\n";
        cout << "/*               CodeMentor++                      */\n";
        cout << "/*    \"Hot, spicy basics of C++ served daily!\"     */\n";
        cout << "/* ----------------------------------------------- */\n\n";

        cout << "-|{ Welcome, " << user.username << "! }|-: " << endl;
        cout << "1. Free Version\n";
        cout << "2. Premium Version\n";
        cout << "3. Back to Signup Page\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                clearScreen();
                showFreeMenu(user);
                system("pause");
                break;
            case 2:
                clearScreen();
                if (isPremium(user.username)) {
                    showPremiumMenu(user);
                } else {
                    cout << "You are not a Premium User.\n";
                    cout << "Would you like to upgrade to Premium? (y/n): ";
                    char upgradeChoice;
                    cin >> upgradeChoice;
                    cin.ignore();

                    if (upgradeChoice == 'y' || upgradeChoice == 'Y') {
                        string key;
                        cout << "Enter your Premium Key: ";
                        getline(cin, key);

                        if (validatePremiumKey(key)) {
                            savePremiumUser(user.username);
                            cout << "Key Validated! You are now a Premium User.\n";
                            showPremiumMenu(user);
                        } else {
                            cout << "Invalid key. Returning to Free Version.\n";
                            showFreeMenu(user);
                        }
                    } else {
                        cout << "Returning to Free Version.\n";
                        showFreeMenu(user);
                    }
                }
                system("pause");
                break;
            case 3:
                cout << "Returning to Signup Page...\n";
                return;
            default:
                cout << "Invalid choice. Please try again.\n";
                system("pause");
        }
    } while (choice != 3);
}

// Converts uppercase to lowercase
char toLowerChar(char c) {
    if (c >= 'A' && c <= 'Z') return c + ('a' - 'A');
    return c;
}

// Display file content
void displayFileContent(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cout << "File not found: " << filename << endl;
        return;
    }
    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }
    file.close();
}

// Add bookmark
void addBookmark(const string& entry) {
    ofstream file("bookmarks/user_bookmarks.txt", ios::app);
    if (file) {
        file << entry << endl;
        cout << "Bookmarked!\n";
    } else {
        cout << "Could not open bookmarks file.\n";
    }
    file.close();
}

// Show bookmarks
void showBookmarks() {
    system("cls");
    cout << "--- Your Bookmarks ---\n";
    displayFileContent("bookmarks/user_bookmarks.txt");
}

// Concept/Code/Real-life examples menu
void showConceptMenu(const string& folder, const string& typeLabel) {
    system("cls");
    cout << "\n--- " << typeLabel << " ---\n";
    for (int i = 0; i < CONCEPT_COUNT; i++)
        cout << i + 1 << ". " << conceptList[i] << endl;
    cout << CONCEPT_COUNT + 1 << ". Back\n";
    cout << "Choose a topic: ";

    int choice;
    while (true) {
        string input;
        getline(cin, input);
        bool valid = true;
        for (char c : input)
            if (c < '0' || c > '9') valid = false;
        if (valid && !input.empty()) {
            choice = stoi(input);
            if (choice >= 1 && choice <= CONCEPT_COUNT + 1) break;
        }
        cout << "Invalid input. Try again: ";
    }

    if (choice <= CONCEPT_COUNT) {
        system("cls");
        string filename = folder + "/" + conceptList[choice - 1] + ".txt";
        displayFileContent(filename);
        cout << "\nBookmark this? (y/n): ";

        char ch;
        while (true) {
            string input;
            getline(cin, input);
            if (input.length() == 1) {
                ch = toLowerChar(input[0]);
                if (ch == 'y' || ch == 'n') break;
            }
            cout << "Invalid input. Try again: ";
        }

        if (ch == 'y') {
            addBookmark(typeLabel + ": " + conceptList[choice - 1]);
        }
    }
}

// Load flashcards
void loadFlashcards() {
    flashcardCount = 0;
    ifstream file("flashcards/Facts.txt");
    string line;
    while (getline(file, line)) {
        if (!line.empty() && flashcardCount < MAX_FACTS)
            flashcards[flashcardCount++] = line;
    }
    file.close();
}

// Flashcards viewer (randomized)
void showFlashcardsViewer() {
    loadFlashcards();
    if (flashcardCount == 0) {
        cout << "No flashcards loaded.\n";
        return;
    }

    srand(time(0));
    int choice;
    do {
        cout << "\nHow many random facts would you like to view (1–" << flashcardCount << "): ";

        int count;
        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                count = stoi(input);
                if (count >= 1 && count <= flashcardCount) break;
            }
            cout << "Invalid input. Try again: ";
        }

        bool used[MAX_FACTS] = { false };
        for (int i = 0; i < count;) {
            int randIndex = rand() % flashcardCount;
            if (!used[randIndex]) {
                used[randIndex] = true;
                cout << "\nFact " << (i + 1) << ": " << flashcards[randIndex] << "\n";
                i++;
            }
        }

        cout << "\nOptions:\n1. View More Random Facts\n2. Back to Menu\n";

        while (true) {
            string input;
            getline(cin, input);
            if (input == "1" || input == "2") {
                choice = stoi(input);
                break;
            }
            cout << "Invalid input. Try again: ";
        }

    } while (choice == 1);
}

// Free Version Menu
void showFreeMenu(const User &user) {
    int option;
    do {
        system("cls");
        cout << "\n";
        cout << "/* ----------------------------------------------- */\n";
        cout << "/*               CodeMentor++                      */\n";
        cout << "/*    \"Hot, spicy basics of C++ served daily!\"     */\n";
        cout << "/* ----------------------------------------------- */\n\n";
        cout << "-|{ Free Version Menu }|-: " << endl;
        cout << "1. Concepts\n";
        cout << "2. Code Examples\n";
        cout << "3. Flashcards\n";
        cout << "4. Real Life Examples\n";
        cout << "5. Bookmarks\n";
        cout << "6. Take a Quiz\n";
        cout << "7. MCQs Game\n";
        cout << "8. Ask a Query\n";
        cout << "9. Exit to Main Menu\n";
        cout << "Enter your choice: ";

        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                option = stoi(input);
                if (option >= 1 && option <= 9) break;
            }
            cout << "Invalid input. Try again: ";
        }

        if (option >= 6 && option <= 8 && !isPremium(user.username)) {
            cout << "This is a Premium feature. Please enter a Premium Key to access: ";
            string key;
            getline(cin, key);
            if (validatePremiumKey(key)) {
                savePremiumUser(user.username);
                cout << "Key Validated! You are now a Premium User.\n";
            } else {
                cout << "Invalid key. Access to this feature is restricted.\n";
                system("pause");
                continue;
            }
        }

        switch (option) {
            case 1: showConceptMenu("concepts", "Concept"); break;
            case 2: showConceptMenu("code_examples", "Code Example"); break;
            case 3: showFlashcardsViewer(); break;
            case 4: showConceptMenu("real_life_examples", "Real Life Example"); break;
            case 5: showBookmarks(); break;
            case 6:
                clearScreen();
                cout << "Accessing Take a Quiz...\n";
                cout << "Welcome, " << user.username << "! This is a Premium feature (Quiz).\n";
                showQuizMenu();
                break;
            case 7:
                clearScreen();
                cout << "Accessing MCQs Game...\n";
                cout << "Welcome, " << user.username << "! This is a Premium feature (MCQs Game).\n";
                takeQuiz(user);
                break;
            case 8:
                clearScreen();
                cout << "Accessing Ask a Query...\n";
                cout << "Welcome, " << user.username << "! This is a Premium feature (Query).\n";
                askAQueryMenu(user);
                break;
            case 9: return;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
    } while (option != 9);
}

// Premium Version Menu
void showPremiumMenu(const User &user) {
    int option;
    do {
        system("cls");
        cout << "\n";
        cout << "/* ----------------------------------------------- */\n";
        cout << "/*               CodeMentor++                      */\n";
        cout << "/*    \"Hot, spicy basics of C++ served daily!\"     */\n";
        cout << "/* ----------------------------------------------- */\n\n";
        cout << "-|{ Premium Version Menu }|-: " << endl;
        cout << "1. Concepts\n";
        cout << "2. Code Examples\n";
        cout << "3. Flashcards\n";
        cout << "4. Real Life Examples\n";
        cout << "5. Bookmarks\n";
        cout << "6. Take a Quiz\n";
        cout << "7. MCQs Game\n";
        cout << "8. Ask a Query\n";
        cout << "9. Exit to Main Menu\n";
        cout << "Enter your choice: ";

        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                option = stoi(input);
                if (option >= 1 && option <= 9) break;
            }
            cout << "Invalid input. Try again: ";
        }

        switch (option) {
            case 1: showConceptMenu("concepts", "Concept"); break;
            case 2: showConceptMenu("code_examples", "Code Example"); break;
            case 3: showFlashcardsViewer(); break;
            case 4: showConceptMenu("real_life_examples", "Real Life Example"); break;
            case 5: showBookmarks(); break;
            case 6:
                clearScreen();
                cout << "Accessing Take a Quiz...\n";
                cout << "Welcome, " << user.username << "! Enjoy the Premium Quiz feature.\n";
                showQuizMenu();
                break;
            case 7:
                clearScreen();
                cout << "Accessing MCQs Game...\n";
                cout << "Welcome, " << user.username << "! Enjoy the Premium MCQs Game feature.\n";
                takeQuiz(user);
                break;
            case 8:
                clearScreen();
                cout << "Accessing Ask a Query...\n";
                cout << "Welcome, " << user.username << "! Enjoy the Premium Query feature.\n";
                askAQueryMenu(user);
                break;
            case 9: return;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
    } while (option != 9);
}

bool userExists(const string &username) {
    ifstream file("user_data/login.txt");
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string u, g, p;
        if (iss >> quoted(u) >> g >> p) {
            if (u == username) return true;
        }
    }
    return false;
}

bool isValidGmail(const string &gmail) {
    return gmail.find('@') != string::npos;
}

bool isPremium(const string &username) {
    ifstream file("user_data/premium.txt");
    string name;
    while (getline(file, name)) {
        if (name == username) return true;
    }
    return false;
}

string trim(const string &str) {
    size_t first = str.find_first_not_of(" \t\n\r");
    if (first == string::npos) return "";
    size_t last = str.find_last_not_of(" \t\n\r");
    return str.substr(first, last - first + 1);
}

bool validatePremiumKey(const string &key) {
    ifstream file("user_data/keys.txt");
    if (!file) {
        cout << "Error: Unable to open user_data/keys.txt. Ensure the file exists.\n";
        return false;
    }

    string inputKey = trim(key);
    string lowerInputKey = inputKey;
    transform(lowerInputKey.begin(), lowerInputKey.end(), lowerInputKey.begin(), ::tolower);

    string line;
    bool hasKeys = false;
    cout << "Debug: Reading keys from user_data/keys.txt:\n";
    while (getline(file, line)) {
        string trimmedLine = trim(line);
        if (trimmedLine.empty()) continue;
        hasKeys = true;
        cout << "Debug: Found key: '" << trimmedLine << "'\n";
        string lowerLine = trimmedLine;
        transform(lowerLine.begin(), lowerLine.end(), lowerLine.begin(), ::tolower);
        if (lowerLine == lowerInputKey) {
            file.close();
            cout << "Debug: Key '" << inputKey << "' matched.\n";
            return true;
        }
    }

    file.close();
    if (!hasKeys) {
        cout << "Error: user_data/keys.txt is empty or contains no valid keys.\n";
    } else {
        cout << "Debug: No matching key found for '" << inputKey << "' in user_data/keys.txt.\n";
    }
    return false;
}

void savePremiumUser(const string &username) {
    ofstream file("user_data/premium.txt", ios::app);
    file << username << "\n";
}

bool isValidChoice(char ch) {
    return (ch == 'A' || ch == 'a' || ch == 'B' || ch == 'b' ||
            ch == 'C' || ch == 'c' || ch == 'D' || ch == 'd');
}

char normalizeChoice(char ch) {
    if (ch >= 'a' && ch <= 'z')
        return ch - ('a' - 'A');
    return ch;
}

int loadMCQs(const string& filename, MCQ mcqs[], int maxQuestions) {
    ifstream file(filename);
    if (!file) {
        cout << "Error opening MCQ file: " << filename << endl;
        exit(1);
    }

    int count = 0;
    string line;
    while (count < maxQuestions && getline(file, line)) {
        mcqs[count].question = line;

        for (int i = 0; i < 4; i++) {
            if (!getline(file, mcqs[count].options[i])) {
                cout << "Error reading options from file." << endl;
                exit(1);
            }
        }

        if (!getline(file, line) || line.length() != 1) {
            cout << "Error reading answer from file." << endl;
            exit(1);
        }
        mcqs[count].answer = line[0];

        count++;
    }
    file.close();
    return count;
}

void saveScore(const string& filename, const string& name, int score, int total) {
    ofstream file(filename, ios::app);
    if (file) {
        file << name << ": " << score << " out of " << total << "\n";
        file.close();
    }
}

void showScores(const string& filename) {
    ifstream scoreFile(filename);
    if (!scoreFile) {
        cout << "No scores recorded yet." << endl;
        return;
    }
    cout << "\n----- Previous Scores -----\n";
    string line;
    bool hasLines = false;
    while (getline(scoreFile, line)) {
        cout << line << endl;
        hasLines = true;
    }
    if (!hasLines) {
        cout << "No scores recorded yet." << endl;
    }
    cout << "---------------------------\n";
    scoreFile.close();
}

void takeQuiz(const User &user) {
    const string MCQ_FILE = "games/mcqs.txt";
    const string SCORE_FILE = "games/score.txt";
    MCQ mcqs[MAX_QUESTIONS];
    int totalQuestions = loadMCQs(MCQ_FILE, mcqs, MAX_QUESTIONS);

    if (totalQuestions < 1) {
        cout << "No questions found!" << endl;
        system("pause");
        return;
    }

    int choice;
    do {
        clearScreen();
        cout << "\n===== C++ Tutor MCQs Menu =====\n";
        cout << "1. Take a Quiz\n";
        cout << "2. Show Scores\n";
        cout << "3. Back to Menu\n";
        cout << "Enter your choice: ";

        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                choice = stoi(input);
                if (choice >= 1 && choice <= 3) break;
            }
            cout << "Invalid input. Try again: ";
        }

        switch (choice) {
            case 1: {
                clearScreen();
                char showPrev;
                do {
                    cout << "Would you like to see previous scores before starting? (Y/N): ";
                    cin >> showPrev;
                    showPrev = normalizeChoice(showPrev);
                    if (showPrev != 'Y' && showPrev != 'N') {
                        cout << "Invalid input. Please enter Y or N." << endl;
                    } else {
                        break;
                    }
                } while (true);

                clearScreen();
                if (showPrev == 'Y') {
                    showScores(SCORE_FILE);
                    cout << "\nPress Enter to continue...";
                    cin.ignore();
                    cin.get();
                }

                int n;
                do {
                    cout << "How many questions would you like to answer? (1 to " << totalQuestions << "): ";
                    string input;
                    getline(cin, input);
                    bool valid = true;
                    for (char c : input)
                        if (c < '0' || c > '9') valid = false;
                    if (valid && !input.empty()) {
                        n = stoi(input);
                        if (n >= 1 && n <= totalQuestions) break;
                    }
                    cout << "Invalid input. Please enter a number between 1 and " << totalQuestions << "." << endl;
                } while (true);

                int chosenIndices[MAX_QUESTIONS] = {0};
                int chosenCount = 0;

                while (chosenCount < n) {
                    int r = rand() % totalQuestions;
                    bool found = false;
                    for (int i = 0; i < chosenCount; i++) {
                        if (chosenIndices[i] == r) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        chosenIndices[chosenCount] = r;
                        chosenCount++;
                    }
                }

                int score = 0;

                for (int i = 0; i < n; i++) {
                    MCQ& q = mcqs[chosenIndices[i]];
                    cout << "\nQuestion " << (i + 1) << ": " << q.question << endl;
                    for (int j = 0; j < 4; j++) {
                        cout << q.options[j] << endl;
                    }

                    char userChoice;
                    do {
                        cout << "Enter your choice (A, B, C, or D): ";
                        cin >> userChoice;
                        if (!isValidChoice(userChoice)) {
                            cout << "Invalid choice. Please enter A, B, C, or D." << endl;
                        } else {
                            break;
                        }
                    } while (true);
                    cin.ignore();

                    userChoice = normalizeChoice(userChoice);

                    if (userChoice == q.answer) {
                        cout << "Correct!" << endl;
                        score++;
                    } else {
                        cout << "Wrong! Correct answer is " << q.answer << "." << endl;
                    }
                }

                cout << "\nQuiz complete! Your score: " << score << " out of " << n << endl;
                saveScore(SCORE_FILE, user.username, score, n);
                break;
            }
            case 2:
                clearScreen();
                showScores(SCORE_FILE);
                break;
            case 3:
                return;
        }

        cout << "\nPress Enter to continue...";
        cin.ignore();
    } while (choice != 3);
}

void showQuizMenu() {
    const string basePath = "quizzes";
    const string quizFolders[3] = {
        "Code_Writing_Practice",
        "Dry_Run_Practice",
        "Error_Analysis_Practice"
    };

    while (true) {
        system("cls");
        cout << "====== Take a Quiz ======\n";
        cout << "1. Code Writing Practice\n";
        cout << "2. Dry Run Practice\n";
        cout << "3. Error Analysis Practice\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";

        int choice;
        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                choice = stoi(input);
                if (choice >= 1 && choice <= 4) break;
            }
            cout << "Invalid input. Try again: ";
        }

        if (choice == 4)
            break;

        system("cls");
        cout << "==== Concepts in " << quizFolders[choice - 1] << " ====\n";
        for (int i = 0; i < CONCEPT_COUNT; ++i) {
            cout << (i + 1) << ". " << conceptList[i] << "\n";
        }

        int conceptChoice;
        cout << "\nSelect a concept number: ";
        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                conceptChoice = stoi(input);
                if (conceptChoice >= 1 && conceptChoice <= CONCEPT_COUNT) break;
            }
            cout << "Invalid input. Try again: ";
        }

        string conceptName = conceptList[conceptChoice - 1];
        string quizFile = basePath + "/" + quizFolders[choice - 1] + "/" + conceptName + ".txt";
        string answerFile = basePath + "/" + quizFolders[choice - 1] + "/" + conceptName + "_Answers.txt";

        system("cls");
        ifstream file(quizFile);
        if (!file) {
            cout << "Quiz file not found: " << quizFile << "\n";
        } else {
            string line;
            cout << "===== Quiz: " << conceptName << " =====\n\n";
            while (getline(file, line)) {
                cout << line << "\n";
            }
            file.close();
        }

        cout << "\nWould you like to view the answers? (y/n): ";
        char seeAnswer;
        cin >> seeAnswer;
        cin.ignore();

        if (seeAnswer == 'y' || seeAnswer == 'Y') {
            ifstream answer(answerFile);
            system("cls");

            if (!answer) {
                cout << "Answer file not found: " << answerFile << "\n";
            } else {
                string line;
                cout << "===== Answers: " << conceptName << " =====\n\n";
                while (getline(answer, line)) {
                    cout << line << "\n";
                }
                answer.close();
            }
        }
    }
}

void showQueryFile(const string& fileName, const string& title) {
    string fullPath = "queries/" + fileName;
    ifstream file(fullPath);

    cout << "\n[" << title << "]\n";

    if (!file) {
        cout << "File not found: " << fullPath << endl;
        return;
    }

    string line;
    while (getline(file, line)) {
        cout << line << endl;
    }

    file.close();
}

void askAQueryMenu(const User &user) {
    int choice;
    do {
        system("cls");
        cout << "\n========== ASK A QUERY ==========\n";
        cout << "1. Best Practices\n";
        cout << "2. FAQs\n";
        cout << "3. Syntax Help\n";
        cout << "4. Go Back\n";
        cout << "Enter your choice (1-4): ";

        while (true) {
            string input;
            getline(cin, input);
            bool valid = true;
            for (char c : input)
                if (c < '0' || c > '9') valid = false;
            if (valid && !input.empty()) {
                choice = stoi(input);
                if (choice >= 1 && choice <= 4) break;
            }
            cout << "Invalid input. Try again: ";
        }

        switch (choice) {
            case 1:
                system("cls");
                showQueryFile("best_practices.txt", "Best Practices");
                break;
            case 2:
                system("cls");
                showQueryFile("faqs.txt", "Frequently Asked Questions");
                break;
            case 3:
                system("cls");
                showQueryFile("syntax_help.txt", "Syntax Help");
                break;
            case 4:
                cout << "Returning to previous menu...\n";
                break;
            default:
                cout << "Invalid choice. Please enter 1–4.\n";
        }

        if (choice != 4) {
            cout << "\nPress Enter to continue...";
            cin.ignore();
        }
    } while (choice != 4);
}
