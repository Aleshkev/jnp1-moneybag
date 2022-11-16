#include "moneybag.h"

constexpr std::partial_ordering
Moneybag::operator<=>(const Moneybag &that) const {
  if (livres == that.livres && soliduses == that.soliduses &&
      deniers == that.deniers)
    return std::partial_ordering::equivalent;
  if (livres < that.livres && soliduses < that.soliduses &&
      deniers < that.deniers)
    return std::partial_ordering::less;
  if (livres > that.livres && soliduses > that.soliduses &&
      deniers > that.deniers)
    return std::partial_ordering::greater;
  return std::partial_ordering::unordered;
}
constexpr Moneybag Moneybag::operator+(const Moneybag &that) const {
  return {add_unsigned(livres, that.livres),
          add_unsigned(soliduses, that.soliduses),
          add_unsigned(deniers, that.deniers)};
}
constexpr Moneybag Moneybag::operator-(const Moneybag &that) const {
  return {subtract_unsigned(livres, that.livres),
          subtract_unsigned(soliduses, that.soliduses),
          subtract_unsigned(deniers, that.deniers)};
}
constexpr Moneybag Moneybag::operator*(Moneybag::coin_number_t that) const {
  return {multiply_unsigned(livres, that), multiply_unsigned(soliduses, that),
          multiply_unsigned(deniers, that)};
}
Moneybag &Moneybag::operator+=(const Moneybag &that) {
  set_value(add_unsigned(livres, that.livres),
            add_unsigned(soliduses, that.soliduses),
            add_unsigned(deniers, that.deniers));
  return *this;
}
Moneybag &Moneybag::operator-=(const Moneybag &that) {
  set_value(subtract_unsigned(livres, that.livres),
            subtract_unsigned(soliduses, that.soliduses),
            subtract_unsigned(deniers, that.deniers));
  return *this;
}
Moneybag &Moneybag::operator*=(Moneybag::coin_number_t that) {
  set_value(multiply_unsigned(livres, that),
            multiply_unsigned(soliduses, that),
            multiply_unsigned(deniers, that));
  return *this;
}

std::string quantity(const std::string &name, const std::string &plural_suffix,
                     Moneybag::coin_number_t x) {
  return std::to_string(x) + " " + name + (x == 1 ? "" : plural_suffix);
}

std::ostream &operator<<(std::ostream &o, const Moneybag &bag) {
  return o << "(" << quantity("livre", "s", bag.livre_number()) << ", "
           << quantity("solidus", "es", bag.solidus_number()) << ", "
           << quantity("denier", "s", bag.denier_number()) << ")";
}

constexpr Value::Value(Moneybag bag)
    : deniers(add_unsigned(
          add_unsigned(multiply_unsigned(bag.livre_number(),
                                         (Moneybag::coin_number_t)240),
                       multiply_unsigned(bag.solidus_number(),
                                         (Moneybag::coin_number_t)20)),
          bag.denier_number())) {}

constexpr Value::Value(Moneybag::coin_number_t deniers) : deniers(deniers) {}

Value::operator std::string() const { return std::to_string(deniers); }
// constexpr std::strong_ordering
// Value::operator<=>(Moneybag::coin_number_t that) const {
//   return deniers <=> that;
// }
constexpr std::strong_ordering Value::operator<=>(const Value &that) const {
  return deniers <=> that.deniers;
}
