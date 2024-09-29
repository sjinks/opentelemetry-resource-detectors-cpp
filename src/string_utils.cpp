#include "string_utils.h"

std::vector<std::string_view> split(std::string_view str, char delim)
{
    std::vector<std::string_view> tokens;
    tokens.reserve(10);  // NOLINT(*-magic-numbers)

    std::string_view::size_type start = 0;
    std::string_view::size_type end   = 0;

    while ((end = str.find(delim, start)) != std::string_view::npos) {
        tokens.push_back(str.substr(start, end - start));
        start = end + 1;
    }

    tokens.push_back(str.substr(start));
    return tokens;
}

bool starts_with(std::string_view str, std::string_view prefix)
{
#if __cplusplus >= 202002L
    return str.starts_with(prefix);
#else
    return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
#endif
}

bool ends_with(std::string_view str, std::string_view suffix)
{
#if __cplusplus >= 202002L
    return str.ends_with(suffix);
#else
    return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
#endif
}

std::string_view nth_word(std::string_view str, std::size_t n)
{
    std::string_view::size_type start = str.find_first_not_of(' ');
    std::string_view::size_type end   = 0;
    std::size_t word_count            = 1;

    while (start != std::string_view::npos && end != std::string_view::npos) {
        end = str.find(' ', start);
        if (word_count == n) {
            return str.substr(start, end - start);
        }

        start = str.find_first_not_of(' ', end + 1);
        ++word_count;
    }

    return {};
}
