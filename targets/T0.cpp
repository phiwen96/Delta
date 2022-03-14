import Delta.AIO;

// #include <aio.h>
// #include <stdio.h>
// #include <errno.h>

// auto write (int fd, auto* src, auto len) -> void
// 	{
		
// 		struct aiocb op 
// 		{
			
// 		};

// 		op.aio_fildes = fd;
// 		op.aio_offset = 0;
// 		op.aio_buf = (void*) src; 
// 		op.aio_nbytes = len * sizeof (char);

// 		if (aio_write (&op) != 0) 
// 		{
// 			// printf ("")
// 		}
// 	}

auto main (int, char**) -> int 
{
	async_write (1, "hello world", 12);
	

	return 0;
}