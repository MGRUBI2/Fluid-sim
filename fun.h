#include <span>
#include <vector>
#include <iostream>
#include <array>
#include <queue>
#include <SDL3/SDL.h>
#include <iterator>
#include <algorithm>
#include <functional>
#include <thread>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <mutex>
#include <condition_variable>



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
	Drop(float x, float y) : radius(5) { pos[0] = x; pos[1] = y; velocity[0] = 0; velocity[1] = 0; acceleration[0] = 0; acceleration[1] = 0; }

	void add_u(float x, float y) { velocity.at(0) += x; velocity.at(1) += y; }
	void add_a(float x, float y) { acceleration.at(0) += x; acceleration.at(1) += y; }
	//void set_pos(float x, float y) { pos[0] = x; pos[1] = y; }
	void add_posx(float x) { pos[0] += x; } 
	void add_posy(float x) { pos[1] += x; }
	void set_posx(float x) { pos[0] = x; }
	void set_posy(float x) { pos[1] = x; }
	void set_radius(float x) { radius = x; }
	void set_cellID_row(int id) { cellID[0] = id; }
	void set_cellID_column(int id) { cellID[1] = id; }

	unsigned get_cellID_row() { return cellID[0]; }
	unsigned get_cellID_column() { return cellID[1]; }


	std::array<float, 2>  get_pos() { return pos; }
	std::array<float, 2> get_a() { return acceleration; }
	float get_posx() { return pos[0]; }
	float get_posy() { return pos[1]; }
	float get_radius() { return radius; }

	friend class Spatial_hash;

	void render(SDL_Renderer* renderer) {
		std::vector<SDL_Vertex> v=drawDrop();
		/*std::vector<SDL_Vertex> v= midPointAlg();*/


		//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		//SDL_RenderPoints(renderer, arr, v.size());
		//SDL_RenderLines(renderer, v.data(), v.size());//zaminit sa SDL_RenderGeometry()
		SDL_RenderGeometry(renderer, NULL, v.data(), v.size(), NULL, 0);
	}

	void motionUpdate(double dt) {
		/*u.at(0) += a.at(0);
		u.at(1) += a.at(1);

		pos.at(0) += u.at(0);
		pos.at(1) += u.at(1);*/

		//if (a.at(0)>0)//otpor zraka
		//	a.at(0)-=1* dt;
		//else
		//	a.at(0) += 1 * dt;
		//nisan stavia za y jer cu to rjesit priko gravitacije

	}

	void gravityEffect(double dt) {//vjv temp
		velocity.at(1) += 100 * dt;

		pos.at(1) += velocity[1];
	}

	void collision() {

	}

	std::vector<SDL_Vertex> drawDrop() {
		std::vector<SDL_Vertex> v1;
		SDL_Color color = { 255,255,255,255 };
		SDL_FPoint t = { 0.0f,0.0f };

		v1.push_back({pos.at(0),pos.at(1),255,255,255,255});
		v1.push_back({ pos.at(0)+radius,pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0),pos.at(1)-radius,255,255,255,255 });

		v1.push_back({ pos.at(0),pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0) + radius,pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0),pos.at(1) + radius,255,255,255,255 });

		v1.push_back({ pos.at(0),pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0) - radius,pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0),pos.at(1) - radius,255,255,255,255 });

		v1.push_back({ pos.at(0),pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0) - radius,pos.at(1),255,255,255,255 });
		v1.push_back({ pos.at(0),pos.at(1) + radius,255,255,255,255 });
	

		return v1;
	}

	//std::vector<SDL_Vertex> midPointAlg() {//vraca vector struktura priko kojih cemo crtat krug
	//	int x = r, y = 0;

	//	std::vector<SDL_Vertex> v1;

	//	v1.push_back({ {x + pos.at(0),y + pos.at(1)} , {255, 255, 255, 255 },{0.0f, 0.0f} });
	//	v1.push_back({ {x + pos.at(0),-y + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f} });
	//	v1.push_back({ { y + pos.at(0),x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f} });
	//	v1.push_back({ { -y + pos.at(0),x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f} });


	//	int p = 1 - r;
	//	while (x > y) {
	//		y++;

	//		if (p <= 0)
	//			p = p + 2 * y + 1;
	//		else {
	//			x--;
	//			p = p + 2 * y - 2 * x + 1;
	//		}

	//		if (x < y)
	//			break;

	//		v1.push_back({ {x + pos.at(0),y + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f }});
	//		v1.push_back({ {- x + pos.at(0),y + pos.at(1)}, {255, 255, 255, 255}, {0.0f, 0.0f} });
	//		v1.push_back({ {x + pos.at(0),-y + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f } });
	//		v1.push_back({ {-x + pos.at(0),-y + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f } });


	//		if (x != y) {
	//			v1.push_back({{ y + pos.at(0),x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f }});
	//			v1.push_back({{ -y + pos.at(0),x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f }});
	//			v1.push_back({{ y + pos.at(0),-x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f }});
	//			v1.push_back({{ -y + pos.at(0),-x + pos.at(1) }, { 255, 255, 255, 255 }, { 0.0f, 0.0f } });

	//		}

	//	}
	//	return v1;
	//}

	void boundControl() {
		if (pos[0] > 1280)
			pos[0] = 1280;
		if (pos[0] < 0)
			pos[0] = 0;
		if (pos[1] > 720){
			pos[1] = 720;
			velocity.at(1) *= -0.25;
		}			
		if (pos[1] < 0) {
			pos[1] = 0;
			velocity.at(1) *= -0.25;
		}
	}


};



class Spatial_hash {
	int n;
	std::vector<std::vector<std::reference_wrapper<Drop>>> v;
	
public:
	Spatial_hash() :n(307) {} /*prost broj, velicina odabrana odokativno*/

	//std::vector<std::vector<Drop>> ret_vec() { return v; }

	void insert(Drop& a) {
		size_t x = hasingFun(a);

		v.at(x).push_back(a);
	}

	void fill(std::vector<Drop>& water) {
		v.clear();
		v.resize(n);

		std::for_each(water.begin(), water.end(), [this](Drop& x) {cellID_fill(x); insert(x);});

		//std::for_each(v.begin(), v.end(), [](std::vector<std::reference_wrapper<Drop>> x) {for (Drop a : x) { std::cout << " ovo je red:" << a.cellID.at(0) << " ovo je stupac:" << a.cellID.at(1); }std::cout << std::endl; });
	}
	void fill(std::span<Drop> water) {//verzija funkcije sa span strukturom
		v.clear();
		v.resize(n);

		std::for_each(water.begin(), water.end(), [this](Drop& x) {cellID_fill(x); insert(x); });

		//std::for_each(v.begin(), v.end(), [](std::vector<std::reference_wrapper<Drop>> x) {for (Drop a : x) { std::cout << " ovo je red:" << a.cellID.at(0) << " ovo je stupac:" << a.cellID.at(1); }std::cout << std::endl; });
	}


	size_t hasingFun(Drop a) {
		size_t i = static_cast<size_t> (a.get_cellID_row());
		size_t j = static_cast<size_t> (a.get_cellID_column());

		return ((i * 129285928214159ULL) ^ (j * 109413475968683ULL)) % n; //variable su pomnozene sa sto vecim prostim brojem
	}

	size_t hasingFun(unsigned a,unsigned b) {
		return ((a * 129285928214159ULL) ^ (b * 109413475968683ULL)) % n;
	}


	////////////////////////////////////////////////////////////////////////////////////////////////////////////////triba optimizirat fun jer je trnutno O(2n), a moze bit O(2nlog) ili O(2*1)!!!!!!!
	void cellID_fill(Drop& a) {//funkcija prepoznaje gdje se objekt nalazi na gridu i dodjeljuje id objektu
		unsigned cellSize = static_cast<int> (2 * a.get_radius());//celija je velicine diametra objekta
		unsigned id = 0;

		for (int i = cellSize; i <= 720; i +=cellSize) {//720 je hardkodirana rezolucija
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


	void colision_det(double dt) {//funkcija pregledava colision unutar celije, unutar celije i sljedece celije i donje celije

		for (int z = 0; z < v.size(); z++) {
			std::vector< std::reference_wrapper<Drop>>& x = v.at(z);

			for (int i = 0; i < x.size(); i++) {
				for (int j = i + 1; j < x.size(); j++) {
					dropColision(x.at(i), x.at(j), dt);

					
					if (z != v.size() - 1) {
						std::vector< std::reference_wrapper<Drop>>& x_nextCell = v.at(hasingFun(x.at(0).get().get_cellID_row(), x.at(0).get().get_cellID_column()) + 1); // ode se trazi susjedna celija na temelju trenutnog id-a trenutne celije i stavlja se u hashing fun tako da se nade bucket u kojem je spremljena
						if(!x_nextCell.empty() && i< x_nextCell.size())
							dropColision(x_nextCell.at(i), x.at(j), dt);
					}


					if (x.at(0).get().get_cellID_row() != 72) { //72 celija je zadnja celija i nemoze se ispod nje nista pregledat, ovaj x ima dodatni .get() zbog reference_wrapper!
						std::vector< std::reference_wrapper<Drop>>& x_lowerCell = v.at(hasingFun(x.at(0).get().get_cellID_row()+1, x.at(0).get().get_cellID_column()));
						if (!x_lowerCell.empty() && i < x_lowerCell.size())
							dropColision(x_lowerCell.at(i), x.at(j), dt);
					}
					
				}
			}
		}


	}

	void dropColision(Drop& a, Drop& b, double dt) {
		float r = a.get_radius()+3;
		float distance = sqrt(pow((b.get_posx() - a.get_posx()), 2) + pow((b.get_posy() - a.get_posy()), 2));

		if (distance < r) { 

			if (a.get_posx() > b.get_posx()) {
				a.add_posx(r/2);
				b.add_posx(-r/2);
			}
			else if (a.get_posx() <= b.get_posx()) {
				a.add_posx(-r/2);
				b.add_posx(r/2);
			}

			if (a.get_posy() > b.get_posy()) {
				b.set_posy(a.get_posy() -r/2);
				b.velocity.at(1)= (b.velocity.at(1)+a.velocity.at(1))/2;//formula for law of conservation of linear momentum for non elastic colision with same mass
			}

			else if (a.get_posy() <= b.get_posy()) {
				a.set_posy(b.get_posy() - r/2);
				a.velocity.at(1)= (b.velocity.at(1)+a.velocity.at(1))/2;
			}
		}
	}
};



class Thread_pool {
private:
	std::queue<std::function<void()>> q; //red funkcija
	std::vector<std::thread> t;
	size_t threadNum;
	std::condition_variable cv;
	std::mutex q_mutex;
	bool ready_for_work=true;
public:
	Thread_pool() : threadNum(std::thread::hardware_concurrency()) {for (int i = 0; i < threadNum; i++) t.push_back(std::thread([this]() {work(); }));}
	~Thread_pool() {for (auto& x : t) { if (x.joinable()) x.join(); }t.clear(); }

	size_t get_threadNum() { return threadNum; };

	void enque(std::function<void()> x) { q.push(std::move(x)); cv.notify_one(); };
	void deque(std::function<void()> x) { q.pop(); };
	std::thread::id get_id() { return std::this_thread::get_id(); }

	void work() {
		while (true) {//not sure if multthreading works correct now

		/*	if (q.empty()) {
				lock.unlock();
				std::this_thread::sleep_for(std::chrono::microseconds(250));
				continue;
			}*/
			std::function<void()> task;
			{
				std::unique_lock<std::mutex> lock(q_mutex);
				cv.wait(lock, [this]() {return !q.empty();});
				task = std::move(q.front());
				q.pop();
			}


			if (task)
				task();		
		}
	}

};


void init();
void kill();
void input(bool* running, std::vector<Drop>& water);
void render(std::vector<Drop> water);
void motionUpdate(std::vector<Drop>& water, double dt);
void motionUpdate2(std::vector<Drop>& water, double dt,size_t threadNum, Thread_pool& TP);
void motionUpdate2_1(std::vector<Drop>& water, double dt,size_t threadNum, Thread_pool& TP);