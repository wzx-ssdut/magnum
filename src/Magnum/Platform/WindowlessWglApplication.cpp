/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "WindowlessWglApplication.h"

#include <Corrade/Utility/Assert.h>
#include <Corrade/Utility/Debug.h>

#include "Magnum/Platform/Context.h"

namespace Magnum { namespace Platform {

#ifndef DOXYGEN_GENERATING_OUTPUT
int WindowlessWglApplication::create(LRESULT(CALLBACK windowProcedure)(HWND, UINT, WPARAM, LPARAM)) {
    const WNDCLASS wc{
        0,
        windowProcedure,
        0,
        0,
        GetModuleHandle(nullptr),
        nullptr,
        nullptr,
        HBRUSH(COLOR_BACKGROUND),
        nullptr,
        L"Magnum Windowless Application"
    };
    if(!RegisterClass(&wc)) return 1;

    CreateWindowW(wc.lpszClassName, L"Magnum Windowless Application",
        WS_OVERLAPPEDWINDOW, 0, 0, 32, 32, 0, 0, wc.hInstance, 0);

    return 0;
}
#endif

#ifndef DOXYGEN_GENERATING_OUTPUT
WindowlessWglApplication::WindowlessWglApplication(const Arguments& arguments): WindowlessWglApplication{arguments, Configuration{}} {}
#endif

WindowlessWglApplication::WindowlessWglApplication(const Arguments& arguments, const Configuration& configuration): WindowlessWglApplication{arguments, nullptr} {
    createContext(configuration);
}

WindowlessWglApplication::WindowlessWglApplication(const Arguments& arguments, std::nullptr_t): _window(arguments.window) {}

void WindowlessWglApplication::createContext() { createContext({}); }

void WindowlessWglApplication::createContext(const Configuration& configuration) {
    if(!tryCreateContext(configuration)) std::exit(1);
}

bool WindowlessWglApplication::tryCreateContext(const Configuration&) {
    CORRADE_ASSERT(!_context, "Platform::WindowlessWglApplication::tryCreateContext(): context already created", false);

    /* Get device context */
    _deviceContext = GetDC(_window);

    /* Use first provided pixel format  */
    constexpr static const PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        /* Double-buffered with OpenGL support */
        PFD_DRAW_TO_WINDOW|PFD_SUPPORT_OPENGL|PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,          /* RGBA */
        32,                     /* 32 bit */
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,                     /* 24-bit depth buffer */
        8,                      /* 8-bit stencil buffer */
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };
    const int pixelFormat = ChoosePixelFormat(_deviceContext, &pfd);
    SetPixelFormat(_deviceContext, pixelFormat, &pfd);

    /* Create context and make it current */
    _renderingContext = wglCreateContext(_deviceContext);
    if(!_renderingContext) {
        Error() << "Platform::WindowlessWglApplication::tryCreateContext(): cannot create context:" << GetLastError();
        return false;
    }

    /* Set OpenGL context as current */
    if(!wglMakeCurrent(_deviceContext, _renderingContext)) {
        Error() << "Platform::WindowlessWglApplication::tryCreateContext(): cannot make context current:" << GetLastError();
        return false;
    }

    _context.reset(new Platform::Context);
    return true;
}

WindowlessWglApplication::~WindowlessWglApplication() {
    _context.reset();

    wglMakeCurrent(_deviceContext, nullptr);
    wglDeleteContext(_renderingContext);
}

}}
