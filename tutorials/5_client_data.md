---
layout: post
title: "5. Reasoning About Client State in SmartACE"
subtitle: "Local Reasoning Part Two"
date: 2020-05-25 12:00:00
categories: [smartace, verification, model checking, local reasoning]
---

# 5. Reasoning About Client State in SmartACE

[Last time](4_arbitrary_clients.md) we used SmartACE to prove safety properties
for any number of clients. The example was by no means trivial, but it was in
many ways a toy. Namely, the `Manager` bundle we analyzed did not keep mappings
from client addresses to client data. In such cases we must
summarize both the client addresses and the client data.

In this tutorial we will prove a simple invariant with client data. First, we
extend the `Manager` bundle to use maps, and describe the property of interest.
We then revisit network topology to better understand the impact of client data.
Using this insight, we then work on summarizing the client data. Finally, we
show how SmartACE applies this theory in practice, and then use it to verify our
new property.

**Note**: This tutorial assumes all commands are run from within the
[SmartAce container](1_installation.md). All tutorial files are available within
the container from the home directory.

## Extending Our Running Example

Let's start by returning to the `Manager` bundle. To briefly recap, the
`Manager` bundle consists of two smart contracts: a `Fund` for clients to
`deposit()` Ether, and a `Manager` which owns and controls access to the `Fund`.
In the past two tutorials we
[identified an ownership exploit](3_transactions.md), patched the bug, and then
[verified its absence for any number of clients](4_arbitrary_clients.md).

We now wish to move on to mapping properties. To keep our example realistic, we
add an `investment` mapping to `Fund` which tracks how much Ether each client
deposits. We add a `Fund.transfer(address, uint256)` method which allows a
client to relinquish `_amount` Ether to `_destination`. The new contract is
given below:

```solidity
contract Fund {
    bool isOpen;
    address owner;

    mapping(address => uint) invested;

    constructor() public { owner = msg.sender; }

    // Access controls.
    modifier ownerOnly() { require(owner == msg.sender); _; }
    function releaseTo(address _new) public ownerOnly { owner = _new; }
    function open() public ownerOnly { isOpen = true; }
    function close() public ownerOnly { isOpen = false; }

    // Invest money into fund.
    function deposit() public payable {
        require(isOpen);
        invested[msg.sender] += msg.value;
    }

    // Move money between accounts.
    function transfer(address _destination, uint _amount) public {
        require(_amount > 0);
        require(invested[_destination] + _amount > invested[_destination]);
        require(invested[msg.sender] >= _amount);

        invested[msg.sender] -= _amount;
        invested[_destination] += _amount;
    }
}

contract Manager {
    Fund fund;

    constructor() public { fund = new Fund(); }

    function openFund() public { fund.open(); }
}
```

We can now consider local client properties. In these properties, taken an
invariant for a small set of properties. For example, the total investment for
two fixed clients is conserved after calling `Fund.transfer(address, uint256)`.
We can then extend this into a local client property by requiring that for any
two clients, their total investments are conserved by
`Fund.transfer(address, uint256)`. In fact, this will be our running example for
the remainder of the tutorial. In past linear temporal logic (pLTL) we would
say:

> It is *always* the case that whenever `msg.sender` calls `Fund.transfer()` and
> sends `_amount` to some other `_dst`, then `Fund.invested[msg.sender]` is
> decreased by  `_amount` while  `Fund.invested[_destination]` is increased by
> `_amount`.

We then formalize the property in the
[VerX Specification Language](https://verx.ch/docs/spec.html). Recall that
`FUNCTION` is the name of the last method called, while `prev(v)` is the value
of `v` before `FUNCTION` was called. `once` and `always` are pLTL operators,
where `once(p)` is true if `p` has ever been true while `always(p)` is true if
`p` has always been true. We write `Fund.transfer(address, uint256)[i]` to refer
to the i-th argument passed to `Fund.transfer(address, uint256)`. This gives the
formalization:

```
always(
    (
        (FUNCTION == Fund.transfer)
        &&
        (msg.sender != Fund.transfer(address,uint256)[0])
    )
    ==>
    (
      (Fund.invested[msg.sender]
            + Fund.transfer(address,uint256)[1]
            == prev(Fund.invested[msg.sender]))
        &&
        (Fund.invested[Fund.transfer(address,uint256)[0]]
            - Fund.transfer(address,uint256)[1]
            == prev(Fund.invested[msg.sender]))
    )
)
```

As a final step, we can constructor a monitor for the property. The monitor will
detect if the property becomes violated, and will allow us to verify the claim.
Our property is essentially a functional post-condition, so the monitor is very
compact. First we introduce the following predicates:

  * `is_transfer := FUNCTION == Fund.transfer`
  * `distinct := msg.sender != _destination`
  * `sent := invested[msg.sender] + _amount = prev(invested[msg.sender])`
  * `recv := prev(invested[_destination]) + _amount = prev(invested[_destination]`

And then give the regular expression
`(!(is_transfer && distinct) || (send && recv))*`.

## Local Reasoning Over Client Mappings

Mappings allow us to associated variables with clients. This fundamentally
changes the `Manager` bundle topology. We can think of this new topology as a
graph with two types of vertices: *process vertices* and *data vertices*. Each
process vertex is assigned to one or more transactions in the `Manager` bundle,
whereas each data vertex is assigned to a single mapping entry. If a process can
write to a mapping entry, there is a directed edge from the process vertex to
the data vertex. If a process can read from a mapping vertex, there is a
directed edge from the data vertex to the process vertex.

This leads us to the general case of local reasoning. We have a network which is
parameterized by the number of processes. Each process has access to some finite
set of shared variables. We will show that all processes accessing the same
variable obey some compositional invariant. We then combine these invariants to
find an invariant of the entire network. We can think of the
[previous tutorial](4_arbitrary_clients.md) as the degenerate case where each
client has zero mapping entries.

### Topology in the `Manager` Bundle

First we need to characterize the `Manager` bundle topology. We will do this by
looking at a given *instance* of the bundle. That is, the topology of the bundle
for a fixed number of clients. We will see that the topology exhibits symmetries
which generalize to any number of clients.

To begin, let's fix the number of clients and consider an arbitrary instance. We
saw in the [previous tutorial](4_arbitrary_clients.md) that each transaction can
touch at most 6 clients at once. Three of these clients are fixed, namely
`address(0)`, `address(Fund)`, and `address(Manager)`. If we fix the final three
clients, we obtain a subset of transactions. If we enumerate all client choices,
we enumerate all possible transactions.

We can use this intuition to assign transitions to processes. First we select
one such subset of clients. We then take the transactions whose addresses are
restricted to these clients. To make this idea more concrete, we can imagine
writing a new contract in which we apply the following modifier to all input
addresses:

```solidity
modifier restrictClients(address _client) {
    require(
        _client == address(0) || _client == address(Manager) ||
        _client == address(Fund) || _client == addr1 ||
        _client == addr3 || _client == addr3
    );
}
```

After restricting the clients, we then assign the transactions to a unique
process vertex. From this vertex, we connect edges to data vertices of each
client. If we repeat this process for each subset of clients, we will map every
transaction onto some process in the network topology.

To illustrate this construction, and to highlight its symmetry, we give the
graphs for 6 and 7 clients. To improve readability, we do not show the data
vertices common to all processes. We name the arbitrary address `A1`, `A2`,
`A3`, and `A4` to stress the point that each address is strictly an identifier.
The topology for 6 clients is:

![](5_one_process.svg)

The topology for 7 clients is:

![](5_four_process.svg)

### Compositional Invariants in the `Manager` Bundle

Local reasoning allows us to take a sufficiently large neighbourhood, against
which we can then prove properties for the entire network. We do this in two
steps. First, we no longer think of each data vertex as belonging to a single
client. Instead, we now let it represent a group of similar clients (formally,
this is an *equivalence class*). We then replace each data vertex with an
invariant which summarizes all possible values at the vertex. We call this
a compositional invariant.

This invariant can be any predicate over the state of the neighbourhood.
Specifically, it can be aware of the class it is summarizing. However, to be
compositional, it must also satisfy three properties.

  1. (Initialization) When the neighbourhood is zero-initialized, the zero
     satisfies the invariant.
  2. (Transaction) If the invariant holds of some clients before they perform a
     transaction, the invariant still holds afterwards.
  3. (Non-interference) If the invariant holds of some client, the actions of
     any other clients cannot break it.

In other words, the compositional invariant holds initially, and is never
violated by any clients. This ensures that the values written into data
vertices are never "forgotten". Using this intuition, it is not hard to see why
local reasoning with a compositional invariant is guaranteed to cover all paths
of execution.

While compositionality ensures that we are complete, it does not necessarily
ensure soundness. If our summary is too weak, it might permit counterexamples
which do not exist in the original bundle. We say that a compositional invariant
is *adequate* if it additionally blocks all counterexamples.

We can use the insight of compositional invariants to better justify our
restricted address values in the [previous tutorial](4_arbitrary_clients.md). As
each address is really a summary of one or more clients, the address values are
no longer meaningful. Instead, what matters are the relationships satisfied by
two or more addresses. We used the program syntax to identify that only equality
mattered, and then we constructed a set of address values which could satisfy
this relation (formally, we created an *abstract domain* of addresses). We then
ensured adequacy by blocking trivial counterexamples (i.e., no two contracts
have the same address).

### Local Reasoning in SmartACE

When we apply local reasoning in SmartACE, we partition the addresses into
*representatives* and *interference*. Representative addresses are used when the
address must refer to a single client, such as a literal address or a contract
address. Interference addresses are used when the address does not refer to a
distinct client. This distinction is important, as at any point during analysis,
we know the exact value of each representative vertex.

[TODO: perhaps we should change the terminology to "distinguished" and
"representative". Distinguished (currently representative[OLD]) addresses are
used to designate a single client. Representative[NEW] (currently interference)
addresses stand in for any member from a group of clients. In other words, it is
the "representative of an equivalence class".]

Before applying local reasoning, SmartACE requires three parameters: the number
of representative addresses, the number of interference addresses, and a
candidate invariant. SmartACE will discover the minimum number of representative
and interference addresses automatically, using the techniques outlined in the
[previous tutorial](4_arbitrary_clients.md). Conversely, the candidate invariant
must be provided manually. SmartACE will verify that the candidate is both
compositional and adequate. In the future, SmartACE will also automate searching
for the invariant.

As an example, we will now walk through verifying a candidate invariant. We
start with the weakest candidate, namely `True`. As `True` implies `True`, this
candidate is compositional by definition. Instead, we can focus entirely on
adequacy. We will do this in two steps. The next section takes a detour and will
briefly outline how maps are modelled in SmartACE. The following section walks
through adequacy checks in SmartACE. With this in mind, let's start by
generating the model:

  * `solc fund.sol --bundle=Manager --c-model --output-dir=fund`
  * `cd fund ; mkdir build ; cd build`
  * `CC=clang-10 CXX=clang++-10 cmake ..`
  * `cmake --build . --target run-clang-format`

#### Mappings in SmartACE

Open `cmodel.c`. At line 8 we can find the mapping itself.

```cpp
struct Map_1 {
  sol_uint256_t data_0;
  sol_uint256_t data_1;
  /* ... Other entries ... */
  sol_uint256_t data_5;
};
```

We observe that the mapping stores a single entry for each restricted address
value. The intention is that during transactions, the `data_` variables form a
snapshot of some neighbourhood within the network. As addresses `0` to `2` are
representatives, these entries are shared between all possible neighbourhoods.
For addresses `3` to `5`, however, the entries are interference and need not
persist across neighbourhoods. This insight will be important once we instrument
the model.

Moving to lines 147 to 181, we will find the operations defined on the mapping.

```cpp
sol_uint256_t Read_Map_1(struct Map_1 *arr, sol_address_t key_0) {
  if (5 == key_0.v)
  {
    return arr->data_5;
  }
  /* ... Other cases ... */
  else if (0 == key_0.v)
  {
    return arr->data_0;
  }
  /* ... Unreachable error case ... */
}

void Write_Map_1(struct Map_1 *arr, sol_address_t key_0, sol_uint256_t dat) {
  if (5 == key_0.v)
  {
    arr->data_5 = dat;
  }
  /* ... Other cases ... */
  else if (0 == key_0.v)
  {
    arr->data_0 = dat;
  }
}
```

The `Read_Map_` and `Write_Map_` are used to read from and write to mappings. We
chose to use structures and methods over C-style arrays for two reasons. First,
C-style arrays do not permit for statically sized, multi-dimensional arrays,
whereas structures can easily encode any statically sized data type. Second, the
use of the `Read_Map_` and `Write_Map_` allow for SmartACE to easily instrument
mapping accesses. We will see in the next tutorial that write instrumentation
allows us to reason locally about global mapping sums.

#### Instrumenting the Model

Now that we understand how SmartACE models a mapping, we are ready to instrument
the adequacy check. There are two parts to the adequacy check:

  1. We must instrument the property, just as we have done in
     [previous tutorials](3_transactions.md).
  2. Before each transaction, we must place new values in each interference
     entry.

If we were to stop after step one, we would be left with a bounded model, just
as we had constructed in [tutorial 3](3_transactions.md). To generalize this to
an arbitrary number of clients, we must begin each transaction by placing new
values at each interference entry. Intuitively, we can think of this as allowing
a new client from the same client group to make a move. Let's see how this looks
in practice.

In general, we must instrument the property against any arbitrary clients. In
fact, we will see an example of this in the next tutorial. However, in our
property, the clients are not arbitrary. They are bounded to the sender and
recipient of each `Fund.transfer(address,uint256)` call. Given this insight, we
first add global ghost variables to track the pre- and post-investments of both
clients:

```cpp
GHOST_VAR sol_uint256_t recipient_pre;
GHOST_VAR sol_uint256_t recipient_post;
GHOST_VAR sol_uint256_t sender_pre;
GHOST_VAR sol_uint256_t sender_post;
```

We then instrument the check in `Fund.transfer(address,uint256)` at line 105:

```cpp
void Fund_Method_transfer(/* Blockchain state */,
                          sol_address_t func_user___destination,
                          sol_uint256_t func_user___amount) {
  sol_uint256_t dest_bal = Read_Map_1(&self->user_invested, func_user___destination);
  sol_uint256_t sender_bal = Read_Map_1(&self->user_invested, sender);

  /* [START] INSTRUMENTATION */
  recipient_pre = dest_bal;
  sender_pre = sender_bal;
  /* [ END ] INSTRUMENTATION */

  sol_require(func_user___amount.v > 0, 0);
  sol_require(dest_bal.v + func_user___amount.v > dest_bal.v, "Require failed.");
  sol_require(sender_bal.v >= func_user___amount.v, "Require failed.");

  Write_Map_1(&self->user_invested, sender,
              Init_sol_uint256_t(sender_bal.v - func_user___amount.v));
  Write_Map_1(&self->user_invested, func_user___destination,
              Init_sol_uint256_t(dest_bal.v + unc_user___amount.v));

  /* [START] INSTRUMENTATION */
  sender_post = Read_Map_1(&self->user_invested, sender);
  recipient_post = Read_Map_1(&self->user_invested, func_user___destination);
  if (sender.v != func_user___destination.v)
  {
    sol_assert(sender_pre.v == sender_post.v + func_user___amount.v, "Failure.");
    sol_assert(recipient_post.v == recipient_pre.v + func_user___amount.v, "Failure.");
  }
  /* [ END ] INSTRUMENTATION */
}
```

Next we instrument the transaction loop. At the start of each transaction, we
must select a process to run the transaction. Since the processes in our network
model are conceptual, this equates to initializing a new neighbourhood. We know
that representative clients are shared between all neighbourhoods, so this
reduces to selecting new values for all interference vertices. We make this
selection in accordance with the compositional invariant.

First, let's make the compositional invariant concrete. By definition, the
compositional invariant is a predicate over process configurations. Therefore,
we add the following definition at the top of `cmodel.c`:

```cpp
// The `True` compositional invariant.
int invariant(struct Manager *c0, struct Fund *c1)
{
  // We ignore the process state...
  (void) c0; (void) c1;
  // ... and always return true.
  return 1;
}
```

Now let's populate the interference vertices. We will do this before dispatching
the transaction in the transaction loop. Navigate to line 240:

```cpp
/* Updates the interference vertices. */
Write_Map_1(&contract_1->user_investments,
            Init_sol_address_t(3),
            Init_sol_uint256_t(nd_uint256_t("investments[3]")));
Write_Map_1(&contract_1->user_investments,
            Init_sol_address_t(4),
            Init_sol_uint256_t(nd_uint256_t("investments[4]")));
Write_Map_1(&contract_1->user_investments,
            Init_sol_address_t(5),
            Init_sol_uint256_t(nd_uint256_t("investments[5]")));

/* Assumes that the compositional invariant holds. */
sol_require(invariant(&contract_0, contract_1), "Bad arrangement.");

switch (next_call) { /* ... Cases and check ... */ }

/* The invariant is `True`, so it trivially hold afterwards. */
```

This new model replaces entries 3, 4 and 5 with the compositional invariant. If
this new program is safe, the invariant must be adequate.

## Proving the Property

Now let's see if the bundle meets its specifications. As before, run:

```
cmake --build . --target verify
```

We find that no counterexample exists. However, we are not given a proof
certificate. At first this may seem like an error, but it is in fact correct. 
If no certificate is given, then additional invariants are not necessary to
prove the property. If we look at how we instrumented the property, this isn't
surprising. Essentially, our property summarizes the operations of
`Fund.transfer(address,uint256)`, and this follows trivially.

## Conclusion

In this tutorial, we learned how SmartACE models bundles with client-owned data.
We were introduced to compositional invariants and adequacy checks. Using these
techniques, we verified a property over all pairs of mapping entires against a
trivial compositional invariant. In the next tutorial, we will look at verifying
more difficult properties against non-trivial compositional invariants.

## References
