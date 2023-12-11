#include "module.h"
#include <algorithm>
#include <stdexcept>

void SuccGraph::add_successor(const std::string &pred, const std::string &succ,
                              const std::string &tag) {
    auto link = Link{tag, succ};
    auto &pred_links = graph[pred];

    // C++20 ranges are also a welcoming upgrade.
    auto it = std::lower_bound(pred_links.begin(), pred_links.end(), link);
    if (it == pred_links.end() || it->tag != tag) {
        pred_links.insert(it, link);
    } else if (it->tag == tag) {
        // NOTE: We're assuming this should never happen.
        throw std::runtime_error(
            "A basic block cannot have more than one successor with the same tag!");
    }
}

bool SuccGraph::remove_successor(const std::string &pred, const std::string &succ) {
    auto &pred_links = graph[pred];

    const auto it = std::ranges::find_if(pred_links, [&](const Link &l) { return succ == l.succ; });

    if (it == pred_links.end()) {
        return false;
    }

    pred_links.erase(it);
    return true;
}

void Function::insert_basic_block(const std::string &blk_name) noexcept {
    // TODO: Maybe we should check if it already exists.
    blocks.push_back(BasicBlock(blk_name));
}

bool Function::remove_basic_block(const std::string &blk_name) {
    if (entry.get_name() == blk_name) {
        // NOTE: We're assuming this should never happen. Otherwise we could return an optional
        // value.
        throw std::runtime_error("We can never remove the entry BasicBlock!");
    }

    for (auto it = blocks.begin(); it != blocks.end(); it++) {
        if (it->get_name() == blk_name) {
            blocks.erase(it);
            return true;
        }
    }

    return false;
}

void Function::add_successor(const std::string &pred, const std::string &succ,
                             const std::string &tag) {
    succ_graph.add_successor(pred, succ, tag);
}

bool Function::remove_successor(const std::string &pred, const std::string &succ) {
    return succ_graph.remove_successor(pred, succ);
}

void Module::insert(const Function &fn) noexcept { functions.push_back(fn); }

void Module::insert_basic_block(const std::string &fn_name, const std::string &blk_name) noexcept {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            it->insert_basic_block(blk_name);
            return;
        }
    }

    functions.push_back(Function(fn_name, BasicBlock(blk_name)));
}

bool Module::remove(const std::string &fn_name) {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            functions.erase(it);
            return true;
        }
    }

    // NOTE: We're assuming this should never happen. Otherwise we could return an optional value.
    throw std::runtime_error("There is no Function with this name!");
}

bool Module::remove_basic_block(const std::string &fn_name, const std::string &blk_name) {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            return it->remove_basic_block(blk_name);
        }
    }

    // NOTE: We're assuming this should never happen. Otherwise we could return an optional value.
    throw std::runtime_error("There is no Function with this name!");
}