/*
Copyright (C) 2010, Mathias Gottschlag

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in
the Software without restriction, including without limitation the rights to
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
the Software, and to permit persons to whom the Software is furnished to do so,
subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#include "Texture2DOpenGL.hpp"
#include "CoreRender/render/Renderer.hpp"
#include "../VideoDriver.hpp"

#include <GL/glew.h>

namespace cr
{
namespace render
{
namespace opengl
{
	static bool translateInternalFormat(TextureFormat::List format,
	                                    unsigned int &internal)
	{
		switch (format)
		{
			case TextureFormat::RGBA8:
				internal = GL_RGBA8;
				return true;
			case TextureFormat::RGBA16F:
				internal = GL_RGBA16F_ARB;
				return true;
			case TextureFormat::RGBA32F:
				internal = GL_RGBA32F_ARB;
				return true;
			case TextureFormat::RGB_DXT1:
				internal = GL_COMPRESSED_RGB_S3TC_DXT1_EXT;
				return true;
			case TextureFormat::RGBA_DXT1:
				internal = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
				return true;
			case TextureFormat::RGBA_DXT3:
				internal = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
				return true;
			case TextureFormat::RGBA_DXT5:
				internal = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
				return true;
			case TextureFormat::Depth24Stencil8:
				internal = GL_DEPTH24_STENCIL8_EXT;
				return true;
			case TextureFormat::Depth16:
				internal = GL_DEPTH_COMPONENT16;
				return true;
			case TextureFormat::Depth24:
				internal = GL_DEPTH_COMPONENT24;
				return true;
			default:
				// TODO: R/RG formats
				return false;
		}
	}

	static bool translateFormat(TextureFormat::List format,
	                            unsigned int &openglfmt,
	                            unsigned int &component)
	{
		switch (format)
		{
			case TextureFormat::RGBA8:
				openglfmt = GL_RGBA;
				component = GL_UNSIGNED_BYTE;
				return true;
			case TextureFormat::RGBA16F:
				// TODO
				return false;
			case TextureFormat::RGBA32F:
				openglfmt = GL_RGBA;
				component = GL_FLOAT;
				return true;
			case TextureFormat::Depth24Stencil8:
				openglfmt = GL_DEPTH_STENCIL_EXT;
				component = GL_UNSIGNED_INT_24_8_EXT;
				return true;
			case TextureFormat::Depth16:
				// TODO
				return false;
			case TextureFormat::Depth24:
				// TODO
				return false;
			default:
				// TODO: R/RG formats
				return false;
		}
	}

	Texture2DOpenGL::Texture2DOpenGL(Renderer *renderer,
	                                 res::ResourceManager *rmgr,
	                                 const std::string &name)
		: Texture2D(renderer, rmgr, name)
	{
	}
	Texture2DOpenGL::~Texture2DOpenGL()
	{
	}

	bool Texture2DOpenGL::create()
	{
		glGenTextures(1, &handle);
		glBindTexture(GL_TEXTURE_2D, handle);
			glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		glTexParameterf(GL_TEXTURE_2D,
		                GL_TEXTURE_MIN_FILTER,
		                GL_LINEAR_MIPMAP_NEAREST);
		glTexParameterf(GL_TEXTURE_2D,
		                GL_TEXTURE_MAG_FILTER,
		                GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, true);
		// TODO: Error checking
		return true;
	}
	bool Texture2DOpenGL::destroy()
	{
		glDeleteTextures(1, &handle);
		return true;
	}
	bool Texture2DOpenGL::upload()
	{
		const RenderCaps &caps = getRenderer()->getDriver()->getCaps();
		{
			tbb::spin_mutex::scoped_lock lock(imagemutex);
			unsigned int internal = 0;
			unsigned int currentformat = 0;
			unsigned int component = 0;
			bool compressed = TextureFormat::isCompressed(format);
			bool intcompressed = TextureFormat::isCompressed(internalformat);
			// Compression only works under certain circumstances
			if (intcompressed)
			{
				if (compressed && format != internalformat)
				{
					core::Log::Ptr log = getRenderer()->getLog();
					log->error("Cannot convert between different compressed formats.");
					uploadFinished();
					return false;
				}
				if (!caps.getFlag(RenderCaps::Flag::TextureCompression))
				{
					if ((internalformat != TextureFormat::RGB_DXT1
					    && internalformat != TextureFormat::RGBA_DXT1)
					    || internalformat != format
					    || !caps.getFlag(RenderCaps::Flag::TextureDXT1))
					{
						core::Log::Ptr log = getRenderer()->getLog();
						log->error("Compressed texture not supported.");
						uploadFinished();
						return false;
					}
				}
			}
			else
			{
				if (compressed)
				{
					core::Log::Ptr log = getRenderer()->getLog();
					log->error("Cannot load compressed data into uncompressed texture.");
					uploadFinished();
					return false;
				}
			}
			// Check for float extension
			if (TextureFormat::isFloat(internalformat)
			    && !caps.getFlag(RenderCaps::Flag::TextureFloat))
			{
				core::Log::Ptr log = getRenderer()->getLog();
				log->error("Floating point texture not supported.");
				uploadFinished();
				return false;
			}
			// Depth-stencil
			if (internalformat == TextureFormat::Depth24Stencil8
			    && !caps.getFlag(RenderCaps::Flag::TextureDepthStencil))
			{
				core::Log::Ptr log = getRenderer()->getLog();
				log->error("Depth-stencil texture not supported.");
				uploadFinished();
				return false;
			}
			// TODO: Depth textures?
			// Translate internal format
			if (!translateInternalFormat(internalformat, internal))
			{
				core::Log::Ptr log = getRenderer()->getLog();
				log->error("Error translating internal texture format.");
				uploadFinished();
				return false;
			}
			if (data)
			{
				if (!translateFormat(format, currentformat, component))
				{
					core::Log::Ptr log = getRenderer()->getLog();
					log->error("Unsupported source format.");
					uploadFinished();
					return false;
				}
			}
			// Upload texture data
			glBindTexture(GL_TEXTURE_2D, handle);
			if (data)
			{
				// TODO
				if (compressed)
				{
					unsigned int size = TextureFormat::getSize(format,
					                                           width * height);
					glCompressedTexImage2D(GL_TEXTURE_2D,
					                       0,
					                       internal,
					                       width,
					                       height,
					                       0,
					                       size,
					                       data);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D,
					             0,
					             internal,
					             width,
					             height,
					             0,
					             currentformat,
					             component,
					             data);
				}
			}
			else
			{
				if (intcompressed)
				{
					// TODO: Does this make any sense?
					glCompressedTexImage2D(GL_TEXTURE_2D,
					                       0,
					                       internal,
					                       width,
					                       height,
					                       0,
					                       0,
					                       0);
				}
				else
				{
					glTexImage2D(GL_TEXTURE_2D,
					             0,
					             internal,
					             width,
					             height,
					             0,
					             GL_RGBA,
					             GL_UNSIGNED_BYTE,
					             0);
				}
			}
			// Error checking
			GLenum error = glGetError();
			if (error != GL_NO_ERROR)
			{
				core::Log::Ptr log = getRenderer()->getLog();
				log->error("Uploading texture: %s", gluErrorString(error));
			}
		}
		glBindTexture(GL_TEXTURE_2D, 0);
		uploadFinished();
		return true;
	}
}
}
}
