export module Delta;

export import Delta.Types;
export import Delta.Mimic;
export import Delta.Concepts;
export import Delta.Algorithms;
export import Delta.Async;
export import Delta.Net;

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

