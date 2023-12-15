//
// Created by Richard Vogel on 14.12.23.
//

#include "file_utils.h"
#include <fstream>
#include <iostream>

namespace aoc2024::utils {
    std::vector<std::string> read_file_lines(const std::string_view& path) {
        std::ifstream file(path.data());
        if (!file.is_open()) {
            std::cerr << "Could not open file " << path << std::endl;
            return {};
        }
        std::vector<std::string> lines;
        std::string line;
        while (std::getline(file, line)) {
            lines.push_back(line);
        }
        file.close();
        return lines;
    }

    std::vector<std::string> load_day(size_t day, RIDDLE_TYPE type) {
        std::string path = BASE;
        path += "/days/day";
        path += std::to_string(day);
        path += "/";
        switch (type) {
            case RIDDLE_TYPE::TEST:
                path += "in_test.txt";
                break;
            case RIDDLE_TYPE::TASK:
                path += "in_task.txt";
                break;
        }
        return read_file_lines(path);
    }
}