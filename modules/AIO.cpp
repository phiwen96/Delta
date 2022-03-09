export module Delta.AIO;

import Delta.String;

#include <aio.h>
#include <stdio.h>
#include <errno.h>



export namespace delta::aio
{
	inline auto write (int fd, char const* src)
	{
		struct aiocb op 
		{
			
		};

		op.aio_fildes = fd;
		op.aio_offset = 0;
		op.aio_buf = (void*) src; 
		// op.aio_nbytes = strlen (src) * sizeof (char);

		// if (aio_write (&op) != 0) 
		// {
		// 	// printf ("")
		// }
	}
}