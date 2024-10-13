#include <type_traits>
#include <functional>
#include <functional>
#include <catch2/catch_test_macros.hpp>
#include <glube/Attributes.hpp>
#include <glm/vec3.hpp>
#include <mj/lambda.hpp>
#include <mj/nameof.hpp>
#include "fakegl.hpp"

TEST_CASE("Attributes type decoding", "[attributes]")
{
    fakegl_init();
    glube::Program program;
    glube::Buffer buffer;
    glube::Attributes attributes;
    GLenum type_{};
    glVertexArrayAttribFormat = mj::capturing_lambda_pointer(
        [&]([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint attribindex, [[maybe_unused]] GLint size, [[maybe_unused]] GLenum type, [[maybe_unused]] GLboolean normalized, [[maybe_unused]] GLuint relativeoffset)
        {
            type_ = type;
        });

    struct Vector
    {
        glm::vec3 position;
        float normal[3];
        int color;
    };

    attributes.add(program, buffer, nameof(Vector::position), &Vector::position);
    REQUIRE(type_ == GL_FLOAT);
    attributes.add(program, buffer, nameof(Vector::normal), &Vector::normal);
    REQUIRE(type_ == GL_FLOAT);
    attributes.add(program, buffer, nameof(Vector::color), &Vector::color);
    REQUIRE(type_ == GL_INT);
}

TEST_CASE("Attributes size decoding", "[attributes]")
{
    fakegl_init();
    glube::Program program;
    glube::Buffer buffer;
    glube::Attributes attributes;
    GLint size_{};
    glVertexArrayAttribFormat = mj::capturing_lambda_pointer(
        [&]([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint attribindex, [[maybe_unused]] GLint size, [[maybe_unused]] GLenum type, [[maybe_unused]] GLboolean normalized, [[maybe_unused]] GLuint relativeoffset)
        {
            size_ = size;
        });

    struct Vector
    {
        glm::vec3 position;
        float normal[2];
        int color;
    };

    attributes.add(program, buffer, nameof(Vector::position), &Vector::position);
    REQUIRE(size_ == 3);
    attributes.add(program, buffer, nameof(Vector::normal), &Vector::normal);
    REQUIRE(size_ == 2);
    attributes.add(program, buffer, nameof(Vector::color), &Vector::color);
    REQUIRE(size_ == 1);
}

TEST_CASE("Attributes offset decoding", "[attributes]")
{
    fakegl_init();
    glube::Program program;
    glube::Buffer buffer;
    glube::Attributes attributes;
    GLuint offset_{};
    glVertexArrayAttribFormat = mj::capturing_lambda_pointer(
        [&]([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint attribindex, [[maybe_unused]] GLint size, [[maybe_unused]] GLenum type, [[maybe_unused]] GLboolean normalized, [[maybe_unused]] GLuint relativeoffset)
        {
            offset_ = relativeoffset;
        });

    struct Vector
    {
        glm::vec3 position;
        float normal[2];
        int color;
    };

    attributes.add(program, buffer, nameof(Vector::position), &Vector::position);
    REQUIRE(offset_ == 0);
    attributes.add(program, buffer, nameof(Vector::normal), &Vector::normal);
    REQUIRE(offset_ == 12);
    attributes.add(program, buffer, nameof(Vector::color), &Vector::color);
    REQUIRE(offset_ == 20);
}

TEST_CASE("Attributes stride decoding", "[attributes]")
{
    fakegl_init();
    glube::Program program;
    glube::Buffer buffer;
    glube::Attributes attributes;
    GLsizei stride_{};
    glVertexArrayVertexBuffer = mj::capturing_lambda_pointer(
        [&]([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint bindingindex, [[maybe_unused]] GLuint buffer, [[maybe_unused]] GLintptr offset, [[maybe_unused]] GLsizei stride)
        {
            stride_ = stride;
        });

    struct Vector
    {
        glm::vec3 position;
        float normal[2];
        int color;
    };

    attributes.add(program, buffer, nameof(Vector::position), &Vector::position);
    REQUIRE(stride_ == 24);
    attributes.add(program, buffer, nameof(Vector::normal), &Vector::normal);
    REQUIRE(stride_ == 24);
}