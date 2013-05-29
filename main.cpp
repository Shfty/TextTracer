/*

Text Tracer - A real-time raytracer
TODO:
Universal portal transport for all ray types (shadow esp.)
PhysicsObject
Material class for storing lighting factors, reflectiveness etc
Reflectiveness
Fix lighting + cumulative opacity issue (overlaid discs)
Separate 2D geometry into it's own base class (or use PlaneObject)

kD Tree Refactoring
    -Construct AABBs using splitting plane and furthest object instead of just objects
    -Ray Intersection
    -Nearest Neighbour

Sphere orientation calculation (L should be camera-oriented, not viewport-oriented)?

Preprocessor definitions:
LOW_RES
Disabled: Run in full resolution
Enabled: Run in half resolution

REALTIME
Disabled: Update/Draw one frame, then quit
Enabled: Update/Draw according to FPS constants until user quits

DISABLE_LIGHTING:
Disabled: All objects will be rendered at full brightness
Enabled: All objects will be smoothly lit with hard shadows
*/

#include <ctime>

#include "TextTracer.h"
#include "Keyboard.h"

const int RENDER_FPS = 60;
const int UPDATE_FPS = 60;

int worldClock = 0;
int prevRenderTicks = 0;
int prevUpdateTicks = 0;
bool redraw = true;
bool update = true;

bool quit = false;

int main()
{
    // Initialization
    TextTracer textTracer;

#ifdef REALTIME
    while(!quit)
    {
#endif
        // Cap update to tick rate
        worldClock = clock();
        int updateTicks = worldClock % (CLOCKS_PER_SEC / UPDATE_FPS);
        if(updateTicks == 0 && prevUpdateTicks != 0)
        {
            update = true;
        }
        prevUpdateTicks = updateTicks;

        if(update)
        {
            // Quit
            if(Keyboard::IsKeyDown(0x1B)) // Esc
            {
                quit = true;
            }

            textTracer.Update(worldClock);
            update = false;
        }

        // Cap rendering to frame rate
        int renderTicks = worldClock % (CLOCKS_PER_SEC / RENDER_FPS);
        if(renderTicks == 0 && prevRenderTicks != 0)
        {
            redraw = true;
        }
        prevRenderTicks = renderTicks;

        if(redraw)
        {
            textTracer.Draw();
            redraw = false;
        }
#ifdef REALTIME
    }
#endif

    return 0;
}
