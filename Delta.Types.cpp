export module Delta.Types;

// export import :List;
export import Delta.Types.IfElse;
export import Delta.Types.Predicate;
export import Delta.Types.List;
export import Delta.Types.AnyOf;
export import Delta.Types.AllOf;



// export import Types.List;
// export import Types.Transform;



import Delta.Same;

static_assert (not Same <char, typelist <char, int>::get <1>>);
static_assert (Same <typelist <int, double>, typelist <char, typelist <int, double>>::get <1>>);
static_assert (Same <char, typelist <char, int>::get <0>>);
static_assert (Same <int, typelist <char, int>::get <1>>);
static_assert (Same <int, typelist <char, int, bool>::get <1>>);

