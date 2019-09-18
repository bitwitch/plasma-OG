#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class Demo : public olc::PixelGameEngine
{
public:
    Demo()
    {
    }

    int HEIGHT;
    int WIDTH;

    float currentTime;

    bool OnUserCreate() override 
    {
        HEIGHT = ScreenHeight();
        WIDTH = ScreenWidth(); 
        currentTime = 0;
        return true;    
    }
    bool OnUserUpdate(float fElapsedTime) override 
    {

        currentTime += fElapsedTime;
/*        // calculate plasma functions (sin and cos funcs)*/
        //// sin of distance from screen center
    
        //// translating the plasma function over time
        //float x1 = WIDTH + WIDTH * sin(currentTime);
        //float y1 = HEIGHT + HEIGHT * cos(currentTime);
        //float a = 0.5f*WIDTH - x1;
        //float b = 0.5f*HEIGHT - y1;

        //float cDist = sqrt(a*a + b*b);
        //int plasma1 = (int)(128 + 128 * sin(cDist)); // 0 - 255

        //printf("plasma1: %d\n", plasma1);

        // calculate color palette

        // get color value of from plasma functions

        // draw

        int colorVal;
        float a1, b1;
        for(int y = 0; y < HEIGHT; y++) { 
            for(int x = 0; x < WIDTH; x++) { 
                float a1 = 0.5f*WIDTH - x;
                float b1 = 0.5f*HEIGHT - y;
                float dist = sqrt(a1*a1 + b1*b1); 
                int colorVal = (int)(128 + 128 * sin(dist * 0.1 - currentTime));
                Draw(x, y, olc::Pixel(colorVal, colorVal, colorVal));
            }
        }

        return true;    
    }
    bool OnUserDestroy() override 
    {
        return true;    
    }
};


int main () {

    Demo demo;
    if (demo.Construct(800, 600, 1, 1)) {
        demo.Start();
    }
    return 0;
}
