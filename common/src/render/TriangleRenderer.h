/*
 Copyright (C) 2010 Kristian Duske

 This file is part of TrenchBroom.

 TrenchBroom is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 TrenchBroom is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with TrenchBroom. If not, see <http://www.gnu.org/licenses/>.
 */

#pragma once

#include "Color.h"
#include "render/IndexRangeMap.h"
#include "render/Renderable.h"
#include "render/VertexArray.h"

namespace tb::render
{
class RenderContext;

class TriangleRenderer : public DirectRenderable
{
private:
  VertexArray m_vertexArray;
  IndexRangeMap m_indexArray;

  Color m_color;
  bool m_useColor = false;
  Color m_tintColor;
  bool m_applyTinting = false;

public:
  TriangleRenderer();
  TriangleRenderer(VertexArray vertexArray, IndexRangeMap indexArray);
  TriangleRenderer(VertexArray vertexArray, PrimType primType);

  TriangleRenderer(const TriangleRenderer& other) = default;
  TriangleRenderer& operator=(const TriangleRenderer& other) = default;

  void setUseColor(bool useColor);
  void setColor(const Color& color);
  void setApplyTinting(bool applyTinting);
  void setTintColor(const Color& tintColor);

private:
  void doPrepareVertices(VboManager& vboManager) override;
  void doRender(RenderContext& context) override;
};

} // namespace tb::render
