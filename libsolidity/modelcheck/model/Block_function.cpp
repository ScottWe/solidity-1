#include <libsolidity/modelcheck/model/Block.h>

#include <libsolidity/modelcheck/analysis/AnalysisStack.h>
#include <libsolidity/modelcheck/analysis/TypeNames.h>
#include <libsolidity/modelcheck/model/Expression.h>
#include <libsolidity/modelcheck/utils/Function.h>
#include <libsolidity/modelcheck/utils/General.h>
#include <libsolidity/modelcheck/utils/Types.h>

#include <stdexcept>

using namespace std;

namespace dev
{
namespace solidity
{
namespace modelcheck
{

// -------------------------------------------------------------------------- //

FunctionBlockConverter::FunctionBlockConverter(
    FunctionDefinition const& _func, shared_ptr<AnalysisStack const> _stack
): GeneralBlockConverter(
	_func.parameters(),
	_func.returnParameters(),
	_func.body(),
	_stack,
	_func.modifiers().empty(),
	_func.isPayable()
)
{
	if (has_retval())
	{
		// TODO(scottwe): support multiple return types.
		m_rv = _func.returnParameters()[0];
	}
}

// -------------------------------------------------------------------------- //

void FunctionBlockConverter::set_for(FunctionSpecialization const& _for)
{
	m_spec = &_for;
}

// -------------------------------------------------------------------------- //

void FunctionBlockConverter::enter(
    CBlockList & _stmts, VariableScopeResolver & _decls
)
{
	_decls.assign_spec(m_spec);
	if (m_rv && !m_rv->name().empty())
	{
		_decls.record_declaration(*m_rv);
		_stmts.push_back(make_shared<CVarDecl>(
			m_stack->types()->get_type(*m_rv), _decls.resolve_declaration(*m_rv)
		));
	}
}

void FunctionBlockConverter::exit(
    CBlockList & _stmts, VariableScopeResolver & _decls
)
{
    if (m_rv && !m_rv->name().empty())
    {
		auto rv = make_shared<CVarDecl>(
			m_stack->types()->get_type(*m_rv), _decls.resolve_declaration(*m_rv)
		);
		_stmts.push_back(make_shared<CReturn>(rv->id()));
    }
}

// -------------------------------------------------------------------------- //

bool FunctionBlockConverter::visit(Return const& _node)
{
	CExprPtr rv;
	switch (block_type())
	{
	case BlockType::Action:
		new_substmt<CReturn>(nullptr);
		break;
	case BlockType::Operation:
	case BlockType::AddressRef:
		rv = expand(*_node.expression());
		rv = InitFunction::wrap(*m_rv->annotation().type, move(rv));
		if (block_type() == BlockType::AddressRef)
		{
			rv = make_shared<CReference>(rv);
		}
		new_substmt<CReturn>(rv);
		break;
	case BlockType::Initializer:
		new_substmt<CExprStmt>(expand(*_node.expression()));
		break;
	}
	return false;
}

// -------------------------------------------------------------------------- //

}
}
}