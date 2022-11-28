#ifndef MONEYBAG_H
#define MONEYBAG_H
#include <compare>
#include <cstdint>
#include <iostream>
#include <stdexcept>

namespace {
template <typename T> constexpr T add_unsigned(T a, T b) {
  if (a + b < a)
    throw std::out_of_range("can't store the result");
  return a + b;
}

template <typename T> constexpr T subtract_unsigned(T a, T b) {
  if (a < b)
    throw std::out_of_range("can't store the result");
  return a - b;
}
template <typename T> constexpr T multiply_unsigned(T a, T b) {
  if (a > 0 && b > ((T)-1) / a)
    throw std::out_of_range("can't store the result");
  return a * b;
}
} // namespace

class Moneybag {
public:
  using coin_number_t = uint64_t;

private:
  coin_number_t livres, soliduses, deniers;
  constexpr void set_value(coin_number_t new_livres,
                           coin_number_t new_soliduses,
                           coin_number_t new_deniers) {
    livres = new_livres, soliduses = new_soliduses, deniers = new_deniers;
  }

public:
  // tworzenie sakiewki na podstawie liczby poszczególnych monet, np.
  // Moneybag(1, 2, 3) powinno stworzyć sakiewkę, w której znajdują się 1 liwr,
  // 2 solidy i 3 denary; nie powinno być natomiast możliwości utworzenia
  // obiektu bez podania liczby monet
  Moneybag() = delete;
  constexpr Moneybag(coin_number_t livres, coin_number_t soliduses,
                     coin_number_t deniers)
      : livres(livres), soliduses(soliduses), deniers(deniers){};

  // Tworzenie sakiewki przez skopiowanie innej sakiewki.
  constexpr Moneybag(const Moneybag &that) = default;
  Moneybag &operator=(const Moneybag &that) = default;

  // Odczytanie liczby poszczególnych monet za pomocą odpowiednio metod
  // livre_number(), solidus_number() i denier_number().
  [[nodiscard]] constexpr coin_number_t livre_number() const { //
    return livres;
  }
  [[nodiscard]] constexpr coin_number_t solidus_number() const {
    return soliduses;
  }
  [[nodiscard]] constexpr coin_number_t denier_number() const {
    return deniers;
  }

  // Wykonywanie operacji dodawania, odejmowania zawartości sakiewek oraz
  // mnożenia zawartości sakiewki przez liczbę całkowitą nieujemną.
  constexpr Moneybag operator+=(const Moneybag &that) {
    set_value(add_unsigned(livres, that.livres),
              add_unsigned(soliduses, that.soliduses),
              add_unsigned(deniers, that.deniers));
    return *this;
  }

  constexpr Moneybag operator-=(const Moneybag &that) {
    set_value(subtract_unsigned(livres, that.livres),
              subtract_unsigned(soliduses, that.soliduses),
              subtract_unsigned(deniers, that.deniers));
    return *this;
  }

  constexpr Moneybag operator*=(Moneybag::coin_number_t that) {
    set_value(multiply_unsigned(livres, that),
              multiply_unsigned(soliduses, that),
              multiply_unsigned(deniers, that));
    return *this;
  }
  constexpr Moneybag operator+(const Moneybag &that) const {
    Moneybag x = *this;
    x += that;
    return x;
  }
  constexpr Moneybag operator-(const Moneybag &that) const {
    Moneybag x = *this;
    x -= that;
    return x;
  }
  constexpr Moneybag operator*(coin_number_t that) const {
    Moneybag x = *this;
    x *= that;
    return x;
  }

  // Porównywanie zawartości sakiewek – na sakiewkach zdefiniowany jest
  // naturalny porządek częściowy.
  // "Jeśli a >= b to a - b nie zwraca wyjątku."
  constexpr bool operator==(const Moneybag &that) const {
    return livres == that.livres && soliduses == that.soliduses &&
           deniers == that.deniers;
  }
  //!(Moneybag(1, 2, 2) <= Moneybag(2, 2, 1))' failed.
  constexpr std::partial_ordering operator<=>(const Moneybag &that) const {
    if (livres == that.livres && soliduses == that.soliduses &&
        deniers == that.deniers)
      return std::partial_ordering::equivalent;
    if (livres <= that.livres && soliduses <= that.soliduses &&
        deniers <= that.deniers)
      return std::partial_ordering::less;
    if (livres >= that.livres && soliduses >= that.soliduses &&
        deniers >= that.deniers)
      return std::partial_ordering::greater;
    return std::partial_ordering::unordered;
  }

  // Rzutowanie sakiewki na typ bool, informujące, czy sakiewka jest niepusta
  // (zawiera choć jedną monetę).
  constexpr explicit operator bool() const {
    return livres > 0 || soliduses > 0 || deniers > 0;
  }
};
constexpr Moneybag operator*(Moneybag::coin_number_t a, const Moneybag &b) {
  return b * a;
}

// Użycie inicjowanych w trakcie kompilowania i stałych obiektów Denier,
// Livre oraz Solidus, reprezentujących poszczególne monety.
constexpr Moneybag Livre(1, 0, 0);
constexpr Moneybag Solidus(0, 1, 0);
constexpr Moneybag Denier(0, 0, 1);

// Wysłanie na strumień wyjściowy napisu reprezentującego zawartość sakiewki.
std::ostream &operator<<(std::ostream &o, const Moneybag &bag);

class Value {
private:
  const __uint128_t deniers = 0;

public:
  using value_t = __uint128_t;
  // Utworzenie wartości na podstawie sakiewki lub liczby całkowitej
  // nieujemnej.
  constexpr Value() = default;
  constexpr explicit Value(Moneybag bag)
      : deniers(add_unsigned(
            add_unsigned(
                multiply_unsigned((value_t)bag.livre_number(), (value_t)240),
                multiply_unsigned((value_t)bag.solidus_number(),
                                  (value_t)240 / 20)),
            (value_t)bag.denier_number())) {}

  constexpr Value(value_t deniers) : deniers(deniers) {}

  // Kopiowanie wartości.
  constexpr Value(const Value &that) = default;

  // Porównywanie wartości – na wartościach zdefiniowany jest naturalny
  // porządek liniowy.
  constexpr bool operator==(const Value &that) const {
    return deniers == that.deniers;
  }
  constexpr std::strong_ordering operator<=>(const Value &that) const {
    return deniers <=> that.deniers;
  }
  // Porównywanie wartości z liczbami całkowitymi nieujemnymi.
  constexpr bool operator==(value_t that) const { return deniers == that; }

  constexpr std::strong_ordering
  operator<=>(Moneybag::coin_number_t that) const {
    return deniers <=> that;
  };

  // Rzutowanie wartości na obiekt klasy string reprezentujący wartość w
  // zapisie dziesiętnym.
  explicit operator std::string() const {
    if (deniers / 1000 > 0) {
      auto right = std::to_string((Moneybag::coin_number_t)(deniers % 1000));
      return std::to_string((Moneybag::coin_number_t)(deniers / 1000)) +
             std::string(3 - right.size(), '0') + right;
    } else {
      return std::to_string((Moneybag::coin_number_t)(deniers));
    }
  }
};

#endif // MONEYBAG_H
