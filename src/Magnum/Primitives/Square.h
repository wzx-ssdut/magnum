#ifndef Magnum_Primitives_Square_h
#define Magnum_Primitives_Square_h
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

/** @file
 * @brief Class @ref Magnum::Primitives::Square
 */

#include "Magnum/Primitives/visibility.h"
#include "Magnum/Trade/Trade.h"

namespace Magnum { namespace Primitives {

/**
@brief 2D square primitive

2x2 square.
@see @ref Plane
*/
class MAGNUM_PRIMITIVES_EXPORT Square {
    public:
        /** @brief Whether to generate texture coordinates */
        enum class TextureCoords: UnsignedByte {
            DontGenerate,    /**< Don't generate texture coordinates */

            /** Generate texture coordinates with origin in bottom left corner. */
            Generate
        };

        /**
         * @brief Solid square
         *
         * Non-indexed @ref MeshPrimitive::TriangleStrip.
         */
        static Trade::MeshData2D solid(TextureCoords textureCoords = TextureCoords::DontGenerate);

        /**
         * @brief Wireframe square
         *
         * Non-indexed @ref MeshPrimitive::LineLoop.
         */
        static Trade::MeshData2D wireframe();

        Square() = delete;
};

}}

#endif
