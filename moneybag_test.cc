#include "moneybag.h"
#include <cassert>
#include <cstdint>

// 1 - konstrukcja Moneybag, operacje arytmetyczne na Moneybag, porównywanie Moneybag, przypisywanie Moneybag
// 2 - poprawność zgłaszania wyjątków i sprawdzenie, czy obiekt nie został zmodyfikowany
// 3 - konstrukcja Value, porównywanie Value
// 4 - wypisywanie Moneybag (operartor <<) i Value (konwersja do string)
// 5 - obsługa wartości Value powyżej 2^64 - 1
// 6 - rzutowanie Moneybag na bool, const, constinit, constexpr i konstrukcje, które nie powinny się kompilować

#if TEST_NUM == 102 || TEST_NUM == 302 || TEST_NUM == 502 || TEST_NUM == 602
#include <compare>
#endif

#if TEST_NUM == 201 || TEST_NUM == 202 || TEST_NUM == 202
#include <stdexcept>
#endif

#if TEST_NUM == 401 || TEST_NUM == 602
#include <string>
#include <ostream>
#include <sstream>
#endif

#if TEST_NUM == 402
#include <string>
#endif

#if TEST_NUM == 501
#include <boost/multiprecision/cpp_int.hpp>
#include <string>
#endif

#if TEST_NUM == 104
// Ta funkcja jest zdefiniowana w pliku moneybag_test_external.cc.
uint64_t return_zero();
#endif

namespace {
    const uint64_t max_u32 = UINT32_MAX;
    const uint64_t max_u64 = UINT64_MAX;

    [[maybe_unused]] constexpr bool check(Moneybag const &m, uint64_t l,  uint64_t s, uint64_t d) {
        return m.livre_number() == l and m.solidus_number() == s and m.denier_number() == d;
    }
}

#if TEST_NUM == 603
namespace {
    constexpr bool test603(uint64_t l1, uint64_t s1, uint64_t d1,
                           uint64_t l2, uint64_t s2, uint64_t d2) {
        Moneybag m0(0, 0, 0), m1(l1, s1, d1), m2(l2, s2, d2);

        m0 += (m1 * 2) + (3 * m2);
        m0 *= 2;
        m0 -= m2 - m1;

        Value v1(m1), v2(m2);

        return check(m0, 15, 15, 15) && m0 && m1 && m2 &&
            !(m1 == m2) && m1 != m2 && m1 <= m2 && m1 < m2 && !(m1 >= m2) && !(m1 > m2) &&
            m1 <=> m2 == std::weak_ordering::less &&
            !(v1 == v2) && v1 != v2 && v1 <= v2 && v1 < v2 && !(v1 >= v2) && !(v1 > v2) &&
            v1 <=> v2 == std::strong_ordering::less;
    }
}
#endif

int main() {
#if TEST_NUM == 101 // konstruowanie obiektu typu Moneybag
    Moneybag m0(0, 0, 0);
    assert(check(m0, 0, 0, 0));

    Moneybag m1 = Moneybag(1, 1, 1);
    assert(check(m1, 1, 1, 1));

    Moneybag m2 = m1;
    assert(check(m2, 1, 1, 1));

    Moneybag m3(max_u64, 0, 1);
    assert(check(m3, max_u64, 0, 1));

    Moneybag m4(0, 1, max_u64);
    assert(check(m4, 0, 1, max_u64));

    Moneybag m5(1, max_u64, 0);
    assert(check(m5, 1, max_u64, 0));

    Moneybag m6(max_u64, max_u64, max_u64);
    assert(check(m6, max_u64, max_u64, max_u64));

    Moneybag m7{1, 3, 5};
    assert(check(m7, 1, 3, 5));

    Moneybag m8 = Moneybag{4, 4, 4};
    assert(check(m8, 4, 4, 4));

    Moneybag m9(m8);
    assert(check(m9, 4, 4, 4));
#endif // 101

#if TEST_NUM == 102 // porównywanie obiektów typu Moneybag
#include "moneybag_test_102.cc"
#endif //102

#if TEST_NUM == 103 // arytmetyka z maksymalnymi wartościami na obiektach typu Moneybag
    assert(check(Moneybag(max_u64, max_u64, max_u64) + Moneybag(0, 0, 0), max_u64, max_u64, max_u64));

    assert(check(Moneybag(max_u64, max_u32, 0) + Moneybag(0, max_u64 - max_u32, max_u64), max_u64, max_u64, max_u64));
    assert(check(Moneybag(max_u64, 0, max_u32) + Moneybag(0, max_u64, max_u64 - max_u32), max_u64, max_u64, max_u64));
    assert(check(Moneybag(max_u32, max_u64, 0) + Moneybag(max_u64 - max_u32, 0, max_u64), max_u64, max_u64, max_u64));
    assert(check(Moneybag(max_u32, 0, max_u64) + Moneybag(max_u64 - max_u32, max_u64, 0), max_u64, max_u64, max_u64));
    assert(check(Moneybag(0, max_u32, max_u64) + Moneybag(max_u64, max_u64 - max_u32, 0), max_u64, max_u64, max_u64));
    assert(check(Moneybag(0, max_u64, max_u32) + Moneybag(max_u64, 0, max_u64 - max_u32), max_u64, max_u64, max_u64));

    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(max_u64, max_u64, max_u64), 0, 0, 0));

    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(max_u64, max_u32, 0), 0, max_u64 - max_u32, max_u64));
    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(max_u64, 0, max_u32), 0, max_u64, max_u64 - max_u32));
    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(max_u32, max_u64, 0), max_u64 - max_u32, 0, max_u64));
    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(max_u32, 0, max_u64), max_u64 - max_u32, max_u64, 0));
    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(0, max_u32, max_u64), max_u64, max_u64 - max_u32, 0));
    assert(check(Moneybag(max_u64, max_u64, max_u64) - Moneybag(0, max_u64, max_u32), max_u64, 0, max_u64 - max_u32));

    for (uint64_t i = 0; i < 100; i++) {
        assert(check(i * Moneybag(max_u32, 1, 0), i * max_u32, i, 0));
        assert(check(i * Moneybag(max_u32, 0, 1), i * max_u32, 0, i));
        assert(check(i * Moneybag(1, max_u32, 0), i, i * max_u32, 0));
        assert(check(i * Moneybag(1, 0, max_u32), i, 0, i * max_u32));
        assert(check(i * Moneybag(0, max_u32, 1), 0, i * max_u32, i));
        assert(check(i * Moneybag(0, 1, max_u32), 0, i, i * max_u32));

        assert(check(Moneybag(max_u32, 1, 0) * i, i * max_u32, i, 0));
        assert(check(Moneybag(max_u32, 0, 1) * i, i * max_u32, 0, i));
        assert(check(Moneybag(1, max_u32, 0) * i, i, i * max_u32, 0));
        assert(check(Moneybag(1, 0, max_u32) * i, i, 0, i * max_u32));
        assert(check(Moneybag(0, max_u32, 1) * i, 0, i * max_u32, i));
        assert(check(Moneybag(0, 1, max_u32) * i, 0, i, i * max_u32));
    }

    for (uint64_t i = max_u32 - 100; i <= max_u64 / max_u32; i++) {
        assert(check(i * Moneybag(max_u32, 1, 0), i * max_u32, i, 0));
        assert(check(i * Moneybag(max_u32, 0, 1), i * max_u32, 0, i));
        assert(check(i * Moneybag(1, max_u32, 0), i, i * max_u32, 0));
        assert(check(i * Moneybag(1, 0, max_u32), i, 0, i * max_u32));
        assert(check(i * Moneybag(0, max_u32, 1), 0, i * max_u32, i));
        assert(check(i * Moneybag(0, 1, max_u32), 0, i, i * max_u32));

        assert(check(Moneybag(max_u32, 1, 0) * i, i * max_u32, i, 0));
        assert(check(Moneybag(max_u32, 0, 1) * i, i * max_u32, 0, i));
        assert(check(Moneybag(1, max_u32, 0) * i, i, i * max_u32, 0));
        assert(check(Moneybag(1, 0, max_u32) * i, i, 0, i * max_u32));
        assert(check(Moneybag(0, max_u32, 1) * i, 0, i * max_u32, i));
        assert(check(Moneybag(0, 1, max_u32) * i, 0, i, i * max_u32));
    }
#endif // 103

#if TEST_NUM == 104 // obiekty Livre, Solidus, Denier, operatory przypisania w klasie Moneybag
    Moneybag m1 = Livre;
    Moneybag m2 = Solidus;
    Moneybag m3 = Denier;

    Moneybag m4 = Moneybag(max_u32, 0, max_u32);
    Moneybag m5 = Moneybag(0, max_u32, max_u32);
    Moneybag m6 = Moneybag(max_u32, max_u32, 0);

    Moneybag m7 = Moneybag(max_u64, 0, max_u32);
    Moneybag m8 = Moneybag(0, max_u32, max_u64);
    Moneybag m9 = Moneybag(max_u32, max_u64, 0);

    m7 -= m4;
    assert(check(m7, max_u64 - max_u32, 0, 0));

    m8 -= m5;
    assert(check(m8, 0, 0, max_u64 - max_u32));

    m9 -= m6;
    assert(check(m9, 0, max_u64 - max_u32, 0));

    m7 += m4;
    assert(check(m7, max_u64, 0, max_u32));

    m8 += m5;
    assert(check(m8, 0, max_u32, max_u64));

    m9 += m6;
    assert(check(m9,  max_u32, max_u64, 0));

    m7 *= 1;
    assert(check(m7, max_u64, 0, max_u32));

    m8 *= 1;
    assert(check(m8, 0, max_u32, max_u64));

    m9 *= 1;
    assert(check(m9,  max_u32, max_u64, 0));

    m7 *= return_zero();
    assert(check(m7, 0, 0, 0));

    m8 *= return_zero();
    assert(check(m8, 0, 0, 0));

    m9 *= return_zero();
    assert(check(m9, 0, 0, 0));

    m1 *= max_u64;
    assert(check(m1, max_u64, 0, 0));

    m2 *= max_u64;
    assert(check(m2, 0, max_u64, 0));

    m3 *= max_u64;
    assert(check(m3, 0, 0, max_u64));

    m4 *= max_u32;
    assert(check(m4, max_u32 * max_u32, 0, max_u32 * max_u32));

    m5 *= max_u32;
    assert(check(m5, 0, max_u32 * max_u32, max_u32 * max_u32));

    m6 *= max_u32;
    assert(check(m6, max_u32 * max_u32, max_u32 * max_u32, 0));

    m4 += max_u32 * Denier;
    assert(check(m4, max_u32 * max_u32, 0, (max_u32 + 1) * max_u32));

    m5 += max_u32 * Solidus;
    assert(check(m5, 0, (max_u32 + 1) * max_u32, max_u32 * max_u32));

    m6 += max_u32 * Livre;
    assert(check(m6, (max_u32 + 1) * max_u32, max_u32 * max_u32, 0));

    m4 += max_u64 * Solidus;
    assert(check(m4, max_u32 * max_u32, max_u64, (max_u32 + 1) * max_u32));

    m5 += max_u64 * Livre;
    assert(check(m5, max_u64, (max_u32 + 1) * max_u32, max_u32 * max_u32));

    m6 += max_u64 * Denier;
    assert(check(m6, (max_u32 + 1) * max_u32, max_u32 * max_u32, max_u64));
#endif // 104

#if TEST_NUM == 105 // obiekt modyfikujący się sam
    Moneybag m(1, 2, 3);

    m += m;
    assert(check(m, 2, 4, 6));

    m = 3 * m + m * 4 - 5 * m;
    assert(check(m, 4, 8, 12));

    m -= m;
    assert(check(m, 0, 0, 0));
#endif // 105

#if TEST_NUM == 106 // składowa coin_number_t
    Moneybag::coin_number_t i = 3, j = 5, k = 7;
    assert(check(Moneybag(i, j, k), i, j, k));
#endif // 106

#if TEST_NUM == 201 // wyjątki w klasie Moneybag
    bool success;

    Moneybag m1 = Moneybag(0, 1, max_u64);
    Moneybag m2 = Moneybag(max_u64, 0, 1);
    Moneybag m3 = Moneybag(1, max_u64, 0);

    success = false;
    try {
        m1 += Denier;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m1 + Denier;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m2 += Livre;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m2 + Livre;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m3 += Solidus;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);

    success = false;
    try {
        m3 + Solidus;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);

    success = false;
    try {
        m1 *= 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m1 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        2 * m1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m2 *= 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m2 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        2 * m2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m3 *= 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);

    success = false;
    try {
        m3 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);

    success = false;
    try {
        2 * m3;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);

    success = false;
    try {
        m1 -= Livre;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m1 - Livre;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64));
    assert(success);

    success = false;
    try {
        m2 -= Solidus;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m2 - Solidus;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64, 0, 1));
    assert(success);

    success = false;
    try {
        m3 -= Denier;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);
    success = false;
    try {
        m3 - Denier;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64, 0));
    assert(success);
#endif // 201

#if TEST_NUM == 202 // wyjątki w klasie Moneybag
    bool success;

    Moneybag m1 = Moneybag(0, 1, max_u32 + 1);
    Moneybag m2 = Moneybag(max_u32 + 1, 0, 1);
    Moneybag m3 = Moneybag(1, max_u32 + 1, 0);

    success = false;
    try {
        m1 *= max_u32 + 1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u32 + 1));
    assert(success);

    success = false;
    try {
        m1 * (max_u32 + 1);
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u32 + 1));
    assert(success);

    success = false;
    try {
        (max_u32 + 1) * m1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u32 + 1));
    assert(success);

    success = false;
    try {
        m2 *= max_u32 + 1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u32 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        m2 * (max_u32 + 1);
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u32 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        (max_u32 + 1) * m2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u32 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        m3 *= max_u32 + 1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u32 + 1, 0));
    assert(success);

    success = false;
    try {
        m3 * (max_u32 + 1);
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u32 + 1, 0));
    assert(success);

    success = false;
    try {
        (max_u32 + 1) * m3;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u32 + 1, 0));
    assert(success);
#endif // 202

#if TEST_NUM == 203 // wyjątki w klasie Moneybag
    bool success;

    Moneybag m1 = Moneybag(0, 1, max_u64 / 2 + 1);
    Moneybag m2 = Moneybag(max_u64 / 2 + 1, 0, 1);
    Moneybag m3 = Moneybag(1, max_u64 / 2 + 1, 0);

    success = false;
    try {
        m1 += m1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64 / 2 + 1));
    assert(success);

    success = false;
    try {
        m1 + m1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64 / 2 + 1));
    assert(success);

    success = false;
    try {
        m1 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64 / 2 + 1));
    assert(success);

    success = false;
    try {
        2 * m1;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m1, 0, 1, max_u64 / 2 + 1));
    assert(success);

    success = false;
    try {
        m2 += m2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64 / 2 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        m2 + m2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64 / 2 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        m2 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64 / 2 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        2 * m2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m2, max_u64 / 2 + 1, 0, 1));
    assert(success);

    success = false;
    try {
        m3 += m3;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64 / 2 + 1, 0));
    assert(success);

    success = false;
    try {
        m3 + m3;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64 / 2 + 1, 0));
    assert(success);

    success = false;
    try {
        m3 * 2;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64 / 2 + 1, 0));
    assert(success);

    success = false;
    try {
        2 * m3;
    } catch (std::out_of_range const &e) {
        success = true;
    }
    assert(check(m3, 1, max_u64 / 2 + 1, 0));
    assert(success);
#endif // 203

#if TEST_NUM == 301 // konstruowanie obiektu typu Value
    Value v1;
    assert(v1 == Value(0));

    Value v2(1);
    assert(v2 == Value(1));

    Value v3(Moneybag(1, 1, 1));
    assert(v3 == Value(253));

    Value v4(Denier);
    assert(v4 == Value(1));

    Value v5(Solidus);
    assert(v5 == Value(12));

    Value v6(Livre);
    assert(v6 == Value(240));

    Value v7 = 7;
    assert(v7 == Value(7));

    Value v8{8};
    assert(v8 == Value(8));
#endif // 301

#if TEST_NUM == 302 // porównywanie obiektów typu Value
    Value v1(0);
    Value v2(max_u32);
    Value v3(max_u64);

    assert(v1 == v1);
    assert(v1 >= v1);
    assert(v1 <= v1);
    assert(!(v1 != v1));
    assert(!(v1 > v1));
    assert(!(v1 < v1));
    assert(v1 <=> v1 == std::strong_ordering::equivalent);

    assert(v2 == v2);
    assert(v2 >= v2);
    assert(v2 <= v2);
    assert(!(v2 != v2));
    assert(!(v2 > v2));
    assert(!(v2 < v2));
    assert(v2 <=> v2 == std::strong_ordering::equivalent);

    assert(v3 == v3);
    assert(v3 >= v3);
    assert(v3 <= v3);
    assert(!(v3 != v3));
    assert(!(v3 > v3));
    assert(!(v3 < v3));
    assert(v3 <=> v3 == std::strong_ordering::equivalent);

    assert(!(v1 == v2));
    assert(!(v1 >= v2));
    assert(v1 <= v2);
    assert(v1 != v2);
    assert(!(v1 > v2));
    assert(v1 < v2);
    assert(v1 <=> v2 == std::strong_ordering::less);

    assert(!(v1 == v3));
    assert(!(v1 >= v3));
    assert(v1 <= v3);
    assert(v1 != v3);
    assert(!(v1 > v3));
    assert(v1 < v3);
    assert(v1 <=> v3 == std::strong_ordering::less);

    assert(!(v2 == v1));
    assert(v2 >= v1);
    assert(!(v2 <= v1));
    assert(v2 != v1);
    assert(v2 > v1);
    assert(!(v2 < v1));
    assert(v2 <=> v1 == std::strong_ordering::greater);

    assert(!(v2 == v3));
    assert(!(v2 >= v3));
    assert(v2 <= v3);
    assert(v2 != v3);
    assert(!(v2 > v3));
    assert(v2 < v3);
    assert(v2 <=> v3 == std::strong_ordering::less);

    assert(!(v3 == v1));
    assert(v3 >= v1);
    assert(!(v3 <= v1));
    assert(v3 != v1);
    assert(v3 > v1);
    assert(!(v3 < v1));
    assert(v3 <=> v1 == std::strong_ordering::greater);

    assert(!(v3 == v2));
    assert(v3 >= v2);
    assert(!(v3 <= v2));
    assert(v3 != v2);
    assert(v3 > v2);
    assert(!(v3 < v2));
    assert(v3 <=> v2 == std::strong_ordering::greater);
#endif // 302

#if TEST_NUM == 303 // porównywanie obiektów typu Value skonstruowanych z różnych obiektów typu Moneybag dających tę samą wartość
    for (uint64_t i = 0; i < max_u32; i += max_u32 / 5) {
        for (uint64_t j = 0; j < max_u32; j += max_u32 / 5) {
            for (uint64_t k = 0; k < max_u32; k += max_u32 / 5) {
                assert(Value(Moneybag(i + 1, j, k)) == Value(Moneybag(i, j, k + 240)));
                assert(Value(Moneybag(i + 1, j, k)) == Value(Moneybag(i, j + 20, k)));
                assert(Value(Moneybag(i + 1, j, k)) == Value(Moneybag(i, j + 10, k + 120)));
                assert(Value(Moneybag(i + 1, j + 1, k)) == Value(Moneybag(i, j, k + 252)));
                assert(Value(Moneybag(i + 1, j + 1, k)) == Value(Moneybag(i, j + 2, k + 228)));
                assert(Value(Moneybag(i + 1, j + 1, k)) == Value(Moneybag(i, j + 7, k + 168)));
            }
        }
    }

    assert(Value(Moneybag(max_u64 - 2, max_u64, max_u64)) == Value(Moneybag(max_u64 - 1, max_u64 - 20, max_u64)));
    assert(Value(Moneybag(max_u64 - 2, max_u64, max_u64)) == Value(Moneybag(max_u64 - 1, max_u64, max_u64 - 240)));
    assert(Value(Moneybag(max_u64 - 2, max_u64, max_u64)) == Value(Moneybag(max_u64 - 1, max_u64 - 1, max_u64 - 228)));
#endif // 303

#if TEST_NUM == 401 // wypisywanie obiektu typu Moneybag na strumień
    Moneybag m1 = Moneybag(73, 82, 1);
    Moneybag m2 = Moneybag(1, 11, 32);
    Moneybag m3 = Moneybag(1, 1, 0);
    Moneybag m4 = Moneybag(5, 4, 2);
    Moneybag m5 = Moneybag(0, 1, max_u64);
    Moneybag m6 = Moneybag(1, max_u64, 0);
    Moneybag m7 = Moneybag(max_u64, 0, 1);

    std::ostringstream sout1;
    sout1 << m1;
    assert(sout1.str().compare("(73 livres, 82 soliduses, 1 denier)") == 0);

    std::ostringstream sout2;
    sout2 << m2;
    assert(sout2.str().compare("(1 livr, 11 soliduses, 32 deniers)") == 0);

    std::ostringstream sout3;
    sout3 << m3;
    assert(sout3.str().compare("(1 livr, 1 solidus, 0 deniers)") == 0);

    std::ostringstream sout4;
    sout4 << m4;
    assert(sout4.str().compare("(5 livres, 4 soliduses, 2 deniers)") == 0);

    std::ostringstream sout5;
    sout5 << m5;
    assert(sout5.str().compare("(0 livres, 1 solidus, 18446744073709551615 deniers)") == 0);

    std::ostringstream sout6;
    sout6 << m6;
    assert(sout6.str().compare("(1 livr, 18446744073709551615 soliduses, 0 deniers)") == 0);

    std::ostringstream sout7;
    sout7 << m7;
    assert(sout7.str().compare("(18446744073709551615 livres, 0 soliduses, 1 denier)") == 0);
#endif // 401

#if TEST_NUM == 402 // konwesja obiektu typu Value do obiektu typu string
    Value v1(0);
    assert(((std::string)v1).compare("0") == 0);

    for (uint64_t i = 1; i <= max_u64 / 2; i *= 2) {
        Value v(i);
        assert(((std::string)v).compare(std::to_string(i)) == 0);
    }
#endif // 402

#if TEST_NUM == 501 // wypisywanie wartości obiektu typu Value przekraczającej max_u64
    Value v1(Moneybag(max_u64, max_u64, max_u64));
    assert(((std::string)v1).compare((253 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v2(Moneybag(max_u64, max_u64, 0));
    assert(((std::string)v2).compare((252 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v3(Moneybag(max_u64, 0, max_u64));
    assert(((std::string)v3).compare((241 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v4(Moneybag(0, max_u64, max_u64));
    assert(((std::string)v4).compare((13 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v5(Moneybag(max_u64, 0, 0));
    assert(((std::string)v5).compare((240 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v6(Moneybag(0, max_u64, 0));
    assert(((std::string)v6).compare((12 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);

    Value v7(Moneybag(0, 0, max_u64));
    assert(((std::string)v7).compare((1 * (boost::multiprecision::uint128_t)max_u64).str()) == 0);
#endif // 501

#if TEST_NUM == 502 // porównywanie obiektów typu Value o wartości przekraczającej max_u64
    Value v1(max_u32);
    Value v2(Moneybag(max_u64, 0, 1));
    Value v3(Moneybag(max_u64, 1, 0));

    assert(v1 == v1);
    assert(v1 >= v1);
    assert(v1 <= v1);
    assert(!(v1 != v1));
    assert(!(v1 > v1));
    assert(!(v1 < v1));
    assert(v1 <=> v1 == std::strong_ordering::equivalent);

    assert(v2 == v2);
    assert(v2 >= v2);
    assert(v2 <= v2);
    assert(!(v2 != v2));
    assert(!(v2 > v2));
    assert(!(v2 < v2));
    assert(v2 <=> v2 == std::strong_ordering::equivalent);

    assert(v3 == v3);
    assert(v3 >= v3);
    assert(v3 <= v3);
    assert(!(v3 != v3));
    assert(!(v3 > v3));
    assert(!(v3 < v3));
    assert(v3 <=> v3 == std::strong_ordering::equivalent);

    assert(!(v1 == v2));
    assert(!(v1 >= v2));
    assert(v1 <= v2);
    assert(v1 != v2);
    assert(!(v1 > v2));
    assert(v1 < v2);
    assert(v1 <=> v2 == std::strong_ordering::less);

    assert(!(v1 == v3));
    assert(!(v1 >= v3));
    assert(v1 <= v3);
    assert(v1 != v3);
    assert(!(v1 > v3));
    assert(v1 < v3);
    assert(v1 <=> v3 == std::strong_ordering::less);

    assert(!(v2 == v1));
    assert(v2 >= v1);
    assert(!(v2 <= v1));
    assert(v2 != v1);
    assert(v2 > v1);
    assert(!(v2 < v1));
    assert(v2 <=> v1 == std::strong_ordering::greater);

    assert(!(v2 == v3));
    assert(!(v2 >= v3));
    assert(v2 <= v3);
    assert(v2 != v3);
    assert(!(v2 > v3));
    assert(v2 < v3);
    assert(v2 <=> v3 == std::strong_ordering::less);

    assert(!(v3 == v1));
    assert(v3 >= v1);
    assert(!(v3 <= v1));
    assert(v3 != v1);
    assert(v3 > v1);
    assert(!(v3 < v1));
    assert(v3 <=> v1 == std::strong_ordering::greater);

    assert(!(v3 == v2));
    assert(v3 >= v2);
    assert(!(v3 <= v2));
    assert(v3 != v2);
    assert(v3 > v2);
    assert(!(v3 < v2));
    assert(v3 <=> v2 == std::strong_ordering::greater);
#endif // 502

// Testy, które powinny się kompilować.
#if TEST_NUM == 601 // rzutowanie Moneybag na typ bool
    Moneybag m0(0, 0, 0);
    Moneybag m1(0, 0, 1);
    Moneybag m2(0, 1, 0);
    Moneybag m3(0, 1, 1);
    Moneybag m4(1, 0, 0);
    Moneybag m5(1, 0, 1);
    Moneybag m6(1, 1, 0);
    Moneybag m7(1, 1, 1);

    if (!m0)
        assert(true);
    else
        assert(false);

    if (m1)
        assert(true);
    else
        assert(false);

    if (m2)
        assert(true);
    else
        assert(false);

    if (m3)
        assert(true);
    else
        assert(false);

    if (m4)
        assert(true);
    else
        assert(false);

    if (m5)
        assert(true);
    else
        assert(false);

    if (m6)
        assert(true);
    else
        assert(false);

    if (m7)
        assert(true);
    else
        assert(false);
#endif // 601

#if TEST_NUM == 602 // poprawność const
    Moneybag m0(0, 0, 0);
    Moneybag const m1(1, 1, 1);
    Moneybag const m2(2, 2, 2);

    m0 += m1;
    m0 += m1 + m1;
    m0 += 2 * m1;
    m0 += m2 * 3;
    m0 -= m1;
    m0 -= m2 - m1;
    assert(check(m0, 9, 9, 9));

    assert(!(m1 == m2));
    assert(m1 != m2);
    assert(m1 <= m2);
    assert(m1 < m2);
    assert(!(m1 >= m2));
    assert(!(m1 > m2));
    assert(m1 <=> m2 == std::weak_ordering::less);

    if (m1)
        assert(true);
    else
        assert(false);

    std::ostringstream sout;
    sout << m2;
    assert(sout.str().compare("(2 livres, 2 soliduses, 2 deniers)") == 0);

    Value const v1(m1);
    Value const v2(m2);

    assert(!(v1 == v2));
    assert(v1 != v2);
    assert(v1 <= v2);
    assert(v1 < v2);
    assert(!(v1 >= v2));
    assert(!(v1 > v2));
    assert(v1 <=> v2 == std::weak_ordering::less);

    assert(((std::string)v1).compare("253") == 0);
#endif // 602

#if TEST_NUM == 603 // poprawność constexpr
    static_assert(test603(1, 1, 1, 2, 2, 2));
#endif // 603

// Testy, które nie powinny się kompilować.
#if TEST_NUM == 604
    Moneybag m;
#endif // 604

#if TEST_NUM == 605
    Moneybag m(1);
#endif // 605

#if TEST_NUM == 606
    Moneybag m(1, 2);
#endif // 606

#if TEST_NUM == 607
    Moneybag m(1, 1, 1, 1);
#endif // 607

#if TEST_NUM == 608
    Livre = Denier;
#endif // 608

#if TEST_NUM == 609
    Denier = Solidus;
#endif // 609

#if TEST_NUM == 610
    Solidus = Livre;
#endif // 610

#if TEST_NUM == 611
    Livre = Livre;
#endif // 611

#if TEST_NUM == 612
    Denier = Denier;
#endif // 612

#if TEST_NUM == 613
    Solidus = Solidus;
#endif // 613

#if TEST_NUM == 614
    int k = Moneybag(1, 2, 3);
#endif // 614

#if TEST_NUM == 615
    long k = Moneybag(1, 2, 3);
#endif // 615

#if TEST_NUM == 616
    char k = Moneybag(1, 2, 3);
#endif // 616
}
