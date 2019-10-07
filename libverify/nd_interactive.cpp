/**
 * Defines an interactive implementation of all non-deterministic primitive
 * value calls. Data is generated by prompting the user.
 * @date 2019
 */

#include "verify.h"

#include <stdint.h>
#include <stdio.h>
#include <iostream>

using namespace std;

void on_entry(const char* _type, const char* _msg)
{
    std::cout << "%s [%s]: " << _msg << _type;
}

uint8_t rt_nd_byte(const char* _msg)
{
    on_entry("uint8", _msg);
    uint8_t retval;
    scanf("%hhu", &retval);
    return retval;
}

sol_raw_int8_t nd_int8_t(const char* _msg)
{
    on_entry("int8", _msg);
    sol_raw_int8_t retval = 0;

    #ifdef MC_USE_STDINT
    scanf("%hhu", &retval);
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int16_t nd_int16_t(const char* _msg)
{
    on_entry("int16", _msg);
    sol_raw_int16_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int24_t nd_int24_t(const char* _msg)
{
    on_entry("int24", _msg);
    sol_raw_int24_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int32_t nd_int32_t(const char* _msg)
{
    on_entry("int32", _msg);
    sol_raw_int32_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int40_t nd_int40_t(const char* _msg)
{
    on_entry("int40", _msg);
    sol_raw_int40_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int48_t nd_int48_t(const char* _msg)
{
    on_entry("int48", _msg);
    sol_raw_int48_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int56_t nd_int56_t(const char* _msg)
{
    on_entry("int56", _msg);
    sol_raw_int56_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_int64_t nd_int64_t(const char* _msg)
{
    on_entry("int64", _msg);
    sol_raw_int64_t retval = 0;
    cin >> retval;
    return retval;
}

#ifdef MC_USE_STDINT
__int128_t nd_stdint_int128_t(void)
{
    char input[41];
    scanf("%s", input);

    const int IS_NEG = (input[0] == '-');
    const int FIRST_DIGIT_POS = (IS_NEG ? 1 : 0);

    __int128_t retval = 0;
    for (unsigned int i = FIRST_DIGIT_POS; input[i] != 0; ++i)
    {
        retval *= 10;
        retval += (__int128_t)(input[i] - '0');
    }

    if (IS_NEG) retval = -retval;

    return retval;
}
#endif

sol_raw_int72_t nd_int72_t(const char* _msg)
{
    on_entry("int72", _msg);
    sol_raw_int72_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int80_t nd_int80_t(const char* _msg)
{
    on_entry("int80", _msg);
    sol_raw_int80_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int88_t nd_int88_t(const char* _msg)
{
    on_entry("int88", _msg);
    sol_raw_int88_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int96_t nd_int96_t(const char* _msg)
{
    on_entry("int96", _msg);
    sol_raw_int96_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int104_t nd_int104_t(const char* _msg)
{
    on_entry("int104", _msg);
    sol_raw_int104_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int112_t nd_int112_t(const char* _msg)
{
    on_entry("int112", _msg);
    sol_raw_int112_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int120_t nd_int120_t(const char* _msg)
{
    on_entry("int120", _msg);
    sol_raw_int120_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int128_t nd_int128_t(const char* _msg)
{
    on_entry("int128", _msg);
    sol_raw_int128_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int136_t nd_int136_t(const char* _msg)
{
    on_entry("int136", _msg);
    sol_raw_int136_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int144_t nd_int144_t(const char* _msg)
{
    on_entry("int144", _msg);
    sol_raw_int144_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int152_t nd_int152_t(const char* _msg)
{
    on_entry("int152", _msg);
    sol_raw_int152_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int160_t nd_int160_t(const char* _msg)
{
    on_entry("int160", _msg);
    sol_raw_int160_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int168_t nd_int168_t(const char* _msg)
{
    on_entry("int168", _msg);
    sol_raw_int168_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int176_t nd_int176_t(const char* _msg)
{
    on_entry("int176", _msg);
    sol_raw_int176_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int184_t nd_int184_t(const char* _msg)
{
    on_entry("int184", _msg);
    sol_raw_int184_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int192_t nd_int192_t(const char* _msg)
{
    on_entry("int192", _msg);
    sol_raw_int192_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int200_t nd_int200_t(const char* _msg)
{
    on_entry("int200", _msg);
    sol_raw_int200_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int208_t nd_int208_t(const char* _msg)
{
    on_entry("int208", _msg);
    sol_raw_int208_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int216_t nd_int216_t(const char* _msg)
{
    on_entry("int216", _msg);
    sol_raw_int216_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int224_t nd_int224_t(const char* _msg)
{
    on_entry("int224", _msg);
    sol_raw_int224_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int232_t nd_int232_t(const char* _msg)
{
    on_entry("int232", _msg);
    sol_raw_int232_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int240_t nd_int240_t(const char* _msg)
{
    on_entry("int240", _msg);
    sol_raw_int240_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int248_t nd_int248_t(const char* _msg)
{
    on_entry("int248", _msg);
    sol_raw_int248_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_int256_t nd_int256_t(const char* _msg)
{
    on_entry("int256", _msg);
    sol_raw_int256_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_int128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint8_t nd_uint8_t(const char* _msg)
{
    on_entry("uint8", _msg);
    sol_raw_uint8_t retval = 0;

    #ifdef MC_USE_STDINT
    scanf("%hhu", &retval);
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint16_t nd_uint16_t(const char* _msg)
{
    on_entry("uint16", _msg);
    sol_raw_uint16_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint24_t nd_uint24_t(const char* _msg)
{
    on_entry("uint24", _msg);
    sol_raw_uint24_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint32_t nd_uint32_t(const char* _msg)
{
    on_entry("uint32", _msg);
    sol_raw_uint32_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint40_t nd_uint40_t(const char* _msg)
{
    on_entry("uint40", _msg);
    sol_raw_uint40_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint48_t nd_uint48_t(const char* _msg)
{
    on_entry("uint48", _msg);
    sol_raw_uint48_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint56_t nd_uint56_t(const char* _msg)
{
    on_entry("uint56", _msg);
    sol_raw_uint56_t retval = 0;
    cin >> retval;
    return retval;
}

sol_raw_uint64_t nd_uint64_t(const char* _msg)
{
    on_entry("uint64", _msg);
    sol_raw_uint64_t retval = 0;
    cin >> retval;
    return retval;
}

#ifdef MC_USE_STDINT
__int128_t nd_stdint_uint128_t(void)
{
    char input[40];
    scanf("%s", input);

    __int128_t retval = 0;
    for (unsigned int i = 0; input[i] != 0; ++i)
    {
        retval *= 10;
        retval += (__int128_t)(input[i] - '0');
    }

    return retval;
}
#endif

sol_raw_uint72_t nd_uint72_t(const char* _msg)
{
    on_entry("uint72", _msg);
    sol_raw_uint72_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint80_t nd_uint80_t(const char* _msg)
{
    on_entry("uint80", _msg);
    sol_raw_uint80_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint88_t nd_uint88_t(const char* _msg)
{
    on_entry("uint88", _msg);
    sol_raw_uint88_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint96_t nd_uint96_t(const char* _msg)
{
    on_entry("uint96", _msg);
    sol_raw_uint96_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint104_t nd_uint104_t(const char* _msg)
{
    on_entry("uint104", _msg);
    sol_raw_uint104_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint112_t nd_uint112_t(const char* _msg)
{
    on_entry("uint112", _msg);
    sol_raw_uint112_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint120_t nd_uint120_t(const char* _msg)
{
    on_entry("uint120", _msg);
    sol_raw_uint120_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint128_t nd_uint128_t(const char* _msg)
{
    on_entry("uint128", _msg);
    sol_raw_uint128_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint136_t nd_uint136_t(const char* _msg)
{
    on_entry("uint136", _msg);
    sol_raw_uint136_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint144_t nd_uint144_t(const char* _msg)
{
    on_entry("uint144", _msg);
    sol_raw_uint144_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint152_t nd_uint152_t(const char* _msg)
{
    on_entry("uint152", _msg);
    sol_raw_uint152_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint160_t nd_uint160_t(const char* _msg)
{
    on_entry("uint160", _msg);
    sol_raw_uint160_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint168_t nd_uint168_t(const char* _msg)
{
    on_entry("uint168", _msg);
    sol_raw_uint168_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint176_t nd_uint176_t(const char* _msg)
{
    on_entry("uint176", _msg);
    sol_raw_uint176_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint184_t nd_uint184_t(const char* _msg)
{
    on_entry("uint184", _msg);
    sol_raw_uint184_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint192_t nd_uint192_t(const char* _msg)
{
    on_entry("uint192", _msg);
    sol_raw_uint192_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint200_t nd_uint200_t(const char* _msg)
{
    on_entry("uint200", _msg);
    sol_raw_uint200_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint208_t nd_uint208_t(const char* _msg)
{
    on_entry("uint208", _msg);
    sol_raw_uint208_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint216_t nd_uint216_t(const char* _msg)
{
    on_entry("uint216", _msg);
    sol_raw_uint216_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint224_t nd_uint224_t(const char* _msg)
{
    on_entry("uint224", _msg);
    sol_raw_uint224_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint232_t nd_uint232_t(const char* _msg)
{
    on_entry("uint232", _msg);
    sol_raw_uint232_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint240_t nd_uint240_t(const char* _msg)
{
    on_entry("uint240", _msg);
    sol_raw_uint240_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint248_t nd_uint248_t(const char* _msg)
{
    on_entry("uint248", _msg);
    sol_raw_uint248_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}

sol_raw_uint256_t nd_uint256_t(const char* _msg)
{
    on_entry("uint256", _msg);
    sol_raw_uint256_t retval = 0;

    #ifdef MC_USE_STDINT
    retval = nd_stdint_uint128_t();
    #elif defined MC_USE_BOOST_MP
    std::cin >> retval;
    #endif

    return retval;
}