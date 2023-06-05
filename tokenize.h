#ifndef CALCULATOR_TOKENIZE_H
#define CALCULATOR_TOKENIZE_H

#include <string>
#include <variant>
#include <vector>

struct PlusToken {};

bool operator==(const PlusToken&, const PlusToken&);

struct MinusToken {};

bool operator==(const MinusToken&, const MinusToken&);

struct MultiplyToken {};

bool operator==(const MultiplyToken&, const MultiplyToken&);

struct DivideToken {};

bool operator==(const DivideToken&, const DivideToken&);

struct ResidualToken {};

bool operator==(const ResidualToken&, const ResidualToken&);

struct OpeningBracketToken {};

bool operator==(const OpeningBracketToken&, const OpeningBracketToken&);

struct ClosingBracketToken {};

bool operator==(const ClosingBracketToken&, const ClosingBracketToken&);

struct SqrToken {};

bool operator==(const SqrToken&, const SqrToken&);

struct MaxToken {};

bool operator==(const MaxToken&, const MaxToken&);

struct MinToken {};

bool operator==(const MinToken&, const MinToken&);

struct AbsToken {};

bool operator==(const AbsToken&, const AbsToken&);

struct NumberToken {
  int value;
};

bool operator==(const NumberToken& first, const NumberToken& second);

struct UnknownToken {
  std::string value;
};

bool operator==(const UnknownToken& first, const UnknownToken& second);

using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken, OpeningBracketToken,
                           ClosingBracketToken, SqrToken, MaxToken, MinToken, AbsToken, NumberToken, UnknownToken>;

std::vector<Token> Tokenize(const std::string& input);

#endif  // CALCULATOR_TOKENIZE_H
