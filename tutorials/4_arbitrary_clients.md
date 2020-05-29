---
layout: post
title: "4. Verifying Client Properties in SmartACE"
subtitle: "An Introduction to Local Reasoning"
date: 2020-05-25 12:00:00
categories: [smartace, verification, model checking, local reasoning]
---

# 4. Verifying Client Properties in SmartACE

In our [previous tutorial](3_transactions.md), we used SmartACE to identify
ownership exploits in a simple smart contract. We concluded the tutorial by
fixing the bug, and proving its absence for executions with two clients.
However, a real developer would want to verify the property holds for any
number of clients. This is known as the parameterized model checking problem,
and in SmartACE, we solve it through local reasoning.

Informally, the parameterized model checking problem asks whether a property
about clients is invariant for any number of clients. Local reasoning allows us
to reduce the parameterized model checking problem to a model with a fixed
number of clients. In this tutorial, we will return to the `Fund` and `Manager`
[example](3_transactions.md), and show that its proof certificate extends to any
number of clients. In the following tutorials, we will extend our technique to
contracts with mappings of client data.

## The Contract and Property

We now return to the `Manager` bundle given below. As before, we have two
smart contracts: `Fund` and `Manager`. The `Manager` constructs a new `Fund`,
for which it is is initially its owner. The `Manager` exposes a single method,
`openFund()`, which allows clients to deposit Ether into `Fund`. To fix the
ownership exploit in the previous example, we have added an `ownerOnly()`
modifier to the protected interfaces of `Fund`, and have replaced `claim()` with
`releaseTo(_new)` so that `Manager` must approve ownership transfers.

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
    ==>
    (BALANCE(Fund) == prev(BALANCE(Fund)))
)
```

We then construct a monitor from the property. A more detailed analysis is
found in the [previous tutorial](3_transactions.md).

## Limiting Addresses in the `Manager` Bundle

In the first tutorial, contracts were *oblivious* to their clients. No matter
which client initiated a transaction, the outcome would always be the same. In
reality, most contracts are *client aware*. They read from `msg.sender`, and
will adjust their behaviour according. This is true of the `Fund` contract, as
it can designate a single client as its `owner`. To show that our property holds
in general, we must prove that adding an additional client will never introduce
a new interaction which violates the property.

We could tackle this problem directly by modeling every client. However, this
would not scale to bundles with even a small amount of client state. Instead, it
would be ideal to summarize the clients, and then retain only a small set of
addresses. Even in the case of `Fund`, where clients lack state, we would still
benefit from reducing the search space of interleaving client transactions.

For this reason, let's try proving the property with a fixed number of
addresses. For convenience, we will refer to the arrangement of clients and
contracts as a *network topology*, and we will refer to the subset of clients as
a *neighbourhood*. We will justify these terms in the next tutorial.

So what happens when we construct a neighbourhood? If we select too few clients,
we may lose behaviours from the original bundle (formally, this is an
*underapproximation*). For instance, if we deployed `Fund` with a single client,
the `owner` of `Fund` would never change. However, if we introduced too many
clients, this could lead to redundant traces (formally, this is *symmetry*). For
example, if we deployed `Fund` with 100 clients, there would be many identical
ways to select an initial `owner`, and then transfer to a secondary `owner`.

If we look closely at `Fund`, we will see that addresses only appear in equality
relations. An equality relation can distinguish between any two address
arguments, or between an address argument and a literal address. Note that due
to the semantics of Solidity, `address(0)` is an implicit literal of all
bundles. We are guaranteed to cover all paths of execution if we can assign a
unique value to each address in any transaction. The specific values are
unimportant, provided that we include all literals inside the neighbourhood.

In other words, we must find the maximum number of addresses use by any
transaction. We call this number the maximum *transactional address footprint*.
As the `Manager` bundle is free from address operators, we only need to count
the number of contract addresses, address variables, and address arguments in
each transaction. We find that `Fund.releaseTo(address)` has the maximum address
footprint, with two contract addresses, one address variable (`owner`) and two
address arguments (`msg.sender` and `_new`). Therefore, our neighbourhood will
have 6 addresses.

SmartACE will perform this analysis automatically. We can verify our results by
runnings the following command and then inspecting the model.

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

The next two sections take a closer look at how SmartACE automatically restricts
the network topology to a sufficient neighbourhood. The reader who is only
interested in the running example can safely skip to the
[final section](#proving-the-correctness-of-fund-and-manager).

## Topology Checking through Program Syntax

To understand how SmartACE automates the address analysis, we will take a closer
look at how we constructed the abstract address domain. At a high level, we
exploited syntactic patterns in each method's source code. We then drew
conclusions about the client interactions. In fact, these patterns expose the
underlying network topology, as we will see in the next tutorial. For now, we
will describe these patterns explicitly, and show that they are applicable to
many smart contracts.

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

When the above patterns hold, SmartACE will compute can abstract address domain.
In theory, we need one address for each distinguishable address, otherwise known
as a *client equivalence class*. In theory, there is one class for each smart
contract and one class for every arbitrary transactional input. Constants and
state variables as not so simple. In theory there is a class for each constant
and each state variable, along with their intersections with any other class.
For example, a smart contract address may be a constant and also used as a state
variable.

However, for the `Manager` bundle we ended up with linearly many addresses,
despite the presence of constants and state variables. Implicitly we made the
following observations. First, we know that each constant is unique, therefore
intersects with a single client or a single contract in any viable execution. We
decide this intersection at the start of the model. We make a similar reduction
for state variables by tracking the active equivalence class throughout the
execution.

Therefore, we require one address for each smart contract, one address for each
transactional input, one address for each state variable, and one address for
each constant.

### Encoding the Addresses in SmartACE

SmartACE encodes these addresses in a consistent way. The address `0` is always
the constant `address(0)`. The addresses immediately following `0` are contract
addresses. The remaining addresses are arbitrary clients, state variable
placeholders, and constant placeholders in any order. For non-zero constants, we
non-deterministically assign it a unique address. As non-zero constants are
uncommon in real smart contracts, this often yields a deterministic assignment.

## Proving the Correctness of `Fund` and `Manager`

Now we finish with the running example. As before, we can run `make verify` to
obtain a proof certificate. The
[certificate](https://arieg.bitbucket.io/pdf/hcvs17.pdf) is given in the form of
an inductive program invariant. Using the invariant, we can prove that along any
program path, every assertion holds. Unfortunately, the invariant is given with
respect to LLVM-bytecode. SmartACE does not yet offer tooling to make the
certificate more readable. Interpreting the certificate requires inspecting the
LLVM-bytecode, and then mapping the registers back to variables. You can
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
