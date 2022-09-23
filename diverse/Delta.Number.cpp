export module Delta:Number;

export import :Number.Floating;
export import :Number.Integer;
export import :Number.Signed;
export import :Number.Unsigned;

export template <typename T>
concept Number = Signed <T> or Unsigned <T>;