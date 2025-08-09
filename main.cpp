#include <iostream>
#include "fun.h"
#include<chrono>


int main() {

	SDL_Init(SDL_INIT_VIDEO);
	init();

	std::vector<Drop> water;

	 
	SDL_Event event;
	bool running = true;

	Uint64 lastTime = 0;

	Thread_pool TP;

	bool mt = false;//multithread switch

	while (running) {
		Uint64 now = SDL_GetTicks();
		double dt = static_cast<double> (now-lastTime)* 1000.0 / SDL_GetPerformanceFrequency();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				running = false;
				break;
			}			
		}

	
		input(&running, water);
		if (!water.empty()) {
			
			render(water);

			auto start = std::chrono::high_resolution_clock::now();


			if (!mt)
				motionUpdate(water, dt);
			else
				motionUpdate2(water, dt,TP.get_threadNum(),TP);
			

			auto end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> trajanje = end - start;

			std::cout << trajanje.count()<<" "<<water.size() << std::endl;
			if (trajanje.count() > 2)
				mt = true;
		}



		lastTime = now;
		SDL_Delay(6.9);//fpslock na 144fps
	}


	kill();
	return 0;
}