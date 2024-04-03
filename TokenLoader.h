#pragma once

#ifndef TOKENLOADER_H
#define TOKENLOADER_H

#include <vector>
#include <string>
class TokenLoader
{
public:

	TokenLoader();

	std::vector<std::string> Tokenize(const std::string& fileName);
};
#endif

