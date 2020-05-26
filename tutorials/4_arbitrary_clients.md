---
layout: post
title: "4. Verifying Client Properties in SmartACE"
subtitle: "An Introduction to Local Reasoning"
date: 2020-05-25 12:00:00
categories: [smartace, verification, model checking, local reasoning]
---

# 4. Verifying Client Properties in SmartACE

In our [previous tutorial](3_transactions.md), we used SmartACE to identify
ownerships exploit in a simple smart contract. We concluded the post by fixing
the bug, and proving its absence for executions of two clients. In practice, we
would like to prove that the property now holds for any number of clients. This
is known as the parameterized model checking problem, and in SmartACE, we solve
it through local reasoning.

Informally, the parameterized model checking problem asks if a property about
clients is invariant for any number of clients. Local reasoning allows us to
solve the parameterized model checking problem with only a fixed number of
clients. In this tutorial, we will return to the `Fund` and `Manager`
[example](3_transactions.md), and show that its proof certificate extends to any
number of clients. In later tutorials we will extend our technique to clients
which store data in mappings.

## The Contract and Property

We now return to the `Manager` bundle. In this new version, we have added an
`ownerOnly()` modifier to the protected interfaces of `Fund`. We have replaced
`claim()` with `releaseTo(address _new)`, so that ownership must be passed on
by the previous owner. As before, the fund should remain closed until
`Manager.openFund()` is called.

```solidity
contract Fund {
    bool isOpen;
    address owner;

    constructor() public { owner = msg.sender; }

    modifier ownerOnly() { require(owner == msg.sender); _; }
    function releaseTo(address _new) public ownerOnly { owner = _new; }

    function open() public ownerOnly { isOpen = true; }
    function close() public ownerOnly { isOpen = false; }
    function deposit() public payable { require(isOpen); }
}

contract Manager {
    Fund fund;

    constructor() public { fund = new Fund(); }

    function openFund() public { fund.open(); }
}
```

As before, we wish to prove the following property:

> It is always the case that if `openFund()` is not called even *once*, then the
> balance of `Manager.fund` prior to and after the last transaction remains
> unchanged.

Using the [VerX Specification Language](https://verx.ch/docs/spec.html) we
formalize the property as:

```
always(
    !(once(FUNCTION == Manager.openFund()))
    =>
    (BALANCE(Fund) == prev(BALANCE(Fund)))
)
```

We then construct a monitor from the property. A more detailed analysis is
given in the [previous tutorial](3_transactions.md).

## Restricting the Addresses of `Fund` and `Manager`

Let's motivate SmartACE's local reasoning techniques with an example. We will
reduce the `Manager` bundle to a fixed number of addresses, and show that if
the original bundle has a counterexample, so does this new bundle. In the
general case, Solidity addresses are both a data domain, and a set of mapping
indices. However, the `Manager` bundle lacks mappings, so we can focus solely on
the data domain. Essentially, we wish to construct an *abstract address domain*,
which is complete relative to the `Manager` bundle.

We will start by looking at the operations used on the abstract address domain
From the source text, we can see that addresses only take part in equality
operations. An equality operation determines whether two addresses are
distinguishable. We can distinguish between any two input addresses in a single
transaction, or between an input address and a constant address. Therefore,
completeness follows if we have enough addresses to distinguish all inputs and
all constants. In other words, we need one distinct address value for each
constant in the bundle, and one distinct address value for each input address.

At first it may seem that the `Manager` bundle is free from constants. However,
the Solidity semantics allows smart contract to implicitly distinguish
`address(0)`. Therefore, the `Manager` bundle has a single constant. Next we
turn our attention to the number address inputs taken by each transaction,
otherwise known as the *transactional address footprint*. To illustrate this
concept, we will find the transactional address footprint of
`Fund.releaseTo(_new)`. Explicitly, each call to `Fund.releaseTo(_new)` takes in
two addresses: `msg.sender` and `_new`. Implicitly, the transaction also has
access to the address of `Manager`, the address of `Fund`, and the value of
`Fund.owner`. This gives a transactional address footprint of 5. If we continued
this analysis for all other transactions, we would find a maximum transactional
address footprint of 5. Therefore, we need 6 distinct addresses in total.

In practice, SmartACE will perform this analysis automatically. We can verify
our results by running:

  * `path/to/solc fund.sol --bundle=Manager --c-model --output-dir=fund`
  * `cd fund ; mkdir build ; cd build`
  * `cmake .. -DSEA_PATH=/path/to/seahorn/bin`
  * `make run-clang-format`


At line 149 of `../cmodel.c`, we find the arbitrary address argument, `_new`,
restricted to a 6 value address space, as expected:

```cpp
case 1: {
    smartace_log("Calling releaseTo(_new) on contract_1]");
    sol_address_t arg__new = Init_sol_address_t(nd_range(0, 6, "_new"));
    Fund_Method_releaseTo(contract_1, sender, value, blocknum, timestamp, paid, origin, arg__new);
    smartace_log("[Call successful]");
    break;
}

```

The next two sections take a closer look at how SmartACE automates local
reasoning. A reader who is more interested in the example property can safely
skip to the [final section](#proving-the-correctness-of-fund-and-manager).

## Topology Checking through Program Syntax

Let's take a closer look at how we constructed the abstract address domain. At a
high level, we exploited syntactic patterns in each method's source code. We
then drew conclusions about the client interactions. In fact, these patterns
expose the underlying network topology, as we will see in the next tutorial. For
now, we will describe these patterns explicitly, and show that they are
applicable to many smart contracts.

The patterns are as follows:

### 1. Equality is the only address relation.

Addresses must be unordered. This disallows many topologies, such as linear
orders. Thankfully, Solidity was designed for wealth transfer between arbitrary
parties, so this restriction is met by major Ethereum protocols such as the
[ERC-20](https://eips.ethereum.org/EIPS/eip-20) and the
[ERC-721](https://eips.ethereum.org/EIPS/eip-721).

### 2. Address operations are never used.

Addresses must also be unstructured. This disallows other topologies, such as
rings and trees. While such topologies can have uses in smart contracts, they
are
[discouraged in relevant examples](https://solidity.readthedocs.io/en/v0.5.3/solidity-by-example.html).

### 3. Address casts are never used.

This follows from the first two restrictions. Address casting is used to apply
arithmetic operators and relations to address values.

### 4. Addresses must come from inputs or named state variables.

This ensures that every transactional address footprint is bounded. In practice,
it prevents iteration over addresses, such as in the following example:

```
contract UnboundedFootprintExample {
    // ...
    function bad(address payable[] _unboundedClients) public {
        for (uint i = 0; i < _unboundedclients.length; i++) {
            applyTo(_unboundedClients[i]);
        }
    }
    // ...
}
```

Solidity
[best practices](https://solidity.readthedocs.io/en/v0.6.8/security-considerations.html#gas-limit-and-loops)
already warn against unbounded loops. Unbounded loops require unbounded gas,
which is [impossible in Ethereum](https://ethgasstation.info/blog/gas-limit/).
As for bounded loops, we can unroll them prior to analysis.

## Restricting Addresses in SmartACE

## Proving the Correctness of `Fund` and `Manager`

As before, we can run `make verify` to obtain a proof certificate. The
[certificate](https://arieg.bitbucket.io/pdf/hcvs17.pdf) is given in the form
of an inductive program invariant. Using the invariant, we can prove that along
any path of the program, every assertion holds. Unfortunately, the invariant is
given with respect to LLVM-bytecode. SmartACE does not yet offer tooling to make
the certificate more readable. Interpreting the certificate requires inspecting
the LLVM-bytecode, and then mapping the registers back to variables. You can
take it on good faith that the certificate states:

  1. `Manager.fund.isOpen => called_openFund`
  2. `Manager.fund.owner == 1`

Lemma one is straight forward. It states that whenever the guard variable for
`Manager.fund.deposit()` is set, then `Manager.openFund()` has been called. We
know that `deposit()` is the only payable method of `Fund`, so the lemma implies
our property. However, this lemma does not ensure that other clients cannot call
`Manager.fund.open()`. It is only inductive relative to lemma two.

Lemma two is about addresses, so we must take more care in interpreting it. From
the previous section, we know that `0` maps to `address(0)`, `1` maps to
`address(Manager)`, and `2` maps to `address(Fund)`. Then we can read lemma two
as `Manager.fund.owner == address(Manager)`. This means that the ownership of
`Manager.fund` is invariant to any sequence of transactions from any sequence of
arbitrary clients (i.e., addresses `3` and `4`). We know that `Manager` cannot
start a transaction, and that only `Manager.fund.owner` can call
`Manager.fund.open()`, so this lemma blocks all counterexamples to lemma one.
