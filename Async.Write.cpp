module;
#include <stdio.h>
#include "aio.h"
#include "errno.h"
export module Delta.Async.Write;

import Delta.String;
import Delta.Range;

export namespace async
{
	auto write (int fd, String auto const& src)
	{
		struct aiocb op
		{
		};

		op.aio_fildes = fd;
		op.aio_offset = 0;
		op.aio_buf = (void *)src;
		op.aio_nbytes = length (src) * sizeof(char);

		if (aio_write(&op) != 0)
		{
			perror("aio_write");
		}
	}
}
