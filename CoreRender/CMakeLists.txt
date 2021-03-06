
project(CoreRender)
cmake_minimum_required(VERSION 2.6.0)


option(CORERENDER_USE_SDL "Use SDL multi-threaded OpenGL contexts." ON)

set(SRC
	include/CoreRender.hpp
	include/CoreRender/core/Color.hpp
	include/CoreRender/core/File.hpp
	include/CoreRender/core/FileList.hpp
	include/CoreRender/core/FileSystem.hpp
	include/CoreRender/core/Functor.hpp
	include/CoreRender/core/Hardware.hpp
	include/CoreRender/core/Log.hpp
	include/CoreRender/core/MemoryPool.hpp
	include/CoreRender/core/ReferenceCounted.hpp
	include/CoreRender/core/Semaphore.hpp
	include/CoreRender/core/StandardFile.hpp
	include/CoreRender/core/StandardFileSystem.hpp
	include/CoreRender/core/Thread.hpp
	include/CoreRender/core/Time.hpp
	include/CoreRender/math/Vector3.hpp
	include/CoreRender/math/Alignment.hpp
	include/CoreRender/math/Quaternion.hpp
	include/CoreRender/math/Matrix4.hpp
	include/CoreRender/math/Math.hpp
	include/CoreRender/math/Vector2.hpp
	include/CoreRender/math/ScreenPosition.hpp
	include/CoreRender/math/StdInt.hpp
	include/CoreRender/res/DefaultResourceFactory.hpp
	include/CoreRender/res/LoadingThread.hpp
	include/CoreRender/res/Resource.hpp
	include/CoreRender/res/ResourceManager.hpp
	include/CoreRender/render/Animation.hpp
	include/CoreRender/render/AnimationFile.hpp
	include/CoreRender/render/FrameBuffer.hpp
	include/CoreRender/render/GeometryFile.hpp
	include/CoreRender/render/IndexBuffer.hpp
	include/CoreRender/render/Model.hpp
	include/CoreRender/render/ModelRenderable.hpp
	include/CoreRender/render/Pipeline.hpp
	include/CoreRender/render/Renderable.hpp
	include/CoreRender/render/RenderPass.hpp
	include/CoreRender/render/RenderResource.hpp
	include/CoreRender/render/RenderThread.hpp
	include/CoreRender/render/RenderStats.hpp
	include/CoreRender/render/RenderCaps.hpp
	include/CoreRender/render/Shader.hpp
	include/CoreRender/render/ShaderText.hpp
	include/CoreRender/render/VideoDriverType.hpp
	include/CoreRender/render/RenderContextOpenGL.hpp
	include/CoreRender/render/RenderContextReuseOpenGL.hpp
	include/CoreRender/render/GeometryManager.hpp
	include/CoreRender/render/RenderPass.hpp
	include/CoreRender/render/Texture.hpp
	include/CoreRender/render/RenderContext.hpp
	include/CoreRender/render/Material.hpp
	include/CoreRender/render/RenderTarget.hpp
	include/CoreRender/render/GraphicsEngine.hpp
	include/CoreRender/render/SpatialRenderable.hpp
	include/CoreRender/render/Texture.hpp
	include/CoreRender/render/Texture1D.hpp
	include/CoreRender/render/Texture2D.hpp
	include/CoreRender/render/Texture3D.hpp
	include/CoreRender/render/TextureCube.hpp
	include/CoreRender/render/UniformData.hpp
	include/CoreRender/render/VertexBuffer.hpp
	include/CoreRender/render/VertexLayout.hpp
	src/core/Log.cpp
	src/core/MemoryPool.cpp
	src/core/Semaphore.cpp
	src/core/StandardFile.cpp
	src/core/StandardFileSystem.cpp
	src/core/Thread.cpp
	src/core/Time.cpp
	src/render/Animation.cpp
	src/render/FrameBuffer.cpp
	src/render/GraphicsEngine.cpp
	src/render/IndexBuffer.cpp
	src/render/Material.cpp
	src/render/Model.cpp
	src/render/ModelRenderable.cpp
	src/render/null/VideoDriverNull.hpp
	src/render/opengl/FrameBufferOpenGL.cpp
	src/render/opengl/FrameBufferOpenGL.hpp
	src/render/opengl/IndexBufferOpenGL.cpp
	src/render/opengl/IndexBufferOpenGL.hpp
	src/render/opengl/RenderCapsOpenGL.cpp
	src/render/opengl/RenderCapsOpenGL.hpp
	src/render/opengl/ShaderOpenGL.cpp
	src/render/opengl/ShaderOpenGL.hpp
	src/render/opengl/Texture2DOpenGL.cpp
	src/render/opengl/Texture2DOpenGL.hpp
	src/render/opengl/VertexBufferOpenGL.cpp
	src/render/opengl/VertexBufferOpenGL.hpp
	src/render/opengl/VideoDriverOpenGL.cpp
	src/render/opengl/VideoDriverOpenGL.hpp
	src/render/Pipeline.cpp
	src/render/RenderContextReuseOpenGL.cpp
	src/render/Renderer.cpp
	src/render/RenderPass.cpp
	src/render/RenderResource.cpp
	src/render/RenderTarget.cpp
	src/render/RenderThread.cpp
	src/render/Shader.cpp
	src/render/ShaderText.cpp
	src/render/Texture.cpp
	src/render/Texture2D.cpp
	src/render/UniformData.cpp
	src/render/VertexBuffer.cpp
	src/render/VideoDriver.hpp
	src/res/LoadingThread.cpp
	src/res/Resource.cpp
	src/res/ResourceManager.cpp
	src/3rdparty/tinystr.cpp
	src/3rdparty/tinystr.h
	src/3rdparty/tinyxml.cpp
	src/3rdparty/tinyxml.h
	src/3rdparty/tinyxmlerror.cpp
	src/3rdparty/tinyxmlparser.cpp
)

set(LIB
	tbb
	freeimage
	freeimageplus
)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(LIB
		${LIB}
		OpenGL32
		ws2_32
		glew32
		glu32
	)
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-unused-parameter -Woverloaded-virtual")

	set(LIB
		${LIB}
		pthread
		GL
		GLEW
	)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")


find_package(SDL)
if(CORERENDER_USE_SDL AND SDL_FOUND)
	include_directories(${SDL_INCLUDE_DIR})
	set(SRC
		${SRC}
		src/render/opengl/RenderContextSDL.cpp
		src/render/opengl/RenderContextSDL.hpp
		src/render/opengl/RenderWindowSDL.cpp
		src/render/opengl/RenderWindowSDL.hpp
	)
	set(LIB
		${LIB}
		${SDL_LIBRARY}
	)
	add_definitions(-DCORERENDER_USE_SDL)
endif(CORERENDER_USE_SDL AND SDL_FOUND)


include_directories(include)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_library(CoreRender STATIC ${SRC})
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	add_library(CoreRender SHARED ${SRC})
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")

target_link_libraries(CoreRender ${LIB})

install(DIRECTORY include/CoreRender DESTINATION include/CoreRender FILES_MATCHING PATTERN *.hpp)
install(FILES include/CoreRender.hpp DESTINATION include)
if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
else(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	install(TARGETS CoreRender LIBRARY DESTINATION lib)
endif(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
