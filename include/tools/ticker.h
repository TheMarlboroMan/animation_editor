#pragma once

namespace tools {

class ticker {

	public:

	void                tick(float);
	float               get() const {return elapsed;}

	private:

	float               elapsed{0.0f};
};

}
