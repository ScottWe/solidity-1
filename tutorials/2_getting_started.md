---
layout: post
title: "2. Finding Representation Invariants for Smart Contracts"
subtitle: "Basic Auditing with SmartACE"
date: 2020-05-18 15:00:00
categories: [smartace, verification, model checking, fuzzing, invariants]
---

# 2. Finding Representation Invariants for Smart Contracts

An important aspect of smart contract analysis is functional verification. That
is, the ability to check if a *bundle* (a set of smart contracts) satisfies a
given requirement. Often these requirements are written independently from the
implementation and refer only to the interface.

In this tutorial we will prove a simple requirement by finding a "representation
invariant", and then use fuzz testing to corroborate our proof. We will learn
to:

  1. Instrument a smart contract at the Solidity level.
  2. Generate a SmartACE model from a single smart contract.
  2. Use software model checking to find a representation invariant.
  4. Use fuzz testing to search for violations in the smart contract.

## Instrumenting a Solidity Smart Contract

Let's start with a *crowdsale* smart contract designed to raise `goal` funds by
`deadline`. Clients may call `finish()` if the goal is reached by `deadline`,
and `cancel()` if the goal is not reached by `deadline`.

```solidity
contract Crowdsale {
    uint raised;
    uint goal;
    uint deadline;

    // bool finished;
    // bool canceled;

    constructor(uint _goal) public {
        goal = _goal;
        deadline = now + 365 day;
    }

    function invest() public payable {
        require(now <= deadline);
        raised += msg.value;
    }

    function finish() public {
        require(address(this).balance >= goal);
        // finished = true;
    }

    function cancel() public {
        require(address(this).balance < goal && now > deadline);
        // canceled = true;
    }

    // function repOK() public view {
    //     assert(!(finished && canceled));
    // }
}
```

A real crowdsale would track each client's contribution through the use of a
map. We will look at map properties in a future tutorial. For now, let's verify
that `finish()` and `cancel()` are mutually exclusion.

Our property says that given a valid implementation, `Crowdsale` should never
move between a *finished* state and *canceled* state. We prove this property by
finding a [representation invariant](http://www.cs.cornell.edu/courses/cs312/2005sp/lectures/lec09.html).
The invariant will summarize the concrete states of `Crowdsale`, and imply that
*finished* and *canceled* are mutually exclusive.

We can instrument this by:

  1. Adding two ghost variable which track when the contract is `finished` or
     `canceled`.
  2. Adding a public method which asserts that both ghost variables are never
     true at the same time.

This instrumentation is given by the comments. In later tutorials we will look
at properties which are infeasible to instrument at the Solidity level. Save the
uncommented code as `crowdsale.sol`.

## Generating a SmartACE Model

You can generate a model checking friendly model by running:

```
/path/to/install/run/bin/solc crowdsale.sol --bundle=Crowdsale --c-model \
    --output-dir=mc
```

You can generate a fuzzer friendly model by running:

```
/path/to/install/run/bin/solc crowdsale.sol --bundle=Crowdsale --concrete \
    --reps=5 --c-model --output-dir=fuzz
```

In future tutorials we will look at the model encoding. For now we focus on the
parameters to the tool.

  * `bundle` is the list of smart contracts we want to verify.
  * `concrete` requests a model with a bounded number of clients.
  * `reps` gives the number of concrete clients ("*representatives*") to
    generate.
  * `c-model` sets the output mode to a SmartACE model.
  * `output-dir` gives the output directory.

To view all options execute `/path/to/install/bin/solc --help`.

## Checking the SmartACE Model

This example requires an installation of `Seahorn` as described
[here](1_installation.md). To run the model checker:

  1. `cd mc ; mkdir build ; cd build`
  2. `cmake .. -DSEA_PATH=/path/to/seahorn/bin`
  3. `make verify`

This will use `Seahorn` to verify the smart contract for an unbounded number of
clients. The default analysis handles integers arithmetically. That is, overflow
and underflow are not modeled. When the analysis completes, you will see a
certificate of correctness. This certificate is the representation invariant.

In future tutorials, we will learn how to interpret this certificate, and how to
debug a model when `Seahorn` fails to verify the property.

## Fuzzing the SmartACE Model

This example requires support for of `libfuzzer` as described
[here](1_installation.md). To run the fuzzer:

  1. `cd fuzz ; mkdir build ; cd build`
  2. `cmake .`
  3. `make fuzz`

By default, `fuzz` will run `1000000` trials with a timeout of `15` seconds.
Each trial terminates if a `require` fails. A trial fails if an `assert` fails.
After a few minutes, all trial will pass, as expected. You will see a summary
of the tests printed to the console.
