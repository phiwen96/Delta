GCC=g++-12 -std=gnu++2a -fcoroutines -fmodules-ts -fconcepts-diagnostics-depth=1
APP=main
apps:= App.Server App.Client
tests:= Test.Yolo Test.Async.Out
all: $(apps) $(tests)

std_headers:
	$(GCC) -xc++-system-header iostream

# Delta.Concepts: 

delta: Delta.Convertible.cpp Delta.Same.cpp Delta.Types.IfElse.cpp Delta.Types.List.cpp Delta.Types.Predicate.cpp Delta.Types.BinaryPredicate.cpp Delta.Types.AnyOf.cpp Delta.Types.AllOf.cpp Delta.Types.Transform.cpp Delta.Types.cpp Delta.Mimic.cpp Delta.Bool.cpp Delta.Number.Floating.cpp Delta.Number.Integer.cpp Delta.Number.Signed.cpp Delta.Number.Unsigned.cpp Delta.Number.cpp Delta.Size.cpp Delta.Pointer.cpp Delta.Class.cpp Delta.Function.cpp Delta.Char.cpp Delta.Iterator.cpp Delta.Range.cpp Delta.Array.cpp Delta.String.cpp Delta.Future.cpp Delta.cpp
	$(GCC) -c Delta.Convertible.cpp
	$(GCC) -c Delta.Same.cpp 
	$(GCC) -c Delta.Types.IfElse.cpp
	$(GCC) -c Delta.Types.List.cpp
	$(GCC) -c Delta.Types.Predicate.cpp
	$(GCC) -c Delta.Types.BinaryPredicate.cpp
	$(GCC) -c Delta.Types.AnyOf.cpp
	$(GCC) -c Delta.Types.AllOf.cpp
	$(GCC) -c Delta.Types.Transform.cpp
	$(GCC) -c Delta.Types.cpp
	$(GCC) -c Delta.Mimic.cpp
	$(GCC) -c Delta.Bool.cpp 
	$(GCC) -c Delta.Number.Floating.cpp
	$(GCC) -c Delta.Number.Integer.cpp
	$(GCC) -c Delta.Number.Signed.cpp
	$(GCC) -c Delta.Number.Unsigned.cpp
	$(GCC) -c Delta.Number.cpp
	$(GCC) -c Delta.Size.cpp
	$(GCC) -c Delta.Pointer.cpp
	$(GCC) -c Delta.Class.cpp
	$(GCC) -c Delta.Function.cpp
	$(GCC) -c Delta.Char.cpp
	$(GCC) -c Delta.Iterator.cpp
	$(GCC) -c Delta.Range.cpp
	$(GCC) -c Delta.Array.cpp
	$(GCC) -c Delta.String.cpp
	$(GCC) -c Delta.Future.cpp
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
