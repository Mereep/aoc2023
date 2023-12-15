#include <iostream>
#include "days/day14/day14.h"
#include "utils/file_utils.h"
#include <iostream>

int main() {
    const auto in14_test = aoc2024::utils::load_day(14, aoc2024::utils::RIDDLE_TYPE::TEST);
    const auto in14_task = aoc2024::utils::load_day(14, aoc2024::utils::RIDDLE_TYPE::TASK);

    // Day 14
    printf("Day 14.1 test: %d\n", aoc2024::day14::day14_1(in14_test));
    printf("Day 14.1 task: %d\n", aoc2024::day14::day14_1(in14_task));
    printf("Day 14.2 test: %d\n", aoc2024::day14::day14_2(in14_test));
    printf("Day 14.2 task: %d\n", aoc2024::day14::day14_2(in14_task));

    return 0;
}
