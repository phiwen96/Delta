module;
#include <stdio.h>
#include "aio.h"
#include "errno.h"
#include <signal.h>
#include <coroutine>
export module Delta.Async.Write;

import Delta.Async.Task;
import Delta.Concepts.String;

export namespace async
{
	auto aio_handler (int sig, siginfo_t *si, void *ucontext) noexcept -> void 
	{
		
		if (si->si_code != SI_ASYNCIO) 
		{
			perror ("whaaaat");
		}

		auto* cbp = (aiocb*) si->si_value.sival_ptr;

        // write(STDOUT_FILENO, "I/O completion signal received\n", 31);


        /* The corresponding ioRequest structure would be available as
               struct ioRequest *ioReq = si->si_value.sival_ptr;
           and the file descriptor would then be available via
               ioReq->aiocbp->aio_fildes */
    } 

	/*
		TODO

		implement ContiguousRange, because "src" needs to be contiguous
	*/
	template <typename T, auto N>
	auto write (int fd, T const (&src) [N]) -> task <bool>
	{

		auto op = aiocb
		{
			.aio_fildes = fd,
			.aio_buf = (void *)  & (*begin (src)),
			.aio_nbytes = ((long) (end (src) - begin (src))) * sizeof (element_type <decltype (src)>),
			.aio_sigevent = {.sigev_notify = SIGEV_THREAD},
			.aio_offset = 0
			// .aio_sigevent.sigev_notify_function = aio_handler
		};

		op.aio_sigevent.sigev_value.sival_ptr = &op;

		// fflush (stdout);

		if (aio_write (&op) != 0)
		{
			perror ("write");
		}

		co_return;

		// auto status = aio_error (&op);

		// if (status == EINPROGRESS)
		// {
		// 	printf ("not done\n");

		// } else if (status != 0)
		// {
		// 	perror ("write");
		// }

		// const struct aiocb  * aiolist [1] = {&op};

		// aio_suspend (aiolist, 1, NULL);
	}
}
