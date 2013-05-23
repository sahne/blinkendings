#include "BlinkenWall.h"

BlinkenWall::BlinkenWall(int default_delay, uint32_t bg_color)
{
	delay = default_delay;
	background = bg_color;
}

void
BlinkenWall::setup()
{
	Tlc.init();
	Tlc.clear();
	clear();
}

void
BlinkenWall::clear()
{
	int pos;
	/* reset zbuffer */
	memset(zbuffer, 0xFF, 8 * 12);
	for (pos = 0; pos < 96; pos++) {
		fbprev[pos/12][pos%12] = fb[pos/12][pos%12];
		draw_raw(pos, background);
	}
}

void
BlinkenWall::update()
{
	update(delay);
}

void
BlinkenWall::update(int del)
{
	/* write diff from fbprev and fb to Tlcs */
	for (int x = 0; x < 8; x++) {
		for (int y = 0; y < 12; y++) {
			if (fb[x][y] == fbprev[x][y])
				continue;
			uint16_t r,g,b;
			int pos = (x * 12) + y;
			int rpos = transform(pos);
			uint32_t color = fb[x][y];
			r = (uint16_t)(((color & 0x00FF0000) >> 16) << MAX_COLOR_VAL);
			g = (uint16_t)(((color & 0x0000FF00) >> 8) << MAX_COLOR_VAL);
			b = (uint16_t)(((color & 0x000000FF)) << MAX_COLOR_VAL);
			updateWall(rpos, r, g, b);
		}
	}
	Tlc.update();
	::delay(del);
}

void
BlinkenWall::setBackGround(uint32_t color)
{
	background = color;
}

void
BlinkenWall::drawAnimation(struct animation *ani, int xoffset, int yoffset, int zoffset, int frame)
{
	int pos;
	uint8_t f = 1 << frame;
	for (pos = 0; pos < 96; pos++) {
		int x = pos / 12;
		int y = pos % 12;
		/* nothing to draw if it's hidden */
		if (zbuffer[x][y] < zoffset)
			continue;
		int col = 0;
		int ax = x - xoffset;
		int ay = y - yoffset;

		if (ax < ani->len_x && ay < ani->len_y && ax >= 0 && ay >= 0)
			col = ani->frames[(ax * ani->len_y) + ay];

		if (col & (int)f) {
			/* get color from animation */
			zbuffer[x][y] = zoffset;
			uint32_t colval = ani->color[(ax * ani->len_y) + ay];
			draw_raw(pos, colval);
		}
	}
}

void
BlinkenWall::drawPoint(int x, int y, int z, uint32_t color)
{
	int pos = (x * 12) + y;
	if (pos >= 96)
		return;
	if (zbuffer[x][y] < z)
		return;
	draw_raw(pos, color);
}

int
BlinkenWall::transform(int pos)
{
	if (pos >= 96)
		return -1;
	int x = pos / 12;
	int y = pos % 12;
	return trans_mat[x][y];
}

void
BlinkenWall::draw_raw(int pos, uint32_t color)
{
	if (pos >= 96)
		return;
	int x = pos / 12;
	int y = pos % 12;
	fb[x][y] = color;
}

void
BlinkenWall::updateWall(TLC_CHANNEL_TYPE pos, uint16_t r, uint16_t g, uint16_t b)
{
	TLC_CHANNEL_TYPE tlc_channel = pos * COLORS;
	Tlc.set(tlc_channel + LED_R, r);
	Tlc.set(tlc_channel + LED_G, g);
	Tlc.set(tlc_channel + LED_B, b);
}

uint32_t *
BlinkenWall::createColor(uint32_t pattern, int lenx, int leny)
{
	int i;
	if (lenx < 1 || leny < 1)
		return NULL;
	uint32_t *color = (uint32_t *) malloc(lenx * leny);
	if (!color)
		return NULL;
	for (i = 0; i < lenx * leny; i++)
		color[i] = pattern;
	return color;
}

void swap(int *x, int *y)
{
    if (x == y || x == NULL || y == NULL)
        return;
    *x ^= *y;
    *y ^= *x;
    *x ^= *y;
    return;
}


void
BlinkenWall::drawRect(int8_t xs, int8_t ys, int8_t xe, int8_t ye, int8_t z, uint32_t color, uint32_t fillColor)
{
    if (xs > xe)
        swap(&xs, &xe);
    if (ys > ye)
        swap(&ys, &ye);
    drawLine(xs, ys, xs, ye, color);
    drawLine(xs, ys, xe, ys, color);
    drawLine(xe, ys, xe, ye, color);
    drawLine(xs, ye, xe, ye, color);
    for (int8_t x = xs + 1; x < xe; x++)
        for (int8_t y = ys + 1; y < ye; y++)
            drawPoint(x, y, z, fillColor);
}

/* drawLine using Bresenham's line algorithm
 * replace it with Wu's algo if we want anti-aliasing
 */

void
BlinkenWall::drawLine(int8_t xs, int8_t ys, int8_t xe, int8_t ye, int8_t z, uint32_t color)
{
    if (xs > xe)
        swap(xs, xe);
    if (ys > ye)
        swap(ys, ye);
    int8_t dx = xe - xs;
    int8_t dy = -(ye - ys);
    int16_t err = dx + dy;
    int16_t err2;
    for (;;) {
        drawPoint(xs, ys);
        if (xs == xe && ys == ye)
            break;
        err2 = 2 * err;
        if (err2 > dy) {
            err += dy;
            xs++;
        }
        if (err2 < dx) {
            err += dx;
            ys++;
        }
    }
}
