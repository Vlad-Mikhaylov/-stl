#ifndef CALCULATOR_H
#define CALCULATOR_H
#include "parser.h"

bool CheckBinary(Token a);

std::vector<Token> TranslateToPolish(const std::vector<Token>& tkn);

void CheckAdequacy(const std::vector<Token>& tkn1);

int CalculateExpression(std::string_view input);

#endif
