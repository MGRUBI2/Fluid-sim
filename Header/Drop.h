#pragma once
#include <array>
#include <SDL3/SDL.h>
#include <vector>
#include <ctype.h>



class Drop {
	std::array<float, 2> pos;
	float radius;
	std::array<float, 2> velocity;// u fizici oznaka "u"
	std::array<float, 2> acceleration;
	float mass;
	float force;
	float density;
	float viscosity;
	float pressure;
	float del;//Del Operator ????
	std::array<unsigned, 2> cellID;

public:
	Drop(float x, float y) : radius(5), mass(5) { pos[0] = x; pos[1] = y; velocity[0] = 0; velocity[1] = 0; acceleration[0] = 0; acceleration[1] = 0; }

	//void set_pos(float x, float y) { pos[0] = x; pos[1] = y; }
	void add_posx(float x) { pos[0] += x; }
	void add_posy(float x) { pos[1] += x; }
	void set_posx(float x) { pos[0] = x; }
	void set_posy(float x) { pos[1] = x; }
	void set_radius(float x) { radius = x; }
	void set_cellID_row(int id) { cellID[0] = id; }
	void set_cellID_column(int id) { cellID[1] = id; }

	unsigned get_cellID_row() const { return cellID[0]; }
	unsigned get_cellID_column() const { return cellID[1]; }


	std::array<float, 2>  get_pos() { return pos; }
	std::array<float, 2> get_a() { return acceleration; }
	float get_velocity_x() { return velocity.at(0); }
	float get_velocity_y() { return velocity.at(1); }
	float get_mass() { return mass; }
	float get_posx() { return pos[0]; }
	float get_posy() { return pos[1]; }
	float get_radius() { return radius; }

	friend class SpatialHash;



	void motionUpdate(double dt) {
		/*	velocity.at(0) += acceleration.at(0)*dt;
			velocity.at(1) += acceleration.at(1)*dt;

			pos.at(0) += velocity.at(0);
			pos.at(1) += velocity.at(1);*/

			//if (a.at(0)>0)//otpor zraka
			//	a.at(0)-=1* dt;
			//else
			//	a.at(0) += 1 * dt;
			//nisan stavia za y jer cu to rjesit priko gravitacije

	}



	void collision() {

	}

	void render(SDL_Renderer* renderer);
	void gravityEffect(double dt);
	std::vector<SDL_Vertex> drawDrop();
	void boundControl();


};