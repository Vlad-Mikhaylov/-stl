#include "../tokenize/tokenize.h"
#include <memory>
#include <cmath>
#include <algorithm>
#ifndef EXPRESSIONS_H
#define EXPRESSIONS_H

struct IExpression {
  virtual int Calculate() const = 0;
  virtual ~IExpression() = default;
  IExpression() = default;
};

struct Constant : public IExpression {
  int value;

  explicit Constant(int value_new) {
    value = value_new;
  }
  int Calculate() const override {
    return value;
  }
  Constant() = default;
};

struct IUnaryOperation : public IExpression {
  int operand;

  IUnaryOperation() = default;
  ~IUnaryOperation() override = default;
  explicit IUnaryOperation(std::unique_ptr<IExpression> expr) {
    operand = expr->Calculate();
  }
  virtual int Operation(int operand) const = 0;
  int Calculate() const override {
    return Operation(operand);
  }
};

struct IBinaryOperation : public IExpression {
  int operand_first, operand_second;

  IBinaryOperation() = default;
  ~IBinaryOperation() override = default;
  IBinaryOperation(std::unique_ptr<IExpression> first_expr, std::unique_ptr<IExpression> second_expr) {
    operand_first = first_expr->Calculate();
    operand_second = second_expr->Calculate();
  }
  virtual int Operation(int operand_first, int operand_second) const = 0;
  int Calculate() const override {
    return Operation(operand_first, operand_second);
  }
};

struct AbsoluteValue : public IUnaryOperation {
  AbsoluteValue() = default;
  explicit AbsoluteValue(std::unique_ptr<IExpression> expr) : IUnaryOperation(std::move(expr)) {
  }
  int Operation(int operand) const override {
    if (operand >= 0) {
      return operand;
    }
    return -1 * operand;
  }
};

struct Square : public IUnaryOperation {
  Square() = default;
  explicit Square(std::unique_ptr<IExpression> expr) : IUnaryOperation(std::move(expr)) {
  }
  int Operation(int operand) const override {
    return pow(operand, 2);
  }
};

struct Sum : public IBinaryOperation {
  Sum() = default;
  ~Sum() override = default;
  Sum(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    return operand_1 + operand_2;
  }
};

struct Subtract : public IBinaryOperation {
  Subtract() = default;
  ~Subtract() override = default;
  Subtract(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    return operand_1 - operand_2;
  }
};

struct Multiply : public IBinaryOperation {
  Multiply() = default;
  ~Multiply() override = default;
  Multiply(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    return operand_1 * operand_2;
  }
};

struct Divide : public IBinaryOperation {
  Divide() = default;
  ~Divide() override = default;
  Divide(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    return operand_1 / operand_2;
  }
};

struct Residual : public IBinaryOperation {
  Residual() = default;
  ~Residual() override = default;
  Residual(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    return operand_1 % operand_2;
  }
};

struct Minimum : public IBinaryOperation {
  Minimum() = default;
  ~Minimum() override = default;
  Minimum(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    if (operand_1 < operand_2) {
      return operand_1;
    }
    return operand_2;
  }
};

struct Maximum : public IBinaryOperation {
  Maximum() = default;
  ~Maximum() override = default;
  Maximum(std::unique_ptr<IExpression> expr_first, std::unique_ptr<IExpression> expr_second)
      : IBinaryOperation(std::move(expr_first), std::move(expr_second)) {
  }
  int Operation(int operand_1, int operand_2) const override {
    if (operand_1 > operand_2) {
      return operand_1;
    }
    return operand_2;
  }
};

#endif
