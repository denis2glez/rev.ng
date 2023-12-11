#include "module.h"
#include <stdexcept>

void Module::insert(const Function &fn) noexcept { functions.push_back(fn); }

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

void Function::insert_basic_block(const std::string &blk_name) noexcept {
    // TODO: Maybe we should check if already exists.
    blocks.push_back(BasicBlock(blk_name));
}

void Module::insert_basic_block(const std::string &fn_name, const std::string &blk_name) noexcept {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            it->insert_basic_block(blk_name);
            return;
        }
    }

    functions.push_back(Function(fn_name, BasicBlock(blk_name)));
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

bool Module::remove_basic_block(const std::string &fn_name, const std::string &blk_name) {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            return it->remove_basic_block(blk_name);
        }
    }

    // NOTE: We're assuming this should never happen. Otherwise we could return an optional value.
    throw std::runtime_error("There is no Function with this name!");
}