export module Delta.Concepts.Range;

// export import Delta.Concepts.Range.Traits;
// import Delta.Concepts.Same;
// import Delta.Concepts.Convertible;
// import Delta.Mimic;
export import Delta.Concepts.Iterator;
export import Delta.Concepts.Range.Begin;
export import Delta.Concepts.Range.End;
export import Delta.Concepts.Array;
// export import Delta.Concepts.Range.Contiguous;

export template <typename T>
concept Range = 
requires (T t)
{
	{begin (t)} noexcept -> Iterator;
	{end (t)} noexcept -> Iterator;
};

// static_assert (Range <char [10]>);
// static_assert (Range <char const [10]>);
// static_assert (Range <char const (&) [10]>);
