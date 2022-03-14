// module;
// #include <aio.h>
// #include <stdio.h>
// #include <errno.h>
// #include <fcntl.h>
export module Delta.AIO;

// import "aio.h";
#include <aio.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>

// export import Delta.String;





export
{
	inline auto async_write (int fd, auto* src, auto len) -> void
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

}