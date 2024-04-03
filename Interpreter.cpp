#include "Interpreter.h"
#include <iostream>
#include <regex>
#include <bitset>


ConfigOption currentConfig;

Interpreter::Interpreter(std::ostream& out_stream) : OutStream(out_stream)
{	
	_position = 0;
}

void Interpreter::Consume(const std::string& token, const std::vector<std::string>& tokens)
{
	const std::string nextToken = Peek(tokens);

	if (nextToken == ETX)
	{
		throw std::runtime_error("consumed past last token \n");
	}
	else if (nextToken != token)
	{
		throw std::runtime_error("could not consume token " + token + "\n");
	}

	++_position;
}

void Interpreter::Evaluate(std::vector<std::string>& tokens)
{
	try {
		_position = 0;

		if (tokens[0] == "config") {
			Parse_ConfigStmt(tokens);			
		}
		else if (tokens[0] == "print") {
			int result = Parse_PrintStmt(tokens);
			//std::cout << "Result: " << result << std::endl; // Print the result if needed
		}
		else if (tokens.size() >= 3 && std::regex_match(tokens[0], std::regex("[a-zA-Z][a-zA-Z0-9]*")) && tokens[1] == "=") {
			Parse_AssignStmt(tokens);
		}
		else {
			throw std::runtime_error("Invalid statement");
		}
	}
	catch (const std::runtime_error& e) {
		std::cerr << "Error: " << e.what() << std::endl;
	}
}


void Interpreter::Parse_ConfigStmt(std::vector<std::string>& tokens)
{
	// Check if the first token is "config"
	if (tokens.size() >= 1 && tokens[0] == "config") {
		// Check if there is an optional configuration option
		if (tokens.size() >= 2 && (tokens[1] == "dec" || tokens[1] == "hex" || tokens[1] == "bin")) {
			// Handle the configuration option (dec, hex, or bin)
			std::string configOption = tokens[1];
			if (configOption == "dec") {
				currentConfig = ConfigOption::DECIMAL;
			}
			else if (configOption == "hex") {
				currentConfig = ConfigOption::HEXADECIMAL;
			}
			else if (configOption == "bin") {
				currentConfig = ConfigOption::BINARY;
			}

			// Remove processed tokens from the vector
			tokens.erase(tokens.begin(), tokens.begin() + 2);
		}
	}
	else {
		// Invalid syntax for ConfigStmt
		throw std::runtime_error("Invalid syntax for ConfigStmt");
	}
	
}

int Interpreter::Parse_AssignStmt(std::vector<std::string>& tokens)
{
	if (tokens.size() >= 3 && std::regex_match(tokens[0], std::regex("[a-zA-Z][a-zA-Z0-9]*")) && tokens[1] == "=") {
		std::string variable = tokens[0];

		// Evaluate the right-hand side expression
		tokens.erase(tokens.begin(), tokens.begin() + 2); // Remove variable, "=", and MathExp tokens
		int result = Parse_MathExp(tokens);

		// Store the result in your variables map
		U_MVariables[variable] = result;

		return result;
	}
	else {
		throw std::runtime_error("Invalid syntax for AssgStmt");
	}
}



int Interpreter::Parse_PrintStmt(std::vector<std::string>& tokens)
{
	int result = 0;

	if (tokens.size() >= 2 && tokens[0] == "print") {
		tokens.erase(tokens.begin()); // Remove "print" token

		if (currentConfig == ConfigOption::DECIMAL)
		{
			result = Parse_MathExp(tokens); // Evaluate result if it's DECIMAL
			OutStream << result << std::endl; // Print the result as DECIMAL
		}
		else if (currentConfig == ConfigOption::HEXADECIMAL)
		{
			result = Parse_MathExp(tokens); // Evaluate result if it's HEXADECIMAL
			OutStream << std::hex << std::showbase << result << std::endl; // Print the result as HEXADECIMAL
		}
		else if (currentConfig == ConfigOption::BINARY)
		{
			result = Parse_MathExp(tokens); // Evaluate result if it's BINARY
			OutStream << std::bitset<32>(result).to_string() << std::endl; // Print the result as BINARY
		}

		return result;
	}
	else {
		throw std::runtime_error("Invalid syntax for PrintStmt");
	}
}




int Interpreter::Parse_MathExp(std::vector<std::string>& tokens)
{
	return Parse_SumExp(tokens);
}

int Interpreter::Parse_SumExp(std::vector<std::string>& tokens)
{
	int result = Parse_ProductExp(tokens);

	std::string nextToken = Peek(tokens);

	while (1)
	{
		if (nextToken == "+")
		{
			Consume("+", tokens);
			result = (result + Parse_ProductExp(tokens));
		}
		else if (nextToken == "-")
		{
			Consume("-", tokens);
			result = (result - Parse_ProductExp(tokens));
		}
		else
		{
			break;
		}
		nextToken = Peek(tokens);
	}
	return result;
}

int Interpreter::Parse_ProductExp(std::vector<std::string>& tokens)
{
	int result = Parse_PrimaryExp(tokens);

	std::string nextToken = Peek(tokens);

	while (1)
	{
		if (nextToken == "*")
		{
			Consume("*", tokens);
			result = (result * Parse_PrimaryExp(tokens));
		}
		else if (nextToken == "/")
		{
			Consume("/", tokens);
			result = (result / Parse_PrimaryExp(tokens));
		}
		else
		{
			break;
		}
		nextToken = Peek(tokens);
	}
	return result;
}

int Interpreter::Parse_PrimaryExp(std::vector<std::string>& tokens)
{
	int value;

	std::string nextToken = Peek(tokens);

	
	if(IsInt(nextToken)) // Token = int
	{
		value = std::stoi(nextToken);
		Consume(nextToken, tokens);
	}
	else if(std::regex_match(nextToken, std::regex("[a-zA-Z][a-zA-Z0-9]*"))) // Token = Variable
	{
		std::string variableName = nextToken;
		Consume(nextToken, tokens);

		// Check if the variable exists and get its value
		if (U_MVariables.find(variableName) != U_MVariables.end()) {
			return U_MVariables[variableName];
		}
		else {
			throw std::runtime_error("Variable '" + variableName + "' not defined \n");
		}
	}
	else if(nextToken == "(") // Token = Parentheses
	{
		Consume("(", tokens);
		value = Parse_MathExp(tokens);
		if (Peek(tokens) == ")")
		{
			Consume(")", tokens);
		}
		else
		{
			throw std::runtime_error("Expected ')' \n");
		}
	}
	else
	{
		throw std::runtime_error("No valid primary expression found \n");
	}
	return value;
}

std::string Interpreter::Peek(const std::vector<std::string>& tokens)
{
	return Peek(0, tokens);
}

std::string Interpreter::Peek(int steps, const std::vector<std::string>& tokens)
{
	if (_position + steps >= tokens.size())
		return ETX;


	return tokens[_position + steps];
}

bool Interpreter::IsInt(const std::string& token)
{	
	try 
	{
		std::stoi(token);
		return true;
	}
	catch (...) 
	{
		return false;
	}
	

}
