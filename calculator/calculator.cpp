#include "../tokenize/tokenize.h"
#include "calculator.h"
#include <iostream>
#include <stack>
#include <string_view>

bool CheckBinary(Token a) {
  return (std::holds_alternative<PlusToken>(a) || std::holds_alternative<MinusToken>(a) ||
          std::holds_alternative<MultiplyToken>(a) || std::holds_alternative<DivideToken>(a) ||
          std::holds_alternative<ResidualToken>(a));
}

std::vector<Token> TranslateToPolish(const std::vector<Token>& token) {
  size_t n = token.size();
  std::vector<Token> token_2;
  std::stack<Token> stack;
  if (n == 0) {
    throw WrongExpressionError{};
  }
  for (size_t i = 0; i < n; i++) {
    if (std::holds_alternative<UnknownToken>(token[i])) {
      throw UnknownSymbolError{};
    }
    if (std::holds_alternative<NumberToken>(token[i])) {
      token_2.emplace_back(token[i]);
    } else if (std::holds_alternative<OpeningBracketToken>(token[i])) {
      stack.push(token[i]);
    } else if (std::holds_alternative<ClosingBracketToken>(token[i])) {
      while (!stack.empty()) {
        if (!std::holds_alternative<OpeningBracketToken>(stack.top())) {
          token_2.emplace_back(stack.top());
          stack.pop();
        } else {
          stack.pop();
          break;
        }
      }
    } else if (CheckBinary(token[i])) {
      if (!(std::holds_alternative<PlusToken>(token[i]) || std::holds_alternative<MinusToken>(token[i]))) {
        Token a;
        for (size_t i = 0; i < stack.size(); i++) {
          if (!std::holds_alternative<OpeningBracketToken>(stack.top())) {
            token_2.emplace_back(stack.top());
            stack.pop();
          } else {
            break;
          }
        }
        stack.push(token[i]);
      } else {
        Token a;
        while (!stack.empty()) {
          if (std::holds_alternative<MultiplyToken>(stack.top()) || std::holds_alternative<DivideToken>(stack.top()) ||
              std::holds_alternative<ResidualToken>(stack.top())) {
            token_2.emplace_back(stack.top());
            stack.pop();
          } else {
            break;
          }
        }
        stack.push(token[i]);
      }
    }
  }
  while (!stack.empty()) {
    token_2.emplace_back(stack.top());
    stack.pop();
  }
  size_t size = token_2.size();
  if (size == 0) {
    throw WrongExpressionError{};
  }
  std::vector<Token> token_3(size);
  for (size_t i = 0; i < size; i++) {
    token_3[i] = token_2[size - i - 1];
  }
  token_2.clear();
  return token_3;
}

void CheckAdequacy(const std::vector<Token>& token1) {
  std::vector<Token> token = TranslateToPolish(token1);
  int64_t n = token.size(), brackets = 0, numbers = 1;
  if ((n == 1 && !(std::holds_alternative<NumberToken>(token[0]))) ||
      (n > 1 && (std::holds_alternative<NumberToken>(token[0])))) {
    throw WrongExpressionError{};
  }
  if (n == 1 && std::holds_alternative<NumberToken>(token[0])) {
    return;
  }
  for (int64_t i = 0; i < n; i++) {
    if (std::holds_alternative<UnknownToken>(token[i])) {
      throw UnknownSymbolError{};
    }
    if (CheckBinary(token[i])) {
      numbers++;
    }
    if (std::holds_alternative<NumberToken>(token[i])) {
      numbers--;
    }
    if (std::holds_alternative<OpeningBracketToken>(token[i])) {
      brackets++;
    }
    if (std::holds_alternative<ClosingBracketToken>(token[i])) {
      brackets--;
    }
    if (brackets < 0 || numbers < 0) {
      throw WrongExpressionError{};
    }
  }
  if (numbers != 0 || brackets != 0) {
    throw WrongExpressionError{};
  }
  token.clear();
}

int CalculateExpression(std::string_view expr) {
  std::vector<Token> expression = Tokenize(expr);
  std::vector<Token> expression2 = expression;
  CheckAdequacy(expression2);
  size_t x = 0;
  return (ParseExpression(expression, x))->Calculate();
}
