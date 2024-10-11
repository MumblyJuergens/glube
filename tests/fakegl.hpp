#include <glad/gl.h>

void fakeglCreateBuffers([[maybe_unused]] GLsizei n, [[maybe_unused]] GLuint *buffers) {}
void fakeglDeleteBuffers([[maybe_unused]] GLsizei n, [[maybe_unused]] const GLuint *buffers) {}
void fakeglNamedBufferStorage([[maybe_unused]] GLuint buffer, [[maybe_unused]] GLsizeiptr size, [[maybe_unused]] const void *data, [[maybe_unused]] GLbitfield flags) {}
void fakeglNamedBufferSubData([[maybe_unused]] GLuint buffer, [[maybe_unused]] GLintptr offset, [[maybe_unused]] GLsizeiptr size, [[maybe_unused]] const void *data) {}

void fakeglAttachShader([[maybe_unused]] GLuint program, [[maybe_unused]] GLuint shader) {}
void fakeglDetachShader([[maybe_unused]] GLuint program, [[maybe_unused]] GLuint shader) {}
GLuint fakeglCreateProgram() { return 0; }
void fakeglDeleteProgram([[maybe_unused]] GLuint program) {}
void fakeglUseProgram([[maybe_unused]] GLuint program) {}
void fakeglLinkProgram([[maybe_unused]] GLuint program) {}
void fakeglGetProgramiv([[maybe_unused]] GLuint program, [[maybe_unused]] GLenum pname, [[maybe_unused]] GLint *params) {}
void fakeglGetProgramInfoLog([[maybe_unused]] GLuint program, [[maybe_unused]] GLsizei bufSize, [[maybe_unused]] GLsizei *length, [[maybe_unused]] GLchar *infoLog) {}

GLuint fakeglCreateShader([[maybe_unused]] GLenum type) { return 0; }
void fakeglDeleteShader([[maybe_unused]] GLuint shader) {}
void fakeglShaderSource([[maybe_unused]] GLuint shader, [[maybe_unused]] GLsizei count, [[maybe_unused]] const GLchar *const *string, [[maybe_unused]] const GLint *length) {}
void fakeglCompileShader([[maybe_unused]] GLuint shader) {}
void fakeglGetShaderiv([[maybe_unused]] GLuint shader, [[maybe_unused]] GLenum pname, [[maybe_unused]] GLint *params) {}
void fakeglGetShaderInfoLog([[maybe_unused]] GLuint shader, [[maybe_unused]] GLsizei bufSize, [[maybe_unused]] GLsizei *length, [[maybe_unused]] GLchar *infoLog) {}

GLint fakeglGetAttribLocation([[maybe_unused]] GLuint program, [[maybe_unused]] const GLchar *name) { return 0; }
void fakeglVertexArrayVertexBuffer([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint bindingindex, [[maybe_unused]] GLuint buffer, [[maybe_unused]] GLintptr offset, [[maybe_unused]] GLsizei stride) {}
void fakeglEnableVertexArrayAttrib([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint index) {}
void fakeglVertexArrayAttribFormat([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint attribindex, [[maybe_unused]] GLint size, [[maybe_unused]] GLenum type, [[maybe_unused]] GLboolean normalized, [[maybe_unused]] GLuint relativeoffset) {}
void fakeglVertexArrayAttribBinding([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint attribindex, [[maybe_unused]] GLuint bindingindex) {}
void fakeglVertexArrayBindingDivisor([[maybe_unused]] GLuint vaobj, [[maybe_unused]] GLuint bindingindex, [[maybe_unused]] GLuint divisor) {}

void fakeglCreateVertexArrays([[maybe_unused]] GLsizei n, [[maybe_unused]] GLuint *arrays) {}
void fakeglDeleteVertexArrays([[maybe_unused]] GLsizei n, [[maybe_unused]] const GLuint *arrays) {}

void fakegl_init()
{
    glCreateBuffers = fakeglCreateBuffers;
    glDeleteBuffers = fakeglDeleteBuffers;
    glNamedBufferStorage = fakeglNamedBufferStorage;
    glNamedBufferSubData = fakeglNamedBufferSubData;

    glAttachShader = fakeglAttachShader;
    glDetachShader = fakeglDetachShader;
    glCreateProgram = fakeglCreateProgram;
    glDeleteProgram = fakeglDeleteProgram;
    glUseProgram = fakeglUseProgram;
    glLinkProgram = fakeglLinkProgram;
    glGetProgramiv = fakeglGetProgramiv;
    glGetProgramInfoLog = fakeglGetProgramInfoLog;

    glCreateShader = fakeglCreateShader;
    glDeleteShader = fakeglDeleteShader;
    glShaderSource = fakeglShaderSource;
    glCompileShader = fakeglCompileShader;
    glGetShaderiv = fakeglGetShaderiv;
    glGetShaderInfoLog = fakeglGetShaderInfoLog;

    glCreateVertexArrays = fakeglCreateVertexArrays;
    glDeleteVertexArrays = fakeglDeleteVertexArrays;
    glGetAttribLocation = fakeglGetAttribLocation;
    glVertexArrayVertexBuffer = fakeglVertexArrayVertexBuffer;
    glEnableVertexArrayAttrib = fakeglEnableVertexArrayAttrib;
    glVertexArrayAttribFormat = fakeglVertexArrayAttribFormat;
    glVertexArrayAttribBinding = fakeglVertexArrayAttribBinding;
    glVertexArrayBindingDivisor = fakeglVertexArrayBindingDivisor;
}