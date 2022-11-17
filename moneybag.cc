#include "moneybag.h"

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
