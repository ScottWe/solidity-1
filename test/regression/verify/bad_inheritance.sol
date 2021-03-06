// RUN: %solc %s --c-model --output-dir=%t
// RUN: cd %t
// RUN: cmake -DSEA_PATH=%seapath -DSEA_ARGS="--verify"
// RUN: make cex
// RUN: [ -f cex.ll ]
// RUN: make witness | OutputCheck %s --comment=//
// CHECK: __VERIFIER_error

/**
 * This checks that a run does not exist where constructor passing fails in a
 * simple constructor chain.
 */

contract A {
    int a = 10;
    constructor(int _a) public {
        a = _a;
    }
}

contract B is A {
    constructor(int _a) public A(_a) {}
}

contract C is B {
    constructor(int _a) public B(5) {
        assert(a != 5);
    }
}
