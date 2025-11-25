#pragma once
#include "Drop.h"
#include <algorithm>
#include <span>
#include <vector>



class SpatialHash {
	int bucket;
	std::vector<std::vector<std::reference_wrapper<Drop>>> v;

public:
	SpatialHash() :bucket(307) {} /*prost broj, velicina odabrana odokativno*/

	//std::vector<std::vector<Drop>> ret_vec() { return v; }

	void insert(Drop& a);
	void fill(std::vector<Drop>& water);
	void fill(std::span<Drop> water);
	size_t hasingFun(Drop a);
	size_t hasingFun(unsigned a, unsigned b);
	void cellID_fill(Drop& a);
	void colision_det(double dt);
	void dropColision(Drop& a, Drop& b, double dt);



};
