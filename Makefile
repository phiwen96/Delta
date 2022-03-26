GCC=g++-11 -std=c++2a -fmodules-ts
APP=main
all: main

std_headers:
	$(GCC) -xc++-system-header iostream

delta: Concepts.Convertible.cpp Concepts.Same.cpp Concepts.Size.cpp Concepts.Pointer.cpp Concepts.Array.cpp Concepts.Iterator.ReadOnly.cpp Concepts.Iterator.WriteOnly.cpp Concepts.Iterator.StepForward.cpp Concepts.Iterator.StepBackward.cpp Concepts.Iterator.Jump.cpp Concepts.Iterator.Input.cpp Concepts.Iterator.Output.cpp Concepts.Iterator.Forward.cpp Concepts.Iterator.Bidirectional.cpp Concepts.Iterator.RandomAccess.cpp Concepts.Iterator.Contiguous.cpp Concepts.Iterator.cpp Concepts.Range.cpp Concepts.Class.cpp Concepts.Char.cpp Concepts.String.cpp Concepts.Future.cpp Concepts.cpp Range.Begin.cpp Range.End.cpp Range.Length.cpp Range.cpp Algorithms.CopyPaste.cpp Algorithms.cpp Async.Write.cpp Async.Out.cpp Async.cpp Net.cpp Delta.cpp
	$(GCC) -c Concepts.Convertible.cpp
	$(GCC) -c Concepts.Same.cpp 
	$(GCC) -c Concepts.Size.cpp
	$(GCC) -c Concepts.Pointer.cpp
	$(GCC) -c Concepts.Array.cpp
	$(GCC) -c Concepts.Iterator.ReadOnly.cpp
	$(GCC) -c Concepts.Iterator.WriteOnly.cpp
	$(GCC) -c Concepts.Iterator.StepForward.cpp
	$(GCC) -c Concepts.Iterator.StepBackward.cpp
	$(GCC) -c Concepts.Iterator.Jump.cpp
	$(GCC) -c Concepts.Iterator.Input.cpp
	$(GCC) -c Concepts.Iterator.Output.cpp
	$(GCC) -c Concepts.Iterator.Forward.cpp
	$(GCC) -c Concepts.Iterator.Bidirectional.cpp
	$(GCC) -c Concepts.Iterator.RandomAccess.cpp
	$(GCC) -c Concepts.Iterator.Contiguous.cpp
	$(GCC) -c Concepts.Iterator.cpp
	$(GCC) -c Concepts.Range.cpp
	$(GCC) -c Concepts.Class.cpp
	$(GCC) -c Concepts.Char.cpp
	$(GCC) -c Concepts.String.cpp
	$(GCC) -c Concepts.Future.cpp
	$(GCC) -c Concepts.cpp
	$(GCC) -c Range.Begin.cpp
	$(GCC) -c Range.End.cpp
	$(GCC) -c Range.Length.cpp
	$(GCC) -c Range.Elements.cpp
	$(GCC) -c Range.cpp
	$(GCC) -c Algorithms.CopyPaste.cpp
	$(GCC) -c Algorithms.cpp
	$(GCC) -c Async.Write.cpp
	$(GCC) -c Async.Out.cpp
	$(GCC) -c Async.cpp
	$(GCC) -c Net.TLS.cpp
	$(GCC) -c Net.cpp
	$(GCC) -c Delta.cpp

main: main.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread

clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f $(APP)