export module Delta.Types;

// export import :List;
export import Delta.Types.IfElse;
export import Delta.Types.Predicate;
export import Delta.Types.BinaryPredicate;
export import Delta.Types.AnyOf;
export import Delta.Types.AllOf;
export import Delta.Types.List;


// export import Delta.Types.List;
// export import Delta.Types.Transform;



import Delta.Concepts.Same;

static_assert (Same <char, typelist <char, int>::get <0>>);
static_assert (Same <int, typelist <char, int>::get <1>>);
static_assert (Same <int, typelist <char, int, bool>::get <1>>);

