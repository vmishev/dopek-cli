// core/common/services/result.h
#pragma once
#include <QString>

struct ResultVoid {
    bool ok = false;
    QString error;

    static ResultVoid success() { return ResultVoid{true, ""}; }
    static ResultVoid failure(const QString& e) { return ResultVoid{false, e}; }
};

template <typename T>
struct Result {
    bool ok = false;
    T value{};
    QString error;

    static Result<T> success(const T& v) { return Result<T>{true, v, ""}; }
    static Result<T> failure(const QString& e) { return Result<T>{false, T{}, e}; }
};
