//
// Created by Richard Vogel on 17.12.23.
//

#ifndef AOC2024_DAY17_H
#define AOC2024_DAY17_H

#include <string>
#include <ranges>
#include <map>
#include <ostream>

namespace aoc2024::day17 {
    using used_straight_moves_t = uint8_t;
    using accumulated_heat_loss_t = std::size_t;

    enum class Direction {
        UP,
        DOWN,
        LEFT,
        RIGHT,
    };

    struct PathDescriptor {
        std::size_t x;
        std::size_t y;
        used_straight_moves_t straight_move_count;
        Direction dir;
        std::size_t accumulated_heat;

    };

    struct ComparePath {
        bool operator()(const PathDescriptor& a, const PathDescriptor& b) {
            return a.accumulated_heat > b.accumulated_heat; // Min-heap based on cost
        }
    };

    const std::map<Direction, std::vector<Direction>> possible_moves = {
            {Direction::UP,    {Direction::UP,   Direction::LEFT, Direction::RIGHT}},
            {Direction::DOWN,  {Direction::DOWN, Direction::LEFT, Direction::RIGHT}},
            {Direction::LEFT,  {Direction::UP,   Direction::DOWN, Direction::LEFT}},
            {Direction::RIGHT, {Direction::UP,   Direction::DOWN, Direction::RIGHT}},
    };

    const std::map<Direction, std::pair<std::size_t, size_t>> dx_dy = {
            {Direction::DOWN,  {0,  1}},
            {Direction::UP,    {0,  -1}},
            {Direction::LEFT,  {-1, 0}},
            {Direction::RIGHT, {1,  0}},
    };

    struct FieldType {
        /// how much we loose when touching this field
        uint8_t heat_loss;

        /// Stores how much heat we accumulated so far when touching this field under a certain direction
        std::map<Direction,
                std::map<used_straight_moves_t ,
                        accumulated_heat_loss_t>> visited_state;
    };

    class Field {
    public:
        static Field parse(const std::vector<std::string> &input);

        void init_field(size_t width, size_t height) {
            m_width = width;
            m_height = height;
            m_field = std::vector<FieldType>(width * height);
            std::fill(m_field.begin(), m_field.end(), FieldType{});
            reset();
        }

        void reset() {
            for (auto &f: m_field) {
                f.visited_state = {
                        {Direction::UP,    {}},
                        {Direction::DOWN,  {}},
                        {Direction::LEFT,  {}},
                        {Direction::RIGHT, {}},
                };
            }
        }

        [[nodiscard]] size_t width() const {
            return m_width;
        }

        [[nodiscard]] size_t height() const {
            return m_height;
        }

        [[nodiscard]] FieldType get(size_t x, size_t y) const {
            return m_field[y * m_width + x];
        }

        FieldType &get_mut(size_t x, size_t y) {
            return m_field[y * m_width + x];
        }

        void set_heat_loss(size_t x, size_t y, uint8_t heat_loss) {
            m_field[y * m_width + x].heat_loss = heat_loss;
        }

        std::string to_string() const;


        [[nodiscard]] std::size_t do_steps(PathDescriptor start, bool second_task=false);

    private:
        std::size_t m_width;
        std::size_t m_height;
        std::vector<FieldType> m_field;
    };

    int day17_1(const std::vector<std::string> &input);

    int day17_2(const std::vector<std::string> &input);
}
#endif //AOC2024_DAY17_H
