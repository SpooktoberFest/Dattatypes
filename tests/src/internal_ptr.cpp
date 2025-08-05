
#include <iostream>
#include <stdexcept>
#include <vector>


#include "debug.hpp"
#include "internal_ptr.hpp"


static constexpr auto src = "internal_ptr:TEST";
using namespace std;
using namespace Dattatypes;

struct Entity;
typedef internal_ptr<Entity> ie_ptr;
typedef internal_ref<Entity> ie_ref;

struct Entity {
    Entity() = default;
    Entity(int value) : _value(value) {};

    ie_ref _entity_ref;
    ie_ptr _entity_ptr;
    int _value;
};



int main() {
    LOG_INFO("=== Beginning Tests for internal_ptr ===");

    int num=0;
    std::vector<Entity> list = {0, 1, 2 , 3};
    ie_ptr outsider;


    LOG_WARN("Test {} - has_valid_target correctness checks", ++num);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), false, "l0.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "l1.has_valid_target");
    runtime_assert(outsider.has_valid_target(), false, "outsider.has_valid_target");

    LOG_WARN("Test {} - Set targets (l0->l2 & outsider->l3)", ++num);
    list[0]._entity_ptr.set_target(&list[2]._entity_ref);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), true, "l0.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "l1.has_valid_target");
    runtime_assert(list[0]._entity_ptr->_value, 2, "l0->l2");
    outsider.set_target(&list[3]._entity_ref);
    runtime_assert(outsider.has_valid_target(), true, "outsider.has_valid_target");
    runtime_assert(outsider->_value, 3, "outsider->l3");

    LOG_WARN("Test {} - Swap target entities l2 & l3 without effecting pointers", ++num);
    std::swap(list[2], list[3]);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), true, "l0.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), false, "l1.has_valid_target");
    runtime_assert(outsider.has_valid_target(), true, "outsider.has_valid_target");
    runtime_assert(list[0]._entity_ptr->_value, 2, "l0->l3");
    runtime_assert(outsider->_value, 3, "outsider->l2");

    LOG_WARN("Test {} - Swap targeter entities l0 & l1 without effecting pointers", ++num);
    std::swap(list[0], list[1]);
    runtime_assert(list[0]._entity_ptr.has_valid_target(), false, "l0.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), true, "l1.has_valid_target");
    runtime_assert(list[1]._entity_ptr->_value, 2, "l1->l3");
    runtime_assert(outsider->_value, 3, "outsider->l2");

    LOG_WARN("Test {} - Erase target l2", ++num);
    list.erase(list.begin() + 2);
    runtime_assert(outsider.has_valid_target(), false, "outsider.has_valid_target");
    runtime_assert(list[1]._entity_ptr.has_valid_target(), true, "l1.has_valid_target");

    LOG_INFO("=== All tests for internal_ptr passed! ===\n\n");
    return 0;
}
