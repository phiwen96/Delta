GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
APP=main
apps:= App.Server App.Client
tests:= Test.Yolo Test.Async.Out
all: $(apps) $(tests)

std_headers:
	$(GCC) -xc++-system-header iostream

# Delta.Concepts: 

delta: Delta.Concepts.Convertible.cpp Delta.Concepts.Same.cpp Delta.Types.IfElse.cpp Delta.Types.List.cpp Delta.Types.Predicate.cpp Delta.Types.BinaryPredicate.cpp Delta.Types.AnyOf.cpp Delta.Types.AllOf.cpp Delta.Types.Transform.cpp Delta.Types.cpp Delta.Mimic.cpp Delta.Concepts.Bool.cpp Delta.Concepts.Number.Floating.cpp Delta.Concepts.Number.Integer.cpp Delta.Concepts.Number.Signed.cpp Delta.Concepts.Number.Unsigned.cpp Delta.Concepts.Number.cpp Delta.Concepts.Size.cpp Delta.Concepts.Pointer.Traits.cpp Delta.Concepts.Pointer.cpp Delta.Concepts.Class.cpp Delta.Concepts.Function.Traits.Params.cpp Delta.Concepts.Function.Traits.cpp Delta.Concepts.Function.cpp Delta.Concepts.Char.cpp Delta.Concepts.Iterator.cpp Delta.Concepts.Range.cpp Delta.Concepts.Array.cpp Delta.Concepts.String.cpp Delta.Concepts.Future.cpp Delta.Concepts.cpp Delta.Algorithms.CopyPaste.cpp Delta.Algorithms.cpp Delta.Async.Task.cpp Delta.Async.Write.cpp Delta.Async.Out.cpp Delta.Async.cpp Delta.Net.cpp Delta.cpp
	$(GCC) -c Delta.Concepts.Convertible.cpp
	$(GCC) -c Delta.Concepts.Same.cpp 
	$(GCC) -c Delta.Types.IfElse.cpp
	$(GCC) -c Delta.Types.List.cpp
	$(GCC) -c Delta.Types.Predicate.cpp
	$(GCC) -c Delta.Types.BinaryPredicate.cpp
	$(GCC) -c Delta.Types.AnyOf.cpp
	$(GCC) -c Delta.Types.AllOf.cpp
	$(GCC) -c Delta.Types.Transform.cpp
	$(GCC) -c Delta.Types.cpp
	$(GCC) -c Delta.Mimic.cpp
	$(GCC) -c Delta.Concepts.Bool.cpp 
	$(GCC) -c Delta.Concepts.Number.Floating.cpp
	$(GCC) -c Delta.Concepts.Number.Integer.cpp
	$(GCC) -c Delta.Concepts.Number.Signed.cpp
	$(GCC) -c Delta.Concepts.Number.Unsigned.cpp
	$(GCC) -c Delta.Concepts.Number.cpp
	$(GCC) -c Delta.Concepts.Size.cpp
	$(GCC) -c Delta.Concepts.Pointer.Traits.cpp
	$(GCC) -c Delta.Concepts.Pointer.cpp
	$(GCC) -c Delta.Concepts.Class.cpp
	$(GCC) -c Delta.Concepts.Function.Traits.Params.cpp
	$(GCC) -c Delta.Concepts.Function.Traits.cpp
	$(GCC) -c Delta.Concepts.Function.cpp
	$(GCC) -c Delta.Concepts.Char.cpp
	$(GCC) -c Delta.Concepts.Iterator.cpp
	$(GCC) -c Delta.Concepts.Range.cpp
	$(GCC) -c Delta.Concepts.Array.cpp
	$(GCC) -c Delta.Concepts.String.cpp
	$(GCC) -c Delta.Concepts.Future.cpp
	$(GCC) -c Delta.Concepts.cpp
	$(GCC) -c Delta.Algorithms.CopyPaste.cpp
	$(GCC) -c Delta.Algorithms.cpp
	$(GCC) -c Delta.Async.Task.cpp
	$(GCC) -c Delta.Async.Write.cpp
	$(GCC) -c Delta.Async.Out.cpp
	$(GCC) -c Delta.Async.cpp
	$(GCC) -c Delta.Net.TLS.cpp
	$(GCC) -c Delta.Net.cpp
	$(GCC) -c Delta.cpp

# App.Server: App.Server.cpp delta std_headers
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

# App.Client: App.Client.cpp delta 
# 	$(GCC) $< *.o -o $@ -lrt -lpthread

App.%: App.%.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread

Test.%: Test.%.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread
	

clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f $(apps)
	@rm -f $(tests)
