#include <iostream>
#include <string>
#include <conio.h>
#include <windows.h>

void printNameBox(const std::string& message, int consoleWidth) {
    int messageLength = message.length();
    int padding = (consoleWidth - messageLength - 2) / 2;
    int extraSpace = (consoleWidth - messageLength - 2) % 2;

    std::cout << std::string(consoleWidth, '*') << std::endl;
    std::cout << "*" << std::string(padding, ' ') << message << std::string(padding + extraSpace, ' ') << "*" << std::endl;
    std::cout << std::string(consoleWidth, '*') << std::endl;
}

int getConsoleWidth() {
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    int columns;

    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    columns = csbi.srWindow.Right - csbi.srWindow.Left + 1;

    return columns;
}

std::string convertToBits(const std::string& input) {
    std::string bits;
    for (char c : input) {
        for (int i = 7; i >= 0; i--) {
            bits += ((c >> i) & 1) ? '1' : '0';
        }
        bits += ' ';
    }
    return bits;
}

std::string convertToAlphabetPositions(const std::string& input) {
    std::string positions;
    for (char c : input) {
        if (isalpha(c)) {
            int position = toupper(c) - 'A' + 1;
            positions += std::to_string(position) + " ";
        }
        else {
            positions += " ";
        }
    }
    return positions;
}

std::string convertToMorse(const std::string& input) {
    // Simplified Morse code conversion (A-Z only)
    std::string morseCodes[] = { ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
                                 "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
                                 "..-", "...-", ".--", "-..-", "-.--", "--.." };
    std::string morse;
    for (char c : input) {
        if (isalpha(c)) {
            morse += morseCodes[toupper(c) - 'A'] + " ";
        }
        else {
            morse += " ";
        }
    }
    return morse;
}

int navigateMenu(const std::string& message) {
    int selectedOption = 0;
    char key;
    const int totalOptions = 3;

    while (true) {
        system("cls");
        int consoleWidth = getConsoleWidth();

        // Re-print the input message (user-provided) at the top
        printNameBox(message, consoleWidth);

        std::cout << "\nOdaberi jednu od opcija i pretvori svoje ime u:\n";

        for (int i = 0; i < totalOptions; i++) {
            if (i == selectedOption) {
                std::cout << "> "; // Highlight selected option
            }
            else {
                std::cout << "  "; // Indent unselected options
            }

            switch (i) {
            case 0: std::cout << "Bitove\n"; break;
            case 1: std::cout << "Redni broj slova u abecedi\n"; break;
            case 2: std::cout << "Morzeovu azbuku\n"; break;
            }
        }

        key = _getch();
        if (key == 72) { // Up arrow key
            selectedOption = (selectedOption - 1 + totalOptions) % totalOptions;
        }
        else if (key == 80) { // Down arrow key
            selectedOption = (selectedOption + 1) % totalOptions;
        }
        else if (key == '\r') { // Enter key
            return selectedOption;
        }
    }
}

int postConversionMenu(const std::string& convertedMessage) {
    int selectedOption = 0;
    char key;

    while (true) {
        system("cls");
        // Display the converted message above the final prompt
        std::cout << "\nRezultat: " << convertedMessage << "\n";
        std::cout << "Odaberi:\n";

        std::cout << "1. Vrati se na izbor konverzije\n";
        std::cout << "2. Promijeni uneseno ime\n";
        std::cout << "3. Zatvori aplikaciju\n";

        key = _getch();
        if (key == '1') {
            return 0; // Return to conversion selection
        }
        else if (key == '2') {
            return 1; // Change input string
        }
        else if (key == '3') {
            return 2; // Close the app
        }
    }
}

int main() {
    std::string input;
    std::string message;
    int consoleWidth;

    while (true) {
        // Step 1: Get user input
        std::cout << "Unesi ime: ";
        std::getline(std::cin, input);

        // Step 2: Prepend "Pozdrav, " to the input string
        message = "Pozdrav, " + input + ".";

        // Step 3: Get the console width
        consoleWidth = getConsoleWidth();

        // Step 4: Display the name surrounded by asterisks at the top
        system("cls");
        printNameBox(message, consoleWidth);

        while (true) {
            // Step 5: Display menu and let user select an option
            int selectedOption = navigateMenu(message);

            // Step 6: Perform the selected conversion
            std::string convertedMessage;
            switch (selectedOption) {
            case 0:
                convertedMessage = convertToBits(input);
                break;
            case 1:
                convertedMessage = convertToAlphabetPositions(input);
                break;
            case 2:
                convertedMessage = convertToMorse(input);
                break;
            }

            // Step 7: Clear console and show the final result under the name
            // Already handled in postConversionMenu
            // Print the message box again to show the name
            system("cls");
            printNameBox(message, consoleWidth);

            // Step 8: Show post-conversion menu with converted message
            int postOption = postConversionMenu(convertedMessage);
            if (postOption == 0) {
                // Go back to the conversion selection
                continue;
            }
            else if (postOption == 1) {
                // Change the input string
                break; // Exit to the main input loop to enter a new name
            }
            else if (postOption == 2) {
                // Close the app
                return 0;
            }
        }
    }
}
