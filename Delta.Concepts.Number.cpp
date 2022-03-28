export module Delta.Concepts.Number;

export import Delta.Concepts.Number.Floating;
export import Delta.Concepts.Number.Integer;
export import Delta.Concepts.Number.Signed;
export import Delta.Concepts.Number.Unsigned;

export template <typename T>
concept Number = Signed <T> or Unsigned <T>;