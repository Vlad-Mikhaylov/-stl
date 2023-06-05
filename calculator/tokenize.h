#include <string>
#include <string_view>
#include <variant>
#include <vector>
#ifndef TOKENIZE_H
#define TOKENIZE_H

struct PlusToken {
  bool operator==(const PlusToken&) const;
};

struct MinusToken {
  bool operator==(const MinusToken&) const;
};

struct MultiplyToken {
  bool operator==(const MultiplyToken&) const;
};

struct DivideToken {
  bool operator==(const DivideToken&) const;
};

struct ResidualToken {
  bool operator==(const ResidualToken&) const;
};

struct OpeningBracketToken {
  bool operator==(const OpeningBracketToken&) const;
};

struct ClosingBracketToken {
  bool operator==(const ClosingBracketToken&) const;
};

struct SqrToken {
  bool operator==(const SqrToken&) const;
};

struct MaxToken {
  bool operator==(const MaxToken&) const;
};

struct MinToken {
  bool operator==(const MinToken&) const;
};

struct AbsToken {
  bool operator==(const AbsToken&) const;
};

struct NumberToken {
  int value;

  bool operator==(const NumberToken& other) const;
  int Get();
  explicit NumberToken(int num);
};

struct UnknownToken {
  std::string value;

  bool operator==(const UnknownToken& other) const;
  std::string Get();
  explicit UnknownToken(std::string other);
};

using Token = std::variant<PlusToken, MinusToken, MultiplyToken, DivideToken, ResidualToken,
                           OpeningBracketToken, ClosingBracketToken, SqrToken, MaxToken, MinToken,
                           AbsToken, NumberToken, UnknownToken>;

std::vector<Token> Tokenize(std::string_view string);

#endif
