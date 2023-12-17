//
// Created by Richard Vogel on 16.12.23.
//

#ifndef AOC2024_DAY16_H
#define AOC2024_DAY16_H

#include <vector>
#include <optional>
#include <list>

namespace aoc2024::day16 {

    /** We encode a direction such that we can use bitwise
     * operations to check to check for equalness on overlapping
     * directions.
     */
    enum class Direction {
        UP = 0b0001,
        DOWN = 0b0010,
        LEFT = 0b0100,
        RIGHT = 0b1000,
    };

    enum class FieldType {
        SPACE,
        REFLECTOR_UPWARDS,
        REFLECTOR_DOWNWARDS,
        SPLITTER_VERTICAL,
        SPLITTER_HORIZONTAL,
    };

    struct Beam {
        size_t x;
        size_t y;
        Direction dir;
    };

    class Field {
    public:
        static Field parse(const std::vector<std::string> &input);


        [[nodiscard]] size_t width() const {
            return m_width;
        }

        [[nodiscard]] size_t height() const {
            return m_height;
        }

        void init_field(size_t width, size_t  height) {
            m_width = width;
            m_height = height;
            m_field = std::vector<FieldType>(width * height);
            m_visited_states = std::vector<uint8_t>(width * height);
            std::fill(m_field.begin(), m_field.end(), FieldType::SPACE);
            reset();
        }

        void reset() {
            std::fill(m_visited_states.begin(), m_visited_states.end(), 0);
            m_beams = {};
            first_move = true;
        }

        [[nodiscard]] std::optional<FieldType> get(size_t x, size_t y) const {
            if (x >= width() || y >= height()) {
                return std::nullopt;
            }
            return m_field[y * width() + x];
        }

        void set(size_t x, size_t y, FieldType type) {
            m_field[y * width() + x] = type;
        }

        [[nodiscard]] bool is_energized(size_t x, size_t y) const {
            return m_visited_states[y * width() + x] > 0;
        }

        /***
         * adds a [Beam] if it is not out of bounds
         * **/
        void add_beam(Beam beam) {
            if (beam.x >= width() || beam.y >= height()) {
                return;
            }
            m_beams.push_back(beam);
        }

        /***
         * will do one `frame`
         */
        void move_beams();

        [[nodiscard]] bool has_beams() {
            return !m_beams.empty();
        }

        [[nodiscard]] std::list<Beam> beams() const {
            return m_beams;
        }

        [[nodiscard]] uint8_t get_visit_state(uint8_t x, uint8_t y) const {
            return m_visited_states[y * width() + x];
        }

        void add_visited_state(uint8_t x, uint8_t y, Direction d) {
            m_visited_states[y * width() + x] |= static_cast<uint8_t>(d);
        }

        /***
         * Normal to string function. Will print the field with the beams
         * beams will overwrite the field if they are on the same position
         * if multiple beams are on the same position, a number will be printed
         * @return
         */
        [[nodiscard]] std::string to_string() const;

        [[nodiscard]] std::string to_visited_map_string() const;

        /***
         * returns the field with energized fields marked with a `#`
         * @return
         */
        [[nodiscard]] std::string energy_map() const;
        [[nodiscard]] size_t energy_level() const;
    private:

        std::vector<FieldType> m_field;

        /// we do a list here so we can remove beams that have been visited from all directions without
        /// reclaiming memory
        std::list<Beam> m_beams;

        /// key to remember which directions we have visited (i.e., which paths we have already taken)
        /// if we have visited a field from all directions, we can remove it from the list of beams
        /// if we do not do this, we will have a lot of duplicate paths (i.e., we will not finish)
        std::vector<uint8_t> m_visited_states;
        size_t m_width;
        size_t m_height;
        bool first_move = true;
    };

    int day16_1(const std::vector<std::string> &input);

    int day16_2(const std::vector<std::string> &input);
}
#endif //AOC2024_DAY16_H
