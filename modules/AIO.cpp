export module Delta.AIO;

export import Delta.String;

#include <aio.h>
#include <stdio.h>
#include <errno.h>



// export
// {
	// inline auto write (int fd, auto* src, auto len)
	// {
		
	// 	struct aiocb op 
	// 	{
			
	// 	};

	// 	op.aio_fildes = fd;
	// 	op.aio_offset = 0;
	// 	op.aio_buf = (void*) src; 
	// 	op.aio_nbytes = len * sizeof (char);

	// 	if (aio_write (&op) != 0) 
	// 	{
	// 		// printf ("")
	// 	}
	// }

// }