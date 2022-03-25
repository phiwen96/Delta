export module Delta.Concepts.Range.Iterator;



export 
{
	template <typename T>
	concept Input_iterator = requires (T t)
	{
		++t;
		t++;
		*t;
	};

	template <typename T>
	concept Output_iterator = requires (T t, decltype (*t) u)
	{
		++t;
		t++;
		*t = u;
	};
	template <typename T>
	concept Iterator = 
		Input_iterator <T> or 
		Output_iterator <T>;

	auto begin (auto& t) noexcept -> Iterator auto 
	requires requires 
	{
		{t.begin ()} -> Iterator;
	}
	{
		return t.begin ();
	}

	auto begin (auto const& t) noexcept -> Iterator auto 
	requires requires 
	{
		{t.begin ()} -> Iterator;
	}
	{
		return t.begin ();
	}

	auto begin (auto const* t) noexcept -> Iterator auto 
	{
		return t;
	}

	

	auto end (auto& t) noexcept -> Iterator auto 
	requires requires 
	{
		{t.end ()} -> Iterator;
	}
	{
		return t.end ();
	}

	auto end (auto const& t) noexcept -> Iterator auto 
	requires requires 
	{
		{t.end ()} -> Iterator;
	}
	{
		return t.end ();
	}
		// Forward <T> or 
		// Bidirectional <T> or
		// Random_access <T>;
}
	