#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

#define PI 3.14159265358979323846

typedef unsigned char u8;

class Demo : public olc::PixelGameEngine
{
public:
    Demo() { sAppName = "Plasma OG Master"; }

    int HEIGHT;
    int WIDTH;

    float current_time;

    u8 *plasma1; // lookup tables
    u8 *plasma2; // 
    u8 *palette;

    void precalculate_plasma(u8 *buffer1, u8 *buffer2, int w, int h)
    {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                buffer1[y*w+x] = (u8)(64 + 63 * sin(hypot(0.5*w - x, 0.5*h - y) * 0.0666));
                buffer2[y*w+x] = (u8)(64 + 63 * sin((float)x/(37+15*cos((float)y/74)))
                                                * cos((float)y/(31+11*sin((float)x/57))));
            }
        }
    }

    bool OnUserCreate() override 
    {
        HEIGHT = ScreenHeight();
        WIDTH = ScreenWidth(); 
        current_time = 0;
        plasma1 = new u8[WIDTH*2*HEIGHT*2];
        plasma2 = new u8[WIDTH*2*HEIGHT*2];
        palette = new u8[256*3];

        precalculate_plasma(plasma1, plasma2, WIDTH*2, HEIGHT*2);
        return true;    
    }

    bool OnUserUpdate(float fElapsedTime) override 
    {
        current_time += fElapsedTime;
        int x1 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * cos(current_time/9));
        int x2 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * sin(-current_time/11));
        int x3 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * sin(-current_time/13));
        int y1 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * sin(current_time/12));
        int y2 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * cos(-current_time/7));
        int y3 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * cos(-current_time/10));
        int i_plasma1 = y1 * WIDTH * 2 + x1;
        int i_plasma2 = y2 * WIDTH * 2 + x2;
        int i_plasma3 = y3 * WIDTH * 2 + x3;

        // set color palette
        u8 r, g, b;
        int i, dst = 0;
        for (i=0; i<256; ++i) 
        {
            r = (u8)(32 + 31 * cos(i * PI / 128 + current_time/7));
            g = (u8)(32 + 31 * sin(i * PI / 128 + current_time/6));
            b = (u8)(32 - 31 * cos(i * PI / 128 + current_time/8));

            palette[dst++] = r; 
            palette[dst++] = g; 
            palette[dst++] = b; 
        }

        // draw
        int color_val;
        for(int y = 0; y < HEIGHT; y++) { 
            for(int x = 0; x < WIDTH; x++) { 
                int color_val = (int)(u8)(plasma1[i_plasma1] + plasma2[i_plasma2] + plasma2[i_plasma3]); // cast to u8 to wrap overflow
                // TODO(shaw): is it significantly faster to write to a 
                // buffer and draw whole buffer once? and is this possible
                // in olcPixelEngine?
                //Draw(x, y, olc::Pixel(color_val, color_val, color_val));
                Draw(x, y, olc::Pixel(palette[color_val*3], 
                                      palette[color_val*3+1], 
                                      palette[color_val*3+2]));

                ++i_plasma1;
                ++i_plasma2;
                ++i_plasma3;
            }
            i_plasma1 += WIDTH;
            i_plasma2 += WIDTH;
            i_plasma3 += WIDTH;
        }

        return true;    
    }

    bool OnUserDestroy() override 
    {
        delete [] plasma1;
        delete [] plasma2;
        delete [] palette;
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

