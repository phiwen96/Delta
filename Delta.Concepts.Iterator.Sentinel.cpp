export module Delta.Concepts.Iterator.Sentinel;

export template <typename T>
struct sentinel_value_t{};

template <typename T>
concept HasSentinelValue = requires 
{
	// sentinel_value_t <T>::value;
	true;
};

export template <HasSentinelValue T>

requires 
{
	// sentinel_value_t <T>::value;
	// true;
}
constexpr auto sentinel_value = sentinel_value_t <T>::value;