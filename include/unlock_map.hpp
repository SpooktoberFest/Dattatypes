#pragma once
// === HEADER ONLY ===

#include <vector>
#include <type_traits>

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

		// TODO: Insert a single value into the interval set
		void insert(const T item) {}

		// Insert a range [item_begin, item_end] of values into the interval set
		void insert(const T item_begin, const T item_end) {
			U lower_value = U(item_begin);
			U upper_value = U(item_end);

			if (lower_value > upper_value) return;

			if (_data.empty() || _data.back() < lower_value) {
				_data.insert(_data.end(), {lower_value, upper_value+1});
				return;
			}
			if (_data.front() > lower_value) {
				_data.insert(_data.begin(), {lower_value, upper_value+1});
				return;
			}

			// Find the first i where: _data[i] <= lower_value
			size_t i = 0;
			while (i < _data.size() && _data[i] > lower_value) ++i;

			// Find the first j where: _data[j] < upper_value
			size_t j = i;
			while (j < _data.size() && _data[j] >= upper_value) ++j;




			const auto src = "TMP:UnlockMap:TEST";
			LOG_WARN("I: {}, J: {}", i, j);



			// Case of lower touching other interval
			bool i_is_start = (i & 1);
			if (!i_is_start && (lower_value == _data[i])) {
				--i; i_is_start=true;
			}

			// Case of upper touching other interval
			bool j_is_end = !(j & 1);
			if (!j_is_end && j != _data.size() && (upper_value+1 == _data[j])) {
				++j; j_is_end=true;
			}
			if (j == _data.size()) {
				LOG_WARN("J is at size!!");
			}

			// Case of whole interval being if empty region
			if ((i + 1 == j) && !i_is_start) {
				_data.insert(_data.begin(), {lower_value, upper_value+1});
				return;
			}

			// Force alignment
			i += !i_is_start;
			j -= !j_is_end;

			// Modify values
			_data[i] = std::min(_data[i], lower_value);
			_data[j] = std::max(_data[j], upper_value);

			// Erase any gaps in the interval
			if (i + 1 != j) _data.erase(_data.begin()+i+1, _data.begin()+j-1);
		}

		// TODO: Erase a single value from the interval set
		void erase(const T item) {}

		// TODO: Erase a range [item_begin, item_end] of values from the interval set
		void erase(const T item_begin, const T item_end) {}


		// Check whether the item is in any stored interval
		bool check(const T item) const {
			U item_value = U(item);
			size_t i = 0;

			// Catch empty data or item_values smaller than first data_value
			if (_data.empty() || _data[0] > item_value) return false;

			// i : data <= item
			while (i < _data.size() && _data[i] > item_value) ++i;
			return !(i & 1); // Item already exists if index is odd.
		}

		constexpr bool empty() const { return _data.empty(); }
		constexpr void clear() { _data.clear(); }
		constexpr void reserve(std::size_t __n) { _data.reserve(__n); }
		constexpr void shrink_to_fit() { _data.shrink_to_fit(); }

		constexpr size_t size() const {
			size_t sum=0, i=0;
			for (; i<_data.size() ; i+=2)
				sum += size_t(_data[i+1]-_data[i]-1);
			return sum;
		}

	};


}; // namespace Dattatypes
