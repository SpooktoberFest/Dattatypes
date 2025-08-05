#pragma once
// === HEADER ONLY ===

#include <vector>
#include <cstdint>
#include <utility>
#include <algorithm>
#include <stdexcept>

#include "debug.hpp"


namespace Dattatypes {

    /**
     * Smart Internal Pointer
     * Designed to allow moving objects to reference eachother without wrapping or owning them.
     * Should live inside the referenced object, and should be initialized with a pointer to the parent.
     * Holds a pointer to its target.
     */
    class internal_ptr {
        static constexpr const auto src = "TMP V";
    public:
        internal_ptr() { _internal_ptrs.emplace_back(this); }
        internal_ptr(void* parent, internal_ptr* target = nullptr)
            : _parent(parent), _target_ptr(target) { _internal_ptrs.emplace_back(this); }
        internal_ptr(const internal_ptr &other)
            : _target_ptr(other.get_target()) { _internal_ptrs.emplace_back(this); }
        internal_ptr(internal_ptr &&other)
            : _target_ptr(other.get_target()) {
            // Replace references to `other` with `this`.
            for (internal_ptr* fellow : _internal_ptrs) {
                if (fellow->get_target() == &other)
                fellow->set_target(this);
            }
            other.clear_target();
            _internal_ptrs.emplace_back(this);
        }
        ~internal_ptr() {
            // Clear all references to `this`.
            for (internal_ptr* fellow : _internal_ptrs) {
                if (fellow->get_target() == this)
                    fellow->clear_target();
            }
            // Remove this from _internal_ptrs.
            auto it = std::find(_internal_ptrs.begin(), _internal_ptrs.end(), this);
            if (it != _internal_ptrs.end()) {
                std::iter_swap(it, _internal_ptrs.end() - 1);
                _internal_ptrs.pop_back();
            }
        }

        // Setters
        internal_ptr& set_target(internal_ptr* target) { _target_ptr = target; return *this; }
        internal_ptr& clear_target() { _target_ptr = nullptr; return *this; }

        // Getters
        internal_ptr* get_target() const { return _target_ptr; }
        void* get_parent() const { return static_cast<void*>(_parent); }

        // Operators
        internal_ptr& operator=(internal_ptr && other) {
            if (this == &other) return *this;
            // Clear all references to `this` and replace references to `other` with `this`.
            for (internal_ptr* fellow : _internal_ptrs) {
                const auto target = fellow->get_target();
                if (target == this) fellow->clear_target();
                else if (target == &other) fellow->set_target(this);
            }
            _target_ptr = other.get_target();
            other.clear_target();
            return *this;
        }
        internal_ptr& operator=(const internal_ptr &other) { set_target(other.get_target()); return *this; }
        void* operator->() const { return get_target()->get_parent(); }

        // Safety Checks
        bool has_valid_target() const { return (_target_ptr); }

    private:
        void* _parent;

    protected:
        internal_ptr* _target_ptr = nullptr;
        static std::vector<internal_ptr*> _internal_ptrs;
    };


    /**
     * Smart Internal Template Pointer - "Tinternal pointer"
     * Designed to allow moving objects to reference eachother without wrapping or owning them.
     * Should live inside the referenced object, and should be initialized with a pointer to the parent.
     * Holds a pointer to its target.
     */
    template <typename ThisParent, typename TargetParent>
    class t_internal_ptr : internal_ptr {
        using ThisType = t_internal_ptr<ThisParent, TargetParent>;
        using TargetType = t_internal_ptr<TargetParent, ThisParent>; // TODO

        // Deleted
        t_internal_ptr(void* parent, internal_ptr* target = nullptr) = delete;
        t_internal_ptr(const internal_ptr & other) = delete;
        t_internal_ptr(internal_ptr && other) = delete;
        internal_ptr& operator=(const internal_ptr & other) = delete;
        internal_ptr& operator=(internal_ptr && other) = delete;
        internal_ptr &set_target(internal_ptr* target) = delete;

    public:
        t_internal_ptr() = default;
        t_internal_ptr(ThisParent* parent, TargetType* target = nullptr) {
            set_target(target);
            _dist = intptr_t(this) - intptr_t(parent);
        }
        t_internal_ptr(const ThisType &other) : internal_ptr(other) {}
        t_internal_ptr(ThisType &&other) : internal_ptr(std::move(other)) {}
        ~t_internal_ptr() = default;

        // Setters
        ThisType &set_target(TargetType* target) { _target_ptr = reinterpret_cast<internal_ptr*>(target); return *this; }
        ThisType &clear_target() { _target_ptr = nullptr; return *this; }

        // Getters
        TargetType* get_target() const { return reinterpret_cast<TargetType*>(_target_ptr); }
        ThisParent* get_parent() const {
            return reinterpret_cast<ThisParent*>(reinterpret_cast<intptr_t>(this) - _dist);
        }

        // Operators
        ThisType& operator=(ThisType && other) {
            internal_ptr::operator=(std::move(other));
            return *this;
        }
        ThisType& operator=(const ThisType &other) { set_target(other.get_target()); return *this; }
        TargetParent& operator*() const { return *get_target()->get_parent(); }
        TargetParent* operator->() const { return get_target()->get_parent(); }

        // Safety Checks
        bool has_valid_target() const { return (_target_ptr); }

    private:
        static intptr_t _dist;
    };


    std::vector<internal_ptr*> internal_ptr::_internal_ptrs;
    template <typename ThisParent, typename TargetParent>
    intptr_t t_internal_ptr<ThisParent, TargetParent>::_dist = -1; // Initialized with invalid value;







}; // namespace Dattatypes
