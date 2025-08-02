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

			Range r = find_range(lower_value, upper_value);
			insertion_logic(lower_value, upper_value, r);
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

			Range r = find_range(lower_value, upper_value);
			insertion_logic(lower_value, upper_value, r);
		}

		// Erase a single value from the interval set
		void erase(const T item) {
			U lower_value = U(item);
			U upper_value = U(item)+1;

			if (_data.empty() || (_data.back() < lower_value)) [[unlikely]]
				return;
			if (_data.front() > upper_value)
				return;

			Range r = find_range(lower_value, upper_value);

			// Case of whole interval being inside filled region
			if ((r.upper_it - r.lower_it == 1) && r.lower_is_start) {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}

			// Force alignment
			r.lower_it -= !r.lower_is_start;
			r.upper_it -= !r.upper_is_end;

			std::cout <<"m2 Range from " << int(r.lower_it-_data.begin()) << " to " << int(r.upper_it-_data.begin()) << std::endl;

			// Modify values
			*r.lower_it = std::max(*r.lower_it, lower_value);
			*r.upper_it = std::min(*r.upper_it, upper_value);

			// Erase any gaps in the interval
			if (r.upper_it - r.lower_it == 1) _data.erase(r.lower_it+1, r.upper_it-1);
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

			Range r = find_range(lower_value, upper_value);

			std::cout <<"m1 Range from " << int(r.lower_it-_data.begin()) << " to " << int(r.upper_it-_data.begin()) << std::endl;
			std::cout << "where: " << int(r.lower_is_start) << int(r.upper_is_end) << std::endl;

			// Case of whole interval being inside filled region
			if ((r.upper_it == r.upper_it == 1) && r.lower_is_start) {
				_data.insert(_data.begin(), {lower_value, upper_value});
				return;
			}

			// Force alignment
			r.lower_it += !r.lower_is_start;
			r.upper_it -= !r.upper_is_end;

			std::cout <<"m2 Range from " << int(r.lower_it-_data.begin()) << " to " << int(r.upper_it-_data.begin()) << std::endl;

			std::cout << "min(" << int(*r.lower_it) << ", " << int(lower_value) << ") => " << int(std::min(*r.lower_it, lower_value)) << std::endl;
			std::cout << "max(" << int(*r.upper_it) << ", " << int(upper_value) << ") => " << int(std::max(*r.upper_it, upper_value)) << std::endl;

			// Modify values
			*r.lower_it = std::max(*r.lower_it, lower_value);
			*r.upper_it = std::min(*r.upper_it, upper_value);

			// Erase any gaps in the interval
			if (r.upper_it - r.lower_it == 1) _data.erase(r.lower_it+1, r.upper_it-1);
		}

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
		void insertion_logic(const U lower_value, const U upper_value, Range r) {

			// Case of upper touching other interval
			if (!r.upper_is_end && r.upper_it != _data.end() && (upper_value == *r.upper_it)) {
				++r.upper_it; r.upper_is_end=true;
			}
			if (r.upper_it == _data.end()) {
				std::cout <<"upper_it is at end!!" << std::endl;
			}

			std::cout <<"i2 Range from " << int(r.lower_it-_data.begin()) << " to " << int(r.upper_it-_data.begin()) << std::endl;


			// Case of whole interval being inside empty region
			if ((r.upper_it - r.lower_it == 1) && !r.lower_is_start) {
				_data.insert(r.lower_it, {lower_value, upper_value});
				return;
			}

			// Force alignment
			r.lower_it += !r.lower_is_start;
			r.upper_it -= !r.upper_is_end;


			std::cout <<"i2 Range from " << int(r.lower_it-_data.begin()) << " to " << int(r.upper_it-_data.begin()) << std::endl;


			// Modify values
			*r.lower_it = std::min(*r.lower_it, lower_value);
			*r.upper_it = std::max(*r.upper_it, upper_value);

			// Erase any gaps in the interval
			if (r.upper_it - r.lower_it > 1) _data.erase(r.lower_it+1, r.upper_it-1);


		}


	};



}; // namespace Dattatypes
