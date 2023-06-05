#include <string>
#include <stdexcept>
#include "expressions.h"
#ifndef PARSER_H
#define PARSER_H

class UnknownSymbolError : public std::runtime_error {
 public:
  UnknownSymbolError(const std::string& symbol = "") : std::runtime_error("UnknownSymbolError: " + symbol) {  //  NOLINT
  }
};

class WrongExpressionError : public std::runtime_error {
 public:
  WrongExpressionError(const std::string& msg = "") : std::runtime_error("WrongExpressionError: " + msg) {  //  NOLINT
  }
};

size_t FindClosingBracket(std::vector<Token> tkn, size_t start, size_t end);

size_t FIndAddNUm(const std::vector<Token>& tokens, size_t& pos);

std::shared_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& pos);

std::shared_ptr<IExpression> ParseAddendum(const std::vector<Token>& tokens, size_t& pos);

std::shared_ptr<IExpression> ParseMultiplier(const std::vector<Token>& tokens, size_t& pos);

bool CheckBinary(Token a);

#endif
