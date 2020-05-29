---
layout: post
title: "5. Reasoning About Client State in SmartACE"
subtitle: "Local Reasoning Part Two"
date: 2020-05-25 12:00:00
categories: [smartace, verification, model checking, local reasoning]
---

# 5. Reasoning About Client State in SmartACE

## Extending Our Running Example

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
    function transfer(address _dst, uint _amt) {
        require(_amt > 0);
        require(invested[_to] + _amt > invested[_to]);
        require(invested[msg.sender] >= _amt);

        invested[msg.sender] -= _amt;
        invested[_to] += _amt;
    }
}

contract Manager {
    Fund fund;

    constructor() public { fund = new Fund(); }

    function openFund() public { fund.open(); }
}
```

> It is *always* the case that whenever `msg.sender` calls `Fund.transfer()` to
> send `_amt` to `_dst`, then `Fund.invested[msg.sender]` decrease by `_amt` and
> `Fund.invested[_to]` increases by amount.

```
always(
    (FUNCTION == Fund.transfer)
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
saw in the previous tutorial that each transaction can touch at most 6 clients
at once. Three of these clients are fixed, namely `address(0)`, `address(Fund)`,
and `address(Manager)`. If we fix the final three clients, we obtain a subset of
transactions. If we enumerate all client choices, we enumerate all possible
transactions.

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

### Local Reasoning in SmartACE

## Proving the Property
