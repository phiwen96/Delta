import Delta;

enum operation_code : uint_fast8_t {
	OP_RETURN,
	OP_UNDO,
	OP_CONSTANT,
	OP_ADD,
	OP_SUBTRACT,
	OP_MULTIPLY,
	OP_DIVIDE,
	OP_NEGATE
};

using operation_code_t = std::underlying_type_t <operation_code>;





struct value {
	union {
		bool boolean;
		double number;
		char character;
	} as;
	enum {
		VAL_BOOL,
		VAL_NUMBER,
		VAL_CHARACTER
	} type;
};

struct chunk {
	std::vector <operation_code_t> code;
	std::vector <value> values;

	auto add_instruction (operation_code_t opcode) noexcept -> chunk & {
		code.push_back (opcode);
		return *this;
	}

	// returns the index of the constant in values array
	auto add_constant (value v) noexcept -> operation_code_t {
		values.push_back (v);
		return values.size() - 1;
	}

	auto disassemble () noexcept -> void {
		for (auto offset = 0; offset < code.size();) {
			offset = disassemble_instruction (offset);
		}
	}

	private:

	auto disassemble_instruction (int offset) noexcept -> int {

		auto const opcode = code [offset];

		switch (opcode) {

			case OP_RETURN: {
				std::cout << "OP_RETURN" << std::endl;
				return offset + 1;
			}

			case OP_UNDO: {
				std::cout << "OP_UNDO" << std::endl;
				return offset + 1;
			}

			case OP_CONSTANT: {
				if (auto const & value = values [code [offset + 1]]; value.type == value::VAL_NUMBER)
					printf ("%-16s %4f \n", "OP_CONSTANT", values [code [offset + 1]].as.number);
				return offset + 2;
			}

			case OP_ADD: {
				std::cout << "OP_ADD" << std::endl;
				return offset + 1;
			}

			case OP_SUBTRACT: {
				std::cout << "OP_NEGATE" << std::endl;
				return offset + 1;
			}

			case OP_MULTIPLY: {
				std::cout << "OP_MULTIPLY" << std::endl;
				return offset + 1;
			}

			case OP_DIVIDE: {
				std::cout << "OP_MULTIPLY" << std::endl;
				return offset + 1;
			}
			
			case OP_NEGATE: {
				std::cout << "OP_NEGATE" << std::endl;
				return offset + 1;
			}
			
			default: {
				std::cout << "unknown operation code" << std::endl;
				return offset + 1;
			}
		}
	}
	// friend auto operator << (std::ostream& os, chunk const& c) noexcept -> std::ostream & {

	// }
};

template <int max_stack>
struct VM {

	enum Result {
		INTERPRET_OK,
		INTERPRET_COMPILE_ERROR,
		INTERPRET_RUNTIME_ERROR
	};

	auto interpret (chunk & c) noexcept -> Result {

		auto * ip = c.code.data();

		for (;;) {

			auto instruction = *ip++;

			switch (instruction) {
				
				case OP_RETURN : {
					return INTERPRET_OK;
					break;
				}

				case OP_CONSTANT: {
					auto constant = c.values[*ip++];
					push (constant);
					break;
				}

				case OP_ADD: {
					push ({.as.number = pop ().as.number + pop ().as.number, .type = value::VAL_NUMBER});
					break;
				}

				case OP_SUBTRACT: {
					push ({.as.number = pop ().as.number - pop ().as.number, .type = value::VAL_NUMBER});
					break;
				}

				case OP_MULTIPLY: {
					push ({.as.number = pop ().as.number * pop ().as.number, .type = value::VAL_NUMBER});
					break;
				}

				case OP_DIVIDE: {
					push ({.as.number = pop ().as.number / pop ().as.number, .type = value::VAL_NUMBER});
					break;
				}
				
				case OP_NEGATE: {
					push ({.as.number = -pop ().as.number, .type = value::VAL_NUMBER});
					break;
				}
			}
		}
	}

	auto push (value v) noexcept -> void {
		*top = v;
		++top;
	}

	auto pop () noexcept -> value {
		--top;
		return *top;
	}

	value stack [max_stack];
	value * top = stack;
};






auto main (int argc, char** argv) -> int {

	auto code = chunk {};
	code.add_instruction (OP_CONSTANT).add_instruction (code.add_constant ({.as.number = 2, .type = value::VAL_NUMBER}))
		.add_instruction (OP_CONSTANT).add_instruction (code.add_constant ({.as.number = 3, .type = value::VAL_NUMBER}))
		.add_instruction (OP_MULTIPLY)
		.add_instruction (OP_CONSTANT).add_instruction (code.add_constant ({.as.number = 1.2, .type = value::VAL_NUMBER}))
		.add_instruction (OP_ADD);
		// .add_instruction (OP_RETURN);

	code.disassemble ();

	VM<128> v {};
	v.interpret (code);
	// std::cout << v.pop().as.number << std::endl;
	for (auto * i = v.stack; i < v.top; ++i) {
		std::cout << i->as.number << std::endl;
	}

	return 0;

	return 0;
}