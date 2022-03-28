GCC=g++-11 -std=c++2a -fmodules-ts
APP=main
apps: App.Client App.Server
all: apps

std_headers:
	$(GCC) -xc++-system-header iostream

delta: Delta.Concepts.Convertible.cpp Delta.Concepts.Same.cpp Delta.Concepts.Number.Floating.cpp Delta.Concepts.Number.Integer.cpp Delta.Concepts.Number.Signed.cpp Delta.Concepts.Number.Unsigned.cpp Delta.Concepts.Size.cpp Delta.Concepts.Pointer.cpp Delta.Concepts.Array.cpp Delta.Concepts.Iterator.ReadOnly.cpp Delta.Concepts.Iterator.WriteOnly.cpp Delta.Concepts.Iterator.StepForward.cpp Delta.Concepts.Iterator.StepBackward.cpp Delta.Concepts.Iterator.Jump.cpp Delta.Concepts.Iterator.Input.cpp Delta.Concepts.Iterator.Output.cpp Delta.Concepts.Iterator.Forward.cpp Delta.Concepts.Iterator.Bidirectional.cpp Delta.Concepts.Iterator.RandomAccess.cpp Delta.Concepts.Iterator.Contiguous.cpp Delta.Concepts.Iterator.cpp Delta.Concepts.Range.Begin.cpp Delta.Concepts.Range.End.cpp Delta.Concepts.Range.Contiguous.cpp Delta.Concepts.Range.cpp Delta.Concepts.Class.cpp Delta.Concepts.Char.cpp Delta.Concepts.String.cpp Delta.Concepts.Future.cpp Delta.Concepts.cpp Delta.Range.Begin.cpp Delta.Range.End.cpp Delta.Range.Length.cpp Delta.Range.cpp Delta.Algorithms.CopyPaste.cpp Delta.Algorithms.cpp Delta.Async.Write.cpp Delta.Async.Out.cpp Delta.Async.cpp Delta.Net.cpp Delta.cpp
	$(GCC) -c Delta.Concepts.Convertible.cpp
	$(GCC) -c Delta.Concepts.Same.cpp 
	$(GCC) -c Delta.Concepts.Number.Floating.cpp
	$(GCC) -c Delta.Concepts.Number.Integer.cpp
	$(GCC) -c Delta.Concepts.Number.Signed.cpp
	$(GCC) -c Delta.Concepts.Number.Unsigned.cpp
	$(GCC) -c Delta.Concepts.Number.cpp
	$(GCC) -c Delta.Concepts.Size.cpp
	$(GCC) -c Delta.Concepts.Pointer.cpp
	$(GCC) -c Delta.Concepts.Array.cpp
	$(GCC) -c Delta.Concepts.Iterator.ReadOnly.cpp
	$(GCC) -c Delta.Concepts.Iterator.WriteOnly.cpp
	$(GCC) -c Delta.Concepts.Iterator.StepForward.cpp
	$(GCC) -c Delta.Concepts.Iterator.StepBackward.cpp
	$(GCC) -c Delta.Concepts.Iterator.Jump.cpp
	$(GCC) -c Delta.Concepts.Iterator.Input.cpp
	$(GCC) -c Delta.Concepts.Iterator.Output.cpp
	$(GCC) -c Delta.Concepts.Iterator.Forward.cpp
	$(GCC) -c Delta.Concepts.Iterator.Bidirectional.cpp
	$(GCC) -c Delta.Concepts.Iterator.RandomAccess.cpp
	$(GCC) -c Delta.Concepts.Iterator.Contiguous.cpp
	$(GCC) -c Delta.Concepts.Iterator.cpp
	$(GCC) -c Delta.Concepts.Range.Begin.cpp
	$(GCC) -c Delta.Concepts.Range.End.cpp
	$(GCC) -c Delta.Concepts.Range.Contiguous.cpp
	$(GCC) -c Delta.Concepts.Range.cpp
	$(GCC) -c Delta.Concepts.Class.cpp
	$(GCC) -c Delta.Concepts.Char.cpp
	$(GCC) -c Delta.Concepts.String.cpp
	$(GCC) -c Delta.Concepts.Future.cpp
	$(GCC) -c Delta.Concepts.cpp
	$(GCC) -c Delta.Range.Begin.cpp
	$(GCC) -c Delta.Range.End.cpp
	$(GCC) -c Delta.Range.Length.cpp
	$(GCC) -c Delta.Range.Elements.cpp
	$(GCC) -c Delta.Range.cpp
	$(GCC) -c Delta.Algorithms.CopyPaste.cpp
	$(GCC) -c Delta.Algorithms.cpp
	$(GCC) -c Delta.Async.Write.cpp
	$(GCC) -c Delta.Async.Out.cpp
	$(GCC) -c Delta.Async.cpp
	$(GCC) -c Delta.Net.TLS.cpp
	$(GCC) -c Delta.Net.cpp
	$(GCC) -c Delta.cpp

main: main.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread

App.Client: App.Client.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread

App.Server: App.Server.cpp delta
	$(GCC) $< *.o -o $@ -lrt -lpthread





clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f $(APP)