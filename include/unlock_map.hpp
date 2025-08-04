#pragma once
// === HEADER ONLY ===

#include <vector>
#include <type_traits>
#include <algorithm>

#include "debug.hpp"

/**
 *
 * Efficiently maps enum->bool
 */
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

		// Insert a single value into the interval set
		void insert(const T item) {
			U lower_value = U(item);
			U upper_value = U(item)+1;

			if (_data.empty() || (_data.front() > upper_value)) [[unlikely]] {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}
			if (_data.back() < lower_value) {
				_data.insert(_data.end(), {lower_value, upper_value});
				return;
			}

			Range range = find_range(lower_value, upper_value);
			insertion_logic(lower_value, upper_value, range);
		}

		// Insert a range [item_begin, item_end] of values into the interval set
		void insert(const T item_begin, const T item_end) {
			if (item_begin > item_end) return;

			U lower_value = U(item_begin);
			U upper_value = U(item_end)+1;

			if (_data.empty() || (_data.back() < lower_value)) [[unlikely]] {
				_data.insert(_data.end(), {lower_value, upper_value});
				return;
			}
			if (_data.front() > upper_value) {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}

			Range range = find_range(lower_value, upper_value);
			insertion_logic(lower_value, upper_value, range);
		}

		// Erase a single value from the interval set
		void erase(const T item) {
			U lower_value = U(item);
			U upper_value = U(item)+1;

			if (_data.empty() || (_data.back() < lower_value)) [[unlikely]]
				return;
			if (_data.front() > upper_value)
				return;

			Range range = find_range(lower_value, upper_value);
			erasure_logic(lower_value, upper_value, range);
		}

		// Erase a range [item_begin, item_end] of values from the interval set
		void erase(const T item_begin, const T item_end) {
			if (item_begin > item_end) return;

			U lower_value = U(item_begin);
			U upper_value = U(item_end)+1;

			if (_data.empty() || (_data.back() < lower_value)) [[unlikely]]
				return;
			if (_data.front() > upper_value)
				return;

			Range range = find_range(lower_value, upper_value);
			erasure_logic(lower_value, upper_value, range);
		}

		// Check whether the item exists in any stored interval
		bool check(const T item) const {
			if (_data.empty() || _data[0] > U(item)) return false;
			// Last element not greater than item
			auto it = std::upper_bound(_data.begin(), _data.end(), U(item)) -1;
			int index = it - _data.begin();
			return !(index & 1); // Item already exists if index is odd.
		}

		constexpr bool empty() const { return _data.empty(); }
		constexpr void clear() { _data.clear(); }
		constexpr void reserve(std::size_t __n) { _data.reserve(__n); }
		constexpr void shrink_to_fit() { _data.shrink_to_fit(); }

		// Calculates the number of values covered
		constexpr size_t size() const {
			if (_data.empty()) return 0;
			size_t sum=0, i=0;
			for (; i+1<_data.size() ; i+=2)
				sum += size_t(_data[i+1]-_data[i]);
			return sum;
		}

	private:

		struct Range {
			std::vector<U>::iterator lower_it, upper_it;
			bool lower_is_start , upper_is_end;
		};

		// Unsafe: Get the range
		Range find_range(const U lower_value, const U upper_value) {

			// Last element not lesser than lower_value
			auto lower_it = std::upper_bound(_data.begin(), _data.end(), lower_value)-1;

			// First element not greater than upper_value
			auto upper_it = std::lower_bound(lower_it, _data.end(), upper_value);

			// Check if the iterators are at starts or ends of intervals based on index evenness
			bool lower_is_start = !((lower_it - _data.begin()) & 1);
			bool upper_is_end = ((upper_it - _data.begin()) & 1);

			std::cout <<"r1 Range from " << int(lower_it-_data.begin()) << " to " << int(upper_it-_data.begin()) << std::endl;
			std::cout << "where: " << int(lower_is_start) << int(upper_is_end) << std::endl;

			// Case of lower touching other interval
			if (!lower_is_start && (lower_value == *lower_it)) {
				--lower_it; lower_is_start=true;
			}

			std::cout <<"r2 Range from " << int(lower_it-_data.begin()) << " to " << int(upper_it-_data.begin()) << std::endl;

			return Range{lower_it, upper_it, lower_is_start, upper_is_end};
		}


		// Unsafe: Insertion logic
		void insertion_logic(const U lower_value, const U upper_value, Range range) {

			// Case of upper touching other interval
			if (!range.upper_is_end && range.upper_it != _data.end() && (upper_value == *range.upper_it)) {
				++range.upper_it; range.upper_is_end=true;
			}

			std::cout <<"i2 Range from " << int(range.lower_it-_data.begin()) << " to " << int(range.upper_it-_data.begin()) << std::endl;

			// Case of whole interval being inside empty region
			if ((range.upper_it - range.lower_it == 1) && !range.lower_is_start) {
				_data.insert(range.upper_it, {lower_value, upper_value});
				return;
			}

			// Force alignment
			range.lower_it += !range.lower_is_start;
			range.upper_it -= !range.upper_is_end;

			std::cout <<"i2 Range from " << int(range.lower_it-_data.begin()) << " to " << int(range.upper_it-_data.begin()) << std::endl;

			// Modify values
			*range.lower_it = std::min(*range.lower_it, lower_value);
			*range.upper_it = std::max(*range.upper_it, upper_value);

			// Erase any gaps in the interval
			if (range.upper_it - range.lower_it > 1) _data.erase(range.lower_it+1, range.upper_it);
		}


		// Unsafe: Erasure logic
		void erasure_logic(const U lower_value, const U upper_value, Range range) {

			// Case of upper touching other interval
			if (!range.upper_is_end && range.upper_it != _data.end() && (upper_value == *range.upper_it)) {
				++range.upper_it; range.upper_is_end=true;
			}

			std::cout <<"e2 Range from " << int(range.lower_it-_data.begin()) << " to " << int(range.upper_it-_data.begin()) << std::endl;


			// Case of whole interval being inside empty region
			if ((range.upper_it - range.lower_it == 1) && range.lower_is_start) {
				_data.insert(range.upper_it, {lower_value, upper_value});
				return;
			}

			// Force alignment
			range.lower_it += range.lower_is_start;
			range.upper_it -= range.upper_is_end;

			std::cout <<"e2 Range from " << int(range.lower_it-_data.begin()) << " to " << int(range.upper_it-_data.begin()) << std::endl;

			// Modify values
			*range.lower_it = std::min(*range.lower_it, lower_value);
			*range.upper_it = std::max(*range.upper_it, upper_value);

			// Erase any gaps in the interval
			if (range.upper_it - range.lower_it > 1) _data.erase(range.lower_it+1, range.upper_it);

		}


	};



}; // namespace Dattatypes
