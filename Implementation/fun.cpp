#include "fun.h"

SDL_Window* window;
SDL_Renderer* renderer;

void input(bool* isRunning, std::vector<Drop>& water,std::mutex &waterMutex) {
	float x = 0, y = 0;

	const bool* keystate = SDL_GetKeyboardState(NULL);//returns list of pressed buttons
	unsigned mousestate = SDL_GetMouseState(&x, &y);

	if (keystate[SDL_SCANCODE_ESCAPE]) {// simulation termination
		*isRunning = false;
		return;
	}

	if (mousestate == SDL_BUTTON_MASK(1)){//spawns drop by drop
		std::unique_lock<std::mutex> lock(waterMutex);
		water.push_back(Drop(x, y));
		}

	if (keystate [SDL_SCANCODE_SPACE]) {//spawns bigger group of drops.
		int x = 100, y = 100;

		std::unique_lock<std::mutex> lock(waterMutex);

		for (int i = 0; i < 3; i++) {
			for (int j= 0; j < 70; j++) {
				water.push_back(Drop(x, y));
				x += 15;
			}
			y+= 25;
			x = 100;
		}
	}
}

void colision(std::vector<Drop>& water, double dt) {
	float r = water.at(0).get_radius();

	SpatialHash h;
	h.fill(water);

	h.colision_det(dt);

}

void colision(const std::span<Drop>& water, double dt) {//verzija funkcije sa span strukturom
	float r = water[0].get_radius();

	SpatialHash h;
	h.fill(water); 

	h.colision_det(dt);

}

void motionUpdate(std::vector<Drop>& water, double dt) {
	std::for_each(water.begin(), water.end(), [dt](Drop& x) {x.gravityEffect(dt); x.boundControl(); x.motionUpdate(dt); });

	colision(water, dt);
}


void motionUpdate2_1(std::vector<Drop>& water, double dt, size_t threadNum, ThreadPool& TP) {
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
	std::sort(water.begin(), water.end(), [](const Drop& a,const Drop& b) {return (a.get_cellID_row() * 1000 + a.get_cellID_column()) < (b.get_cellID_row() * 1000 + b.get_cellID_column()); });
}

void render(const std::vector<Drop>& water) {

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	std::for_each(water.begin(), water.end(), [](Drop x) {x.render(renderer); });//Drop render 
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