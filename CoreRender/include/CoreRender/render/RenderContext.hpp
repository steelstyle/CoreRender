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

#ifndef _CORERENDER_RENDER_RENDERCONTEXT_HPP_INCLUDED_
#define _CORERENDER_RENDER_RENDERCONTEXT_HPP_INCLUDED_

#include "VideoDriverType.hpp"
#include "../core/ReferenceCounted.hpp"

namespace cr
{
namespace render
{
	class GraphicsEngine;

	class RenderContext : public core::ReferenceCounted
	{
		public:
			RenderContext()
				: width(0), height(0)
			{
			}
			virtual ~RenderContext()
			{
			}

			virtual bool resize(unsigned int width,
			                    unsigned int height,
			                    bool fullscreen)
			{
				return false;
			}

			unsigned int getWidth()
			{
				return width;
			}
			unsigned int getHeight()
			{
				return height;
			}

			typedef core::SharedPointer<RenderContext> Ptr;

			virtual void makeCurrent(bool current = true) = 0;
			virtual void swapBuffers() = 0;

			virtual RenderContext::Ptr clone()
			{
				return 0;
			}

			virtual void update(GraphicsEngine *inputreceiver)
			{
			}
			virtual VideoDriverType::List getDriverType() = 0;
		protected:
			unsigned int width;
			unsigned int height;
	};
}
}

#endif
