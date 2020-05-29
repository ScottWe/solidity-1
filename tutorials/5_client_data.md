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
