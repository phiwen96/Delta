// module;
// #include <utility>
export module Delta.Iterator;
export import Delta.Common;



export struct iterator_tag
{
	struct INPUT
	{
	};
	struct OUTPUT
	{
	};
	struct FORWARD
	{
	};
	struct BIDIRECTIONAL
	{
	};
	struct RANDOM_ACCESS
	{
	};
	struct CONTIGUOUS
	{
	};
};

export template <typename T>
concept IteratorTraits = requires
{
	typename T::element_type;
	typename T::iterator_type;
};

export template <typename... T>
struct iterator_traits_t;

export template <typename T>
concept HasDefinedIteratorTraits = IteratorTraits<iterator_traits_t<T>>;

export template <typename A, typename B, typename C>
struct iterator_traits_t<A, B, C>
{
	using iterator_tag = A;
	using iterator_type = B;
	using element_type = C;
};

export template <typename T>
using defer = decltype(*mimic<T>());

// static_assert (Char<defer<char const*>>);

export template <typename T>
concept ReadOnly = requires(T t)
{
	*t;
};

export template <typename T>
concept WriteOnly = requires(T t, defer<T> u)
{
	*t = u;
};

export template <typename T>
concept StepForward = requires(T t)
{
	++t;
	t++;
};

export template <typename T>
concept StepBackward = requires(T t)
{
	--t;
	t--;
};

export template <typename T>
concept Jump = requires(T t)
{
	t + 10;
	t += 10;

	t - 10;
	t -= 10;
};

export template <typename T>
concept InputIterator = ReadOnly<T> and StepForward<T>;

export template <typename T>
concept OutputIterator = WriteOnly<T> and StepForward<T>;

export template <typename T>
concept ForwardIterator = InputIterator<T> and WriteOnly<T>;

export template <typename T>
concept BidirectionalIterator = ForwardIterator<T> and StepBackward<T>;

export template <typename T>
concept RandomAccessIterator = BidirectionalIterator<T> and Jump<T>;

export template <typename T>
struct is_contiguous_iterator_t
{
	constexpr static auto result = false;
};

export template <typename T>
constexpr auto is_contiguous_iterator = is_contiguous_iterator_t<T>::result;

export template <typename T>
concept ContiguousIterator = RandomAccessIterator<T> and is_contiguous_iterator<T>;

export template <typename T>
concept Iterator = InputIterator<T> or OutputIterator<T> or ForwardIterator<T> or BidirectionalIterator<T> or RandomAccessIterator<T> or ContiguousIterator<T>;

export template <typename...>
struct element_type_t;

export template <Iterator T>
struct element_type_t<T>
{
	using result = defer<T>;
};

export template <typename T>
using element_type = typename element_type_t<T>::result;

export template <typename...>
struct sentinel_value_t;

// export template <typename T>
// concept HasDefinedSentinelTraits = SentinelTraits <sentinel_value_t <T>>;

// static_assert (HasDefinedSentinelTraits <char const*>);

export template <typename T>
concept Sentinel = Iterator<T> and requires {sentinel_value_t <T>::value;};