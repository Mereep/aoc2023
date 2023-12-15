//
// Created by Richard Vogel on 14.12.23.
//

#ifndef AOC2024_DAY14_H
#define AOC2024_DAY14_H
#include <memory>
#include <vector>
#include <iostream>

namespace aoc2024::day14 {

    /**
     * @brief The type of a field (see description of the riddle)
     * @param field
     * @return
     */
    enum class FieldType {
        MOVEABLE_STONE,
        FIXED_STONE,
        FREE,
    };

    /***
     * @brief The direction to tilt the field
     */
    enum class TiltDir {
        NORTH,
        EAST,
        SOUTH,
        WEST,
    };

    /***
     * @brief The field class (basically a x*y grid)
     */
    class Field {
    public:
        /***
         * @brief Parses a field from a vector of strings (line by line)
         * @param field
         * @return
         */
        static Field parse(const std::vector<std::string>& field);

        /***
         * @brief Checks if a stone can be moved in a direction
         * @param x
         * @param y
         * @param dir
         * @return
         */
        [[nodiscard]] inline bool can_move(size_t x, size_t y, TiltDir dir) const;

        /***
         * @brief Moves a stone in a direction
         *
         * @param x
         * @param y
         * @param dir
         * @param keep_moving recurse until the stone can't be moved anymore
         * @return
         */
        std::optional<std::pair<size_t, size_t>> move(size_t x, size_t y, TiltDir dir, bool keep_moving=false);

        /***
         * @brief Tilts the field in a direction
         *
         * @param dir
         */
        void tilt(TiltDir dir);

        Field(size_t width, size_t height) : m_width(width), m_height(height) {
            m_field = std::vector<FieldType>(width * height);
        }

        [[nodiscard]] inline FieldType get(size_t x, size_t y) const {
            return m_field[y * m_width + x];
        }

        inline void set(size_t x, size_t y, FieldType type) {
            m_field[y * m_width + x] = type;
        }

        [[nodiscard]] inline size_t width() const {
            return m_width;
        }

        [[nodiscard]] inline size_t height() const {
            return m_height;
        }

        std::string to_string() const {
            std::string s;
            for (size_t y = 0; y < m_height; ++y) {
                for (size_t x = 0; x < m_width; ++x) {
                    switch (get(x, y)) {
                        case FieldType::MOVEABLE_STONE:
                            s += "0";
                            break;
                        case FieldType::FIXED_STONE:
                            s += "#";
                            break;
                        case FieldType::FREE:
                            s += ".";
                            break;
                    }
                    // std::cout << " ";
                }
            }
            return s;
        }

        void print_field() {
            for (size_t y = 0; y < m_height; ++y) {
                for (size_t x = 0; x < m_width; ++x) {
                    switch (get(x, y)) {
                        case FieldType::MOVEABLE_STONE:
                            std::cout << "0";
                            break;
                        case FieldType::FIXED_STONE:
                            std::cout << "#";
                            break;
                        case FieldType::FREE:
                            std::cout << ".";
                            break;
                    }
                    // std::cout << " ";
                }
                std::cout << std::endl;
            }
        }

        /***
         * @brief Returns the sum of the field (see description of the riddle)
         *
         * @return
         */
        [[nodiscard]] std::size_t sum_field() const;

        /***
         * @brief Returns a hash of the field so we can remember if we already seen it
         *
         * @return
         */
        [[nodiscard]] std::string hash() const {
            return to_string();
        }
    private:
        std::vector<FieldType> m_field;
        size_t m_width = 0;
        size_t m_height = 0;
    };

    int day14_1(const std::vector<std::string>& input);
    int day14_2(const std::vector<std::string>& input);

}
#endif //AOC2024_DAY14_H
