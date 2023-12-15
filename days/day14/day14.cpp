//
// Created by Richard Vogel on 14.12.23.
//

#include "day14.h"
#include "ranges"
#include "map"
#include "set"

namespace aoc2024::day14 {
    inline std::optional<std::pair<size_t, size_t>> Field::move(size_t x, size_t y, TiltDir dir, bool keep_moving) {
        if (!can_move(x, y, dir)) {
            return std::nullopt;
        }

        set(x, y, FieldType::FREE);
        const auto new_pos = [&]() -> std::pair<size_t, size_t> {
            switch (dir) {
                case TiltDir::NORTH:
                    set(x, y - 1, FieldType::MOVEABLE_STONE);
                    return {x, y - 1};

                case TiltDir::EAST:
                    set(x + 1, y, FieldType::MOVEABLE_STONE);
                    return {x + 1, y};

                case TiltDir::SOUTH:
                    set(x, y + 1, FieldType::MOVEABLE_STONE);
                    return {x, y + 1};

                case TiltDir::WEST:
                    set(x - 1, y, FieldType::MOVEABLE_STONE);
                    return {x - 1, y};
            }
        }();

        if (!keep_moving) {
            return new_pos;
        }

        return move(new_pos.first, new_pos.second, dir, keep_moving);
    }

    inline bool Field::can_move(size_t x, size_t y, TiltDir dir) const {
        if (get(x, y) != FieldType::MOVEABLE_STONE) {
            return false;
        }

        switch (dir) {
            case TiltDir::NORTH:
                if (y <= 0) {
                    return false;
                }
                return get(x, y - 1) == FieldType::FREE;

            case TiltDir::EAST:
                if (x >= width() - 1) {
                    return false;
                }
                return get(x + 1, y) == FieldType::FREE;

            case TiltDir::SOUTH:
                if (y >= height() - 1) {
                    return false;
                }
                return get(x, y + 1) == FieldType::FREE;

            case TiltDir::WEST:
                if (x <= 0) {
                    return false;
                }
                return get(x - 1, y) == FieldType::FREE;
        }
        return false;
    }

    inline void Field::tilt(TiltDir dir) {
        // move from the direction we move TO backwards
        // e.g., NORTH means we start at the bottom of the field and move up line by line
        // if we find a stone we move it down as far as we can and continue
        switch (dir) {
            case TiltDir::NORTH: {
                const auto base_range = std::views::iota(1, static_cast<int>(height()));
                for (const auto &y: base_range) {
                    for (size_t x = 0; x < width(); x++) {
                        move(x, y, dir, true);
                    }
                }
                break;
            }

            case TiltDir::SOUTH: {
                const auto base_range = std::views::iota(0, static_cast<int>(height())) | std::views::reverse;
                for (const auto &y: base_range) {
                    for (size_t x = 0; x < width(); x++) {
                        move(x, y, dir, true);
                    }
                }
                break;
            }

            case TiltDir::EAST: {
                const auto base_range = std::views::iota(0, static_cast<int>(width())) | std::views::reverse;
                for (const auto &x: base_range) {
                    for (size_t y = 0; y < height(); y++) {
                        move(x, y, dir, true);
                    }
                }
                break;
            }

            case TiltDir::WEST: {
                const auto base_range = std::views::iota(1, static_cast<int>(width()));
                for (const auto &x: base_range) {
                    for (size_t y = 0; y < height(); y++) {
                        move(x, y, dir, true);
                    }
                }
                break;
            }
        }

    }

    std::size_t Field::sum_field() const {
        size_t line = 0;
        size_t score = 0;
        for (size_t y = 0; y < m_height; ++y) {
            for (size_t x = 0; x < m_width; ++x) {
                if (get(x, y) == FieldType::MOVEABLE_STONE) {
                    score += height() - line;
                }
            }
            line++;
        }

        return score;
    }

    Field Field::parse(const std::vector<std::string> &in) {
        if (in.empty()) {
            std::cerr << "Cannot parse empty field" << std::endl;
            return {0, 0};
        }

        const size_t height = in.size();
        const size_t width = in[0].size();
        Field field(width, height);
        size_t y = 0;
        size_t x;
        for (const auto &line: in) {
            x = 0;
            for (const char &chr: line) {
                switch (chr) {
                    case '#':
                        field.set(x, y, FieldType::FIXED_STONE);
                        break;
                    case 'O':
                        field.set(x, y, FieldType::MOVEABLE_STONE);
                        break;
                    case '.':
                        field.set(x, y, FieldType::FREE);
                        break;
                    default:
                        std::cerr << "Unknown character " << chr << std::endl;
                        break;
                }
                ++x;
            }
            ++y;
        }
        return field;
    }

    int day14_1(const std::vector<std::string> &in) {
        Field field = Field::parse(in);
        // field.print_field();
        field.tilt(TiltDir::NORTH);
        // std::cout << std::endl;
        // field.print_field();
        return field.sum_field();
    }

    int day14_2(const std::vector<std::string> &in) {
        // we cannot iterate 1000000000 times (takes too long)
        // so idea is that we search until we find a loop
        // after each loop, the result is the same like after the loop before
        // until we find the loop, we have to first find a stable state (start of loop)

        // this variable remembers the hashes of the fields we have seen
        auto seen = std::set<std::string>();
        Field field = Field::parse(in);

        // until we first find a repetitions
        size_t loop_start = 0;

        // from first to second repetition
        size_t loop_size = 0;
        for (size_t i = 0; i < 1000000000 /** we will skip most of this*/; ++i) {
            // std::cout << i << std::endl;
            // tilt in every direction
            field.tilt(TiltDir::NORTH);
            field.tilt(TiltDir::WEST);
            field.tilt(TiltDir::SOUTH);
            field.tilt(TiltDir::EAST);
            const auto hash = field.hash();
            if (seen.contains(hash)) { // seen already
                if (loop_start == 0) {
                    // we found this configuration for the first time
                    // std::cout << "Found loop at " << i << std::endl;
                    loop_start = i;
                    seen.clear(); // we will start again until this happens again
                } else {
                    // we found the same configuration again

                    // we know the loop size now
                    loop_size = i - loop_start;
                    // std::cout << "Found loop size " << loop_size << std::endl;

                    // count how often we can put the loop into the remaining iterations
                    const auto remainder = (1000000000 - i) / loop_size;
                    // and skip
                    i += remainder * loop_size;

                    // just very negligible performance improvement
                    seen.clear();
                    // std::cout << " skip to " << i << std::endl;
                    // std::cout << field.sum_field() << std::endl;
                }
            }
            seen.insert(hash);
            // std::cout << field.sum_field() << std::endl;
        }
        // field.print_field();

        return field.sum_field();
    }
}
