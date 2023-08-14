#pragma once

#include "utility.h"
#include "system.h"



INLINE void ResetRenderTarget()
{
	SDL_SetRenderTarget(main_renderer, main_texture);
}

INLINE void SetColor(
	const uint8_t r,
	const uint8_t g,
	const uint8_t b,
	const uint8_t a
)
{
	SDL_SetRenderDrawColor(main_renderer, r, g, b, a);
}

INLINE void SetColorI(
	const uint32_t color
)
{
	SDL_SetRenderDrawColor(
		main_renderer,
		(uint8_t)((color & 0x000000ff)),
		(uint8_t)((color & 0x0000ff00) >> 8),
		(uint8_t)((color & 0x00ff0000) >> 16),
		(uint8_t)((color & 0xff000000) >> 24)
	);
}

INLINE void ClearScreen()
{
	SDL_RenderClear(main_renderer);
}

INLINE void DrawLine(
	const int x1,
	const int y1,
	const int x2,
	const int y2
)
{
	SDL_RenderDrawLine(main_renderer, x1, y1, x2, y2);
}

INLINE void DrawLineF(
	const float x1,
	const float y1,
	const float x2,
	const float y2
)
{
	SDL_RenderDrawLineF(main_renderer, x1, y1, x2, y2);
}

INLINE void DrawRect(
	const int x,
	const int y,
	const int w,
	const int h
)
{
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderDrawRect(main_renderer, &rect);
}

INLINE void FillRect(
	const int x,
	const int y,
	const int w,
	const int h
)
{
	SDL_Rect rect = { x, y, w, h };
	SDL_RenderFillRect(main_renderer, &rect);
}
