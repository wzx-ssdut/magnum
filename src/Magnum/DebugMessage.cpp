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

#include "DebugMessage.h"

#include <Corrade/Utility/Assert.h>

#include "Magnum/Context.h"
#include "Magnum/Extensions.h"
#include "Magnum/Implementation/State.h"
#include "Magnum/Implementation/DebugState.h"

namespace Magnum {

namespace {

#if !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
void
#ifdef CORRADE_TARGET_WINDOWS
APIENTRY
#endif
callbackWrapper(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam) {
    Context::current()->state().debug->messageCallback(DebugMessage::Source(source), DebugMessage::Type(type), id, DebugMessage::Severity(severity), std::string(message, length), userParam);
}
#endif

void defaultCallback(const DebugMessage::Source source, const DebugMessage::Type type, const UnsignedInt id, const DebugMessage::Severity severity, const std::string& string, const void*) {
    switch(severity) {
        case DebugMessage::Severity::High:
            Error() << source << type << id << severity << "\n   " << string;
            break;

        case DebugMessage::Severity::Medium:
        case DebugMessage::Severity::Low:
            Warning() << source << type << id << severity << "\n   " << string;
            break;

        default: Debug() << source << type << id << severity << "\n   " << string;
    }
}

}

Int DebugMessage::maxLoggedMessages() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::KHR::debug>())
        return 0;

    GLint& value = Context::current()->state().debug->maxLoggedMessages;

    if(value == 0) {
        #ifndef MAGNUM_TARGET_GLES
        glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES, &value);
        #else
        glGetIntegerv(GL_MAX_DEBUG_LOGGED_MESSAGES_KHR, &value);
        #endif
    }

    return value;
}

Int DebugMessage::maxMessageLength() {
    if(!Context::current()->isExtensionSupported<Extensions::GL::KHR::debug>())
        return 0;

    GLint& value = Context::current()->state().debug->maxMessageLength;

    if(value == 0) {
        #ifndef MAGNUM_TARGET_GLES
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH, &value);
        #else
        glGetIntegerv(GL_MAX_DEBUG_MESSAGE_LENGTH_KHR, &value);
        #endif
    }

    return value;
}

void DebugMessage::setCallback(const Callback callback, const void* userParam) {
    Context::current()->state().debug->messageCallbackImplementation(callback, userParam);
}

void DebugMessage::setDefaultCallback() {
    setCallback(defaultCallback, nullptr);
}

void DebugMessage::insertInternal(const Source source, const Type type, const UnsignedInt id, const Severity severity, const Containers::ArrayReference<const char> string) {
    Context::current()->state().debug->messageInsertImplementation(source, type, id, severity, string);
}

void DebugMessage::insertImplementationNoOp(Source, Type, UnsignedInt, Severity, const Containers::ArrayReference<const char>) {}

void DebugMessage::insertImplementationKhr(const Source source, const Type type, const UnsignedInt id, const Severity severity, const Containers::ArrayReference<const char> string) {
    #ifndef MAGNUM_TARGET_GLES
    glDebugMessageInsert(GLenum(source), GLenum(type), id, GLenum(severity), string.size(), string.data());
    #elif !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
    glDebugMessageInsertKHR(GLenum(source), GLenum(type), id, GLenum(severity), string.size(), string.data());
    #else
    static_cast<void>(source);
    static_cast<void>(type);
    static_cast<void>(id);
    static_cast<void>(severity);
    static_cast<void>(string);
    CORRADE_ASSERT_UNREACHABLE();
    #endif
}

void DebugMessage::insertImplementationExt(Source, Type, UnsignedInt, Severity, const Containers::ArrayReference<const char> string) {
    #if !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
    glInsertEventMarkerEXT(string.size(), string.data());
    #else
    static_cast<void>(string);
    CORRADE_ASSERT_UNREACHABLE();
    #endif
}

#ifndef MAGNUM_TARGET_GLES
void DebugMessage::insertImplementationGremedy(Source, Type, UnsignedInt, Severity, const Containers::ArrayReference<const char> string) {
    glStringMarkerGREMEDY(string.size(), string.data());
}
#endif

void DebugMessage::setEnabledInternal(const GLenum source, const GLenum type, const GLenum severity, const std::initializer_list<UnsignedInt> ids, const bool enabled) {
    Context::current()->state().debug->messageControlImplementation(source, type, severity, ids, enabled);
}

void DebugMessage::controlImplementationNoOp(GLenum, GLenum, GLenum, std::initializer_list<UnsignedInt>, bool) {}

void DebugMessage::controlImplementationKhr(const GLenum source, const GLenum type, const GLenum severity, const std::initializer_list<UnsignedInt> ids, const bool enabled) {
    #ifndef MAGNUM_TARGET_GLES
    glDebugMessageControl(source, type, severity, ids.size(), ids.begin(), enabled);
    #elif !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
    glDebugMessageControlKHR(source, type, severity, ids.size(), ids.begin(), enabled);
    #else
    static_cast<void>(source);
    static_cast<void>(type);
    static_cast<void>(severity);
    static_cast<void>(ids);
    static_cast<void>(enabled);
    CORRADE_ASSERT_UNREACHABLE();
    #endif
}

void DebugMessage::callbackImplementationNoOp(Callback, const void*) {}

void DebugMessage::callbackImplementationKhr(const Callback callback, const void* userParam) {
    /* Replace the callback */
    const Callback original = Context::current()->state().debug->messageCallback;
    Context::current()->state().debug->messageCallback = callback;

    /* Adding callback */
    if(!original && callback) {
        #ifndef MAGNUM_TARGET_GLES
        glDebugMessageCallback(callbackWrapper, userParam);
        #elif !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
        glDebugMessageCallbackKHR(callbackWrapper, userParam);
        #else
        static_cast<void>(userParam);
        CORRADE_ASSERT_UNREACHABLE();
        #endif

    /* Deleting callback */
    } else if(original && !callback) {
        #ifndef MAGNUM_TARGET_GLES
        glDebugMessageCallback(nullptr, nullptr);
        #elif !defined(CORRADE_TARGET_EMSCRIPTEN) && !defined(CORRADE_TARGET_NACL)
        glDebugMessageCallbackKHR(nullptr, nullptr);
        #else
        CORRADE_ASSERT_UNREACHABLE();
        #endif
    }
}

#ifndef DOXYGEN_GENERATING_OUTPUT
Debug operator<<(Debug debug, const DebugMessage::Source value) {
    switch(value) {
        #define _c(value) case DebugMessage::Source::value: return debug << "DebugMessage::Source::" #value;
        _c(Api)
        _c(WindowSystem)
        _c(ShaderCompiler)
        _c(ThirdParty)
        _c(Application)
        _c(Other)
        #undef _c
    }

    return debug << "DebugMessage::Source::(invalid)";
}

Debug operator<<(Debug debug, const DebugMessage::Type value) {
    switch(value) {
        #define _c(value) case DebugMessage::Type::value: return debug << "DebugMessage::Type::" #value;
        _c(Error)
        _c(DeprecatedBehavior)
        _c(UndefinedBehavior)
        _c(Portability)
        _c(Performance)
        _c(Other)
        _c(Marker)
        #undef _c
    }

    return debug << "DebugMessage::Type::(invalid)";
}

Debug operator<<(Debug debug, const DebugMessage::Severity value) {
    switch(value) {
        #define _c(value) case DebugMessage::Severity::value: return debug << "DebugMessage::Severity::" #value;
        _c(High)
        _c(Medium)
        _c(Low)
        _c(Notification)
        #undef _c
    }

    return debug << "DebugMessage::Severity::(invalid)";
}
#endif

}
