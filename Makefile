
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
	# exit
endif
ifeq ($(detected_OS),Darwin)
	GCC = /opt/homebrew/Cellar/gcc/12.2.0/bin/g++-12
	CXX_FLAGS += -D MACOS -D FONTS_DIR=\"/System/Library/Fonts/Supplemental\"
	#CXX_GRAPHICS_LIBS += -l/opt/homebrew/Cellar/glfw/3.3.7/lib/libglfw.3.3.dylib -I/opt/homebrew/Cellar/glfw/3.3.7/include
	# CXX_GRAPHICS_LIBS += -lglfw
	# CXX_INCLUDES += -I/opt/homebrew/Cellar/glfw/3.3.7/include
	# CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.7/lib -lglfw3
	# CXX_LIBS = -lglfw3
	CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.8/lib -lglfw -lvulkan.1.3.216 -L/opt/homebrew/Cellar/freetype/2.12.1/lib -lfreetype -L/Users/philipwenkel/VulkanSDK/1.3.216.0/macOS/lib
	# FONTS_DIR = /System/Library/Fonts/Supplemental
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
# all: $(apps) $(tests)
# all: HejHej Graphics.Test

# std_headers:
# 	$(GCC) -xc++-system-header iostream

# Delta.Concepts: 

# Delta.pcm: Delta.cpp Delta.Graphics.pcm #Delta.String.pcm 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Graphics.pcm: Delta.Graphics.cpp Delta.Graphics.Font.pcm Delta.Range.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(CXX_INCLUDES)

# Delta.Graphics.Font.pcm: Delta.Graphics.Font.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(CXX_INCLUDES)


# Delta.Coro.Implementation.o: Delta.Coro.Implementation.cpp Delta.Coro.Interface.pcm
# 	$(CXX) $(CXX_FLAGS) -fmodules -fbuiltin-module-map $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@ $(CXX_INCLUDES)

# Delta.Coro.Implementation.pcm: Delta.Coro.Implementation.cpp 
	# $(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< $(CXX_INCLUDES)

# Delta.Coro.Interface.o: Delta.Coro.Interface.cpp Delta.Coro.Implementation.pcm
# 	clang++ -std=c++2b -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. -Xclang -emit-module-interface -o Delta.Coro.Implementation.o -fmodule-file=Delta.Coro.Implementation.pcm -c Delta.Coro.Interface.cpp

# Delta.Coro.Interface.pcm: Delta.Coro.Interface.cpp
	# $(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(CXX_INCLUDES)


# Delta.Coro.Implementation.o: Delta.Coro.Implementation.cpp Delta.Coro.Interface.pcm
	# $(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@ $(CXX_INCLUDES)

# Delta.Coro.Implementation.pcm: Delta.Coro.Implementation.cpp
# 	clang++ -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. --precompile -x c++-module Delta.Coro.Implementation.cpp -o Delta.Coro.Implementation.pcm

# Delta.Coro.Implementation.pcm: Delta.Coro.Implementation.cpp 
# 	clang++ -std=c++2b -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. --precompile -c Delta.Coro.Implementation.cpp -o Delta.Coro.Implementation.pcm -x c++-module -Xclang -fmodules-embed-all-files


# Delta.Coro.Interface.pcm: Delta.Coro.Interface.cpp Delta.Coro.Implementation.pcm
	# $(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -Xclang -emit-module-interface -o $@ $(CXX_INCLUDES)

# Delta.Coro.Interface.pcm: Delta.Coro.Interface.cpp Delta.Coro.Implementation.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< $(CXX_INCLUDES)
# Delta.Coro.Implementation.pcm.o: Delta.Coro.Implementation.pcm
# 	clang++ -std=c++2b -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. -o Delta.Coro.Implementation.pcm.o -c Delta.Coro.Implementation.pcm

# Delta.Coro.Interface.o: Delta.Coro.Interface.cpp Delta.Coro.Implementation.pcm
# 	clang++ -std=c++2b -fmodules-ts -x c++ -o Delta.Coro.Interface.o  -fmodule-file=Delta.Coro.Implementation.pcm -c Delta.Coro.Interface.cpp
# Delta.Coro.Interface.pcm: Delta.Coro.Interface.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ $(CXX_INCLUDES)

# Delta.Coro.pcm: Delta.Coro.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@ 

#Delta.String.pcm: Delta.String.cpp Delta.Array.pcm
#	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

#Delta.Array.pcm: Delta.Array.cpp Delta.Range.pcm
#	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Range.pcm: Delta.Range.cpp #Delta.Iterator.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

#Delta.Iterator.pcm: Delta.Iterator.cpp Delta.Common.pcm
#	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

#Delta.Common.pcm: Delta.Common.cpp
#	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.String.pcm: Delta.String.cpp Delta.Char.pcm Delta.Array.pcm 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Array.pcm: Delta.Array.cpp Delta.Range.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Range.pcm: Delta.Range.cpp Delta.Types.pcm Delta.Function.pcm Delta.Iterator.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Iterator.pcm: Delta.Iterator.cpp Delta.Mimic.pcm Delta.Size.pcm Delta.Convertible.pcm Delta.Same.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Size.pcm: Delta.Size.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Char.pcm: Delta.Char.cpp Delta.Types.pcm Delta.Strip.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Pointer.pcm: Delta.Pointer.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Function.pcm: Delta.Function.cpp Delta.Types.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.pcm: Delta.Types.cpp Delta.Types.IfElse.pcm Delta.Types.Predicate.pcm Delta.Types.List.pcm Delta.Types.AnyOf.pcm Delta.Types.AllOf.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.AllOf.pcm: Delta.Types.AllOf.cpp Delta.Types.Predicate.pcm Delta.Types.List.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.AnyOf.pcm: Delta.Types.AnyOf.cpp Delta.Types.Predicate.pcm Delta.Types.List.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.List.pcm: Delta.Types.List.cpp Delta.Types.IfElse.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.Predicate.pcm: Delta.Types.Predicate.cpp Delta.Same.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Types.IfElse.pcm: Delta.Types.IfElse.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Convertible.pcm: Delta.Convertible.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Strip.pcm: Delta.Strip.cpp Delta.Same.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Same.pcm: Delta.Same.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# Delta.Mimic.pcm: Delta.Mimic.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@


# App.Server: App.Server.cpp delta std_headers
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

# App.Client: App.Client.cpp delta 
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

# Delta.pcm: Delta.cpp
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -Xclang -emit-module-interface -o $@

# App.%: App.%.cpp Delta.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $^ -o $@ -lrt -lpthread






# Test.%: Test.%.o 
# 	$(CXX) $(CXX_FLAGS) $< -o $@ $(CXX_APP_FLAGS)

# Test.%.o: Test.%.cpp Delta.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@

# App.%: App.%.o Delta.pcm 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) Delta.pcm $< -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

# App.%.o: App.%.cpp Delta.pcm 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@


# Graphics.Triangle: Graphics.Triangle.o 
# 	$(CXX) $(CXX_FLAGS) $< -o $@ $(CXX_LIBS)

# Graphics.Triangle.o: Graphics.Triangle.cpp Graphics.Triangle.vert.spv Graphics.Triangle.frag.spv Delta.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -fmodule-file=Delta.pcm -c $< -o $@ $(CXX_INCLUDES)

# Graphics.Text: Graphics.Text.o 
# 	$(CXX) $(CXX_FLAGS) $< -o $@ $(CXX_LIBS)

# Graphics.Text.o: Graphics.Text.cpp Graphics.Text.vert.spv Graphics.Text.frag.spv
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -o $@ $(CXX_INCLUDES)

# App.Graphics.Info: App.Graphics.Info.o Delta.pcm Delta.Graphics.pcm
# 	$(CXX) $(CXX_FLAGS) $^ -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

# App.Graphics.Info.o: App.Graphics.Info.cpp Delta.pcm Delta.Graphics.pcm App.Graphics.Info.vert.spv App.Graphics.Info.frag.spv
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -o $@ $(CXX_INCLUDES)

# HejHej: HejHej.o Delta.pcm
# 	$(CXX) $(CXX_FLAGS) $^ -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

# HejHej.o: HejHej.cpp Delta.pcm
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -o $@ $(CXX_INCLUDES)

# Graphics.Test: Graphics.Test.o Delta.pcm Delta.Graphics.pcm
# 	$(CXX) $(CXX_FLAGS) $^ -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

# Graphics.Test.o: Graphics.Test.cpp Delta.pcm Delta.Graphics.pcm Graphics.Test.vert.spv Graphics.Test.frag.spv Graphics.Test.Line.vert.spv Graphics.Test.Line.frag.spv 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -o $@ $(CXX_INCLUDES)

# Delta.Coro.Interface.pcm: Delta.Coro.Interface.cpp
# 	clang++ -std=c++2b -stdlib=libc++ -fmodules -fmodules-ts -fbuiltin-module-map -Xclang -emit-module-interface -c Delta.Coro.Interface.cpp -o Delta.Coro.Interface.pcm

# Delta.Coro.Interface.o: Delta.Coro.Interface.cpp
# 	clang++ -std=c++2b -stdlib=libc++ -fmodules -fbuiltin-module-map -c Delta.Coro.Interface.cpp -o Delta.Coro.Interface.o

# Delta.Coro.Implementation.pcm: Delta.Coro.Implementation.cpp Delta.Coro.Interface.pcm
# 	clang++ -std=c++2b -stdlib=libc++ -fmodules -fbuiltin-module-map -fmodule-file=Delta.Coro.Interface.pcm -c Delta.Coro.Implementation.cpp -o Delta.Coro.Implementation.o

# all: std_headers Oj
tests:= Test.Async
all: Oj $(tests)

# std_headers:
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header iostream
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header coroutine
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header utility
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header vector
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header array 
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header string
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header tuple
	# $(GCC) -std=c++2b -fmodules-ts -x c++-system-header fstream

# Delta.Coro-Promise.o: Delta.Coro-Promise.cpp std_headers
# 	$(GCC) -c -std=c++2b -fmodules-ts $< -o $@

# Promise.o: Promise.cpp
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# _Promise.o: _Promise.cpp Promise.o
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# first partition unit then the module interface
# Coro.o: Coro.cpp _Promise.o
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# then the partition implemention
# _Promise.o: _Promise.cpp Co

# then the module implementation
# _Coro.o: _Coro.cpp Coro.o
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# _Promise.o: _Promise.cpp Coro.o
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# _Promise.o: _Promise.cpp _Promise.o
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

# Delta: _Promise.o Promise.o _Coro.o Coro.o 
# 	$(GCC) -shared $^ -o libDelta.so

# Oj: Oj.cpp Delta#Delta.Coro.Impl.o
# 	$(GCC) -std=c++2b -fmodules-ts $< -o $@ -Xlinker ./libDelta.soEE€E


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

# -laio -pthread

# Delta.Graphics.o: Delta.Graphics.cpp
# 	$(GCC) $(CXX_FLAGS) -std=c++2b -fmodules-ts -c $< $(CXX_INCLUDES)

# Async: Async.o Async.IO.o Async.IO.CompletionQueue.o Async.IO.SubmissionQueue.o Async.IO.CompletionQueue.Implementation.o Async.IO.CompletionQueue.Interface.o Async.IO.SubmissionQueue.Implementation.o Async.IO.SubmissionQueue.Interface.o

# Delta: Coro.o Coro.Type.o Coro.Type.Interface.o Coro.Type.Implementation.o Promise.o Promise.Type.o Promise.Type.Interface.o Promise.Type.Implementation.o 
# 	$(GCC) -shared $^ -o libDelta.so

Delta: $(Async) $(Coro)
	ar -cr libDelta.a $^

# Oj.o: Oj.cpp Delta
# 	$(GCC) -std=c++2b -fmodules-ts -c $<

Oj: Oj.cpp Delta
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $< -L. -lDelta $(CXX_LIBS) $(CXX_INCLUDES)

# Graphics.Test: Graphics.Test.cpp Delta
# 	$(GCC) -std=c++2b -fmodules-ts -o $@ $< -L. -lDelta $(CXX_INCLUDES) $(CXX_LIBS)



#### TESTS ####
Test.Async: Test.Async.cpp $(Async)
	$(GCC) $(CXX_FLAGS) -Werror=unused-result -o $@ $^ $(CXX_LIBS) $(CXX_INCLUDES)



# Delta2: _Promise.o Promise.o _Coro.o Coro.o 
# 	ar -cr libDelta2.a $^

# Oj.o: Oj.cpp Delta2
# 	$(GCC) -std=c++2b -fmodules-ts $< -o $@ 



# Oj: Oj.cpp Delta2
# 	$(GCC) -std=c++2b -fmodules-ts -o $@ $< -L. -lDelta2





#-Xlinker #./libDelta.so


# App.Compiler: App.Compiler.o Delta.pcm 
# 	$(CXX) $(CXX_FLAGS) $^ -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

# App.Compiler.o: App.Compiler.cpp Delta.pcm 
# 	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) -c $< -o $@ $(CXX_INCLUDES)

GLSLC_COMPILER = /Users/philipwenkel/VulkanSDK/1.2.182.0/macOS/bin/glslc

%.vert.spv: %.vert 
	$(GLSLC_COMPILER) $< -o $@

%.frag.spv: %.frag 
	$(GLSLC_COMPILER) $< -o $@


# %.spv: %.vert 
# 	$(GLSLC_COMPILER) $< -o $@

# %.spv: %.frag 
# 	$(GLSLC_COMPILER) $< -o $@

# Test.%: Test.%.cpp delta
# 	$(GCC) $< *.o -o $@ -lrt -lpthread
	

clean:
	@rm -f libDelta.a
	@rm -rf gcm.cache
	@rm -f *.o
	@rm -f *.pcm 
	@rm -f *.spv
	@rm -f $(apps)
	@rm -f $(tests)
