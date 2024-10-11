#pragma once

#ifndef GLUBE_NO_GL
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#endif

namespace glube
{

    enum class Key
    {
        Escape = GLFW_KEY_ESCAPE,
    };

    enum class KeyMod
    {

    };

    enum class KeyAction
    {
        Pressed = GLFW_PRESS,
    };

    struct KeyEvent
    {
        class Window *Context;
        Key Key;
        KeyMod KeyMod;
        KeyAction KeyAction;
        int Scancode;
    };

    using KeyEventHandler = void(KeyEvent);

} // End namespace glube.