#pragma once
#include <QString>

struct ResultVoid {
    bool ok = true;
    QString error;

    static ResultVoid success() { return {true, ""}; }
    static ResultVoid fail(const QString& e) { return {false, e}; }
};

template <typename T>
struct Result {
    bool ok = true;
    T value{};
    QString error;

    static Result<T> success(const T& v) {
        Result<T> r; r.ok = true; r.value = v; return r;
    }
    static Result<T> fail(const QString& e) {
        Result<T> r; r.ok = false; r.error = e; return r;
    }
};
