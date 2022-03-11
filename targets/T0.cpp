import Delta;

#include <aio.h>
// #include <stdio.h>
// #include <errno.h>

auto write (int fd, auto* src, auto len) -> void
	{
		
		struct aiocb op 
		{
			
		};

		op.aio_fildes = fd;
		op.aio_offset = 0;
		op.aio_buf = (void*) src; 
		op.aio_nbytes = len * sizeof (char);

		if (aio_write (&op) != 0) 
		{
			// printf ("")
		}
	}

auto main (int, char**) -> int 
{
	// write (1, "hello world", 12);
	auto fd = 1;
	auto len = 12;
	auto src = "hello world";
	struct aiocb op 
		{
			
		};

		op.aio_fildes = fd;
		op.aio_offset = 0;
		op.aio_buf = (void*) src; 
		op.aio_nbytes = len * sizeof (char);

		if (aio_write (&op) != 0) 
		{
			// printf ("")
		}

	return 0;
}