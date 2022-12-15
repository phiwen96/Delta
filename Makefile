
# sudo apt -y install liburing-dev

# GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
CXX = clang++
# --verbose
CXX_FLAGS = -D DEBUG -std=c++2b -fmodules-ts -fconcepts-diagnostics-depth=1 -fcompare-debug-second
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
	GLSLC_COMPILER = /Users/philipwenkel/VulkanSDK/1.3.231.1/macOS/bin/glslc
	GCC = /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12
	CXX_FLAGS += -D MACOS -D FONTS_DIR=\"/System/Library/Fonts/Supplemental\"
	CXX_LIBS = -L/opt/homebrew/lib -L/opt/homebrew/Cellar/glfw/3.3.8/lib -lglfw -L/Users/philipwenkel/VulkanSDK/1.3.231.1/macOS/lib -lvulkan.1.3.231 -L/opt/homebrew/Cellar/freetype/2.12.1/lib -lfreetype
	# CXX_INCLUDES += -I/opt/homebrew/include
endif
ifeq ($(detected_OS),Linux)
	GLSLC_COMPILER = /usr/bin/glslc
	GCC = /usr/bin/g++-12
	CXX_FLAGS += -D LINUX
	# CXX_LIBS += -lglfw
    CXX_APP_FLAGS += -lrt
	CXX_LIBS = -lrt -lglfw -lvulkan -luring -lfreetype
	CXX_INCLUDES += -I/usr/include/freetype2
endif



APP=main
apps:= Nej#Graphics.Test Oj #App.Server App.FileNotifier Graphics.Triangle App.Graphics.Info#App.Client
tests:= Test.Coro # Test.Async Test.App
# all: $(tests) $(apps)
all: $(apps) Test.Compute Test.Graphics

std_headers:
	$(GCC) -std=c++2b -fmodules-ts -x c++-header /usr/include/GLFW/glfw3.h
	$(GCC) -std=c++2b -fmodules-ts -x c++-header /usr/include/glm/glm.hpp
	$(GCC) -std=c++2b -fmodules-ts -x c++-header /usr/include/vulkan/vulkan_core.h
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header array
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header vector
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header iostream
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header tuple
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header utility
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header coroutine
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header type_traits
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header string
	$(GCC) -std=c++2b -fmodules-ts -x c++-system-header algorithm


Awaitable.o: Awaitable.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Coro.o: Coro.cpp
	$(GCC) $(CXX_FLAGS) -c $<

Bool.o: Bool.cpp 
	$(GCC) $(CXX_FLAGS) -c $<

Mector.o: Mector.cpp 
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Vulkan.o: Vulkan.cpp Mector.o 
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Delta.o: Delta.cpp Vulkan.o
	$(GCC) $(CXX_FLAGS) -c $< $(CXX_INCLUDES)

Array.o: Array.cpp 
	$(GCC) $(CXX_FLAGS) -c $<


Nej: Nej.cpp Delta.o
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< Delta.o Vulkan.o Mector.o $(CXX_LIBS) $(CXX_INCLUDES)

Test.Compute.Shader.spv: Test.Compute.Shader.comp
	$(GLSLC_COMPILER) $< -o $@

Test.Compute: Test.Compute.cpp Delta.o Test.Compute.Shader.spv
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< Delta.o Vulkan.o Mector.o $(CXX_LIBS) $(CXX_INCLUDES)

Test.Graphics: Test.Graphics.cpp Delta.o
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< Delta.o Vulkan.o Mector.o $(CXX_LIBS) $(CXX_INCLUDES)


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