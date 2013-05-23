#include "tlc_config.h"
#include "Tlc5940.h"
#include "BlinkenWall.h"
#include "Animations.h"

BlinkenWall wall(180, 0xff0000);

void setup()
{
    wall.setup();
}

static const char bar[] = {
  1,1,1,1,1,1,1,1,1,1,1,1
};

static const char vbar[] = {
  1,1,1,1,1,1,1,1
};

static const uint32_t orange[] =
{
    0xDD9900,0xDD9900,0xDD9900,0xDD9900,
    0xDD9900,0xDD9900,0xDD9900,0xDD9900,
    0xDD9900,0xDD9900,0xDD9900,0xDD9900
};

static const uint32_t green[] =
{
    0x00DD00,0x00DD00,0x00DD00,0x00DD00,
    0x00DD00,0x00DD00,0x00DD00,0x00DD00,
    0x00DD00,0x00DD00,0x00DD00,0x00DD00
};

struct animation bargreen = {1,1,12,bar, green};
struct animation barorange = {1,1,12,bar, orange};
struct animation vbargreen = {1,8,1,vbar, green};
struct animation vbarorange = {1,8,1,vbar, orange};

int hbars()
{
    static int bgo = 0;
    static int diff = 1;
    wall.drawAnimation(&bargreen, bgo, 0, 0, 0);
    wall.drawAnimation(&barorange, bgo+1, 0, 0, 0);
    wall.drawAnimation(&barorange, 8-(bgo+1), 0, 0, 0);
    wall.drawAnimation(&bargreen, 8-bgo, 0,0,0);
    bgo += diff; 
    if (bgo == 3)
      diff = -1;
    if (bgo == -2) {
      diff = 1;
      return 1;
    }
    return 0;
}

int vbars()
{
    static int bgo = 0;
    static int diff = 1;
    wall.drawAnimation(&vbargreen, 0, bgo,0 , 0);
    wall.drawAnimation(&vbarorange, 0, bgo+1, 0, 0);
    wall.drawAnimation(&vbarorange, 0, 12-(bgo+1), 0, 0);
    wall.drawAnimation(&vbargreen, 0, 12-bgo,0,0);
    bgo += diff; 
    if (bgo == 4)
      diff = -1;
    if (bgo == -2) {
      diff = 1;
      return 1;
    }
    return 0;
}

void loop()
{
    static int anim = 0;
    switch (anim) {
    case 0:
      anim += hbars();
      break;
    case 1:
      anim += vbars();
      break;
    default:
      break;
    }
    anim = anim % 2;
    wall.update();
    wall.clear();
}
