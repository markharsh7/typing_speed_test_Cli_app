#include <iostream>
#include <ctime>
#include <conio.h>
#include <windows.h>  // For console manipulation
#include <string>
#include <fstream>
#include <filesystem>

using namespace std;

std::string readFileContent(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open file: " << filePath << std::endl;
        return "heloo world how are you";
    }

    // Check if the file extension is .txt
    if (std::filesystem::path(filePath).extension() != ".txt"){
        std::cerr << "Error: File is not a text file: " << filePath << std::endl;
        return "holw world how are you";
    }

    std::string content;
    std::string line;
    while (std::getline(file, line)) {
        content += line + "\n";
    }

    file.close();
    return content;

}


void gotoxy(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void clearLine(int y) {
    gotoxy(0, y);
    string spaces(120, ' '); // Adjust width as needed
    cout << spaces;
    gotoxy(0, y);
}

void spaces() {
    cout << "\n\n\n";
}

string firstScreen() {
    system("cls"); // Only clear once at the start
    char ch = 1;
    string name;
    spaces();
    cout << "\t\t\t\t    WELCOME TO THE TYPING SPEED TEST\n";
    spaces();
    cout << "\t\t\t--------------------------------------------------------------------\n";
    cout << "\t\t\t|   Practice and improve your Typing speed with this tool " << ch << "               |\n";
    cout << "\t\t\t|   Enter you first name, location of txt file that you wish to type :                   |\n";
    cout << "\t\t\t--------------------------------------------------------------------\n\t\t\t\t\t\t";
    cin >> name;
    string holder;
    cin>>holder;
    string content=readFileContent(holder);
    system("cls"); // Clear once before starting the test
    return name, content;
}

void paragraph(string name , string content) {
    gotoxy(0, 0);
    cout << "\n\t\t\t\t\t\t\t\t   WELCOME " << name << "\n";
    spaces();
    cout << "\t\t\t\t\t\t\t\tTYPING SPEED TEST\n";
    spaces();
    cout << content;
    spaces();
}

void updateStats(int totalWords, int totalLetters, int linePosition) {
    clearLine(linePosition);
    cout << "\n\nTotal Letters = " << totalLetters - 1;
    clearLine(linePosition + 2);
    cout << "\n\nTotal Words = " << totalWords;
}

void updateTime(int minutes, int seconds, int linePosition) {
    clearLine(linePosition);
    cout << "Time passed  : " << minutes << " : " << seconds;
}

void userStatus(int totalWords, int totalLetters, int seconds, string name, string content) {
    system("cls"); // Final stats can clear screen once
    paragraph(name, content);
    cout << "\n\t\t\t\t\t\t\t\t   " << name << " STATS "
         << "\n\n";
    cout << "\t\t\t\t\t\t\t\tTOTAL WORDS   : " << totalWords << "\n";
    cout << "\t\t\t\t\t\t\t\tTOTAL LETTERS : " << totalLetters << "\n";
    cout << "\t\t\t\t\t\t\t\tTOTAL TIME    : " << seconds / 60 << ":" << seconds % 60 << "\n";
    cout << "\t\t\t\t\t\t\t\tWords/minute  : " << ((totalWords *60) / (seconds )) << "\n\n\n";
}

int UI(char &currentLetter, string userName, string content) {
    paragraph(userName, content);
    cout << "To start the Test, press enter \'S-key\' \n";
    currentLetter = getch();
    currentLetter = tolower(currentLetter);
    while (currentLetter != 's') {
        cout << "Please press the valid key : \n";
        currentLetter = getch();
    }
    cout << "\t\t\t YOUR TEST STARTS, start typing \n\n\n";
    return time(0);
}

int timePassedAway(int &startTime) {
    int currentTime = time(0);
    int sec = currentTime - startTime;
    static int min;
    if (sec > 59) {
        min++;
        startTime = time(0);
        sec = 0;
    }
    updateTime(min, sec, 20); // Adjust line position as needed
    return (min * 60) + sec;
}

void Calcualting_completeSentence(char *&str, const char currentLetter, int &totalLetters, int &totalWords) {
    if (currentLetter != 8) {
        char *temp = new char[totalLetters + 1];
        for (int i = 0; str[i] != '\0'; i++) {
            temp[i] = str[i];
        }
        temp[totalLetters - 1] = currentLetter;
        temp[totalLetters] = '\0';
        delete[] str;
        str = temp;
        totalLetters++;
    }
    if (currentLetter == 8 && totalLetters > 1) {
        char *temp = new char[totalLetters - 1];
        for (int i = 0; i < totalLetters - 2; i++) {
            temp[i] = str[i];
        }
        temp[totalLetters - 2] = '\0';
        delete[] str;
        str = temp;
        totalLetters--;
    }

    totalWords = 0;
    for (int i = 0; str[i] != '\0'; i++) {
        if (i > 0 && str[i - 1] != 32 && str[i] == 32) {
            totalWords++;
        }
    }
}

void displayingSentences(char &currentLetter, char *&str, int &totalLetters, int &startTime, string userName, string content) {
    paragraph(userName, content);
    int typingLine = 25; // Adjust based on your layout

    do {
        int totalWords = 0;
        currentLetter = getch();
        if ((currentLetter >= 32 && currentLetter <= 126) || currentLetter == 13 || currentLetter == 8) {
            Calcualting_completeSentence(str, currentLetter, totalLetters, totalWords);

            updateStats(totalWords, totalLetters, 15); // Adjust line position as needed
            timePassedAway(startTime);

            // Clear and update typing area
            clearLine(typingLine);
            gotoxy(0, typingLine);
            for (int i = 0; str[i] != '\0'; i++) {
                cout << str[i];
            }

            gotoxy(0, typingLine + 5);
            cout << "To END : PRESS ENTER-KEY";
            gotoxy(totalLetters - 1, typingLine);
        }

        if (currentLetter == 13) {
            userStatus(totalWords, totalLetters, timePassedAway(startTime), userName, content);
        }
    } while (currentLetter != 13);
}

int main() {
    // Set up console
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hConsole, &cursorInfo);
    cursorInfo.bVisible = true; // Show cursor
    SetConsoleCursorInfo(hConsole, &cursorInfo);

    int totalLetters = 1;
    char *str = new char[totalLetters]{'\0'};
    char currentLetter;

    string userName,content = firstScreen();
    int startTime = UI(currentLetter, userName,content);

    displayingSentences(currentLetter, str, totalLetters, startTime, userName,content);

    delete[] str;
    str = nullptr;

    return 0;
}
