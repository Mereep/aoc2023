//
// Created by Richard Vogel on 16.12.23.
//

#include "day16.h"
#include <iostream>
#include <set>

namespace aoc2024::day16 {
    std::string Field::energy_map() const {
        std::string result;
        for (size_t y = 0; y < height(); ++y) {
            for (size_t x = 0; x < width(); ++x) {
                result += is_energized(x, y) ? '#' : '.';
            }
            result += '\n';
        }
        return result;
    }

    std::string Field::to_string() const {
        std::string result;
        for (size_t y = 0; y < height(); ++y) {
            for (size_t x = 0; x < width(); ++x) {
                size_t found_beam = 0;
                for (const auto &beam: m_beams) {
                    if (beam.x == x && beam.y == y) {
                        found_beam += 1;
                        if (found_beam > 1) {
                            result[result.size() - 1] = std::to_string(found_beam).at(0);
                        } else {
                            switch (beam.dir) {
                                case Direction::UP:
                                    result += '^';
                                    break;
                                case Direction::DOWN:
                                    result += 'v';
                                    break;
                                case Direction::LEFT:
                                    result += '<';
                                    break;
                                case Direction::RIGHT:
                                    result += '>';
                                    break;
                            }
                        }
                    }
                }

                if (found_beam == 0) {

                    switch (get(x, y).value()) {
                        case FieldType::SPACE:
                            result += '.';
                            break;
                        case FieldType::REFLECTOR_UPWARDS:
                            result += '/';
                            break;
                        case FieldType::REFLECTOR_DOWNWARDS:
                            result += '\\';
                            break;
                        case FieldType::SPLITTER_HORIZONTAL:
                            result += '-';
                            break;
                        case FieldType::SPLITTER_VERTICAL:
                            result += '|';
                            break;
                    }
                }
            }
            result += '\n';
        }
        return result;
    }

    std::string Field::to_visited_map_string() const {
        std::string result;
        for (size_t y = 0; y < height(); ++y) {
            for (size_t x = 0; x < width(); ++x) {
                uint8_t dir_num = get_visit_state(x, y);
                if (dir_num == 0) {
                    result += '.';
                } else {
                    result += std::to_string(dir_num);
                }
            }
            result += '\n';
        }
        return result;
    }

    Field Field::parse(const std::vector<std::string> &input) {
        Field field;
        if (input.empty()) {
            std::cerr << "Input is empty" << std::endl;
            return field;
        }
        size_t width = input[0].size();
        size_t height = input.size();
        field.init_field(width, height);
        for (size_t y = 0; y < height; ++y) {
            for (size_t x = 0; x < width; ++x) {
                switch (input[y][x]) {
                    case '.':
                        field.set(x, y, FieldType::SPACE);
                        break;
                    case '/':
                        field.set(x, y, FieldType::REFLECTOR_UPWARDS);
                        break;
                    case '\\':
                        field.set(x, y, FieldType::REFLECTOR_DOWNWARDS);
                        break;
                    case '-':
                        field.set(x, y, FieldType::SPLITTER_HORIZONTAL);
                        break;
                    case '|':
                        field.set(x, y, FieldType::SPLITTER_VERTICAL);
                        break;
                    default:
                        std::cerr << "Unknown character " << input[y][x] << std::endl;
                        return field;
                }
            }
        }

        return field;
    }

    void Field::move_beams() {
        // remember beams to add on splitters
        std::vector<Beam> new_beams;

        // move all beams
        for (auto it = m_beams.begin(); it != m_beams.end();) {
            auto &beam = *it;
            // calculate new position
            auto new_x = beam.x + ((beam.dir == Direction::RIGHT) ? 1 : beam.dir == Direction::LEFT ? -1 : 0);
            auto new_y = beam.y + ((beam.dir == Direction::DOWN) ? 1 : beam.dir == Direction::UP ? -1 : 0);

            // If the very first guy is already a bouncy thing we will need to "activate" it -> we `step back`
            if (!first_move) {
                beam.x = new_x;
                beam.y = new_y;
            } else {
                new_x = beam.x;
                new_y = beam.y;
            }

            // check whats on the new field
            const auto new_field = get(new_x, new_y);
            // nothin?
            if (new_field == std::nullopt) { // out of bounds
                it = m_beams.erase(it); // remove the beam
                continue;
            }

            switch (new_field.value()) {
                case FieldType::SPACE: // just move (we already did that)
                    break;
                case FieldType::REFLECTOR_UPWARDS: // `/`
                    switch (beam.dir) {
                        case Direction::UP:
                            beam.dir = Direction::RIGHT;
                            break;
                        case Direction::DOWN:
                            beam.dir = Direction::LEFT;
                            break;
                        case Direction::LEFT:
                            beam.dir = Direction::DOWN;
                            break;
                        case Direction::RIGHT:
                            beam.dir = Direction::UP;
                            break;
                    }
                    break;
                case FieldType::REFLECTOR_DOWNWARDS: // `\`
                    switch (beam.dir) {
                        case Direction::UP:
                            beam.dir = Direction::LEFT;
                            break;
                        case Direction::DOWN:
                            beam.dir = Direction::RIGHT;
                            break;
                        case Direction::LEFT:
                            beam.dir = Direction::UP;
                            break;
                        case Direction::RIGHT:
                            beam.dir = Direction::DOWN;
                            break;
                    }
                    break;
                case FieldType::SPLITTER_HORIZONTAL: // `-`
                    switch (beam.dir) {
                        case Direction::UP: // those will split the guy up
                        case Direction::DOWN:
                            // new_beams.emplace_back(new_x - 1, new_y, Direction::LEFT);
                            // new_beams.emplace_back(new_x + 1, new_y, Direction::RIGHT);
                            new_beams.emplace_back(Beam{.x=new_x, .y=new_y, .dir=Direction::LEFT});
                            new_beams.emplace_back(Beam{.x=new_x, .y=new_y, .dir=Direction::RIGHT});
                            it = m_beams.erase(it);
                            continue; // step out (we deleted the current beam and remember the new ones)
                        case Direction::LEFT:       // those do nothing
                        case Direction::RIGHT:
                            break;
                    }
                    break;

                case FieldType::SPLITTER_VERTICAL: // `|`; analogous to `-`
                    switch (beam.dir) {
                        case Direction::LEFT: // those will split the guy up
                        case Direction::RIGHT:
                            new_beams.emplace_back(Beam{.x=new_x, .y=new_y, .dir=Direction::UP});
                            new_beams.emplace_back(Beam{.x=new_x, .y=new_y, .dir=Direction::DOWN});
                            it = m_beams.erase(it);
                            continue;
                        case Direction::UP:       // those do nothing
                        case Direction::DOWN:
                            break;
                    }
                    break;
            }

            it++; // progress
        }

        // add the new beams
        for (const auto &beam: new_beams) {
            add_beam(beam);
        }

        // remove all states we saw already
        // i.e., we do not follow paths we already visited
        for (auto it = m_beams.begin(); it != m_beams.end();) {
            const Beam& beam = *it;
            if (get_visit_state(beam.x, beam.y) & static_cast<uint8_t>(beam.dir)) {
                // visited already
                it = m_beams.erase(it);
                continue;
            } else {
                // remember visit
                add_visited_state(beam.x, beam.y, beam.dir);
                it++;
            }
        }

        // remember to progress after the first move
        first_move = false;
    }

    std::size_t Field::energy_level() const {
        return std::count_if(m_visited_states.begin(), m_visited_states.end(), [](bool b) { return b > 0; });
    }

    int day16_1(const std::vector<std::string> &input) {
        Field field = Field::parse(input);
        field.add_beam(Beam{.x=0, .y=0, .dir = Direction::RIGHT});
        while (field.has_beams()) {
            field.move_beams();
        }
        // std::cout << field.to_string() << std::endl;
        // std::cout << field.energy_map() << std::endl;
        return static_cast<int>(field.energy_level());
    }

    int day16_2(const std::vector<std::string> &input) {
        Field field = Field::parse(input);
        std::vector<Beam> start_beams = {};

        // we add beams for all outer positions

        // left and right
        for (size_t y = 1; y < field.height() -1; ++y) {
            start_beams.emplace_back(Beam{.x=0, .y=y, .dir = Direction::RIGHT});
            start_beams.emplace_back(Beam{.x=field.width() - 1, .y=y, .dir = Direction::LEFT});
        }

        // top and bottom
        for (size_t x = 1; x < field.width() -1; ++x) {
            start_beams.emplace_back(Beam{.x=x, .y=0, .dir = Direction::DOWN});
            start_beams.emplace_back(Beam{.x=x, .y=field.height() - 1, .dir = Direction::UP});
        }

        // add for corners
        // 0,0
        start_beams.emplace_back(Beam{.x=0, .y=0, .dir = Direction::RIGHT});
        start_beams.emplace_back(Beam{.x=0, .y=0, .dir = Direction::DOWN});

        // 0, height - 1
        start_beams.emplace_back(Beam{.x=0, .y=field.height() - 1, .dir = Direction::RIGHT});
        start_beams.emplace_back(Beam{.x=0, .y=field.height() - 1, .dir = Direction::UP});

        // width - 1, 0
        start_beams.emplace_back(Beam{.x=field.width() - 1, .y=0, .dir = Direction::LEFT});
        start_beams.emplace_back(Beam{.x=field.width() - 1, .y=0, .dir = Direction::DOWN});

        // width - 1, height - 1
        start_beams.emplace_back(Beam{.x=field.width() - 1, .y=field.height() - 1, .dir = Direction::LEFT});
        start_beams.emplace_back(Beam{.x=field.width() - 1, .y=field.height() - 1, .dir = Direction::UP});

        std::size_t max_score = 0;
        for (const auto& beam: start_beams) {
            field.reset();
            field.add_beam(beam);
            while (field.has_beams()) {
                field.move_beams();
            }
            max_score = std::max(max_score, field.energy_level());
        }

        return static_cast<int>(max_score);
    }
}