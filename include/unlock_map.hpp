#pragma once
// === HEADER ONLY ===

#include <vector>
#include <type_traits>
#include <algorithm>

#include "debug.hpp"


namespace Dattatypes {

	template<typename T>
	class unlock_map {
		static_assert(std::is_enum_v<T>, "T must be an enum type");

		// Underlying integer type
		using U = std::underlying_type_t<T>;
	public:
		unlock_map() = default;
		unlock_map(std::vector<U> vec) : _data(std::move(vec)) {};
		~unlock_map() = default;

		std::vector<U> _data; // pairs of [start, end)

		// NOP
		// TODO: Insert a single value into the interval set
		void insert(const T item) {}

		// Insert a range [item_begin, item_end] of values into the interval set
		void insert(const T item_begin, const T item_end) {
			if (item_begin > item_end) return;

			U lower_value = U(item_begin);
			U upper_value = U(item_end)+1;

			if (_data.empty() || _data.back() < lower_value) {
				_data.insert(_data.end(), {lower_value, upper_value});
				return;
			}
			if (_data.front() > lower_value) {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}

			// First element not greater than lower_value
			auto lower_it = std::upper_bound(_data.begin(), _data.end(), lower_value) -1;

			// First element not greater than upper_value
			auto upper_it = std::upper_bound(_data.begin(), _data.end(), lower_value) -1;

			// Check if the iterators are at starts or ends of intervals based on index evenness
			bool lower_is_start = ((lower_it - _data.begin()) & 1);
			bool upper_is_end = !((lower_it - _data.begin()) & 1);

			// Case of lower touching other interval
			if (!lower_is_start && (lower_value == *lower_it)) {
				--lower_it; lower_is_start=true;
			}

			// Case of upper touching other interval
			if (!upper_is_end && upper_it != _data.end() && (upper_value == *upper_it)) {
				++upper_it; upper_is_end=true;
			}
			if (upper_it == _data.end()) {
				std::cout <<"upper_it is at end!!" << std::endl;
			}

			// Case of whole interval being if empty region
			if ((upper_it - upper_it == 1) && !lower_is_start) {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}

			// Force alignment
			lower_it += !lower_is_start;
			upper_it -= !upper_is_end;

			// Modify values
			*lower_it = std::min(*lower_it, lower_value);
			*upper_it = std::max(*upper_it, upper_value);

			// Erase any gaps in the interval
			if (upper_it - upper_it == 1) _data.erase(lower_it+1, upper_it-1);
		}

		// NOP
		// TODO: Erase a single value from the interval set
		void erase(const T item) {}

		// NOP
		// TODO: Erase a range [item_begin, item_end] of values from the interval set
		void erase(const T item_begin, const T item_end) {}

		// Check whether the item exists in any stored interval
		bool check(const T item) const {
			if (_data.empty() || _data[0] > U(item)) return false;

			// Last element not greater than item
			auto it = std::upper_bound(_data.begin(), _data.end(), U(item)) -1;

			int index = it - _data.begin();

			// const auto src = "TMP";
			// LOG_DEBUG("Got index: {} while looking for {}", index, item);

			return !(index & 1); // Item already exists if index is odd.
		}

		constexpr bool empty() const { return _data.empty(); }
		constexpr void clear() { _data.clear(); }
		constexpr void reserve(std::size_t __n) { _data.reserve(__n); }
		constexpr void shrink_to_fit() { _data.shrink_to_fit(); }

		// Calculates the number of values covered
		constexpr size_t size() const {
			size_t sum=0, i=0;
			for (; i<_data.size() ; i+=2)
				sum += size_t(_data[i+1]-_data[i]);
			return sum;
		}

	};


}; // namespace Dattatypes
