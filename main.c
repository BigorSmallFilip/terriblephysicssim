#include "utility.h"
#include "system.h"
#include "graphics.h"

#include "physics.h"

int main(int argc, char** argv)
{
	if (!SystemInit(
		"Mei guh rjen",
		640,				// Window width
		360,				// Window height
		SDL_RENDERER_ACCELERATED |	// Render flags
		//SDL_RENDERER_PRESENTVSYNC |
		SDL_RENDERER_TARGETTEXTURE,
		640,				// Screen width
		360)				// Screen height
		) return -1;

	InitPhysics();

	unsigned long long time_at_startup = SDL_GetPerformanceCounter();
	unsigned long long timer_start = time_at_startup;
	unsigned long long timer_end = time_at_startup;
	double smoothed_framerate = 144;
	double dt = 0;
	double totaldt = 0;
	double time_since_startup = time_at_startup;
	
	while (!(system_inputs & SYSINP_EXIT))
	{
		timer_end = SDL_GetPerformanceCounter();
		dt = ((double)(timer_end - timer_start)) / (double)SDL_GetPerformanceFrequency();
		timer_start = timer_end;
		
		UpdateInputs();

		ResetRenderTarget();
		SetColorI(0);
		ClearScreen();

		for (int i = 0; i < worldRigidbodies.count; i++)
		{
			Rigidbody* rb = worldRigidbodies.items[i];
			if (!rb->is_static && rb->y > (screen_height >> 1))
				ApplyForce(
					rb,
					0,
					0,
					-600 * (sin(rb->y / 20 + totaldt * 3) / 2) * rb->mass,
					-600 * (sin(rb->x / 50 + totaldt * 2) / 2 + 0.5) * rb->mass,
					dt);
		}

		UpdatePhysics(dt);

		RenderPhysicsObjects();

		UpdateWindow();

		double framerate;
		if (dt == 0) framerate = 0; else framerate = 1.0 / dt;
		totaldt += dt;
		smoothed_framerate = (smoothed_framerate * 99 + framerate) / 100;
		time_since_startup = (double)(SDL_GetPerformanceCounter() - time_at_startup) / (double)SDL_GetPerformanceFrequency();
		printf("FPS = % 6u     Smoothed FPS = % 6u     dt = % f   sum of dt = %.3f   realtime = %.3f   err = %.7f\r",
			(unsigned int)framerate,
			(unsigned int)smoothed_framerate,
			dt,
			totaldt,
			time_since_startup,
			time_since_startup - totaldt
		);
	}

	SystemQuit();

	return 0;
}
