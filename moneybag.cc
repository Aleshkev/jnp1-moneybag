#include "moneybag.h"

namespace {
std::string quantity(const std::string &name, const std::string &plural_suffix,
                     Moneybag::coin_number_t x) {
  return std::to_string(x) + " " + name + (x == 1 ? "" : plural_suffix);
}
} // namespace

std::ostream &operator<<(std::ostream &o, const Moneybag &bag) {
  return o << "(" << quantity("livr", "es", bag.livre_number()) << ", "
           << quantity("solidus", "es", bag.solidus_number()) << ", "
           << quantity("denier", "s", bag.denier_number()) << ")";
}
