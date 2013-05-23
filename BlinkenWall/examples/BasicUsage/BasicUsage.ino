#include "tlc_config.h"
#include "Tlc5940.h"
#include "BlinkenWall.h"
#include "Animations.h"

/* create a BlinkenWall object with default delay
 * of 180 ms and no background color
 */
BlinkenWall wall(180, 0x000000);

/* setup the Wall */
void setup()
{
    wall.setup();
}

void loop()
{
    /* frame counter */
    static int frame = 0;
    /* draw its animation to Wall */
    wall.drawAnimation(&itssyndikat, 2, 0, 0, frame);
    /* increment frame counter for next run */
    frame++;
    /* reset frame counter if needed */
    if (frame == itssyndikat.num_frames)
        frame = 0;
    /* update Wall (actually draw it out) */
    wall.update();
    /* reset wall and framebuffers */
    wall.clear();
}
