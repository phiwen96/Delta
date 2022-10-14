
# sudo apt -y install liburing-dev

# GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
CXX = clang++
CXX_FLAGS = -D DEBUG -std=c++2b -fmodules-ts -fconcepts-diagnostics-depth=2 
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
	CXX_LIBS = -L$(VULKAN_DIR)\Lib #-lvulkan
	CXX_INCLUDES += -I$(VULKAN_DIR)\Include
endif
ifeq ($(detected_OS),Darwin)
	GCC = /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12
	CXX_FLAGS += -D MACOS -D FONTS_DIR=\"/System/Library/Fonts/Supplemental\"
	CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.8/lib -lglfw -lvulkan.1.3.216 -L/Users/philipwenkel/VulkanSDK/1.3.216.0/macOS/lib -L/opt/homebrew/lib #-L/opt/homebrew/Cellar/freetype/2.12.1/lib -lfreetype
	# CXX_INCLUDES += -I/opt/homebrew/include
endif
ifeq ($(detected_OS),Linux)
	GCC = /usr/bin/g++-12
	CXX_FLAGS += -D LINUX
	# CXX_LIBS += -lglfw
    CXX_APP_FLAGS += -lrt
	CXX_LIBS = -lrt -lglfw -lvulkan -luring -lfreetype
	CXX_INCLUDES += -I/usr/include/freetype2
endif



APP=main
apps:= Express#Graphics.Test Oj #App.Server App.FileNotifier Graphics.Triangle App.Graphics.Info#App.Client
tests:= Test.Coro # Test.Async Test.App
all: $(tests) $(apps)

std_headers:
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header iostream
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header coroutine

Awaitable.Type.Interface.o: Awaitable.Type.Interface.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Awaitable.Type.Implementation.o: Awaitable.Type.Implementation.cpp Awaitable.Type.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Awaitable.Type.o: Awaitable.Type.cpp Awaitable.Type.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Awaitable.Concept.o: Awaitable.Concept.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Awaitable.o: Awaitable.cpp Awaitable.Type.o Awaitable.Concept.o 
	$(GCC) $(CXX_FLAGS) -c $<

Awaitable := Awaitable.Type.Interface.o Awaitable.Type.Implementation.o Awaitable.Type.o Awaitable.Concept.o Awaitable.o

Promise.Type.Interface.o: Promise.Type.Interface.cpp Awaitable.o
	$(GCC) $(CXX_FLAGS) -c $<

Promise.Type.Implementation.o: Promise.Type.Implementation.cpp Promise.Type.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Promise.Type.o: Promise.Type.cpp Promise.Type.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Promise.Concept.o: Promise.Concept.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Promise.o: Promise.cpp Promise.Type.o Promise.Concept.o 
	$(GCC) $(CXX_FLAGS) -c $<

Promise := $(Awaitable) Promise.Type.Interface.o Promise.Type.Implementation.o Promise.Type.o Promise.Concept.o Promise.o

Coro.Type.Interface.o: Coro.Type.Interface.cpp Promise.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Type.Implementation.o: Coro.Type.Implementation.cpp Coro.Type.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Type.o: Coro.Type.cpp Coro.Type.Implementation.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.Concept.o: Coro.Concept.cpp Promise.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro.o: Coro.cpp Coro.Concept.o Coro.Type.o
	$(GCC) $(CXX_FLAGS) -c $<

Coro:= $(Promise) Coro.Concept.o Coro.Type.Interface.o Coro.Type.Implementation.o Coro.Type.o Coro.o

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

Bool.o: Bool.cpp 
	$(GCC) $(CXX_FLAGS) -c $<

# Window.Interface.o: Window.Interface.cpp Bool.o $(Coro)
# 	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

# Window.Implementation.o: Window.Implementation.cpp Window.Interface.o
# 	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Graphics.Window.o: Graphics.Window.cpp #Window.Interface.o Window.Implementation.o 
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

#Graphics.Window.o:= Window.o # Window.Interface.o Window.Implementation.o Window.o 

# App.Concept.o: App.Concept.cpp $(Coro)
# 	$(GCC) $(CXX_FLAGS) -c $<

# App.Type.Interface.o: App.Type.Interface.cpp $(Window)
# 	$(GCC) $(CXX_FLAGS) -c $<

# App.Type.Implementation.o: App.Type.Implementation.cpp App.Type.Interface.o
# 	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

# App.Type.o: App.Type.cpp App.Type.Interface.o App.Type.Implementation.o
# 	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Graphics.o: Graphics.cpp Graphics.Window.o
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

App.o: App.cpp Graphics.o #App.Type.Interface.o App.Type.Implementation.o App.Type.o
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

App := App.o#App.Type.Interface.o App.Type.Implementation.o App.Type.o App.o 


Vulkan.Interface.o: Vulkan.Interface.cpp Window.o
	$(GCC) $(CXX_FLAGS) -c $<

Vulkan.Implementation.o: Vulkan.Implementation.cpp Vulkan.Interface.o
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Vulkan.o: Vulkan.cpp Vulkan.Implementation.o Vulkan.Interface.o
	$(GCC) $(CXX_FLAGS) -c $<

Vulkan := Vulkan.o Vulkan.Implementation.o Vulkan.Interface.o

Delta := Bool.o $(Coro) Graphics.Window.o Graphics.o $(App) #$(Coro) $(Window) $(Async) $(Vulkan) $(App) 
# libDelta.a: Bool.o # $(App) $(Vulkan) $(Window) $(Async) $(Coro) 
# 	ar -cr libDelta.a $^

Express: Express.cpp $(Delta) Graphics.Triangle.vert.spv Graphics.Triangle.frag.spv
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< $(Delta) $(CXX_LIBS) $(CXX_INCLUDES)
	# $(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< -L. -lDelta $(CXX_LIBS) $(CXX_INCLUDES)
	# $(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $^ $(CXX_LIBS) $(CXX_INCLUDES)
	


# Express: Express.cpp Delta
# 	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< -L. -lDelta $(CXX_LIBS) $(CXX_INCLUDES)

# Oj: Oj.cpp Delta
# 	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< -L. -lDelta $(CXX_LIBS) $(CXX_INCLUDES)

#### TESTS ####
Test.Coro: Test.Coro.cpp $(Delta)
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $^ $(CXX_LIBS) $(CXX_INCLUDES)

# Test.Async: Test.Async.cpp $(Async)
# 	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ Test.Async.cpp $(Async) $(CXX_LIBS) $(CXX_INCLUDES)

# Test.App: Test.App.cpp $(Delta)#$(App) $(Window) Bool.o
# 	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $^ $(CXX_LIBS) $(CXX_INCLUDES)


# GLSLC_COMPILER = /Users/philipwenkel/VulkanSDK/1.2.182.0/macOS/bin/glslc
GLSLC_COMPILER = /Users/philipwenkel/VulkanSDK/1.3.216.0/macOS/bin/glslc

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