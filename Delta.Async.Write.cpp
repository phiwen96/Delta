module;
#include <stdio.h>
#include "aio.h"
#include "errno.h"
export module Delta.Async.Write;

import Delta.Concepts.String;

export namespace async
{
	/*
		TODO

		implement ContiguousRange, because "src" needs to be contiguous
	*/

	auto write (int fd, Range auto const& src)
	{
		// fflush (stdout);
		
		printf ("length: %ld\n", end (src) - begin (src));

		struct aiocb op
		{
		};

		op.aio_fildes = fd;
		op.aio_offset = 0;
		op.aio_buf = (void *)  & (*begin (src));
		op.aio_nbytes = ((long) (end (src) - begin (src))) * sizeof (element_type <decltype (src)>);

		if (aio_write (&op) != 0)
		{
			perror("aio_write");
		}

		auto status = aio_error (&op);

		if (status == EINPROGRESS)
		{
			printf ("not done\n");

		} else if (status != 0)
		{
			perror ("write");
		}

		const struct aiocb  * aiolist [1] = {&op};

		aio_suspend (aiolist, 1, NULL);
	}
}
