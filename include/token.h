#pragma once

struct Token{
	int type;
	std::string value;
	uint line;
	uint column;
};

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
