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
 * @brief Represents the link between an implied predecessor and the successor. Since the tag is
 * unique, it is used as the primary field. Thus each basic block has, at most, one successor per
 * tag.
 */
struct Link {
    std::string tag;
    std::string succ;

    // C++20 operator<=> is definitely a step forward.
    auto operator<=>(const Link &r) const { return tag <=> r.tag; }
};

/**
 * @brief Successor graph represented as an adjacency list from the predecessor to a list of its
 * successors.
 */
class SuccGraph {
    using SuccGraphType = std::unordered_map<std::string, std::vector<Link>>;

    // The successors list is represented as an ordered vector assuming that, in general, its cache
    // locality will have a major impact on performance. Otherwise we could use a set.
    SuccGraphType graph;

  public:
    const SuccGraphType &get_succ_graph() const { return graph; }

    const std::string &get_tag(const std::string &pred, const std::string &succ) const;

    /**
     * @brief Adds a new successor to the basic block @c pred.
     *
     * @param pred
     * @param succ
     * @param tag
     */
    void add_successor(const std::string &pred, const std::string &succ, const std::string &tag);

    /**
     * @brief Removes a successor of the basic block @c pred returns true if it exists and false
     * otherwise.
     *
     * @param pred
     * @param succ
     */
    bool remove_successor(const std::string &pred, const std::string &succ);
};

/**
 * @brief A function contains one or more basic blocks. One of them is the entry basic block and by
 * design it is unique.
 */
class Function {
    std::string name;
    BasicBlock entry;
    std::vector<BasicBlock> blocks;
    SuccGraph succ_graph;

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

    /**
     * @brief Adds a new successor to the basic block @c pred.
     *
     * @param pred
     * @param succ
     * @param tag
     */
    void add_successor(const std::string &pred, const std::string &succ, const std::string &tag);

    /**
     * @brief Removes a successor of the basic block @c pred returns true if it exists and false
     * otherwise.
     *
     * @param pred
     * @param succ
     */
    bool remove_successor(const std::string &pred, const std::string &succ);
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

#endif //!__MODULE__H__