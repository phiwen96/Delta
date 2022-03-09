import Delta;

auto main (int, char**) -> int 
{
	delta::aio::write (1, "hello world");
	return 0;
}