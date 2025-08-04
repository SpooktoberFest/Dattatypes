#pragma once
// === HEADER ONLY ===

#include <vector>
#include <cstdint>
#include <stdexcept>

#include "debug.hpp"


namespace Dattatypes {

    /**
     * Internal Smart Pointer (Template Class)
     * Designed to allow moving objects to reference eachother without wrapping or owning them.
     * Should live inside the referenced object, and should be initialized with a pointer to the parent.
     * Holds a pointer to its target.
     */
    template <typename T>
    class internal_ptr {
        using ThisType = internal_ptr<T>;

    static constexpr const auto src = "internal_ptr";

    public:
        // Default Constructor
        internal_ptr() : _target_ptr(nullptr) { _internal_ptrs.emplace_back(this); }
        // Parameterized Constructor
        internal_ptr(T *parent, ThisType *target = nullptr) :
            _target_ptr(target) {
            _dist = intptr_t(this) - intptr_t(parent);
            if (_dist < 0) throw std::runtime_error("A non-parent object must never be declared as parent.");
            _internal_ptrs.emplace_back(this);
        }
        // Copy Constructor (Non-functional)
        internal_ptr(const ThisType &other) :
            _target_ptr(other.get_target()) {
            _internal_ptrs.emplace_back(this);
        }
        // Move Constructor
        internal_ptr(ThisType &&other) :
            _target_ptr(other.get_target()) {
            // Replace references to `other` with `this`.
            for (ThisType* fellow : _internal_ptrs) {
                if (fellow->has_valid_target() && fellow->get_target() == &other)
                fellow->set_target(this);
            }
            // Clear out other.
            other.clear();
            _internal_ptrs.emplace_back(this);
        }
        // Destructor
        ~internal_ptr() {
            // Clear all references to `this`.
            for (ThisType* fellow : _internal_ptrs) {
                if (fellow->has_valid_target() && fellow->get_target() == this)
                    fellow->clear();
            }
            // Remove this from _internal_ptrs.
            auto it = std::find(_internal_ptrs.begin(), _internal_ptrs.end(), this);
            if (it != _internal_ptrs.end()) {
                std::iter_swap(it, _internal_ptrs.end() - 1);
                _internal_ptrs.pop_back();
            }
        }

        // Setters
        ThisType &set_target(ThisType *target) { _target_ptr = target; return *this; }
        ThisType &clear() { _target_ptr = nullptr; return *this; }

        // Getters
        ThisType *get_target() const { return _target_ptr; }
        T *get_parent() const { return reinterpret_cast<T*>(reinterpret_cast<intptr_t>(this) - _dist); }

        // Operators
        ThisType &operator=(ThisType && other) {
            if (this == &other) return *this;
            // Clear all references to `this` and replace references to `other` with `this`.
            for (ThisType* fellow : _internal_ptrs) {
                if (fellow->has_valid_target()) {
                    const auto target = fellow->get_target();
                    if (target == this) fellow->clear();
                    else if (target == &other) fellow->set_target(this);
                }
            }
            _target_ptr = other.get_target();
            other.clear();
            return *this;
        }
        ThisType &operator=(const ThisType &other) { set_target(other.get_target()); return *this; }
        T &operator*() const { return *_target_ptr->get_parent(); }
        T *operator->() const { return _target_ptr->get_parent(); }

        // Safety Checks
        bool has_valid_target() const { return (_target_ptr); }
        bool is_valid_target() const { return _dist >= 0; }

    private:

        ThisType *_target_ptr = nullptr;
        static intptr_t _dist;
        static std::vector<ThisType *> _internal_ptrs;
    };

    template <typename T>
    std::vector<internal_ptr<T> *> internal_ptr<T>::_internal_ptrs;
    template <typename T>
    intptr_t internal_ptr<T>::_dist = -1; // Initialized with invalid value;


}; // namespace Dattatypes
