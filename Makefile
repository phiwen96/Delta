GCC=g++-11 -std=c++2a -fmodules-ts
APP=main
all: main
# build: std_headers mathlib main.o
# 	$(GCC) *.o -o $(APP) -lrt

std_headers:
	$(GCC) -xc++-system-header iostream

delta: Convertible_to.cpp Same_as.cpp Size.cpp Pointer.cpp Array.cpp Iterator.cpp Range.cpp Class.cpp Char.cpp Async.cpp String.cpp Future.cpp Delta.cpp
	$(GCC) -c Convertible_to.cpp
	$(GCC) -c Same_as.cpp
	$(GCC) -c Size.cpp
	$(GCC) -c Pointer.cpp
	$(GCC) -c Array.cpp
	$(GCC) -c Iterator.cpp
	$(GCC) -c Range.cpp
	$(GCC) -c Class.cpp
	$(GCC) -c Char.cpp
	$(GCC) -c Async.cpp
	$(GCC) -c String.cpp
	$(GCC) -c Future.cpp
	$(GCC) -c Delta.cpp

# async: imp/async.cpp async.cpp
# 	$(GCC) -c imp/async.cpp
# 	$(GCC) -c async.cpp


main: main.cpp delta
	$(GCC) $< *.o -o $@ -lrt

# main.o: main.cpp
# 	$(GCC) -c $<

clean:
	@rm -rf gcm.cache/
	@rm -f *.o
	@rm -f $(APP)