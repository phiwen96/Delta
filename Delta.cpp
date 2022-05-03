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

// T *, T *&, T *&&, T const *, T const *&, T *const, T *const&, T const *const
// static_assert (String <char*>);
static_assert (Sentinel <char*>);
static_assert (Sentinel <char*&>);
static_assert (Sentinel <char*&&>);
static_assert (Sentinel <char const*>);
static_assert (Sentinel <char const*&>);
static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <non_pointer_const_types, char_types>>);
// static_assert (Sentinel <char* const>);
// static_assert (Sentinel <char* const>&);
// static_assert (Sentinel <char const * const>);
// static_assert (AllOf <[]<typename T>{return Sentinel <T>;}, product_type <pointer_types, char_types>>);
// static_assert (not Array <char*>);
