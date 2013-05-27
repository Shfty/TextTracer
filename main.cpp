/*

Text Tracer - A real-time raytracer
TODO:
Per-object AABB for broad-phase collision

Ray Refactoring
    -Move ray-shape intersection tests into Raytracer
    -Cumulative Ray colour (transparency)
    -Should IsectData be a member of Ray?

kD Tree Refactoring
    -Visualisation w/transparent planes
    -Nearest Neighbour
    -Store AABBs instead of WorldObjects

Sphere orientation calculation (L should be camera-oriented, not viewport-oriented)?

Preprocessor definitions:
LOW_RES
Disabled: Run in full resolution at 1x FPS
Enabled: Run in half resolution at 2x FPS

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
