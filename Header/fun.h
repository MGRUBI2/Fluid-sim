#include "ThreadPool.h"
#include "Drop.h"
#include "SpatialHash.h"
#include <iostream>
#include <span>
#include <vector>
#include <algorithm>
#include <SDL3/SDL.h>


void init();
void kill();
void input(bool* running, std::vector<Drop>& water,std::mutex& waterMutex);
void render(const std::vector<Drop>& water);
void motionUpdate(std::vector<Drop>& water, double dt);
void sort_by_cell_id(std::vector<Drop>& water); 
void motionUpdate2(std::vector<Drop>& water, double dt, size_t threadNum, ThreadPool& TP);
void motionUpdate2_1(std::vector<Drop>& water, double dt, size_t threadNum, ThreadPool& TP);