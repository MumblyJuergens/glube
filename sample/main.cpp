#include <print>
#include <glube/glube.hpp>

static void key_callback(glube::KeyEvent event)
{
    if (event.key == glube::Key::escape && event.action == glube::KeyAction::pressed)
    {
        event.window->set_should_close(true);
    }
}

int main()
{
    glube::Window window(1280, 720, "Glube Sample");
    window.set_key_event_handler(key_callback);

    glube::BitmapText writer;
    if (!writer.load_bbf("sample/Roboto.bff"))
    {
        std::println("Could not load bitmap font file");
        return 1;
    }
    writer.set_cursor(50, 50);
    writer.set_texture_unit(0);
    writer.set_window_size({ 1280, 720 });
    writer.set_color({ 1.0f, 0.0f, 0.0f });
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

    while (!window.should_close())
    {
        glClear(GL_COLOR_BUFFER_BIT);

        writer.prepare();
        writer.print("12.45790");
        writer.restore_globals();

        window.poll_events();
        window.swap_buffers();
    }
}