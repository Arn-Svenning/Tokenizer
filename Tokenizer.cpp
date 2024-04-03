// Labb6CPP.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "Interpreter.h"
#include "TokenLoader.h"
#include <sstream>
#include <fstream>

int main() 
{
    std::ifstream inputFile("tokens.txt"); // Open the input file
    if (!inputFile.is_open()) {
        std::cerr << "Failed to open input file." << std::endl;
        return 1;
    }

    Interpreter interpreter(std::cout); // Create an instance of the Interpreter
    TokenLoader tokenLoader;

    std::string line;
    while (std::getline(inputFile, line)) {
        // Tokenize each line
        std::vector<std::string> tokens = tokenLoader.Tokenize(line);

        if (!tokens.empty()) {
            try {
                // Evaluate the tokens using your friend's code
                interpreter.Evaluate(tokens);
            }
            catch (const std::runtime_error& e) {
                std::cerr << "Error: " << e.what() << std::endl;
            }
        }
    }

    inputFile.close(); // Close the input file when done

    return 0;
}



