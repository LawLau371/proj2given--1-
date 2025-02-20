#include "../include/StringUtils.h"
#include <string>
#include <vector>
#include <algorithm>

std::string StringUtils::Slice(const std::string &str, ssize_t start, ssize_t end) noexcept {
    if (end == 0 || (size_t)end > str.size()) end = str.size();
    if (start < 0) start += str.size();
    if (end < 0) end += str.size();
    return str.substr(start, end - start);
}

std::string StringUtils::Capitalize(const std::string &str) noexcept {
    if (str.empty()) return str;
    std::string result = str;
    result[0] = std::toupper(static_cast<unsigned char>(result[0]));
    std::transform(result.begin() + 1, result.end(), result.begin() + 1, ::tolower);
    return result;
}

std::string StringUtils::Upper(const std::string &str) noexcept {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string StringUtils::Lower(const std::string &str) noexcept {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string StringUtils::LStrip(const std::string &str) noexcept {
    size_t start = str.find_first_not_of(" \t\n\r");
    return (start == std::string::npos) ? "" : str.substr(start);
}

std::string StringUtils::RStrip(const std::string &str) noexcept {
    size_t end = str.find_last_not_of(" \t\n\r");
    return (end == std::string::npos) ? "" : str.substr(0, end + 1);
}

std::string StringUtils::Strip(const std::string &str) noexcept {
    return LStrip(RStrip(str));
}

std::string StringUtils::Center(const std::string &str, int width, char fill) noexcept {
    int padding = width - str.size();
    if (padding <= 0) return str;
    int left = padding / 2;
    int right = padding - left;
    return std::string(left, fill) + str + std::string(right, fill);
}

// std::string StringUtils::LJust(const std::string &str, int width, char fill) noexcept;
// std::string StringUtils::RJust(const std::string &str, int width, char fill) noexcept;
// std::string StringUtils::ExpandTabs(const std::string &str, int tabsize) noexcept;
// int StringUtils::EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept;

std::string StringUtils::Replace(const std::string &str, const std::string &old, const std::string &rep) noexcept {
    std::string result = str;
    size_t pos = 0;
    while ((pos = result.find(old, pos)) != std::string::npos) {
        result.replace(pos, old.length(), rep);
        pos += rep.length();
    }
    return result;
}

std::vector<std::string> StringUtils::Split(const std::string &str, const std::string &splt) noexcept {
    std::vector<std::string> result;
    size_t start = 0, end;
    while ((end = str.find(splt, start)) != std::string::npos) {
        result.push_back(str.substr(start, end - start));
        start = end + splt.length();
    }
    result.push_back(str.substr(start));
    return result;
}

std::string StringUtils::Join(const std::string &str, const std::vector<std::string> &vect) noexcept {
    std::string result;
    for (size_t i = 0; i < vect.size(); ++i) {
        if (i > 0) result += str;
        result += vect[i];
    }
    return result;
}

std::string StringUtils::LJust(const std::string &str, int width, char fill) noexcept {
    if (static_cast<int>(str.length()) >= width) return str;
    return str + std::string(width - str.length(), fill);
}

std::string StringUtils::RJust(const std::string &str, int width, char fill) noexcept {
    if (static_cast<int>(str.length()) >= width) return str;
    return std::string(width - str.length(), fill) + str;
}

std::string StringUtils::ExpandTabs(const std::string &str, int tabsize) noexcept {
    std::string result;
    for (char ch : str) {
        if (ch == '\t') {
            result.append(tabsize - (result.length() % tabsize), ' ');
        } else {
            result.push_back(ch);
        }
    }
    return result;
}

int StringUtils::EditDistance(const std::string &left, const std::string &right, bool ignorecase) noexcept {
    std::string l = left, r = right;
    if (ignorecase) {
        std::transform(l.begin(), l.end(), l.begin(), ::tolower);
        std::transform(r.begin(), r.end(), r.begin(), ::tolower);
    }
    
    std::vector<std::vector<int>> dp(l.size() + 1, std::vector<int>(r.size() + 1, 0));
    for (size_t i = 0; i <= l.size(); ++i) dp[i][0] = i;
    for (size_t j = 0; j <= r.size(); ++j) dp[0][j] = j;
    
    for (size_t i = 1; i <= l.size(); ++i) {
        for (size_t j = 1; j <= r.size(); ++j) {
            if (l[i - 1] == r[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1];
            } else {
                dp[i][j] = std::min({dp[i - 1][j] + 1, dp[i][j - 1] + 1, dp[i - 1][j - 1] + 1});
            }
        }
    }
    return dp[l.size()][r.size()];
}
