#include "Window.h"
#include "../../Headers/Libraries_Include.h"

namespace QwerkE {

    Window::Window(int windowWidth, int windowHeight, const char* windowTitle)
        : m_Resolution(windowWidth, windowHeight), m_WindowTitle(windowTitle)
    {
    }

    Window::~Window()
    {
    }

}
