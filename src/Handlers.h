#include "pugixml.hpp"

#define HANDLER_CALLBACK 0x01
#define HANDLER_ACTION 0x02
#define HANDLER_GENERIC 0x04
#define HANDLER_CONTAINER 0x08

class Solver;

class vHandler {
	public:
	int startIter;
	double everyIter;
	pugi::xml_node node;
	virtual int Init(Solver*);
	virtual int DoIt(Solver*);
	virtual int Finish(Solver*);
	virtual int Type();
	inline const bool Now(int iter) {
		if (everyIter) {
			iter -= startIter;
			return floor((iter)/everyIter) > floor((iter-1)/everyIter);
		} else return false;
	}
	inline const int Next(int iter) {
		if (everyIter) {
			iter -= startIter;
			int k = floor((iter)/everyIter);
			return - floor(-(k+1) * everyIter) - iter;
		} else return -1;
	}
	inline const int Prev(int iter) {
		if (everyIter) {
			iter -= startIter;
			int k = floor((iter-1)/everyIter);
			return iter + floor(-k * everyIter);
		} else return -1;
	}
};

vHandler * getHandler(pugi::xml_node);

class Handler {
private:
public:
	vHandler * hand;
	Solver * solver;
	int *ref;
//public:
	inline Handler(pugi::xml_node node, Solver * solver_) {
		solver = solver_;
		hand = getHandler(node);
		if (hand) {
			int ret = hand->Init(solver);
			if (ret) {
				delete hand;
				hand = NULL;
			}
		}
		ref = new int;
		*ref=1;
		DEBUG0(printf("H: create\n");)
	}
	inline Handler(const Handler & that) {
		solver = that.solver;
		hand = that.hand;
		ref = that.ref;
		(*ref)++;
		DEBUG0(printf("H: + %d\n", *ref);)
	}
	inline const int Init() { return hand->Init(solver); }
	inline const int DoIt() { return hand->DoIt(solver); }
	inline const int Type() { return hand->Type(); }
	inline const bool Now(int iter) { return hand->Now(iter); }
	inline const int Next(int iter) { return hand->Next(iter); }
	inline const int Prev(int iter) { return hand->Prev(iter); }
	inline Handler & operator=(const Handler & that) {
		hand = that.hand;
		ref = that.ref;
		(*ref)++;
		DEBUG0(printf("H: + %d\n", *ref);)
		return *this;
	}
	inline ~Handler() {
		(*ref)--;
		DEBUG0(printf("H: - %d\n", *ref);)
		if (ref <= 0) {
			if (hand) {
				hand->Finish(solver);
				delete hand;
			}
			delete ref;
		}
	}
	inline operator bool () { return hand != NULL; }
};



