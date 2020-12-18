#include "tools/ticker.h"

using namespace tools;

void ticker::tick(

	float _delta
) {

	elapsed+=_delta;

	//Each hour this value wraps back up... it would take a lot to wrap up to the
	//negative space, but well...
	if(elapsed >= 3600.f) {

		elapsed -= 3600.f;
	}
}
