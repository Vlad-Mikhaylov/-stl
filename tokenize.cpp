#include <set>
#include <vector>
#include <map>
#include <algorithm>
#include "tokenize.h"

bool IsNumeral(const std::string& s) {
  if (s.empty()) {
    return false;
  }
  if (s.size() == 1) {
    return '0' <= s[0] && s[0] <= '9';
  }
  bool ans = true;
  ans &= all_of(s.begin(), s.end(), [](char c) { return c == '-' || c == '+' || ('0' <= c && c <= '9'); });
  ans &= !std::count(s.begin() + 1, s.end(), '-') && !std::count(s.begin() + 1, s.end(), '+');
  return ans;
}

std::vector<Token> Tokenize(const std::string& input) {
  std::vector<Token> ans;
  std::set<char> terminate_symbols = {' '};
  std::map<std::string, Token> known_strings{
      {"+", PlusToken{}},     {"-", MinusToken{}},          {"*", MultiplyToken{}},       {"/", DivideToken{}},
      {"%", ResidualToken{}}, {"(", OpeningBracketToken{}}, {")", ClosingBracketToken{}}, {"sqr", SqrToken{}},
      {"abs", AbsToken{}},    {"min", MinToken{}},          {"max", MaxToken{}}};
  std::string current_seq;
  for (const auto& it : input) {
    if (terminate_symbols.count(it)) {
      if (!current_seq.empty()) {
        if (known_strings.count(current_seq)) {
          ans.push_back(known_strings[current_seq]);
        } else if (IsNumeral(current_seq)) {
          ans.emplace_back(NumberToken{std::stoi(current_seq)});
        } else {
          ans.emplace_back(UnknownToken{current_seq});
        }
      }
      current_seq = it;
      if (current_seq == " ") {
        current_seq.clear();
      }
    } else {
      current_seq.push_back(it);
    }
  }
  if (!current_seq.empty()) {
    if (known_strings.count(current_seq)) {
      ans.push_back(known_strings[current_seq]);
    } else if (IsNumeral(current_seq)) {
      ans.emplace_back(NumberToken{std::stoi(current_seq)});
    } else {
      ans.emplace_back(UnknownToken{current_seq});
    }
  }
  return ans;
}

bool operator==(const PlusToken&, const PlusToken&) {
  return true;
}

bool operator==(const MinusToken&, const MinusToken&) {
  return true;
}

bool operator==(const MultiplyToken&, const MultiplyToken&) {
  return true;
}

bool operator==(const DivideToken&, const DivideToken&) {
  return true;
}

bool operator==(const ResidualToken&, const ResidualToken&) {
  return true;
}

bool operator==(const OpeningBracketToken&, const OpeningBracketToken&) {
  return true;
}

bool operator==(const ClosingBracketToken&, const ClosingBracketToken&) {
  return true;
}

bool operator==(const SqrToken&, const SqrToken&) {
  return true;
}

bool operator==(const MaxToken&, const MaxToken&) {
  return true;
}

bool operator==(const MinToken&, const MinToken&) {
  return true;
}

bool operator==(const AbsToken&, const AbsToken&) {
  return true;
}

bool operator==(const NumberToken& first, const NumberToken& second) {
  return first.value == second.value;
}

bool operator==(const UnknownToken& first, const UnknownToken& second) {
  return first.value == second.value;
}
