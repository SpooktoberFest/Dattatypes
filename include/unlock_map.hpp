#pragma once
// === HEADER ONLY ===

#include <vector>
#include <type_traits>


namespace Dattatypes {

	template<typename T>
	class unlock_map {
		static_assert(std::is_enum_v<T>, "T must be an enum type");
		using U = std::underlying_type_t<T>;
	public:
		unlock_map() = default;
		unlock_map(std::vector<U> vec) : _data(std::move(vec)) {};
		~unlock_map() = default;


		std::vector<U> _data; // pairs of [start, end)

		// Insert a single value [item, item+1) into the interval set
		void insert(const T& item) {
			U entry = static_cast<U>(item);

			// Find the first index where _data[i] > entry
			std::size_t i = 0;
			while (i < _data.size() && _data[i] <= entry) ++i;

			// If index is odd, the value is already within an interval
			if (i & 1) return;

			// If start touches end of another interval
			if (i != 0 && _data[i - 1] == entry)
				// Merge with previous interval
				++_data[i - 1];
			else
				// Insert new start
				_data.emplace(_data.begin() + i, entry);

			i++;

			// If end touches start of another interval
			if (i < _data.size() && _data[i] == entry + 1)
				// Merge with previous interval
				--_data[i];
			else
				// Insert new end
				_data.emplace(_data.begin() + i, entry + 1);
		}

		// Insert a range [item_begin, item_end] (inclusive) into the interval set
		void insert(const T& item_begin, const T& item_end) {
			U begin = static_cast<U>(item_begin);
			U end = static_cast<U>(item_end);

			if (begin > end) return;  // Ignore invalid range

			++end;  // Convert to half-open [begin, end)

			std::size_t i = 0;
			while (i < _data.size() && _data[i] <= begin) ++i;

			std::size_t j = i;
			while (j < _data.size() && _data[j] < end) ++j;

			// Extend to include existing overlapping intervals
			if (i & 1) begin = _data[i - 1];
			if (j & 1 && j < _data.size()) end = _data[j];

			// Remove overlapping ranges
			_data.erase(_data.begin() + i, _data.begin() + j);

			// Insert the merged interval
			_data.insert(_data.begin() + i, {begin, end});
		}

		// Check whether the item is in any stored interval
		bool check(const T& item) const {
			U entry = static_cast<U>(item);
			for (std::size_t i = 0; i < _data.size(); ++i) {
				if (_data[i] > entry)
					return (i & 1); // Inside interval if index is odd
			}
			return false; // Not found in any interval
		}


		// (De)Serialization
		template <class Archive>
		void serialize(Archive& ar) { ar(_data); }
	};


}; // namespace Dattatypes
