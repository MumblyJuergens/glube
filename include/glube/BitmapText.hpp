#pragma once

#include <filesystem>
#include <fstream>
#include <limits>
#include <ranges>
#include <string_view>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <gsl/narrow>
#include <mj/size.hpp>
#include <mj/nameof.hpp>
#include "glube/Texture.hpp"
#include "glube/AutoBuffer.hpp"

namespace glube
{

    class [[nodiscard]] BitmapText final
    {
        struct Vertex
        {
            glm::vec2 position;
            glm::vec2 texture_coordinates;
        };

        enum BPP : std::uint8_t
        {
            greyscale = 8,
            rbg = 24,
            rbga = 32,
        };

        struct Header
        {
            // int width{};
            // int height{};
            glm::ivec2 size{};
            // int cell_width{};
            // int cell_height{};
            glm::ivec2 cell_size{};
            BPP bits_per_pixel{};
            std::uint8_t base{};

            [[nodiscard]] int pixels() const noexcept { return size.x * size.y; }
            [[nodiscard]] int bytes_per_pixel() const noexcept { return bits_per_pixel / 8; }
            [[nodiscard]] int image_bytes() const noexcept { return pixels() * bytes_per_pixel(); }
        };
        static_assert(sizeof(Header) == 20);


        static constexpr int sc_width_data_offset = 20;
        static constexpr int sc_width_data_length = 256;
        static constexpr int sc_map_data_offset = 276;
        static_assert(sc_map_data_offset == sizeof(Header) + sc_width_data_length);

        // Read from file.
        Header m_header;
        std::vector<std::uint8_t> m_letter_widths;

        // Calculated from file.
        int m_cells_per_row{};
        glm::vec2 m_cell_size_factor{};

        // State.
        glm::ivec2 m_cursor{};
        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_indices;

        Texture m_font_texture;
        Program m_program;
        AutoBuffer m_vbo;
        AutoBuffer m_ibo;
        Attributes m_vao;

        // For restoring state after text (if desired).
        bool m_globals_blend{};
        GLint m_globals_srcrbg{};
        GLint m_globals_dstrbg{};
        GLint m_globals_srcalpha{};
        GLint m_globals_dstalpha{};

        public:

        [[nodiscard]] bool load_bbf(const std::string_view fileName) noexcept
        {
            std::error_code ec;
            const auto fileSize = std::filesystem::file_size(fileName, ec);
            if (ec) return false;

            if (fileSize < sizeof(Header) + 2) return false;

            std::basic_ifstream<std::uint8_t> in{ fileName.data(), std::ios::binary };
            if (!in) return false;

            if (in.get() != 0xBF || in.get() != 0xF2) return false;
            in.read(reinterpret_cast<std::uint8_t *>(&m_header), sizeof(Header));
            if (gsl::narrow<int>(fileSize) != sc_map_data_offset + m_header.image_bytes()) return false;

            m_cells_per_row = m_header.size.x / m_header.cell_size.x;
            m_cell_size_factor = {
                static_cast<float>(m_header.cell_size.x) / static_cast<float>(m_header.size.x),
                static_cast<float>(m_header.cell_size.y) / static_cast<float>(m_header.size.y),
            };

            m_letter_widths.resize(sc_width_data_length);
            in.read(m_letter_widths.data(), sc_width_data_length);

            std::vector<std::uint8_t> image(gsl::narrow<std::size_t>(m_header.image_bytes()), 0);
            in.read(image.data(), m_header.image_bytes());

            const TextureConfig textureConfig = [](const BPP bpp) {
                switch (bpp) {
                case BPP::greyscale: return glube::textureconfigs::D2R8;
                case BPP::rbg: return glube::textureconfigs::D2RGB8;
                case BPP::rbga: return  glube::textureconfigs::D2RGBA8;
                }
                }(m_header.bits_per_pixel);

            m_font_texture.init(textureConfig, m_header.size, image);
            // glTextureParameteri(*m_font_texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            // glTextureParameteri(*m_font_texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            // glTextureParameteri(*m_font_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            // glTextureParameteri(*m_font_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


            Shader vert(ShaderType::vertex,
                R"glsl(#version 460 core
                in vec2 position;
                in vec2 texture_coordinates;
                out vec2 v_texCoords;
                uniform mat4 projection;
                void main() {
                    gl_Position = projection * vec4(position, 0.0, 1.0);
                    v_texCoords = texture_coordinates;
                }
                )glsl");

            Shader frag(ShaderType::fragment,
                R"glsl(#version 460 core
                in vec2 v_texCoords;
                out vec4 o_color;
                uniform vec3 color;
                uniform sampler2D font_texture;
                void main() {
                    o_color = vec4(color, texture(font_texture, v_texCoords).r);
                }
                )glsl");

            m_program.add_shaders(vert, frag);

            m_vao.add(m_program, m_vbo.buffer(), nameof(&Vertex::position), &Vertex::position);
            m_vao.add(m_program, m_vbo.buffer(), nameof(&Vertex::texture_coordinates), &Vertex::texture_coordinates);
            m_vao.set_element_buffer(m_ibo.buffer());

            return true;
        }

        void set_cursor(const glm::ivec2 value) noexcept { m_cursor = value; }
        void set_cursor(const int x, const int y) noexcept { m_cursor = { x, y }; }

        void set_color(const glm::vec3 value) noexcept { m_program.set_uniform("color", value); }
        void set_texture_unit(const int textureUnit)
        {
            glBindTextureUnit(textureUnit, *m_font_texture);
            m_program.set_uniform("font_texture", textureUnit);
        }
        void set_window_size(const glm::ivec2 &windowSize)
        {
            const auto projection = glm::ortho(0.0f, static_cast<float>(windowSize.x), 0.0f, static_cast<float>(windowSize.y), -1.0f, 1.0f);
            m_program.set_uniform("projection", projection);
        }

        void prepare()
        {
            m_vertices.clear();
            m_indices.clear();
            m_globals_blend = glIsEnabled(GL_BLEND);
            glGetIntegerv(GL_BLEND_SRC_RGB, &m_globals_srcrbg);
            glGetIntegerv(GL_BLEND_DST_RGB, &m_globals_dstrbg);
            glGetIntegerv(GL_BLEND_SRC_ALPHA, &m_globals_srcalpha);
            glGetIntegerv(GL_BLEND_DST_ALPHA, &m_globals_dstalpha);
            switch (m_header.bits_per_pixel)
            {
            case BPP::greyscale: glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND); break;
            case BPP::rbg: glDisable(GL_BLEND); break;
            case BPP::rbga: glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA); glEnable(GL_BLEND); break;
            }
        }

        void restore_globals()
        {
            if (m_globals_blend) glEnable(GL_BLEND);
            else glDisable(GL_BLEND);
            glBlendFuncSeparate(m_globals_srcrbg, m_globals_dstrbg, m_globals_srcalpha, m_globals_dstalpha);
        }

        void print(const std::string_view text) noexcept
        {
            unsigned int ci{};
            auto cursor = m_cursor;
            for (const auto letter : text)
            {
                const int letterValue = letter - m_header.base;
                const glm::vec2 letterTexLoc{ letterValue % m_cells_per_row, letterValue / m_cells_per_row };
                const glm::vec2 uv = letterTexLoc * m_cell_size_factor;

                const glm::vec2
                    posBottomLeft{ cursor },
                    posBottomRight{ cursor.x + m_header.cell_size.x, cursor.y },
                    posTopRight{ cursor + m_header.cell_size },
                    posTopLeft{ cursor.x, cursor.y + m_header.cell_size.y };

                const glm::vec2
                    uvBottomLeft{ uv },
                    uvBottomRight{ uv.x + m_cell_size_factor.x, uv.y },
                    uvTopRight{ uv + m_cell_size_factor },
                    uvTopLeft{ uv.x, uv.y + m_cell_size_factor.y };

                const std::array<Vertex, 4> quadv{
                    Vertex{.position = posTopLeft, .texture_coordinates = uvBottomLeft },
                    Vertex{.position = posBottomLeft, .texture_coordinates = uvTopLeft },
                    Vertex{.position = posBottomRight, .texture_coordinates = uvTopRight },
                    Vertex{.position = posTopRight, .texture_coordinates = uvBottomRight },
                };

                const std::array<unsigned int, 6> quadi{
                    ci + 0, ci + 1, ci + 2,
                    ci + 0, ci + 2, ci + 3,
                };
                ci += 4;

                m_vertices.append_range(quadv);
                m_indices.append_range(quadi);

                cursor.x += m_letter_widths.at(mj::sz_t(letter));
            }

            m_vbo.set(m_vertices.size() * sizeof(Vertex), m_vertices.data());
            m_ibo.set(m_indices.size() * sizeof(unsigned int), m_indices.data());

            m_program.activate();
            m_vao.activate();
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
        }
    };

} // End namsepace glube.