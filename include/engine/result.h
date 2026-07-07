#pragma once

#include <cassert>
#include <optional>
#include <utility>
#include <variant>

// A value that is either a success (`T`) or an error (`E`).
//
//   Result<PlayRequirements, PlayError> r = ...;
//   if (r.isOk()) use(r.value());
//   else          show(r.error());
//
// Construct with the factories so the intent is explicit:
//   return Result<T, E>::ok(value);
//   return Result<T, E>::err(error);
//
// `T` and `E` may be the same type — the alternatives are kept distinct by index.
template <typename T, typename E>
class Result {
  public:
    static Result ok(T value) {
        return Result(std::variant<T, E>(std::in_place_index<0>, std::move(value)));
    }
    static Result err(E error) {
        return Result(std::variant<T, E>(std::in_place_index<1>, std::move(error)));
    }

    bool isOk() const {
        return _data.index() == 0;
    }
    bool isErr() const {
        return _data.index() == 1;
    }
    explicit operator bool() const {
        return isOk();
    }

    // value()/error() assert the matching state — calling the wrong one is a bug.
    T& value() {
        assert(isOk());
        return std::get<0>(_data);
    }
    const T& value() const {
        assert(isOk());
        return std::get<0>(_data);
    }

    E& error() {
        assert(isErr());
        return std::get<1>(_data);
    }
    const E& error() const {
        assert(isErr());
        return std::get<1>(_data);
    }

    // Returns the success value, or `fallback` when this is an error.
    T valueOr(T fallback) const {
        return isOk() ? std::get<0>(_data) : std::move(fallback);
    }

  private:
    explicit Result(std::variant<T, E> data) : _data(std::move(data)) {}

    std::variant<T, E> _data;
};

// Result for operations with no success payload — "it happened, or here is
// why it didn't". Same ok/err vocabulary as the primary template:
//
//   Result<void, BoardError> r = board.place(...);
//   if (r.isErr()) return r.error();
template <typename E>
class Result<void, E> {
  public:
    static Result ok() {
        return Result(std::nullopt);
    }
    static Result err(E error) {
        return Result(std::move(error));
    }

    bool isOk() const {
        return !_error.has_value();
    }
    bool isErr() const {
        return _error.has_value();
    }
    explicit operator bool() const {
        return isOk();
    }

    E& error() {
        assert(isErr());
        return *_error;
    }
    const E& error() const {
        assert(isErr());
        return *_error;
    }

  private:
    explicit Result(std::optional<E> error) : _error(std::move(error)) {}

    std::optional<E> _error;
};
