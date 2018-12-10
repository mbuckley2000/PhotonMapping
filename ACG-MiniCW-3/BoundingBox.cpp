#include "pch.h"
#include "BoundingBox.h"

BoundingBox::BoundingBox()
{
	this->minimum << 0, 0, 0;
	this->maximum << 0, 0, 0;
}

void BoundingBox::expand(BoundingBox b)
{
	for (int d = 0; d < 3; d++) {
		if (b.minimum[d] < this->minimum[d]) {
			this->minimum[d] = b.minimum[d];
		}
		if (b.maximum[d] > this->maximum[d]) {
			this->maximum[d] = b.maximum[d];
		}
	}
}

void BoundingBox::expand(Vec3 v)
{
	for (int d = 0; d < 3; d++) {
		if (v[d] < this->minimum[d]) {
			this->minimum[d] = v[d];
		}
		if (v[d] > this->maximum[d]) {
			this->maximum[d] = v[d];
		}
	}
}

int BoundingBox::longestDim()
{
	float longest = 0;
	int dim = 0;

	for (int d = 0; d < 3; d++) {
		const float length = maximum[d] - minimum[d];
		if (length > longest) {
			longest = length;
			dim = d;
		}
	}

	return dim;
}
