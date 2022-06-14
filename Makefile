

# GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
CXX = clang++
CXX_FLAGS = -std=c++2b
CXX_MODULES = -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=.

CXX_INCLUDES = -I/usr/local/include
CXX_APP_FLAGS = -lpthread 

ifeq ($(OS),Windows_NT) 
    detected_OS := Windows
else
    detected_OS := $(shell sh -c 'uname 2>/dev/null || echo Unknown')
endif

ifeq ($(detected_OS),Windows)
	CXX_FLAGS += -D WINDOWS
	# exit
endif
ifeq ($(detected_OS),Darwin)
	CXX_FLAGS += -D MACOS
	#CXX_GRAPHICS_LIBS += -l/opt/homebrew/Cellar/glfw/3.3.7/lib/libglfw.3.3.dylib -I/opt/homebrew/Cellar/glfw/3.3.7/include
	# CXX_GRAPHICS_LIBS += -lglfw
	# CXX_INCLUDES += -I/opt/homebrew/Cellar/glfw/3.3.7/include
	# CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.7/lib -lglfw3
	# CXX_LIBS = -lglfw3
	CXX_LIBS = -L/opt/homebrew/Cellar/glfw/3.3.7/lib -lglfw -lvulkan

endif
ifeq ($(detected_OS),Linux)
	CXX_FLAGS += -D LINUX
	# CXX_LIBS += -lglfw
    CXX_APP_FLAGS += -lrt
	CXX_LIBS = -lrt -lglfw -lvulkan
endif


APP=main
apps:= App.Server App.FileNotifier App.Graphics#App.Client
tests:= Test.Yolo Test.Array Test.Range
all: $(apps) $(tests)

# std_headers:
# 	$(GCC) -xc++-system-header iostream

# Delta.Concepts: 

Delta.pcm: Delta.cpp Delta.String.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.String.pcm: Delta.String.cpp Delta.Array.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Array.pcm: Delta.Array.cpp Delta.Range.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Range.pcm: Delta.Range.cpp Delta.Iterator.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Iterator.pcm: Delta.Iterator.cpp Delta.Common.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Common.pcm: Delta.Common.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

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






Test.%: Test.%.o 
	$(CXX) $(CXX_FLAGS) $< -o $@ $(CXX_APP_FLAGS)

Test.%.o: Test.%.cpp Delta.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@

App.%: App.%.o 
	$(CXX) $(CXX_FLAGS) $< -o $@ $(CXX_INCLUDES) $(CXX_LIBS)

App.%.o: App.%.cpp Delta.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -o $@

# Test.%: Test.%.cpp delta
# 	$(GCC) $< *.o -o $@ -lrt -lpthread
	

clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f *.pcm
	@rm -f $(apps)
	@rm -f $(tests)
