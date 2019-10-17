/**
 * Defines assert, require and nd implementations for symbolic execution.
 *  @date 2019
 */

#include "verify.h"

#include "klee/klee.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

void sol_setup(int _argc, const char **_argv)
{
}

void sol_on_transaction(void){

}

void ll_assume(sol_raw_uint8_t _cond){
	klee_assume(_cond);
}

void sol_assert(sol_raw_uint8_t _cond, const char* _msg)
{
    klee_assert(_cond);
}

void sol_require(sol_raw_uint8_t _cond, const char* _msg)
{
    klee_assume(_cond);
}

uint8_t rt_nd_byte(const char* _msg){
    uint8_t res;
    klee_make_symbolic(&res, sizeof(res), "rt_nd_byte");
    return res;
}

sol_raw_int8_t nd_int8_t(const char* _msg){
    sol_raw_int8_t res;
    klee_make_symbolic(&res, sizeof(res), "nd_int8_t");
    return res;
}
sol_raw_uint8_t nd_uint8_t(const char* _msg){
	sol_raw_uint8_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint8_t");
	return res;
}
sol_raw_int16_t nd_int16_t(const char* _msg){
	sol_raw_int16_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int16_t");
	return res;
}
sol_raw_uint16_t nd_uint16_t(const char* _msg){
	sol_raw_uint16_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint16_t");
	return res;
}
sol_raw_int24_t nd_int24_t(const char* _msg){
	sol_raw_int24_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int24_t");
	return res;
}
sol_raw_uint24_t nd_uint24_t(const char* _msg){
	sol_raw_uint24_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint24_t");
	return res;
}
sol_raw_int32_t nd_int32_t(const char* _msg){
	sol_raw_int32_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int32_t");
	return res;
}
sol_raw_uint32_t nd_uint32_t(const char* _msg){
	sol_raw_uint32_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint32_t");
	return res;
}
sol_raw_int40_t nd_int40_t(const char* _msg){
	sol_raw_int40_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int40_t");
	return res;
}
sol_raw_uint40_t nd_uint40_t(const char* _msg){
	sol_raw_uint40_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint40_t");
	return res;
}
sol_raw_int48_t nd_int48_t(const char* _msg){
	sol_raw_int48_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int48_t");
	return res;
}
sol_raw_uint48_t nd_uint48_t(const char* _msg){
	sol_raw_uint48_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint48_t");
	return res;
}
sol_raw_int56_t nd_int56_t(const char* _msg){
	sol_raw_int56_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int56_t");
	return res;
}
sol_raw_uint56_t nd_uint56_t(const char* _msg){
	sol_raw_uint56_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint56_t");
	return res;
}
sol_raw_int64_t nd_int64_t(const char* _msg){
	sol_raw_int64_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int64_t");
	return res;
}
sol_raw_uint64_t nd_uint64_t(const char* _msg){
	sol_raw_uint64_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint64_t");
	return res;
}
sol_raw_int72_t nd_int72_t(const char* _msg){
	sol_raw_int72_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int72_t");
	return res;
}
sol_raw_uint72_t nd_uint72_t(const char* _msg){
	sol_raw_uint72_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint72_t");
	return res;
}
sol_raw_int80_t nd_int80_t(const char* _msg){
	sol_raw_int80_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int80_t");
	return res;
}
sol_raw_uint80_t nd_uint80_t(const char* _msg){
	sol_raw_uint80_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint80_t");
	return res;
}
sol_raw_int88_t nd_int88_t(const char* _msg){
	sol_raw_int88_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int88_t");
	return res;
}
sol_raw_uint88_t nd_uint88_t(const char* _msg){
	sol_raw_uint88_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint88_t");
	return res;
}
sol_raw_int96_t nd_int96_t(const char* _msg){
	sol_raw_int96_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int96_t");
	return res;
}
sol_raw_uint96_t nd_uint96_t(const char* _msg){
	sol_raw_uint96_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint96_t");
	return res;
}
sol_raw_int104_t nd_int104_t(const char* _msg){
	sol_raw_int104_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int104_t");
	return res;
}
sol_raw_uint104_t nd_uint104_t(const char* _msg){
	sol_raw_uint104_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint104_t");
	return res;
}
sol_raw_int112_t nd_int112_t(const char* _msg){
	sol_raw_int112_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int112_t");
	return res;
}
sol_raw_uint112_t nd_uint112_t(const char* _msg){
	sol_raw_uint112_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint112_t");
	return res;
}
sol_raw_int120_t nd_int120_t(const char* _msg){
	sol_raw_int120_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int120_t");
	return res;
}
sol_raw_uint120_t nd_uint120_t(const char* _msg){
	sol_raw_uint120_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint120_t");
	return res;
}
sol_raw_int128_t nd_int128_t(const char* _msg){
	sol_raw_int128_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int128_t");
	return res;
}
sol_raw_uint128_t nd_uint128_t(const char* _msg){
	sol_raw_uint128_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint128_t");
	return res;
}
sol_raw_int136_t nd_int136_t(const char* _msg){
	sol_raw_int136_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int136_t");
	return res;
}
sol_raw_uint136_t nd_uint136_t(const char* _msg){
	sol_raw_uint136_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint136_t");
	return res;
}
sol_raw_int144_t nd_int144_t(const char* _msg){
	sol_raw_int144_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int144_t");
	return res;
}
sol_raw_uint144_t nd_uint144_t(const char* _msg){
	sol_raw_uint144_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint144_t");
	return res;
}
sol_raw_int152_t nd_int152_t(const char* _msg){
	sol_raw_int152_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int152_t");
	return res;
}
sol_raw_uint152_t nd_uint152_t(const char* _msg){
	sol_raw_uint152_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint152_t");
	return res;
}
sol_raw_int160_t nd_int160_t(const char* _msg){
	sol_raw_int160_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int160_t");
	return res;
}
sol_raw_uint160_t nd_uint160_t(const char* _msg){
	sol_raw_uint160_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint160_t");
	return res;
}
sol_raw_int168_t nd_int168_t(const char* _msg){
	sol_raw_int168_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int168_t");
	return res;
}
sol_raw_uint168_t nd_uint168_t(const char* _msg){
	sol_raw_uint168_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint168_t");
	return res;
}
sol_raw_int176_t nd_int176_t(const char* _msg){
	sol_raw_int176_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int176_t");
	return res;
}
sol_raw_uint176_t nd_uint176_t(const char* _msg){
	sol_raw_uint176_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint176_t");
	return res;
}
sol_raw_int184_t nd_int184_t(const char* _msg){
	sol_raw_int184_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int184_t");
	return res;
}
sol_raw_uint184_t nd_uint184_t(const char* _msg){
	sol_raw_uint184_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint184_t");
	return res;
}
sol_raw_int192_t nd_int192_t(const char* _msg){
	sol_raw_int192_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int192_t");
	return res;
}
sol_raw_uint192_t nd_uint192_t(const char* _msg){
	sol_raw_uint192_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint192_t");
	return res;
}
sol_raw_int200_t nd_int200_t(const char* _msg){
	sol_raw_int200_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int200_t");
	return res;
}
sol_raw_uint200_t nd_uint200_t(const char* _msg){
	sol_raw_uint200_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint200_t");
	return res;
}
sol_raw_int208_t nd_int208_t(const char* _msg){
	sol_raw_int208_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int208_t");
	return res;
}
sol_raw_uint208_t nd_uint208_t(const char* _msg){
	sol_raw_uint208_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint208_t");
	return res;
}
sol_raw_int216_t nd_int216_t(const char* _msg){
	sol_raw_int216_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int216_t");
	return res;
}
sol_raw_uint216_t nd_uint216_t(const char* _msg){
	sol_raw_uint216_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint216_t");
	return res;
}
sol_raw_int224_t nd_int224_t(const char* _msg){
	sol_raw_int224_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int224_t");
	return res;
}
sol_raw_uint224_t nd_uint224_t(const char* _msg){
	sol_raw_uint224_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint224_t");
	return res;
}
sol_raw_int232_t nd_int232_t(const char* _msg){
	sol_raw_int232_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int232_t");
	return res;
}
sol_raw_uint232_t nd_uint232_t(const char* _msg){
	sol_raw_uint232_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint232_t");
	return res;
}
sol_raw_int240_t nd_int240_t(const char* _msg){
	sol_raw_int240_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int240_t");
	return res;
}
sol_raw_uint240_t nd_uint240_t(const char* _msg){
	sol_raw_uint240_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint240_t");
	return res;
}
sol_raw_int248_t nd_int248_t(const char* _msg){
	sol_raw_int248_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int248_t");
	return res;
}
sol_raw_uint248_t nd_uint248_t(const char* _msg){
	sol_raw_uint248_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint248_t");
	return res;
}
sol_raw_int256_t nd_int256_t(const char* _msg){
	sol_raw_int256_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_int256_t");
	return res;
}
sol_raw_uint256_t nd_uint256_t(const char* _msg){
	sol_raw_uint256_t res;
	klee_make_symbolic(&res, sizeof(res), "nd_uint256_t");
	return res;
}
