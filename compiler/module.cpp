#include "module.h"

std::string &Function::get_name() { return name; }

void Module::insert(const Function &fn) { functions.push_back(fn); }

bool Module::remove(const std::string &fn_name) {
    for (auto it = functions.begin(); it != functions.end(); it++) {
        if (it->get_name() == fn_name) {
            functions.erase(it);
            return true;
        }
    }
    return false;
}