#include "SpatialHash.h"



void SpatialHash::insert(Drop& a) {
	size_t x = hasingFun(a);

	v.at(x).push_back(a);
}

void SpatialHash::fill(std::vector<Drop>& water) {
	v.clear();
	v.resize(bucket);

	std::for_each(water.begin(), water.end(), [this](Drop& x) {cellID_fill(x); insert(x); });

}
void SpatialHash::fill(std::span<Drop> water) {//version of function with span structure
	v.clear();
	v.resize(bucket);

	std::for_each(water.begin(), water.end(), [this](Drop& x) {cellID_fill(x); insert(x); });

}


size_t SpatialHash::hasingFun(Drop a) {
	size_t i = static_cast<size_t> (a.get_cellID_row());
	size_t j = static_cast<size_t> (a.get_cellID_column());

	return ((i * 129285928214159ULL) ^ (j * 109413475968683ULL)) % bucket; //variable su pomnozene sa sto vecim prostim brojem
}

size_t SpatialHash::hasingFun(unsigned a, unsigned b) {
	return ((a * 129285928214159ULL) ^ (b * 109413475968683ULL)) % bucket;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////triba optimizirat fun jer je trnutno O(2n), a moze bit O(2nlog) ili O(2*1)!!!!!!!
void SpatialHash::cellID_fill(Drop& a) {//funkcija prepoznaje gdje se objekt nalazi na gridu i dodjeljuje id objektu
	unsigned cellSize = static_cast<int> (6 * a.get_radius());//size of cell is made so 3 stacked drops can fit. not too much so it lowers performance and not to small to worsen detection 
	unsigned id = 0;

	for (int i = cellSize; i <= 720; i += cellSize) {//720 je hardkodirana rezolucija
		if (a.get_posy() < i)
			break;
		id++;
	}
	a.set_cellID_row(id);


	id = 0;//sad za stupce
	for (int i = cellSize; i <= 1280; i += cellSize) {//1280 je hardkodirana rezolucija
		if (a.get_posx() < i)
			break;
		id++;
	}
	a.set_cellID_column(id);
}


void SpatialHash::colision_det(double dt) {//funkcija pregledava colision unutar celije, sljedece celije i donje celije

	for (int z = 0; z < v.size(); z++) {
		std::vector< std::reference_wrapper<Drop>>& x = v.at(z);

		for (int i = 0; i < x.size(); i++) {
			for (int j = i + 1; j < x.size(); j++) {
				dropColision(x.at(i), x.at(j), dt);


				if (z != v.size() - 1) {
					std::vector< std::reference_wrapper<Drop>>& x_nextCell = v.at(hasingFun(x.at(0).get().get_cellID_row(), x.at(0).get().get_cellID_column()) + 1); // ode se trazi susjedna celija na temelju trenutnog id-a trenutne celije i stavlja se u hashing fun tako da se nade bucket u kojem je spremljena
					if (!x_nextCell.empty() && i < x_nextCell.size())
						dropColision(x_nextCell.at(i), x.at(j), dt);
				}


				if (x.at(0).get().get_cellID_row() != 72) { //72 celija je zadnja celija i nemoze se ispod nje nista pregledat, ovaj x ima dodatni .get() zbog reference_wrapper!
					std::vector< std::reference_wrapper<Drop>>& x_lowerCell = v.at(hasingFun(x.at(0).get().get_cellID_row() + 1, x.at(0).get().get_cellID_column()));
					if (!x_lowerCell.empty() && i < x_lowerCell.size())
						dropColision(x_lowerCell.at(i), x.at(j), dt);
				}

			}
		}
	}


}

void SpatialHash::dropColision(Drop& a, Drop& b, double dt) {
	float r = a.get_radius() + 3;
	float distance = sqrt(pow((b.get_posx() - a.get_posx()), 2) + pow((b.get_posy() - a.get_posy()), 2));

	if (distance < r) {

		if (a.get_posx() > b.get_posx()) {
			a.add_posx(r / 2);
			b.add_posx(-r / 2);
		}
		else if (a.get_posx() <= b.get_posx()) {
			a.add_posx(-r / 2);
			b.add_posx(r / 2);
		}

		if (a.get_posy() > b.get_posy()) {
			b.set_posy(a.get_posy() - r / 2);
			b.velocity.at(1) = (b.get_mass() * b.get_velocity_y() + a.get_mass() * a.get_velocity_y()) / (a.get_mass() + b.get_mass()) * dt;  //formula for non elastic colision
		}

		else if (a.get_posy() <= b.get_posy()) {
			a.set_posy(b.get_posy() - r / 2);
			a.velocity.at(1) = (a.get_mass() * a.get_velocity_y() + b.get_mass() * b.get_velocity_y()) / (b.get_mass() + a.get_mass()) * dt;
		}
	}
}