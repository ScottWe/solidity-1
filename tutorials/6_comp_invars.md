---
layout: post
title: "6. Handling Client Interference in SmartACE"
subtitle: "Local Reasoning Part Three"
date: 2020-05-25 12:00:00
categories: [smartace, verification, model checking, local reasoning]
---

# 6. Reasoning About Client State in SmartACE

By Scott Wesley in collaboration Maria Christakis, Arie Gurfinkel, Xinwen Hu,
Jorge Navas, Richard Trefler, and Valentin Wüstholz.

In the [last tutorial](5_client_data.md), we applied local reasoning to verify a
contract with client state. The key insight of our proof was to summarize the
set of acceptable client interactions, and then prove that this summary was
never broken by interfering clients. However, our example was deceptively
simple. Namely, our summary included all interactions, so it was trivially
robust to interference.

In this tutorial, we consider a property which can be violated by certain client
interactions. First, we define a set of interactions which excludes the bad
examples. We then extend the SmartACE model to automatically verify the
robustness of this set under interference.

To get started, let's define the smart contract, and the property of interest.

**Note**: This tutorial assumes all commands are run from within the
[SmartAce container](1_installation.md). All tutorial files are available within
the container from the home directory.

## Extending Our Running Example

We consider another variation on the `Manager` bundle. In short, this bundle
implements a `Manager` contract which controls access to an `Auction` contract.
The `Auction` contract allows clients to `deposit()` and `withdraw()` Ether,
which is then aggregated in a `bids` mapping.

The contract is designed such that maximum bid is strictly increasing. To
achieve this, the `maxBid` variable is used to retain the maximum ibid. Net
deposits must alway exceed `maxBid` while withdraws must always be less than
`maxBid`.

```solidity
contract Auction {
    bool isOpen;
    address owner;

    uint256 maxBid;

    mapping(address => uint) bids;

    constructor() public { owner = msg.sender; }

    // Access controls.
    modifier ownerOnly() { require(owner == msg.sender); _; }
    function releaseTo(address _new) public ownerOnly { owner = _new; }
    function open() public ownerOnly { isOpen = true; }
    function close() public ownerOnly { isOpen = false; }

    // Place a bid.
    function deposit() public payable {
        uint256 bid = bids[msg.sender] + msg.value;
        require(isOpen);
        require(bid > maxBid);
        bids[msg.sender] = bid;
        maxBid = bid;
    }

    // Withdraw a losing bid.
    function withdraw() public payable {
        uint256 bid = bids[msg.sender];
        require(isOpen);
        require(bid < maxBid);
        bids[msg.sender] = 0;
        msg.sender.transfer(bid);
    }
}

contract Manager {
    Auction auction;

    constructor() public { auction = new Auction(); }

    function openAuction() public { auction.open(); }
}
```

The contract is available
[here](https://github.com/ScottWe/smartace-examples/blob/master/tutorials/post-6/auction.sol).

## Defining the Property

The correctness of this contract rests on two key observations:

  1. No client's bid ever exceeds the maximum recorded bid.
  2. After bidding has started, a unique client has the maximum recorded bid.

In this tutorial we focus on property one. Let's start by making this statement
more precise.

> It is always the case that each investment in `Auction.bids` is at most the
> value of `Auction.maxBid`.

Now we can translate the property into the
[VerX Specification Language](https://verx.ch/docs/spec.html). We note that this
property is universally quantified across the set of addresses. This requires
notion not seen in the previous tutorials. In particular, the VerX specification
language allows us to write `property p(X) { <Formula Over X> }`, to denote a
property quantified over all possible X. Therefore, our property becomes:

```
property p(X) {
    always(
        Auction.maxBid >= Auction.bids[X]
    )
}
```

## Compositional Reasoning Revisited

In the [last tutorial](5_client_data.md), we defined the compositional invariant
and the adequate compositional invariant. Intuitively, the compositional
invariant is a summary of the client interactions, while an adequate
compositional invariant is a summary which implies our property of interest.
Formally, an invariant is compositional if it satisfies:

  1. (Initialization) When the neighbourhood is zero-initialized, the data
     vertices satisfy the invariant.
  2. (Local Inductiveness) If the invariant holds for some clients before they
     perform a transaction, the invariant still holds afterwards.
  3. (Non-interference) If the invariant holds for some client, the actions of
     any other clients cannot break it.

Each of these predicates is with respect to a bounded set of clients. We call
this set a local neighbourhood. If our neighbourhood is large enough, the proof
generalizes to any number of clients. The details of this can be found in the
[last tutorial](5_client_data.md).

## Instrumenting the Smart Contract

The previous tutorials have gone into great detail on the instrumentation of
[local safety properties](4_arbitrary_clients.md) and
[adequacy checks](5_client_data.md). Therefore, we focus our attention on the
compositional invariant. We have two challenges. First, we find an adequate
candidate predicate. Second, we must prove that this predicate is compositional.
As of now, the selection of candidate predicates is manual. However, we present
the selection as a mechanical process, which we intend to automate in future
work.

To this end, let's start by generating the model:

  * `solc auction.sol --bundle=Manager --c-model --output-dir=auction`
  * `cd auction ; mkdir build ; cd build`
  * `CC=clang-10 CXX=clang++-10 cmake ..`
  * `cmake --build . --target run-clang-format`

For simplicity, let's also translate the property into a simple `C` method. The
method takes in a manager bundle, and checks the property for a given address:

```cpp
int property(struct Manager *c0, sol_address_t addr)
{
  sol_uint256_t bid = Read_Map_1(&(c0->user_auction.user_bids), addr);
  sol_uint256_t maximum = c0->user_auction.user_maxBid;
  return bid.v <= maximum.v;
}
```

Let's also add a placeholder predicate for the compositional invariant. As
`True` is always compositional, let's use that:

```cpp
int invariant(struct Manager *c0)
{
  return 1;
}
```

Throughout the rest of this example, we produce several variations of the model.
All variations each available online. The above instrumentation can be found at
line 213 of the
[first variation](https://github.com/ScottWe/smartace-examples/blob/master/tutorials/post-6/instrumented/cmodel_0.c).

### Attempt One: The `True` Invariant

We want to show that `for all clients x: property(Manager, x)` is an inductive
invariant of the smart contract. If we were to tackle this directly, we would
prove that the property holds after constructing the bundle, and then continues
to hold after each transaction. However, in the local setting, we need only
prove that the compositional invariant implies the property. This is because the
compositional invariant summarizes all possible clients.

This gives us the following
[model variation](https://github.com/ScottWe/smartace-examples/blob/master/tutorials/post-6/instrumented/cmodel_1.c):

```cpp
/* ... Contract initialization ... */

while (sol_continue()) {
  // [START] INSTRUMENTATION (LINE 250)
  /* Reached upon initialization, and after each iteration. */
  /* First we construct an arbitrary network. */
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(3),
              Init_sol_uint256_t(nd_uint256_t("bids[3]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(4),
              Init_sol_uint256_t(nd_uint256_t("bids[4]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(5),
              Init_sol_uint256_t(nd_uint256_t("bids[5]")));
  sol_require(invariant(&contract_0), "Bad arrangement.");
  /* Next, we check the property against this arbitrary neighbourhood. */
  sol_assert(property(&contract_0, Init_sol_address_t(0)), "Violation of Prop(0).");
  /* ... */
  sol_assert(property(&contract_0, Init_sol_address_t(5)), "Violation of Prop(5).");
  // [ END ] INSTRUMENTATION

  /* ... Call setup ... */

  /* We allow the same local neighbourhood to run the transaction. */
  switch (next_call) { /* ... Run transactions ... */ }
}
```

#### The Adequacy Test

We can check this model by running `cmake --build . --target verify`. This shows
that the property dose not hold. To get some more insight, let's enable logging
and generate a witness:

  * `cmake .. -DSEA_EXELOG=true`
  * `cmake --build . --target witness`

We obtain the following concise trace.

```
[Initializing contract_0 and children]
sender [uint8]: 3
blocknum [uint256]: 0
timestamp [uint256]: 0
[Entering transaction loop]
bids[3] [uint256]: 1
bids[4] [uint256]: 0
bids[5] [uint256]: 0
assert: Violation of Prop(3).
[sea] __VERIFIER_error was executed
```

If we go to the assertion associated with `assert: Violation of Prop(3)` we find
that `bids[3] <= maxBid` has failed. If we walk back through the trace, we find
the root cause of this problem, namely `bids[3] [uint256]: 1`. This says that
`bids[3]` was assigned to a value larger than the maximum bid. Clearly, this
assignment is not feasible.

### Attempt Two: A Refined Invariant

From the above analysis, we can see that the counterexample was spurious. Let's
try to refine our compositional invariant given their trace. Perhaps there is a
linear relation between `bids[3]` and one of the program variables. The obvious
candidate is `bids[3] <= maxBid`. As `address(3)` corresponds to an arbitrary
client, it is likely our invariant generalizes to all clients. This gives us the
new predicate. The loop is bounded, and unrolls to checking `bids[i] <= maxBid`
for each client in the neighbourhood.

```cpp
int invariant(struct Manager *c0)
{
  sol_uint256_t maximum = c0->user_auction.user_maxBid;
  for (int i = 0; i < 6; ++i)
  {
    sol_address_t addr = Init_sol_address_t(i);
    sol_uint256_t bid = Read_Map_1(&(c0->user_auction.user_bids), addr);
    if (bid.v > maximum.v) return 0;
  }
  return 1;
}
```

We can find this new variation
[here](https://github.com/ScottWe/smartace-examples/blob/master/tutorials/post-6/instrumented/cmodel_2.c).
If we rerun `cmake --build . --target verify` we see that this candidate is
indeed adequate.

However, we still have yet to prove the compositionality of our new invariant.
We can automate this check by instrumenting one final model. We do this by
instrumenting `Initialization`, `Local Inductiveness` and `Non-interference` as
program assertions. While most of the code is fairly straight forward, we do
make use of two modeling tricks.

  1. When checking `Initialization` and `Non-interference` we use
     non-deterministic flag variables to model overlap between the two
     neighbourhoods.
  2. To simulate an external process, we compute two neighbourhoods for the same
     program state, and cache the first result. This simulates an untouched
     mapping entry.

To follow along, this last variation is available
[here](https://github.com/ScottWe/smartace-examples/blob/master/tutorials/post-6/instrumented/cmodel_3.c)

```cpp
/* ... Contract initialization ... */

// [START] INSTRUMENTATION (LINE 256)
/* We select some initial neighbourhood. Maps are zero-initialized. */
sol_uint256_t ZERO = Init_sol_uint256_t(0);
if (nd_range(0, 2, "Use external address(3)")) {
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(3), ZERO);
}
if (nd_range(0, 2, "Use external address(4)")) {
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(4), ZERO);
}
if (nd_range(0, 2, "Use external address(5)")) {
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(5), ZERO);
}
sol_assert(invariant(&contract_0), "Initialization is violated.");
// [ END ] INSTRUMENTATION

while (sol_continue()) {
  // [START] INSTRUMENTATION (LINE 270)
  /* Select and cache an arbitrary neighbourhood to check non-interference. */
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(3),
              Init_sol_uint256_t(nd_uint256_t("external bids[3]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(4),
              Init_sol_uint256_t(nd_uint256_t("external bids[4]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(5),
              Init_sol_uint256_t(nd_uint256_t("external bids[5]")));
  sol_uint256_t extern_3
    = Read_Map_1(&contract_1->user_bids, Init_sol_address_t(3));
  sol_uint256_t extern_4
    = Read_Map_1(&contract_1->user_bids, Init_sol_address_t(4));
  sol_uint256_t extern_5
    = Read_Map_1(&contract_1->user_bids, Init_sol_address_t(5));
  sol_require(invariant(&contract_0), "Bad arrangement.");
  // [ END ] INSTRUMENTATION

  // [START] INSTRUMENTATION (LINE 284)
  /* Select a new neighbourhood to take a step. */
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(3),
              Init_sol_uint256_t(nd_uint256_t("bids[3]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(4),
              Init_sol_uint256_t(nd_uint256_t("bids[4]")));
  Write_Map_1(&contract_1->user_bids, Init_sol_address_t(5),
              Init_sol_uint256_t(nd_uint256_t("bids[5]")));
  sol_require(invariant(&contract_0), "Bad arrangement.");
  // [ END ]

  /* ... Call setup ... */

  switch (next_call) { /* ... Run transactions ... */ }

  // [START] INSTRUMENTATION (LINE 347)
  sol_assert(invariant(&contract_0), "Local Inductiveness is violated.");
  // [ END ] INSTRUMENTATION

  // [START] INSTRUMENTATION (LINE 349)
  /* Checks non-interference, taking into account overlapping neighbourhoods. */
  if (nd_range(0, 2, "Use external address(3)")) {
    Write_Map_1(&contract_1->user_invested, Init_sol_address_t(3), extern_3);
  }
  if (nd_range(0, 2, "Use external address(4)")) {
    Write_Map_1(&contract_1->user_invested, Init_sol_address_t(4), extern_4);
  }
  if (nd_range(0, 2, "Use external address(5)")) {
    Write_Map_1(&contract_1->user_invested, Init_sol_address_t(5), extern_5);
  }
  sol_assert(invariant(&contract_0), "Non-interference is violated.");
  // [ END ] INSTRUMENTATION
}
```

#### The Compositionality Test

If we rerun `cmake --build . --target verify`, all assertions will hold. This
proves that our predicate obeys `Initialization`, `Local Inductiveness`, and
`Non-interference`. Seahorn did not produce an invariant, so the predicate was
strong enough on its own. In other words, we have found the adequate
compositional invariant for our property.

#### Some Additional Remarks on Refinement

Let's assume that the second predicate also failed. By failing the
composoitionality test and passing the adequacy test, we would know that the
predicate was too strong. We would need to expand the summary to account for the
missing interactions. This is similar to when we restricted the summary after
failing an adequacy test.

In principle, we could repeat this procedure until finding (or failing to find)
an adequate compositional invariant. As each summary associates a finite space
of shared contract variables to a finite space of local neighbourhoods, we
could enumerate all possible candidates in theory. In practice, most of these
variables would range over 256-bit integers, and therefore, enumerating the
entire space would be infeasible. Automating this search is the topic of future
work.

## Conclusion

In this tutorial we saw how to test the compositionality of a given predicate.
We used this insight to prove the correctness of a global client property. We
also motivated a procedure to find compositional invariants. This concludes our
three part series on the foundations of local reasoning. in the next tutorial,
we will explore more challenging applications of local reasoning in smart
contracts.
