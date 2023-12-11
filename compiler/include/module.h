#ifndef __MODULE__H__
#define __MODULE__H__

#include <string>
#include <vector>

/**
 * @brief A common basic block of code.
 *
 */
class BasicBlock {
    std::string name;

  public:
    // Rule of zero
    /**
     * @brief Construct a new @c BasicBlock object given a name.
     *
     * @param name basic block name.
     */
    explicit BasicBlock(const std::string &name) : name(name) {}
};

/**
 * @brief A function contains one or more basic blocks. One of them is the entry basic block.
 *
 */
class Function {
    std::string name;
    BasicBlock entry;
    std::vector<BasicBlock> blocks;

  public:
    // Rule of zero
    /**
     * @brief Construct a new @c Function object given a name and the entry basic block.
     *
     * @param name module name.
     * @param entry entry basic block.
     */
    explicit Function(const std::string &name, const BasicBlock &entry)
        : name(name), entry(entry) {}

    std::string &get_name();
};

/**
 * @brief A module represents a single input file that will eventually become an object file.
 *
 */
class Module {
    std::string name;
    std::vector<Function> functions;

  public:
    // Rule of zero
    /**
     * @brief Construct a new @c Module object given a name.
     *
     * @param name module name.
     */
    explicit Module(const std::string &name) : name(name) {}

    /**
     *  Inserts the given function into the current module.
     */
    void insert(const Function &fn);
    /**
     * Removes the function with @c fn_name name from the current module.
     * Returns false if the function is not found.
     */
    bool remove(const std::string &fn_name);
};

#endif //!__MODULE__H__