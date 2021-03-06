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

#include "AxisAlignedBox.h"

#include "Magnum/Math/Matrix3.h"
#include "Magnum/Math/Matrix4.h"
#include "Magnum/Shapes/Point.h"

namespace Magnum { namespace Shapes {

template<UnsignedInt dimensions> AxisAlignedBox<dimensions> AxisAlignedBox<dimensions>::transformed(const MatrixTypeFor<dimensions, Float>& matrix) const {
    return AxisAlignedBox<dimensions>(matrix.transformPoint(_min),
                                      matrix.transformPoint(_max));
}

template<UnsignedInt dimensions> bool AxisAlignedBox<dimensions>::operator%(const Point<dimensions>& other) const {
    return (other.position() >= _min).all() &&
           (other.position() < _max).all();
}

#ifndef DOXYGEN_GENERATING_OUTPUT
template class MAGNUM_SHAPES_EXPORT AxisAlignedBox<2>;
template class MAGNUM_SHAPES_EXPORT AxisAlignedBox<3>;
#endif

}}
