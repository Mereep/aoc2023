//
// Created by Richard Vogel on 14.12.23.
//

#ifndef AOC2024_FILE_UTILS_H
#define AOC2024_FILE_UTILS_H
#include <string>
namespace aoc2024::utils {
    constexpr const char* BASE = BASE_DIR;
    enum class RIDDLE_TYPE {
        TEST,
        TASK,
    };
    std::vector<std::string> read_file_lines(const std::string_view& path);
    std::vector<std::string> load_day(size_t day, RIDDLE_TYPE type);
}
#endif //AOC2024_FILE_UTILS_H
