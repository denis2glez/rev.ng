#include "module.h"
#include <algorithm>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_set>

void SuccGraph::add_successor(const std::string &pred, const std::string &succ,
                              const std::string &tag) {
    auto link = Link{tag, succ};
    auto &succ_links = graph[pred];

    // C++20 ranges are also a welcoming upgrade.
    auto it = std::lower_bound(succ_links.begin(), succ_links.end(), link);
    if (it == succ_links.end() || it->tag != tag) {
        succ_links.insert(it, link);
        pred_counter[succ]++;
    } else if (it->tag == tag) {
        // NOTE: We're assuming this should never happen.
        throw std::runtime_error(
            "A basic block cannot have more than one successor with the same tag!");
    }
}

bool SuccGraph::remove_successor(const std::string &pred, const std::string &succ) {
    auto &succ_links = graph[pred];

    auto lambda = [&](const Link &l) { return succ == l.succ; };
    auto it = std::ranges::find_if(succ_links, lambda);

    if (it == succ_links.end()) {
        return false;
    }

    if (--pred_counter[succ] == 0) {
        // When deleting a block that is not a leaf we have to relink the predecessor to the new
        // successors.
        for (auto &[tag, succ_succ] : graph[succ]) {
            add_successor(pred, succ_succ, tag);
        }

        // At this point we are sure it is contained in succ_links but the iterators were probably
        // invalidated.
        succ_links = graph[pred];
        it = std::ranges::find_if(succ_links, lambda);

        graph.erase(succ);
        pred_counter.erase(succ);
    }

    succ_links.erase(it);
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

void Function::generate_dot(std::stringstream &buffer, const std::string &entry_name) {
    std::queue<std::string> queue;
    queue.push(entry_name);
    std::unordered_set<std::string> visited;

    while (!queue.empty()) {
        auto &blk_name = queue.front();
        queue.pop();

        if (visited.contains(blk_name))
            continue;

        auto &successors = succ_graph.get_succ_graph()[blk_name];
        for (const auto &[succ_tag, succ] : successors) {
            buffer << '\t' << blk_name << " -> " << succ << "[label=\"" << succ_tag << "\"];\n";
            queue.push(succ);
        }

        visited.insert(blk_name);
    }
}

std::string Function::generate_dot() {
    std::stringstream buffer;
    buffer << "digraph " << name << " {\n";
    buffer << "\tnode[shape=oval];\n";
    buffer << "\tedge[color=\"red\"];\n";
    generate_dot(buffer, entry.get_name());
    buffer << "}\n";

    return buffer.str();
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