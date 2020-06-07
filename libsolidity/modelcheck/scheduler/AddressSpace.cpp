/**
 * Decouples address logic from the scheduler. This allows for address models to
 * be interchanged or maintained without analysis of the entire scheduler.
 * @date 2020
 */

#include <libsolidity/modelcheck/scheduler/AddressSpace.h>

#include <libsolidity/modelcheck/analysis/MapIndex.h>
#include <libsolidity/modelcheck/codegen/Literals.h>
#include <libsolidity/modelcheck/utils/Contract.h>
#include <libsolidity/modelcheck/utils/Indices.h>
#include <libsolidity/modelcheck/utils/LibVerify.h>

#include <list>
#include <memory>

using namespace std;

namespace dev
{
namespace solidity
{
namespace modelcheck
{

// -------------------------------------------------------------------------- //

AddressSpace::AddressSpace(MapIndexSummary const& _addrdata)
 : MIN_ADDR((_addrdata.literals().find(0) != _addrdata.literals().end()) & 1)
 , MAX_ADDR(_addrdata.representative_count())
 , M_ADDRDATA(_addrdata)
 , m_next_addr(MIN_ADDR)
{
}

// -------------------------------------------------------------------------- //

uint64_t AddressSpace::reserve()
{
    uint64_t retval = m_next_addr;
    if (retval >= MAX_ADDR)
    {
        throw runtime_error("AddressSpace::reserve used without free address.");
    }
    ++m_next_addr;
    return retval;
}

// -------------------------------------------------------------------------- //

void AddressSpace::map_constants(CBlockList & _block) const
{
    list<shared_ptr<CIdentifier>> used_so_far;
    if (M_ADDRDATA.literals().size() > 1)
    {
        LibVerify::log(_block, "[Handling constants]");
    }
    for (auto lit : M_ADDRDATA.literals())
    {
        auto const NAME = modelcheck::Indices::const_global_name(lit);
        auto decl = make_shared<CIdentifier>(NAME, false);

        if (lit == 0)
        {
            _block.push_back(decl->assign(Literals::ZERO)->stmt());
        }
        else
        {
            _block.push_back(decl->assign(
                LibVerify::range(MIN_ADDR, MAX_ADDR, NAME)
            )->stmt());

            for (auto otr : used_so_far)
            {
                // TODO: bad for fuzzing, though used_so_far is often small.
                _block.push_back(make_shared<CBinaryOp>(
                    decl, "!=", otr
                )->stmt());
            }

            used_so_far.push_back(decl);
        }
    }
}

// -------------------------------------------------------------------------- //

}
}
}