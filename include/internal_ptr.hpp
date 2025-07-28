#pragma once
// === HEADER ONLY ===

#include <vector>
#include <algorithm>


namespace Dattatypes {

    /**
     * Internal Smart Pointer (Template Class)
     * Designed to allow objects to reference eachother without wrapping them.
     * Can live inside the referenced object.
     * Holds a pointer to its parent and its target.
     *
     * EXAMPLE:
     * struct Entity {
     *   void attack();
     *   internal_ptr<Entity> _entity_ptr = internal_ptr<Entity>(this);
     * }
     * std::vector<Entity> entity_list(2);
     * entity_list[0]._entity_ptr.set_target(&entity_list[1]._entity_ptr);
     * entity_list[0]._entity_ptr->attack(); // Entity at index 1 attacks.
     */
    template <typename T>
    class internal_ptr {
        using ThisType = internal_ptr<T>;

    public:
        // Default Constructor
        internal_ptr() :
            _target_ptr(nullptr), _parent_ptr(nullptr) {
            _internal_ptrs.emplace_back(this);
        }
        // Parameterized Constructor
        internal_ptr(T *parent, ThisType *target = nullptr) :
            _target_ptr(target), _parent_ptr(parent) {
            _internal_ptrs.emplace_back(this);
        }
        // Copy Constructor (Non-functional)
        internal_ptr(const ThisType &copy_from) :
            _target_ptr(copy_from.get_target()), _parent_ptr(nullptr) {
            _internal_ptrs.emplace_back(this);
        }
        // Move Constructor
        internal_ptr(ThisType &&move_from) :
            _target_ptr(move_from.get_target()), _parent_ptr(move_from.get_parent()) {
            // Redirect all references to move_from to this.
            if (is_valid_target())
                for (ThisType *fellow : _internal_ptrs)
                    if (fellow->has_valid_target() && fellow->get_target() == &move_from)
                        fellow->set_target(this);

            // Clear out move_from.
            move_from.clear_parent();
            move_from.clear_target();

            _internal_ptrs.emplace_back(this);
        }
        // Destructor
        ~internal_ptr() {
            // Clear all references to this.
            if (is_valid_target())
                for (ThisType *fellow : _internal_ptrs)
                    if (fellow->has_valid_target() && fellow->get_target() == this)
                        fellow->clear_target();

            // Remove this from _internal_ptrs.
            auto it = std::find(_internal_ptrs.begin(), _internal_ptrs.end(), this);
            if (it != _internal_ptrs.end())
            {
                std::iter_swap(it, _internal_ptrs.end() - 1);
                _internal_ptrs.pop_back();
            }
        }

        // Setters
        constexpr ThisType &set_target(ThisType *target) { _target_ptr = target; return *this; }
        constexpr ThisType &set_parent(T *parent) { _parent_ptr = parent; return *this; }
        constexpr ThisType &clear_target() { _target_ptr = nullptr; return *this; }
        constexpr ThisType &clear_parent() { _parent_ptr = nullptr; return *this; }

        // Getters
        constexpr ThisType *get_target() const { return _target_ptr; }
        constexpr T *get_parent() const { return _parent_ptr; }

        // Operators
        ThisType &operator=(ThisType &&) = default;
        ThisType &operator=(const ThisType &copy_from) { set_target(copy_from.get_target()); return *this; }
        T &operator*() const { return *_target_ptr->get_parent(); }
        T *operator->() const { return _target_ptr->get_parent(); }

        // Safety Checks
        constexpr bool is_valid_target() const { return _parent_ptr != nullptr; }
        constexpr bool has_valid_target() const {
            return (_target_ptr != nullptr) && (_target_ptr->get_parent() != nullptr);
        }

    private:
        ThisType *_target_ptr = nullptr;
        T *_parent_ptr = nullptr;
        static std::vector<ThisType *> _internal_ptrs;
    };

    template <typename T>
    std::vector<internal_ptr<T> *> internal_ptr<T>::_internal_ptrs;


}; // namespace Dattatypes
