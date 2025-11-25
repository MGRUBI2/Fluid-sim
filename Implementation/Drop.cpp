#include "Drop.h"


void Drop::render(SDL_Renderer* renderer) {
	std::vector<SDL_Vertex> v = drawDrop();
	/*std::vector<SDL_Vertex> v= midPointAlg();*/


	//SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	//SDL_RenderPoints(renderer, arr, v.size());
	//SDL_RenderLines(renderer, v.data(), v.size());//zaminit sa SDL_RenderGeometry()
	SDL_RenderGeometry(renderer, NULL, v.data(), v.size(), NULL, 0);
}



void Drop::gravityEffect(double dt) {//vjv temp
	velocity.at(1) += 100 * dt;

	pos.at(1) += velocity[1];
}

std::vector<SDL_Vertex> Drop::drawDrop() {
	std::vector<SDL_Vertex> v1;
	SDL_Color color = { 255,255,255,255 };
	SDL_FPoint t = { 0.0f,0.0f };

	v1.push_back({ pos.at(0),pos.at(1),255,255,255,255 });
	v1.push_back({ pos.at(0) + radius,pos.at(1),255,255,255,255 });
	v1.push_back({ pos.at(0),pos.at(1) - radius,255,255,255,255 });

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

//std::vector<SDL_Vertex> midPointAlg() {//vraca vector struktura priko kojih cemo crtat krug // netocan 
	//	int x = radius, y = 0;

	//	std::vector<SDL_Vertex> v1;
	//	SDL_Color color = { 255,255,255,255 };
	//	SDL_FPoint t = { 0.0f,0.0f };

	//	v1.push_back( {x + pos.at(0),y + pos.at(1), 255, 255, 255, 255 });
	//	v1.push_back( {x + pos.at(0),-y + pos.at(1) ,  255, 255, 255, 255  });
	//	v1.push_back( { y + pos.at(0),x + pos.at(1) ,  255, 255, 255, 255  });
	//	v1.push_back( { -y + pos.at(0),x + pos.at(1) ,  255, 255, 255, 255 });


	//	int p = 1 - radius;
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

	//		v1.push_back( {x + pos.at(0),y + pos.at(1) ,  255, 255, 255, 255 });
	//		v1.push_back( {- x + pos.at(0),y + pos.at(1),255, 255, 255, 255});
	//		v1.push_back( {x + pos.at(0),-y + pos.at(1) , 255, 255, 255, 255 });
	//		v1.push_back( {-x + pos.at(0),-y + pos.at(1) ,  255, 255, 255, 255 });


	//		if (x != y) {
	//			v1.push_back({ y + pos.at(0),x + pos.at(1) , 255, 255, 255, 255 });
	//			v1.push_back({ -y + pos.at(0),x + pos.at(1), 255, 255, 255, 255 });
	//			v1.push_back({ y + pos.at(0),-x + pos.at(1) ,  255, 255, 255, 255 });
	//			v1.push_back({ -y + pos.at(0),-x + pos.at(1) ,  255, 255, 255, 255 });

	//		}

	//	}
	//	return v1;
	//}

void Drop::boundControl() {
	if (pos[0] > 1280)
		pos[0] = 1280;
	if (pos[0] < 0)
		pos[0] = 0;
	if (pos[1] > 720) {
		pos[1] = 720;
		velocity.at(1) *= -0.25;//floor bounce
	}
	if (pos[1] < 0) {
		pos[1] = 0;
		velocity.at(1) *= -0.25;
	}
}
