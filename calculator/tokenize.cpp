#include <iostream>
#include <cmath>
#include <algorithm>
#include "tokenize.h"

using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken,
                           OpeningBracketToken, ClosingBracketToken, SqrToken, MaxToken, MinToken,
                           AbsToken, NumberToken, UnknownToken>;

bool PlusToken::operator==(const PlusToken&) const {
  return true;
}

bool MinusToken::operator==(const MinusToken&) const {
  return true;
}

bool MultiplyToken::operator==(const MultiplyToken&) const {
  return true;
}

bool DivideToken::operator==(const DivideToken&) const {
  return true;
}

bool ResidualToken::operator==(const ResidualToken&) const {
  return true;
}

bool OpeningBracketToken::operator==(const OpeningBracketToken&) const {
  return true;
}

bool ClosingBracketToken::operator==(const ClosingBracketToken&) const {
  return true;
}

bool SqrToken::operator==(const SqrToken&) const {
  return true;
}

bool MaxToken::operator==(const MaxToken&) const {
  return true;
}

bool MinToken::operator==(const MinToken&) const {
  return true;
}

bool AbsToken::operator==(const AbsToken&) const {
  return true;
}

NumberToken::NumberToken(int num) {
  value = num;
}

bool NumberToken::operator==(const NumberToken& other) const {
  return (value == other.value);
}

int NumberToken::Get() {
  return value;
}

UnknownToken::UnknownToken(std::string other) {
  value = other;
}

bool UnknownToken::operator==(const UnknownToken& other) const {
  return (value == other.value);
}

std::string UnknownToken::Get() {
  return value;
}

std::vector<Token> Tokenize(std::string_view str) {
  int len = static_cast<int>(str.length());
  int i = 0;
  int j = 0;
  std::vector<Token> oper;
  while(i < len) {
    std::string cur = "";
    while(i < len && str[i] == ' ') {
      i++;
    }
    if (i == len) {
      return oper;
    }
    j = i;
    while(j < len && str[j] != ' ') {
      cur += str[j];
      j++;
    }
    if (cur == "+") {
      PlusToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "-") {
      MinusToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "*") {
      MultiplyToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "/") {
      DivideToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "%") {
      ResidualToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "(") {
      OpeningBracketToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == ")") {
      ClosingBracketToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "sqr") {
      SqrToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "max") {
      MaxToken tmp;
      oper.emplace_back(tmp);
    } else if (cur == "min") {
      MinToken tmp;        
      oper.emplace_back(tmp);
    } else if (cur == "abs") {
      AbsToken tmp;
      oper.emplace_back(tmp);
    } else if (isdigit(cur[0]) || ((cur[0] == '+' || cur[0] == '-') && isdigit(cur[1]))) {
      bool checker = false;
      int sign = 1, is_signed = 0;
      int value = 0;
      if (cur[0] == '+') {
        is_signed = 1;
      }
      if (cur[0] == '-') {
        sign = -1;
        is_signed = 1;
      }
      for (size_t k = is_signed; k < cur.size(); k++) {
        if (!isdigit(cur[k])) {
          checker = true;
        }
      }
      if (checker) {
        UnknownToken tmp(cur);    
        oper.emplace_back(tmp);
      } else {
        for (size_t k = is_signed; k < cur.size(); k++) {
          value = value * 10 + (cur[k] - '0');
        }
        value *= sign;
        NumberToken tmp(value);    
        oper.emplace_back(tmp);
      }
    } else {
      UnknownToken tmp(cur);    
      oper.emplace_back(tmp);
    }
    i = j;
  }
  return oper;
}
