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

#ifndef _CORERENDER_RENDER_RENDERPASS_HPP_INCLUDED_
#define _CORERENDER_RENDER_RENDERPASS_HPP_INCLUDED_

#include "RenderTarget.hpp"
#include "RenderPassInfo.hpp"

namespace cr
{
namespace render
{
	class VideoDriver;

	class RenderBatch;

	class RenderPass : public core::ReferenceCounted
	{
		public:
			RenderPass(const std::string &context);
			virtual ~RenderPass();

			void setRenderTarget(RenderTarget::Ptr target);
			RenderTarget::Ptr getRenderTarget();

			void beginFrame();
			void insert(RenderBatch *batch);
			void prepare(RenderPassInfo *info);
			void render(VideoDriver *driver);

			const std::string &getContext()
			{
				return context;
			}

			typedef core::SharedPointer<RenderPass> Ptr;
		private:
			tbb::spin_mutex insertmutex;
			std::vector<RenderBatch*> batches;

			// TODO: We need info about the target etc here
			RenderBatch **prepared;
			unsigned int preparedcount;

			RenderTarget::Ptr target;

			std::string context;
	};
}
}

#endif
