#include <iostream>
#include "fun.h"
#include<chrono>


int main() {

	SDL_Init(SDL_INIT_VIDEO);
	init();

	std::vector<Drop> water;

	 
	SDL_Event event;
	bool isRunning = true;

	Uint64 lastTime = 0;

	ThreadPool TP;

	bool multithread_switch= false;

	std::mutex waterMutex;

	while (isRunning) {
		Uint64 now = SDL_GetTicks();
		double dt = static_cast<double> (now-lastTime)* 1000.0 / SDL_GetPerformanceFrequency();

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				isRunning = false;
				break;
			}			
		}

	
		input(&isRunning, water,waterMutex);
		if (!water.empty()) {
			
			{
				std::unique_lock<std::mutex> lock(waterMutex);

				sort_by_cell_id(water);
				render(water);
			}

			auto start = std::chrono::high_resolution_clock::now();

			{
				std::unique_lock<std::mutex> lock(waterMutex);

				if (!multithread_switch)
					motionUpdate(water, dt);
				else
					motionUpdate2_1(water, dt, TP.get_threadNum(), TP);
			}
			TP.waitAll();

			auto end = std::chrono::high_resolution_clock::now();

			std::chrono::duration<double, std::milli> trajanje = end - start;

			std::cout << trajanje.count()<<" "<<water.size() << std::endl;
			if (trajanje.count() > 2)
				multithread_switch = true;
		}



		lastTime = now;
		SDL_Delay(6.9);//fpslock 144fps
	}


	kill();
	return 0;
}