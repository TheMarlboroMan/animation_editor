#pragma once

namespace tools {

class ticker {

	public:

	void                tick(double);
	double              get() const {return elapsed;}

	private:

	double              elapsed{0.0};
};

}
