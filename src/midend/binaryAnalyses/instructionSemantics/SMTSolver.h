#ifndef Rose_SMTSolver_H
#define Rose_SMTSolver_H

#include "InsnSemanticsExpr.h"

/** Interface to Satisfiability Modulo Theory (SMT) solvers.
 *
 *  The purpose of an SMT solver is to determine if an expression is satisfiable. Although the SMTSolver class was originally
 *  designed to be used by SymbolicExpressionSemantics policy (see SymbolicExpressionSemantics::Policy::set_solver()), but it
 *  can also be used independently. */
class SMTSolver {
public:
    struct Exception {
        Exception(const std::string &mesg): mesg(mesg) {}
        friend std::ostream& operator<<(std::ostream&, const SMTSolver::Exception&);
        std::string mesg;
    };

    /** Satisfiability constants. */
    enum Satisfiable { SAT_NO=0,                /**< Provably unsatisfiable. */
                       SAT_YES,                 /**< Satisfiable and evidence of satisfiability may be available. */
                       SAT_UNKNOWN              /**< Could not be proved satisfiable or unsatisfiable. */
    };
    
    typedef std::set<uint64_t> Definitions;     /**< Free variables that have been defined. */

    SMTSolver(): debug(NULL) {}

    virtual ~SMTSolver() {}

    /** Determines if the specified expression is satisfiable, unsatisfiable, or unknown. */
    virtual Satisfiable satisfiable(const InsnSemanticsExpr::TreeNodePtr &expr);

    /** Determines if the specified collection of expressions is satisfiable. */
    virtual Satisfiable satisfiable(const std::vector<InsnSemanticsExpr::TreeNodePtr> &exprs);

    /** Evidence of satisfiability for a bitvector variable.  If an expression is satisfiable, this function will return
     *  a value for the specified bitvector variable that satisfies the expression in conjunction with the other evidence. Not
     *  all SMT solvers can return this information.  Returns the null pointer if no evidence is available for the variable.
     * @{ */
    virtual InsnSemanticsExpr::TreeNodePtr evidence_for_variable(uint64_t varno) {
        char buf[64];
        snprintf(buf, sizeof buf, "v%"PRIu64, varno);
        return evidence_for_name(buf);
    }
    virtual InsnSemanticsExpr::TreeNodePtr evidence_for_variable(const InsnSemanticsExpr::TreeNodePtr &var) {
        InsnSemanticsExpr::LeafNodePtr ln = var->isLeafNode();
        assert(ln && !ln->is_known());
        return evidence_for_variable(ln->get_name());
    }
    /** @} */

    /** Evidence of satisfiability for a memory address.  If an expression is satisfiable, this function will return
     *  a value for the specified memory address that satisfies the expression in conjunction with the other evidence. Not
     *  all SMT solvers can return this information. Returns the null pointer if no evidence is available for the memory
     *  address. */
    virtual InsnSemanticsExpr::TreeNodePtr evidence_for_address(uint64_t addr) {
        return evidence_for_name(StringUtility::addrToString(addr));
    }

    /** Evidence of satisfiability for a variable or memory address.  If the string starts with the letter 'v' then variable
     *  evidence is returned, otherwise the string must be an address.  The strings are those values returned by the
     *  evidence_names() method.  Not all SMT solvers can return this information.  Returns the null pointer if no evidence is
     *  available for the named item. */
    virtual InsnSemanticsExpr::TreeNodePtr evidence_for_name(const std::string&) {
        return InsnSemanticsExpr::TreeNodePtr();
    }

    /** Names of items for which satisfiability evidence exists.  Returns a vector of strings (variable names or memory
     * addresses) that can be passed to evidence_for_name().  Not all SMT solvers can return this information. */
    virtual std::vector<std::string> evidence_names() {
        return std::vector<std::string>();
    }

    /** Clears evidence information. */
    virtual void clear_evidence() {}

    /** Turns debugging on or off. */
    void set_debug(FILE *f) { debug = f; }

    /** Obtain current debugging setting. */
    FILE *get_debug() const { return debug; }

    /** Returns the number of times satisfiable() was called.  This is a class method that returns the total number of SMT
     * solver calls across all SMT solvers. */
    static size_t get_ncalls() { return total_calls; }

protected:
    /** Generates an input file for for the solver. Usually the input file will be SMT-LIB format, but subclasses might
     *  override this to generate some other kind of input. Throws Excecption if the solver does not support an operation that
     *  is necessary to determine the satisfiability. */
    virtual void generate_file(std::ostream&, const std::vector<InsnSemanticsExpr::TreeNodePtr> &exprs,
                               Definitions*) = 0;

    /** Given the name of a configuration file, return the command that is needed to run the solver. The first line
     *  of stdout emitted by the solver should be the word "sat" or "unsat". */
    virtual std::string get_command(const std::string &config_name) = 0;

    /** Parses evidence of satisfiability.  Some solvers can emit information about what variable bindings satisfy the
     *  expression.  This information is parsed by this function and added to a mapping of variable to value. */
    virtual void parse_evidence() {};

    /** Additional output obtained by satisfiable(). */
    std::string output_text;
    
    /** Tracks the number of times an SMT solver was called. Actually, the number of calls to satisfiable() */
    static size_t total_calls;

private:
    FILE *debug;
};

#endif
