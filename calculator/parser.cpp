#include "parser.h"
#include <iostream>
#include <string_view>

size_t FindAddNum(const std::vector<Token>& tokens, size_t position) {
  size_t i = position;
  while (i < tokens.size()) {
    if (i + 1 < tokens.size() &&
        (!(std::holds_alternative<PlusToken>(tokens[i + 1])) && !(std::holds_alternative<MinusToken>(tokens[i + 1])) &&
         !(std::holds_alternative<ClosingBracketToken>(tokens[i + 1])))) {
      ++i;
    } else {
      return i;
    }
    if (i == tokens.size()) {
      return i;
    }
  }
  return i;
}

size_t FindClosingBracket(std::vector<Token> tkn, size_t start, size_t end) {
  int64_t result, brackets = 0;
  for (size_t i = start; i < end; i++) {
    if (std::holds_alternative<OpeningBracketToken>(tkn[i])) {
      brackets++;
    }
    if (std::holds_alternative<ClosingBracketToken>(tkn[i])) {
      brackets--;
      if (brackets == -1) {
        result = i;
        break;
      }
    }
  }
  return result;
}

std::shared_ptr<IExpression> ParseExpression(const std::vector<Token>& tokens, size_t& position) {
  std::shared_ptr<IExpression> oper = ParseAddendum(tokens, position);
  while (position < tokens.size()) {
    if (std::holds_alternative<PlusToken>(tokens[position])) {
      ++position;
      std::shared_ptr<IExpression> ptr_1 = ParseAddendum(tokens, position);
      Sum b(std::move(oper), std::move(ptr_1));
      oper = std::make_unique<Sum>(b);
    } else if (std::holds_alternative<MinusToken>(tokens[position])) {
      ++position;
      std::shared_ptr<IExpression> ptr_1 = ParseAddendum(tokens, position);
      Subtract b(std::move(oper), std::move(ptr_1));
      oper = std::make_unique<Subtract>(b);
    } else if (std::holds_alternative<ClosingBracketToken>(tokens[position])) {
      ++position;
      return oper;
    }
  }
  return oper;
}

std::shared_ptr<IExpression> ParseAddendum(const std::vector<Token>& tokens, size_t& position) {
  std::shared_ptr<IExpression> oper = ParseMultiplier(tokens, position);
  while (position < tokens.size()) {
    if (std::holds_alternative<PlusToken>(tokens[position]) || std::holds_alternative<MinusToken>(tokens[position]) ||
        std::holds_alternative<ClosingBracketToken>(tokens[position])) {
      return oper;
    }
    if (std::holds_alternative<MultiplyToken>(tokens[position])) {
      ++position;
      std::shared_ptr<IExpression> ptr_1 = ParseMultiplier(tokens, position);
      Multiply c(std::move(oper), std::move(ptr_1));
      oper = std::make_unique<Multiply>(c);
    } else if (std::holds_alternative<DivideToken>(tokens[position])) {
      ++position;
      std::shared_ptr<IExpression> ptr_1 = ParseMultiplier(tokens, position);
      Divide c(std::move(oper), std::move(ptr_1));
      oper = std::make_unique<Divide>(c);
    } else if (std::holds_alternative<ResidualToken>(tokens[position])) {
      ++position;
      std::shared_ptr<IExpression> ptr_1 = ParseMultiplier(tokens, position);
      Residual c(std::move(oper), std::move(ptr_1));
      oper = std::make_unique<Residual>(c);
    }
  }
  return oper;
}

std::shared_ptr<IExpression> ParseMultiplier(const std::vector<Token>& tokens, size_t& position) {
  if (std::holds_alternative<NumberToken>(tokens[position])) {
    Constant a(std::get<NumberToken>(tokens[position]).value);
    ++position;
    std::shared_ptr<Constant> ptr = std::make_unique<Constant>(std::move(a));
    return ptr;
  }
  if (std::holds_alternative<OpeningBracketToken>(tokens[position])) {
    ++position;
    return (ParseExpression(tokens, position));
  }
  return (ParseExpression(tokens, ++position));
}
