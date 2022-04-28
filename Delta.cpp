export module Delta;

export import Delta.String;

// static_assert (Array <char[10]>);
// static_assert (Range <char[10]>);

static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <int, 2>>);
static_assert (AllOf <[]<typename T>{return Array <T>;}, array_types <char, 2>>);
static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <int, 2>>);
// static_assert (AllOf <[]<typename T>{return Range <T>;}, array_types <char, 2>>);