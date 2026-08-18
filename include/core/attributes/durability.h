#pragma once

class Durability {
  public:
    explicit Durability(int value)
        : _value(value) {
    }

    void decrement() {
        _value--;
    }

    int getValue() const {
        return _value;
    }
    bool isDestroyed() const {
        return _value <= 0;
    }

  private:
    int _value;
};