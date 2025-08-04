
#include <iostream>
#include <stdexcept>
#include <vector>


#include "debug.hpp"
#include "internal_ptr.hpp"


static constexpr auto src = "Prec:TEST";
using namespace std;
using namespace Dattatypes;


struct Entity {
    Entity(int value) : _value(value) {}
    int _value;
    internal_ptr<Entity> _entity_ptr = internal_ptr<Entity>(this);
};


// Testing
int main() {
    LOG_INFO("=== Beginning Tests for Internal Pointer ===");

    int num=0;
    std::vector<Entity> list = {0, 1, 2, 3};
    internal_ptr<Entity> outsider;

    LOG_WARN("Test {} - has_valid_target correctness checks", ++num);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), false, "list[0].has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "list[1].has_valid_target");
    runtime_assert(outsider.has_valid_target(), false, "outsider.has_valid_target");

    LOG_WARN("Test {} - Set targets (list[0]->list[2] and outsider->list[3])", ++num);
    list[0]._entity_ptr.set_target(&list[2]._entity_ptr);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), true, "list[0].has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "list[1].has_valid_target");
    runtime_assert(list[0]._entity_ptr->_value, 2, "list[0]->list[2]");
    outsider.set_target(&list[3]._entity_ptr);
    runtime_assert(outsider.has_valid_target(), true, "outsider.has_valid_target");
    runtime_assert(outsider->_value, 3, "outsider->list[3]");

    LOG_WARN("Test {} - Swap target containers without effecting pointers (swap(list[2], list[3]))", ++num);
    std::swap(list[2], list[3]);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), true, "list[0].has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "list[1].has_valid_target");
    runtime_assert(outsider.has_valid_target(), true, "outsider.has_valid_target");
    runtime_assert(list[0]._entity_ptr->_value, 2, "list[0]->list[3]");
    runtime_assert(outsider->_value, 3, "outsider->list[2]");

    LOG_WARN("Test {} - Swap targeter containers without effecting pointers (swap(list[0], list[1]))", ++num);
    std::swap(list[0], list[1]);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), false, "list[0].has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), true, "list[1].has_valid_target");
    runtime_assert(list[1]._entity_ptr->_value, 2, "list[1]->list[3]");
    runtime_assert(outsider->_value, 3, "outsider->list[2]");

    LOG_WARN("Test {} - Erase target (list[2])", ++num);
    list.erase(list.begin() + 2);
    runtime_assert(outsider.has_valid_target(), false, "outsider.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), true, "list[1].has_valid_target");

    LOG_INFO("=== All tests for Internal Pointer passed! ===\n\n");
    return 0;
}
