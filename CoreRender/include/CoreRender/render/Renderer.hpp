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

#ifndef _CORERENDER_RENDER_RENDERER_HPP_INCLUDED_
#define _CORERENDER_RENDER_RENDERER_HPP_INCLUDED_

#include "RenderResource.hpp"
#include "RenderContext.hpp"
#include "../core/Log.hpp"
#include "Shader.hpp"

#include <queue>

namespace cr
{
namespace core
{
	class MemoryPool;
}
namespace render
{
	class VideoDriver;
	class GraphicsEngine;
	struct PipelineInfo;
	struct RenderPassInfo;

	class Renderer
	{
		public:
			Renderer(RenderContext::Ptr primary,
			         RenderContext::Ptr secondary,
			         core::Log::Ptr log,
			         VideoDriver *driver,
			         GraphicsEngine *input);
			~Renderer();

			void registerNew(RenderResource::Ptr res);
			void registerShaderUpload(Shader::Ptr shader);
			void registerUpload(RenderResource::Ptr res);
			void registerDelete(RenderResource *res);

			void enterThread();
			void exitThread();

			void uploadNewObjects();
			void prepareRendering(PipelineInfo *renderdata,
			                      unsigned int pipelinecount);
			void uploadObjects();
			void deleteObjects();

			void render();

			core::Log::Ptr getLog()
			{
				return log;
			}
			core::MemoryPool *getNextFrameMemory()
			{
				return memory[0];
			}
			core::MemoryPool *getCurrentFrameMemory()
			{
				return memory[1];
			}
			VideoDriver *getDriver()
			{
				return driver;
			}
		private:
			void renderPipeline(PipelineInfo *info);
			void renderPass(RenderPassInfo *info);

			RenderContext::Ptr primary;
			RenderContext::Ptr secondary;
			core::Log::Ptr log;
			core::MemoryPool *memory[2];
			VideoDriver *driver;

			tbb::spin_mutex newmutex;
			std::queue<RenderResource::Ptr> newqueue;
			tbb::spin_mutex shaderuploadmutex;
			std::queue<Shader::Ptr> shaderuploadqueue;
			tbb::spin_mutex uploadmutex;
			std::queue<RenderResource::Ptr> uploadqueue;
			tbb::spin_mutex deletemutex;
			std::queue<RenderResource*> deletequeue;

			PipelineInfo *renderdata;
			unsigned int pipelinecount;

			// TODO: We should not have any pointer to the GraphicsEngine here
			GraphicsEngine *input;
	};
}
}

#endif
