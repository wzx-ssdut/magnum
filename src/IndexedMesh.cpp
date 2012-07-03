/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "IndexedMesh.h"

namespace Magnum {

void IndexedMesh::draw() {
    bind();

    /* Finalize the mesh, if it is not already */
    finalize();

    /** @todo Start at given index */
    glDrawElements(static_cast<GLenum>(primitive()), _indexCount, static_cast<GLenum>(_indexType), nullptr);

    unbind();
}

#ifndef DOXYGEN_GENERATING_OUTPUT
void IndexedMesh::finalize() {
    if(isFinalized()) return;

    CORRADE_ASSERT(_indexCount, "IndexedMesh: the mesh has zero index count!", )

    /* Finalize attribute positions */
    Mesh::finalize();

    /* Bind index buffer */
    _indexBuffer.bind();
}
#endif

}
