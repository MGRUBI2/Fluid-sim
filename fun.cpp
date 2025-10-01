#include "fun.h"

SDL_Window* window;
SDL_Renderer* renderer;

void input(bool* running, std::vector<Drop>& water) {
	float x = 0, y = 0;

	const bool* keystate = SDL_GetKeyboardState(NULL);//vraca listu stisnutih tipki
	unsigned mousestate = SDL_GetMouseState(&x, &y);

	if (keystate[SDL_SCANCODE_ESCAPE]) {
		*running = false;
		return;
	}
	if (mousestate == SDL_BUTTON_MASK(1))
		water.push_back(Drop(x, y));
}

void colision(std::vector<Drop>& water, double dt) {
	float r = water.at(0).get_radius();

	Spatial_hash h;
	h.fill(water);

	h.colision_det(dt);

}
void colision(std::span<Drop> water, double dt) {//verzija funkcije sa span strukturom
	float r = water[0].get_radius();

	Spatial_hash h;
	h.fill(water); 

	h.colision_det(dt);

}

void motionUpdate(std::vector<Drop>& water, double dt) {
	std::for_each(water.begin(), water.end(), [dt](Drop& x) {x.gravityEffect(dt); x.boundControl(); x.motionUpdate(dt); });

	colision(water, dt);
}


void motionUpdate2_1(std::vector<Drop>& water, double dt, size_t threadNum, Thread_pool& TP) {
	size_t a = water.size() / threadNum;
	size_t aBegin = 0;
	size_t aEnd = a;

	for (int i = 0; i < threadNum; i++) {
		std::span<Drop> new_vec(water.begin() + aBegin, water.begin() + aEnd);
		TP.enque([b = aBegin, e = aEnd, &water, dt_=dt]() {std::for_each(water.begin() + b, water.begin() + e, [dt_](Drop& x) {x.gravityEffect(dt_); x.boundControl(); x.motionUpdate(dt_); }); });//zbog multithread-anja lambdin captur nisan stavia & nego san zasebno stavia parametre da se nebi sta pomisalo
		TP.enque([vec=new_vec, dt_=dt]() {colision(vec, dt_); });
		aBegin += a;
		aEnd += a;

	}
	TP.enque([b = aBegin-a, e = aEnd, &water, dt_=dt]() {std::for_each(water.begin() + b, water.end(), [dt_](Drop& x) {x.gravityEffect(dt_); x.boundControl(); x.motionUpdate(dt_); }); });//na ovoj i sljedecoj liniji oduziman sa a jer inace je index out of range	
	std::span<Drop> new_vec(water.begin() + aBegin-a, water.end());

	TP.enque([vec = new_vec, dt_=dt]() {colision(vec, dt_); });

}

void sort_by_cell_id(std::vector<Drop>& water) {//sorts them the way they are placed on grid for better colision detection
	std::sort(water.begin(), water.end(), [](Drop a, Drop b) {return (a.get_cellID_row() * 1000 + a.get_cellID_column()) < (b.get_cellID_row() * 1000 + b.get_cellID_column()); });
}

void render(std::vector<Drop> water) {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	std::for_each(water.begin(), water.end(), [](Drop x) {x.render(renderer); });//render kapljica

	SDL_RenderPresent(renderer);
}

void init() {

	window = SDL_CreateWindow("Fluid sim", 1280, 720, 0);

	if (!window)
		std::cout << "ERROR: WINDOW was not created!" << std::endl;

	renderer = SDL_CreateRenderer(window, NULL);

	if (!renderer)
		std::cout << "ERROR: renderer was not created!" << std::endl;


}

void kill() {
	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(renderer);

	SDL_Quit();
}