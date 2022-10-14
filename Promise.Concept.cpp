export module Promise:Concept;

export template <typename T>
concept Promise = requires (T prom) {
	prom.get_return_object();
	prom.initial_suspend();
	prom.final_suspend();
	prom.unhandled_exception();
};