#include <iostream>
#include "days/day14/day14.h"
#include "days/day16/day16.h"
#include "days/day17/day17.h"
#include "utils/file_utils.h"
#include <iostream>

int main() {

    // // Day 14
    const auto in14_test = aoc2024::utils::load_day(14, aoc2024::utils::RIDDLE_TYPE::TEST);
    const auto in14_task = aoc2024::utils::load_day(14, aoc2024::utils::RIDDLE_TYPE::TASK);
    printf("Day 14.1 test: %d\n", aoc2024::day14::day14_1(in14_test));
    printf("Day 14.1 task: %d\n", aoc2024::day14::day14_1(in14_task));
    printf("Day 14.2 test: %d\n", aoc2024::day14::day14_2(in14_test));
    printf("Day 14.2 task: %d\n", aoc2024::day14::day14_2(in14_task));

    // // Day 16
    const auto in16_test = aoc2024::utils::load_day(16, aoc2024::utils::RIDDLE_TYPE::TEST);
    const auto in16_task = aoc2024::utils::load_day(16, aoc2024::utils::RIDDLE_TYPE::TASK);
    printf("Day 16.1 test: %d\n", aoc2024::day16::day16_1(in16_test));
    printf("Day 16.1 task: %d\n", aoc2024::day16::day16_1(in16_task));
    printf("Day 16.2 test: %d\n", aoc2024::day16::day16_2(in16_test));
    printf("Day 16.2 task: %d\n", aoc2024::day16::day16_2(in16_task));

    // Day 17
    const auto in17_test = aoc2024::utils::load_day(17, aoc2024::utils::RIDDLE_TYPE::TEST);
    const auto in17_task = aoc2024::utils::load_day(17, aoc2024::utils::RIDDLE_TYPE::TASK);
    printf("Day 17.1 test: %d\n", aoc2024::day17::day17_1(in17_test));
    printf("Day 17.1 task: %d\n", aoc2024::day17::day17_1(in17_task));
    printf("Day 17.2 test: %d\n", aoc2024::day17::day17_2(in17_test));
    printf("Day 17.2 task: %d\n", aoc2024::day17::day17_2(in17_task));
}
