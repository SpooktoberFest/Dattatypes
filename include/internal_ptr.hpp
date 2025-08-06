#pragma once

#include <vector>
#include <cstdint>
#include <algorithm>


namespace dattatypes {

    template<typename T> class internal_ref;

    class internal_base {
    protected:
        static std::vector<internal_base*> _internal_ptrs;
    };


    template<typename T>
    class internal_ptr : public internal_base {
        using TargetType = internal_ref<T>;
    public:
        internal_ptr() { _internal_ptrs.emplace_back(reinterpret_cast<internal_base*>(this)); }
        internal_ptr(TargetType* target)
            : _target_ptr(target) { _internal_ptrs.emplace_back(reinterpret_cast<internal_base*>(this)); }
        internal_ptr(const internal_ptr &other)
            : _target_ptr(other.get_target()) { _internal_ptrs.emplace_back(reinterpret_cast<internal_base*>(this)); }
        internal_ptr(internal_ptr &&other)
            : _target_ptr(other.get_target()) {
            other.clear_target();
            _internal_ptrs.emplace_back(reinterpret_cast<internal_base*>(this));
        }
        ~internal_ptr() {
            // Remove this from _internal_ptrs.
            auto it = std::find(_internal_ptrs.begin(), _internal_ptrs.end(), reinterpret_cast<internal_base*>(this));
            if (it != _internal_ptrs.end()) {
                std::iter_swap(it, _internal_ptrs.end() - 1);
                _internal_ptrs.pop_back();
            }
        }

        // Getters & Setters
        internal_ptr& set_target(TargetType* target) { _target_ptr = target; return *this; }
        internal_ptr& clear_target() { _target_ptr = nullptr; return *this; }
        TargetType* get_target() const { return _target_ptr; }

        // Operators
        internal_ptr& operator=(const internal_ptr &other) {
            set_target(other.get_target()); return *this;
        }
        internal_ptr& operator=(internal_ptr && other) {
            if (this == &other) return *this;
            _target_ptr = other.get_target();
            other.clear_target();
            return *this;
        }
        T& operator*() const { return *get_target()->get_parent(); }
        T* operator->() const { return get_target()->get_parent(); }

        // Safety Checks
        bool has_valid_target() const { return _target_ptr != nullptr; }

    private:
        TargetType* _target_ptr = nullptr;
    };


    template<typename T>
    class internal_ref : public internal_base {
        using PointerType = internal_ptr<T>;
    public:
        internal_ref() {}
        internal_ref(const internal_ref &) {}
        internal_ref(internal_ref &&other) {
            // Replace references to `other` with `this`.
            for (internal_base* base_ptr : _internal_ptrs) {
                PointerType* t_ptr = reinterpret_cast<PointerType*>(base_ptr);
                if (t_ptr->get_target() == &other)
                t_ptr->set_target(this);
            }
        }
        ~internal_ref() {
            // Clear all references to `this`.
            for (internal_base* base_ptr : _internal_ptrs) {
                PointerType* t_ptr = reinterpret_cast<PointerType*>(base_ptr);
                if (t_ptr->get_target() == this)
                    t_ptr->clear_target();
            }
        }

        // Getter
        T* get_parent() { return reinterpret_cast<T*>(this); }

        // Operators
        internal_ref& operator=(const internal_ref &) { return *this; }
        internal_ref& operator=(internal_ref && other) {
            if (this == &other) return *this;
            // Clear all references to `this` and replace references to `other` with `this`.
            for (internal_base* base_ptr : _internal_ptrs) {
                PointerType* t_ptr = reinterpret_cast<PointerType*>(base_ptr);
                const auto target = t_ptr->get_target();
                if (target == this) t_ptr->clear_target();
                else if (target == &other) t_ptr->set_target(this);
            }
            return *this;
        }
        T& operator*() const { return *get_parent(); }
        T* operator->() const { return get_parent(); }


    };







}; // namespace dattatypes
