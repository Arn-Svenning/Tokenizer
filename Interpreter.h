#pragma once
#include <ostream>
#include <vector>
#include <unordered_map>

#ifndef INTERPRETER_H
#define INTERPRETER_H

extern enum class ConfigOption
{
	DECIMAL,
	HEXADECIMAL,
	BINARY
};

class Interpreter
{
public:
	
	std::ostream& OutStream;

	Interpreter(std::ostream& out_stream);


#pragma region FUNCTIONS

public:

	void Evaluate(std::vector<std::string>& tokens);

protected:

private:

	void Consume(const std::string& token, const std::vector<std::string>& tokens); 

	void Parse_ConfigStmt(std::vector<std::string>& tokens);
	int Parse_AssignStmt(std::vector<std::string>& tokens);
	int Parse_PrintStmt(std::vector<std::string>& tokens);

	int Parse_MathExp(std::vector<std::string>& tokens);
	int Parse_SumExp(std::vector<std::string>& tokens);
	int Parse_ProductExp(std::vector<std::string>& tokens);
	int Parse_PrimaryExp(std::vector<std::string>& tokens);


	std::string Peek(const std::vector<std::string>& tokens);
	std::string Peek(int steps, const std::vector<std::string>& tokens);

	bool IsInt(const std::string& token);


#pragma endregion

#pragma region VARIABLES

public:

protected:

private:

	std::unordered_map<std::string, int> U_MVariables;

	const std::string ETX = "\u0003";

	int _position;

#pragma endregion



};
#endif
