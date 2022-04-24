# GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
CXX = clang++-13
CXX_FLAGS = -std=c++2a
CXX_MODULES = -fmodules-ts -fmodules -fbuiltin-module-map -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=.
APP=main
apps:= #App.Server App.Client
tests:= Test.Yolo Test.Async.Out
all: $(apps) $(tests)

# std_headers:
# 	$(GCC) -xc++-system-header iostream

# Delta.Concepts: 

# Delta.pcm: Delta.cpp
# 	$(CXX) $(CXX_FLAGS) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Iterator.pcm: Delta.Iterator.cpp Delta.Size.pcm Delta.Convertible.pcm Delta.Same.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Size.pcm: Delta.Size.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.pcm: Delta.Types.cpp Delta.Types.IfElse.pcm Delta.Types.Predicate.pcm Delta.Types.List.pcm Delta.Types.AnyOf.pcm Delta.Types.AllOf.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.AllOf.pcm: Delta.Types.AllOf.cpp Delta.Types.Predicate.pcm Delta.Types.List.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.AnyOf.pcm: Delta.Types.AnyOf.cpp Delta.Types.Predicate.pcm Delta.Types.List.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.List.pcm: Delta.Types.List.cpp Delta.Types.IfElse.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.Predicate.pcm: Delta.Types.Predicate.cpp Delta.Same.pcm
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Types.IfElse.pcm: Delta.Types.IfElse.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Convertible.pcm: Delta.Convertible.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Same.pcm: Delta.Same.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

Delta.Mimic.pcm: Delta.Mimic.cpp
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

# App.Server: App.Server.cpp delta std_headers
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

# App.Client: App.Client.cpp delta 
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

Delta.pcm: Delta.cpp Delta.Mimic.pcm Delta.Same.pcm Delta.Convertible.pcm Delta.Types.pcm Delta.Size.pcm Delta.Iterator.pcm 
	$(CXX) $(CXX_FLAGS) $(CXX_MODULES) $(addprefix -fmodule-file=, $(filter-out $<, $^)) -c $< -Xclang -emit-module-interface -o $@

App.%: App.%.cpp Delta.pcm
	$(CXX) $(CXX_FLAGS) $^ -o $@ -lrt -lpthread

Test.%: Test.%.cpp Delta.pcm
	$(CXX) $(CXX_FLAGS) $^ -o $@ -lrt -lpthread

# Test.%: Test.%.cpp delta
# 	$(GCC) $< *.o -o $@ -lrt -lpthread
	

clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f *.pcm
	@rm -f $(apps)
	@rm -f $(tests)
