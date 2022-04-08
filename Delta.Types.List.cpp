export module Delta.Types.List;

export template <typename... T>
struct typelist 
{
	template <template <typename...> typename U>
	using transform = U <T...>;
};




// import Delta.Types.List.Traits;

// export template <typename T>
// concept Typelist = TypelistTraits <typelist_traits <T>>;