/*
Copyright (c) 2009, Mathias Gottschlag

Permission to use, copy, modify, and/or distribute this software for any
purpose with or without fee is hereby granted, provided that the above
copyright notice and this permission notice appear in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#ifndef _CORERENDER_CORE_SEMAPHORE_HPP_INCLUDED_
#define _CORERENDER_CORE_SEMAPHORE_HPP_INCLUDED_

#include "Platform.hpp"

#if defined(CORERENDER_UNIX)
	#include <semaphore.h>
#elif defined(CORERENDER_WINDOWS)
	#include <Windows.h>
#endif

namespace cr
{
namespace core
{
	class Semaphore
	{
		public:
			Semaphore(int value = 0);
			~Semaphore();

			void wait();
			void post();

			int get();
		private:
#if defined(CORERENDER_UNIX)
			sem_t sem;
#elif defined(CORERENDER_WINDOWS)
			HANDLE sem;
#endif
	};
}
}

#endif
