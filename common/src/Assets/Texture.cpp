/*
 Copyright (C) 2010-2017 Kristian Duske

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

#include "Texture.h"
#include "Assets/TextureBuffer.h"
#include "Assets/TextureCollection.h"
#include "Macros.h"
#include "Renderer/GL.h"

#include <vecmath/vec_io.h>

#include <kdl/overload.h>
#include <kdl/reflection_impl.h>

#include <algorithm> // for std::max
#include <cassert>
#include <ostream>

namespace TrenchBroom::Assets
{

std::ostream& operator<<(std::ostream& lhs, const TextureType& rhs)
{
  switch (rhs)
  {
  case TextureType::Opaque:
    lhs << "Opaque";
    break;
  case TextureType::Masked:
    lhs << "Masked";
    break;
    switchDefault();
  }
  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const TextureCulling& rhs)
{
  switch (rhs)
  {
  case TextureCulling::Default:
    lhs << "Default";
    break;
  case TextureCulling::None:
    lhs << "None";
    break;
  case TextureCulling::Front:
    lhs << "Front";
    break;
  case TextureCulling::Back:
    lhs << "Back";
    break;
  case TextureCulling::Both:
    lhs << "Both";
    break;
    switchDefault();
  }
  return lhs;
}

kdl_reflect_impl(TextureBlendFunc);

std::ostream& operator<<(std::ostream& lhs, const TextureBlendFunc::Enable& rhs)
{
  switch (rhs)
  {
  case TextureBlendFunc::Enable::UseDefault:
    lhs << "UseDefault";
    break;
  case TextureBlendFunc::Enable::UseFactors:
    lhs << "UseFactors";
    break;
  case TextureBlendFunc::Enable::DisableBlend:
    lhs << "DisableBlend";
    break;
    switchDefault();
  }
  return lhs;
}

std::ostream& operator<<(std::ostream& lhs, const GameData& rhs)
{
  std::visit(
    kdl::overload(
      [&](const std::monostate&) { lhs << "std::monostate"; },
      [&](const auto& x) { lhs << x; }),
    rhs);
  return lhs;
}

kdl_reflect_impl(Q2Data);

kdl_reflect_impl(Texture);

Texture::Texture(
  std::string name,
  const size_t width,
  const size_t height,
  const Color& averageColor,
  Buffer&& buffer,
  const GLenum format,
  const TextureType type,
  GameData gameData)
  : m_name{std::move(name)}
  , m_width{width}
  , m_height{height}
  , m_averageColor{averageColor}
  , m_usageCount{0u}
  , m_overridden{false}
  , m_format{format}
  , m_type{type}
  , m_culling{TextureCulling::Default}
  , m_blendFunc{TextureBlendFunc::Enable::UseDefault, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
  , m_textureId{0}
  , m_gameData{std::move(gameData)}
{
  assert(m_width > 0);
  assert(m_height > 0);
  assert(buffer.size() >= m_width * m_height * bytesPerPixelForFormat(format));
  m_buffers.push_back(std::move(buffer));
}

Texture::Texture(
  std::string name,
  const size_t width,
  const size_t height,
  const Color& averageColor,
  BufferList buffers,
  const GLenum format,
  const TextureType type,
  GameData gameData)
  : m_name{std::move(name)}
  , m_width{width}
  , m_height{height}
  , m_averageColor{averageColor}
  , m_usageCount{0u}
  , m_overridden{false}
  , m_format{format}
  , m_type{type}
  , m_culling{TextureCulling::Default}
  , m_blendFunc{TextureBlendFunc::Enable::UseDefault, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
  , m_textureId(0)
  , m_buffers{std::move(buffers)}
  , m_gameData{std::move(gameData)}
{
  assert(m_width > 0);
  assert(m_height > 0);

  const auto compressed = isCompressedFormat(format);
  [[maybe_unused]] const auto bytesPerPixel =
    compressed ? 0U : bytesPerPixelForFormat(format);
  [[maybe_unused]] const auto blockSize = compressed ? blockSizeForFormat(format) : 0U;

  for (size_t level = 0; level < m_buffers.size(); ++level)
  {
    [[maybe_unused]] const auto mipSize = sizeAtMipLevel(m_width, m_height, level);
    [[maybe_unused]] const auto numBytes =
      compressed ? (
        blockSize * std::max(size_t(1), mipSize.x() / 4)
        * std::max(size_t(1), mipSize.y() / 4))
                 : (bytesPerPixel * mipSize.x() * mipSize.y());
    assert(m_buffers[level].size() >= numBytes);
  }
}

Texture::Texture(
  std::string name,
  const size_t width,
  const size_t height,
  const GLenum format,
  const TextureType type,
  GameData gameData)
  : m_name{std::move(name)}
  , m_width{width}
  , m_height{height}
  , m_averageColor(Color(0.0f, 0.0f, 0.0f, 1.0f))
  , m_usageCount{0u}
  , m_overridden{false}
  , m_format{format}
  , m_type{type}
  , m_culling{TextureCulling::Default}
  , m_blendFunc{TextureBlendFunc::Enable::UseDefault, GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA}
  , m_textureId{0}
  , m_gameData{std::move(gameData)}
{
}

Texture::~Texture() = default;

Texture::Texture(Texture&& other)
  : m_name{std::move(other.m_name)}
  , m_absolutePath{std::move(other.m_absolutePath)}
  , m_relativePath{std::move(other.m_relativePath)}
  , m_width{std::move(other.m_width)}
  , m_height{std::move(other.m_height)}
  , m_averageColor{std::move(other.m_averageColor)}
  , m_usageCount{static_cast<size_t>(other.m_usageCount)}
  , m_overridden{std::move(other.m_overridden)}
  , m_format{std::move(other.m_format)}
  , m_type{std::move(other.m_type)}
  , m_surfaceParms{std::move(other.m_surfaceParms)}
  , m_culling{std::move(other.m_culling)}
  , m_blendFunc{std::move(other.m_blendFunc)}
  , m_textureId{std::move(other.m_textureId)}
  , m_buffers{std::move(other.m_buffers)}
  , m_gameData{std::move(other.m_gameData)}
{
}

Texture& Texture::operator=(Texture&& other)
{
  m_name = std::move(other.m_name);
  m_absolutePath = std::move(other.m_absolutePath);
  m_relativePath = std::move(other.m_relativePath);
  m_width = std::move(other.m_width);
  m_height = std::move(other.m_height);
  m_averageColor = std::move(other.m_averageColor);
  m_usageCount = static_cast<size_t>(other.m_usageCount);
  m_overridden = std::move(other.m_overridden);
  m_format = std::move(other.m_format);
  m_type = std::move(other.m_type);
  m_surfaceParms = std::move(other.m_surfaceParms);
  m_culling = std::move(other.m_culling);
  m_blendFunc = std::move(other.m_blendFunc);
  m_textureId = std::move(other.m_textureId);
  m_buffers = std::move(other.m_buffers);
  m_gameData = std::move(other.m_gameData);
  return *this;
}

TextureType Texture::selectTextureType(const bool masked)
{
  return masked ? TextureType::Masked : TextureType::Opaque;
}

const std::string& Texture::name() const
{
  return m_name;
}

const IO::Path& Texture::absolutePath() const
{
  return m_absolutePath;
}

void Texture::setAbsolutePath(IO::Path absolutePath)
{
  m_absolutePath = std::move(absolutePath);
}

const IO::Path& Texture::relativePath() const
{
  return m_relativePath;
}

void Texture::setRelativePath(IO::Path relativePath)
{
  m_relativePath = std::move(relativePath);
}

size_t Texture::width() const
{
  return m_width;
}

size_t Texture::height() const
{
  return m_height;
}

const Color& Texture::averageColor() const
{
  return m_averageColor;
}

bool Texture::masked() const
{
  return m_type == TextureType::Masked;
}

void Texture::setOpaque()
{
  m_type = TextureType::Opaque;
}

const std::set<std::string>& Texture::surfaceParms() const
{
  return m_surfaceParms;
}

void Texture::setSurfaceParms(std::set<std::string> surfaceParms)
{
  m_surfaceParms = std::move(surfaceParms);
}

TextureCulling Texture::culling() const
{
  return m_culling;
}

void Texture::setCulling(const TextureCulling culling)
{
  m_culling = culling;
}

void Texture::setBlendFunc(const GLenum srcFactor, const GLenum destFactor)
{
  m_blendFunc.enable = TextureBlendFunc::Enable::UseFactors;
  m_blendFunc.srcFactor = srcFactor;
  m_blendFunc.destFactor = destFactor;
}

void Texture::disableBlend()
{
  m_blendFunc.enable = TextureBlendFunc::Enable::DisableBlend;
}

const GameData& Texture::gameData() const
{
  return m_gameData;
}

size_t Texture::usageCount() const
{
  return static_cast<size_t>(m_usageCount);
}

void Texture::incUsageCount()
{
  ++m_usageCount;
}

void Texture::decUsageCount()
{
  const size_t previous = m_usageCount--;
  assert(previous > 0);
  unused(previous);
}

bool Texture::overridden() const
{
  return m_overridden;
}

void Texture::setOverridden(const bool overridden)
{
  m_overridden = overridden;
}

bool Texture::isPrepared() const
{
  return m_textureId != 0;
}

void Texture::prepare(const GLuint textureId, const int minFilter, const int magFilter)
{
  assert(textureId > 0);
  assert(m_textureId == 0);

  if (!m_buffers.empty())
  {
    const auto compressed = isCompressedFormat(m_format);

    glAssert(glPixelStorei(GL_UNPACK_SWAP_BYTES, false));
    glAssert(glPixelStorei(GL_UNPACK_LSB_FIRST, false));
    glAssert(glPixelStorei(GL_UNPACK_ROW_LENGTH, 0));
    glAssert(glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0));
    glAssert(glPixelStorei(GL_UNPACK_SKIP_ROWS, 0));
    glAssert(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));

    glAssert(glBindTexture(GL_TEXTURE_2D, textureId));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));

    if (m_type == TextureType::Masked)
    {
      // masked textures don't work well with automatic mipmaps, so we force
      // GL_NEAREST filtering and don't generate any
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE));
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }
    else if (m_buffers.size() == 1)
    {
      // generate mipmaps if we don't have any
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE));
    }
    else
    {
      glAssert(glTexParameteri(
        GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(m_buffers.size() - 1)));
    }

    // Upload only the first mipmap for masked textures.
    const auto mipmapsToUpload = (m_type == TextureType::Masked) ? 1u : m_buffers.size();

    for (size_t j = 0; j < mipmapsToUpload; ++j)
    {
      const auto mipSize = sizeAtMipLevel(m_width, m_height, j);

      const auto* data = reinterpret_cast<const GLvoid*>(m_buffers[j].data());
      if (compressed)
      {
        const auto dataSize = static_cast<GLsizei>(m_buffers[j].size());

        glAssert(glCompressedTexImage2D(
          GL_TEXTURE_2D,
          static_cast<GLint>(j),
          m_format,
          static_cast<GLsizei>(mipSize.x()),
          static_cast<GLsizei>(mipSize.y()),
          0,
          dataSize,
          data));
      }
      else
      {
        glAssert(glTexImage2D(
          GL_TEXTURE_2D,
          static_cast<GLint>(j),
          GL_RGBA,
          static_cast<GLsizei>(mipSize.x()),
          static_cast<GLsizei>(mipSize.y()),
          0,
          m_format,
          GL_UNSIGNED_BYTE,
          data));
      }
    }

    m_buffers.clear();
    m_textureId = textureId;
  }
}

void Texture::setMode(const int minFilter, const int magFilter)
{
  if (isPrepared())
  {
    activate();
    if (m_type == TextureType::Masked)
    {
      // Force GL_NEAREST filtering for masked textures.
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST));
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST));
    }
    else
    {
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter));
      glAssert(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter));
    }
    deactivate();
  }
}

void Texture::activate() const
{
  if (isPrepared())
  {
    glAssert(glBindTexture(GL_TEXTURE_2D, m_textureId));

    switch (m_culling)
    {
    case Assets::TextureCulling::None:
      glAssert(glDisable(GL_CULL_FACE));
      break;
    case Assets::TextureCulling::Front:
      glAssert(glCullFace(GL_FRONT));
      break;
    case Assets::TextureCulling::Both:
      glAssert(glCullFace(GL_FRONT_AND_BACK));
      break;
    case Assets::TextureCulling::Default:
    case Assets::TextureCulling::Back:
      break;
    }

    if (m_blendFunc.enable != TextureBlendFunc::Enable::UseDefault)
    {
      glAssert(glPushAttrib(GL_COLOR_BUFFER_BIT));
      if (m_blendFunc.enable == TextureBlendFunc::Enable::UseFactors)
      {
        glAssert(glBlendFunc(m_blendFunc.srcFactor, m_blendFunc.destFactor));
      }
      else
      {
        assert(m_blendFunc.enable == TextureBlendFunc::Enable::DisableBlend);
        glAssert(glDisable(GL_BLEND));
      }
    }
  }
}

void Texture::deactivate() const
{
  if (isPrepared())
  {
    if (m_blendFunc.enable != TextureBlendFunc::Enable::UseDefault)
    {
      glAssert(glPopAttrib());
    }

    switch (m_culling)
    {
    case Assets::TextureCulling::None:
      glAssert(glEnable(GL_CULL_FACE));
      break;
    case Assets::TextureCulling::Front:
      glAssert(glCullFace(GL_BACK));
      break;
    case Assets::TextureCulling::Both:
      glAssert(glCullFace(GL_BACK));
      break;
    case Assets::TextureCulling::Default:
    case Assets::TextureCulling::Back:
      break;
    }

    glAssert(glBindTexture(GL_TEXTURE_2D, 0));
  }
}

const Texture::BufferList& Texture::buffersIfUnprepared() const
{
  return m_buffers;
}

GLenum Texture::format() const
{
  return m_format;
}

TextureType Texture::type() const
{
  return m_type;
}

} // namespace TrenchBroom::Assets
