#pragma once

struct Token{
	int type;
	std::string value;
	uint line;
	uint column;
};
/*
inline bool safe_stoi(const std::string& str, const int format, int& result) {
	size_t idx = 0;

	// Check if the string is empty or whitespace-only
	if (str.empty() || str.find_first_not_of(" \t\n\r") == std::string::npos) {
		return false;
	}

	// Use std::from_chars if available (C++17+)
	if (format == 10) {
		auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
		if (ec != std::errc() || ptr != str.data() + str.size()) {
			return false;
		}
		return true;
	}

	// Fallback: manually validate and convert (simple base 16 and 8 support)
	char* endptr = nullptr;
	errno = 0;
	long value = std::strtol(str.c_str(), &endptr, format);
	if (errno != 0 || endptr != str.c_str() + str.size()) {
		return false;
	}
	// Check bounds
	if (value < std::numeric_limits<int>::min() || value > std::numeric_limits<int>::max()) {
		return false;
	}
	result = static_cast<int>(value);
	return true;
}*/

inline bool safe_stoi(const std::string& str,const int format, int& result) {
	try {
		size_t idx;
		// Check if the string is empty or contains only spaces
		if (str.empty() || str.find_first_not_of(" \t\n\r") == std::string::npos) {
			return false;
		}
		// Attempt conversion
		int value = std::stoi(str, &idx,format);
		// Ensure no extra non-numeric characters exist after the number
		if (idx != str.size()) {
			return false;
		}
		// Success, store result
		result = value;
		return true;
	} catch (const std::invalid_argument& e) {
		// Non-numeric input
		return false;
	} catch (const std::out_of_range& e) {
		// Out-of-bounds integer value
		return false;
	}
}
