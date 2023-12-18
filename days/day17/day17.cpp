//
// Created by Richard Vogel on 17.12.23.
//
#include "day17.h"
#include <iostream>
#include <list>
#include <queue>

namespace aoc2024::day17 {


    Field Field::parse(const std::vector<std::string> &input) {
        Field field;
        if (input.empty()) {
            std::cerr << "Input is empty" << std::endl;
            return field;
        }
        field.init_field(input[0].size(), input.size());
        std::size_t x = 0;
        std::size_t y = 0;
        for (const auto &line: input) {
            x = 0;
            for (const auto &number: line) {
                try {
                    const auto n = static_cast<uint8_t>(number - '0');
                    if (n > 9) {
                        throw std::runtime_error("Number must be between 0 and 9");
                    }
                    field.set_heat_loss(x, y, n);
                } catch (const std::exception &e) {
                    std::cerr << "Invalid input: " << e.what() << std::endl;
                    return field;
                }
                x++;
            }
            y++;
        }

        return field;
    }

    std::string Field::to_string() const {
        std::string result;
        for (auto y = 0; y < height(); y++) {
            for (auto x = 0; x < width(); x++) {
                result += std::to_string(get(x, y).heat_loss);
            }
            result += "\n";
        }
        return result;
    }

    std::size_t Field::do_steps(PathDescriptor start, bool second_task) {
        std::priority_queue<PathDescriptor, std::vector<PathDescriptor>, ComparePath> path = {};
        path.push(std::move(start));
        std::size_t global_min = -1;

        while (!path.empty()) {
            const PathDescriptor curr = path.top();
            path.pop();
            const auto x = curr.x;
            const auto y = curr.y;

            if (x >= width() || y >= height()) {
                continue; // out of bounds
            }

            auto &field = get_mut(x, y);
            const auto accumulated_heat = curr.accumulated_heat + field.heat_loss;

            if (accumulated_heat >= global_min) {
                continue;
            }

            // end condition
            if (x == width() - 1 && y == height() - 1) {
                // std::cout << accumulated_heat << std::endl;
                if (accumulated_heat < global_min) {
                    if (!second_task) {
                        // always valid if we touch the end
                        global_min = accumulated_heat;
                    } else {
                        // only valid if we made at least 4 straight moves
                        if (curr.straight_move_count >= 3) {
                            global_min = accumulated_heat;
                        }
                    }
                }
                continue;
            }

            auto &visited_state_dir = field.visited_state.at(curr.dir);
            if (!visited_state_dir.contains(curr.straight_move_count)) {
                visited_state_dir[curr.straight_move_count] = accumulated_heat;
            } else {
                if (visited_state_dir[curr.straight_move_count] <= accumulated_heat) {
                    continue;
                }
                visited_state_dir[curr.straight_move_count] = accumulated_heat;
            }

            if (!second_task) {
                for (const auto next_dir: possible_moves.at(curr.dir)) {
                    const auto next_pos = std::pair{x + dx_dy.at(next_dir).first, y + dx_dy.at(next_dir).second};
                    if (next_dir == curr.dir) {
                        // we can go straight
                        if (curr.straight_move_count < 3) {
                            path.push({
                                              .x=next_pos.first,
                                              .y=next_pos.second,
                                              .straight_move_count=static_cast<used_straight_moves_t>(
                                                      curr.straight_move_count + 1),
                                              .dir=next_dir,
                                              .accumulated_heat=accumulated_heat,
                                      });
                        }
                    } else {
                        // we can go 90 degrees
                        path.push({.x=next_pos.first,
                                          .y=next_pos.second,
                                          .straight_move_count=1, .dir=next_dir, .accumulated_heat=accumulated_heat,
                                  });
                    }
                }
            } else {
                // second task
                const auto straight_move_cnt = curr.straight_move_count;
                if (straight_move_cnt < 4) {
                    // we must go straight
                    const auto next_pos = std::pair{x + dx_dy.at(curr.dir).first, y + dx_dy.at(curr.dir).second};
                    path.push({
                                      .x=next_pos.first,
                                      .y=next_pos.second,
                                      .straight_move_count=static_cast<used_straight_moves_t>(
                                              curr.straight_move_count + 1),
                                      .dir=curr.dir,
                                      .accumulated_heat=accumulated_heat,
                              });
                } else {
                    // we can go 90 degrees
                    for (const auto next_dir: possible_moves.at(curr.dir)) {
                        const bool is_straight_move = next_dir == curr.dir;
                        if (is_straight_move) {
                            if (curr.straight_move_count > 9) { // we **must** turn here
                                continue;
                            }
                        }

                        const auto next_pos = std::pair{x + dx_dy.at(next_dir).first, y + dx_dy.at(next_dir).second};
                        path.push({.x=next_pos.first,
                                          .y=next_pos.second,
                                          .straight_move_count=!is_straight_move
                                                               ? static_cast<used_straight_moves_t>(1)
                                                               : static_cast<used_straight_moves_t>(
                                                                       curr.straight_move_count +
                                                                       1),
                                          .dir=next_dir,
                                          .accumulated_heat=accumulated_heat,
                                  });
                    }
                }
            }
        }


        return global_min;
    }

    int day17_1(const std::vector<std::string> &input) {
        Field f = Field::parse(input);
        // std::cout << f.to_string() << std::endl;
        auto res = f.do_steps({
                                      .x=0,
                                      .y=0,
                                      .straight_move_count=0,
                                      .dir=Direction::RIGHT,
                                      .accumulated_heat=0,
                              });

        return static_cast<int>(res) - f.get(0, 0).heat_loss; // correct for the first step
    }

    int day17_2(const std::vector<std::string> &input) {
        Field f = Field::parse(input);
        // std::cout << f.to_string() << std::endl;
        auto res = f.do_steps({
                                      .x=0,
                                      .y=0,
                                      .straight_move_count=0,
                                      .dir=Direction::DOWN, // idk why I need to start with down (all examples work with right; however, the task does not state that we need to start with right)
                                      .accumulated_heat=0,
                              }, true);

        return static_cast<int>(res) - f.get(0, 0).heat_loss;
    }


}
