
# sudo apt -y install liburing-dev

# GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
CXX = clang++
CXX_FLAGS = -D DEBUG -std=c++2b -fmodules-ts 
CXX_MODULES = -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=.

CXX_APP_FLAGS = -lpthread 

ifeq ($(OS),Windows_NT) 
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
	CXX_INCLUDES = -I/usr/local/include -I/opt/homebrew/Cellar/glm/0.9.9.8/include -I/opt/homebrew/Cellar/freetype/2.12.1/include/freetype2 #-I/Users/philipwenkel/VulkanSDK/1.3.216.0/macOS/include
endif
ifeq ($(detected_OS),Windows)
	GCC = g++
	CXX_FLAGS += -D WINDOWS
	VULKAN_DIR = C:\VulkanSDK\1.3.224.1
	CXX_LIBS = -I$(VULKAN_DIR)\Include
	CXX_INCLUDES += -L$(VULKAN_DIR)\Lib #-lvulkan
endif
ifeq ($(detected_OS),Darwin)
	GCC = /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12
	CXX_FLAGS += -D MACOS -D FONTS_DIR=\"/System/Library/Fonts/Supplemental\"
	CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.8/lib -lglfw -lvulkan.1.3.216 -L/opt/homebrew/Cellar/freetype/2.12.1/lib -lfreetype -L/Users/philipwenkel/VulkanSDK/1.3.216.0/macOS/lib
endif
ifeq ($(detected_OS),Linux)
	GCC = /usr/bin/g++-12
	CXX_FLAGS += -D LINUX
	# CXX_LIBS += -lglfw
    CXX_APP_FLAGS += -lrt
	CXX_LIBS = -lrt -lglfw -lvulkan -luring
endif

APP=main
apps:= Graphics.Test Oj #App.Server App.FileNotifier Graphics.Triangle App.Graphics.Info#App.Client
tests:= Test.Yolo Test.Array Test.Range

tests:= Test.Async
all: Oj $(tests)

# std_headers:
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header iostream
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header coroutine

Promise.Type.Interface.o: Promise.Type.Interface.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Promise.Type.Implementation.o: Promise.Type.Implementation.cpp Promise.Type.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Promise.Type.o: Promise.Type.cpp Promise.Type.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Promise.o: Promise.cpp Promise.Type.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Type.Interface.o: Coro.Type.Interface.cpp Promise.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Type.Implementation.o: Coro.Type.Implementation.cpp Coro.Type.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Type.o: Coro.Type.cpp Coro.Type.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.o: Coro.cpp Coro.Type.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro:= Coro.o Coro.Type.o Coro.Type.Interface.o Coro.Type.Implementation.o Promise.o Promise.Type.o Promise.Type.Interface.o Promise.Type.Implementation.o

Async.IO.SubmissionQueue.Interface.o: Async.IO.SubmissionQueue.Interface.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.SubmissionQueue.Implementation.o: Async.IO.SubmissionQueue.Implementation.cpp Async.IO.SubmissionQueue.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.CompletionQueue.Interface.o: Async.IO.CompletionQueue.Interface.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.CompletionQueue.Implementation.o: Async.IO.CompletionQueue.Implementation.cpp Async.IO.CompletionQueue.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.SubmissionQueue.o: Async.IO.SubmissionQueue.cpp Async.IO.SubmissionQueue.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.CompletionQueue.o: Async.IO.CompletionQueue.cpp Async.IO.CompletionQueue.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Async.IO.o: Async.IO.cpp Async.IO.SubmissionQueue.o Async.IO.CompletionQueue.o
	$(GCC) $(CXX_FLAGS) -c $<

Async.o: Async.cpp Async.IO.o
	$(GCC) $(CXX_FLAGS) -c $<

Async:= Async.o Async.IO.o Async.IO.CompletionQueue.o Async.IO.SubmissionQueue.o Async.IO.CompletionQueue.Implementation.o Async.IO.CompletionQueue.Interface.o Async.IO.SubmissionQueue.Implementation.o Async.IO.SubmissionQueue.Interface.o

Delta: $(Async) $(Coro)
	ar -cr libDelta.a $^

Oj: Oj.cpp Delta
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< -L. -lDelta $(CXX_LIBS) $(CXX_INCLUDES)

#### TESTS ####
Test.Async: Test.Async.cpp $(Async)
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $^ $(CXX_LIBS) $(CXX_INCLUDES)

GLSLC_COMPILER = /Users/philipwenkel/VulkanSDK/1.2.182.0/macOS/bin/glslc

%.vert.spv: %.vert 
	$(GLSLC_COMPILER) $< -o $@

%.frag.spv: %.frag 
	$(GLSLC_COMPILER) $< -o $@

clean:
	@rm -f libDelta.a
	@rm -rf gcm.cache
	@rm -f *.o
	@rm -f *.pcm 
	@rm -f *.spv
	@rm -f $(apps)
	@rm -f $(tests)