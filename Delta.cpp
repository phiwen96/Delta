export module Delta;
export import :Types;
export import :Same;
export import :Convertible;
export import :Function;
export import :Future;
export import :Char;
export import :Pointer;
export import :Size;
export import :Iterator;
export import :Range;
export import :Array;
export import :String;

// export import Delta.Types;
// export import Delta.Mimic;
// export import Delta.Concepts;
// export import Delta.Async;
// export import Delta.Net;














static_assert (AllOf <[] <typename T> {return Array <T>;}, array_types <int, 10>>);
static_assert (AllOf <[] <typename T> {return Range <T>;}, array_types <int, 10>>);



static_assert (AllOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <typelist <int>>>>);
static_assert (AllOf <[]<typename T>{return Same <T, int>;}, typelist <int, typelist <int, int>>>);
static_assert (AllOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <int, int>, int>>);
static_assert (AllOf <[]<typename T>{return Same <T, int>;}, typelist <int, typelist <typelist <int, int>, int>>>);
static_assert (not AllOf <[]<typename T>{return Same <T, int>;}, typelist <int, typelist <int, int, char>>>);
static_assert (not AllOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <int, int, char>, int>>);
static_assert (not AllOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <int, char, int>, int>>);
static_assert (not AllOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <char, int, int>, int>>);
static_assert (not AllOf <[]<typename T>{return Same <T, int>;}, typelist <int, typelist <typelist <int, char>, int>>>);

static_assert (AnyOf <[]<typename T>{return Same <T, int>;}, typelist <char, int>>);
static_assert (AnyOf <[]<typename T>{return Same <T, int>;}, typelist <char, typelist <char, int>>>);
static_assert (AnyOf <[]<typename T>{return Same <T, int>;}, typelist <typelist <char, int>, char>>);
static_assert (AnyOf <[]<typename T>{return Same <T, int>;}, typelist <char, typelist <typelist <char, int>, char>>>);

