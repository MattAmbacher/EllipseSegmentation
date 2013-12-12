#ifndef GUARD_ARCGROUP_H
#define GUARD_ARCGROUP_H

#include <vector>
#include "LineSegment.h"

class ArcGroup {
public:
	std::vector<LineSegment> LA;
	double xc;
	double yc;
	double R;

	ArcGroup(std::vector<LineSegment>, double, double, double);
};

std::vector<ArcGroup> ArcExtraction(std::vector<LineSegment>);

#endif
