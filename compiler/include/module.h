#ifndef __MODULE__H__
#define __MODULE__H__

#include <string>
#include <unordered_map>
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

    const std::string &get_name() const { return name; }
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

    const std::string &get_name() const { return name; }

    const BasicBlock &get_entry_block() const { return entry; }

    const std::vector<BasicBlock> &get_basic_blocks() const { return blocks; }

    /**
     *  Inserts a new basic block into the current function.
     */
    void insert_basic_block(const std::string &blk_name) noexcept;

    /**
     * Removes a basic block from the current function. Returns false if the block is not found and
     * throws an exception if we tried to remove the entry basic block.
     */
    bool remove_basic_block(const std::string &blk_name);
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

    const std::string &get_name() const { return name; }

    const std::vector<Function> &get_functions() const { return functions; }

    /**
     * Inserts the given function into the current module.
     */
    void insert(const Function &fn) noexcept;
    /**
     * Removes the function with @c fn_name name from the current module and throws an exception if
     * the function does not exist.
     */
    bool remove(const std::string &fn_name);

    /**
     * Inserts a new basic block into the given function, creating a new one the latter does not
     * exist.
     */
    void insert_basic_block(const std::string &fn_name, const std::string &blk_name) noexcept;

    /**
     * Removes a basic block from the given function. Returns false if the block is not found and
     * throws an exception if we tried to remove a entry basic block or the function does not exist.
     */
    bool remove_basic_block(const std::string &fn_name, const std::string &blk_name);
};

/**
 * @brief Represents the link between an implied predecessor and the successor.
 *
 */
struct Link {
    std::string succ;
    std::string tag;
};

/**
 * @brief Adjacency list representing predecessor and successor relationship as a graph from the
 * predecessor to its links.
 *
 */
class Graph {
    std::unordered_map<std::string, std::vector<Link>> graph;

  public:
    const std::string &get_tag(const std::string &pred, const std::string &succ) const;

    void insert_tag(const std::string &pred, const std::string &succ, const std::string &tag);

    bool is_successor(const std::string &pred, const std::string &succ);
};

#endif //!__MODULE__H__