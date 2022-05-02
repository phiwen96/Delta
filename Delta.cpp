export module Delta;

export import Delta.String;

// static_assert (Array <char[10]>);
// static_assert (Range <char[10]>);
// static_assert (AllOf <>);

static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int>>);
static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char>>);
static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int>>);
static_assert (AllOf <[]<typename T> {return Array <T>;}, array_types <char_types>>);
static_assert (AllOf <[]<typename T> {return String <T>;}, string_types>);
// static_assert (Same <array_types <>>)
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);