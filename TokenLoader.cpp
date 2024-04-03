#include "TokenLoader.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


TokenLoader::TokenLoader()
{
}

std::vector<std::string> TokenLoader::Tokenize(const std::string& fileName)
{
    std::vector<std::string> tokens;
    std::istringstream iss(fileName); // Create an input string stream
    std::string token;
    bool isNegative = false;

    while (iss >> token)
    {

        tokens.push_back(token); // Add the token to the list of tokens

    }

    return tokens; // Return the list of tokens
}
