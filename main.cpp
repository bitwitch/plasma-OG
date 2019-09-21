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
    u8 *plasma3; // 
    u8 *plasma4; // 
    u8 *plasma5; // 

    int *palette;

    void precalculate_plasma(u8 *buffer1, u8 *buffer2, u8 *buffer3, u8 *buffer4, u8 *buffer5, int w, int h)
    {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                buffer1[y*w+x] = (u8)(128 + 127 * sin(hypot(0.5*w - x, 0.5*h - y) * 0.0666));
                buffer2[y*w+x] = (u8)(128 + 127 * sin((float)x/(37+15*cos((float)y/74)))
                                                * cos((float)y/(31+11*sin((float)x/57))));
                buffer3[y*w+x] = (u8)(128.0 + 127.0 * sin(x / 16.0));
                buffer4[y*w+x] = (u8)(128.0 + 127.0 * sin(y / 8.0));
                buffer5[y*w+x] = (u8)(128.0 + 127.0 * sin((x + y) / 16.0));
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
        plasma3 = new u8[WIDTH*2*HEIGHT*2];
        plasma4 = new u8[WIDTH*2*HEIGHT*2];
        plasma5 = new u8[WIDTH*2*HEIGHT*2];
        palette = new int[256];

        precalculate_plasma(plasma1, plasma2, plasma3, plasma4, plasma4, WIDTH*2, HEIGHT*2);
        return true;    
    }

    bool OnUserUpdate(float fElapsedTime) override 
    {
        current_time += fElapsedTime;
        int x1 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * cos(current_time/9.7));
        int x2 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * sin(-current_time/11.4));
        int x3 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * sin(current_time/13.7));
        int x4 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * cos(-current_time/11.1));
        int x5 = (int)(0.5*WIDTH + (0.5*WIDTH-1) * sin(-current_time/9.6));
        int y1 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * sin(current_time/12.3));
        int y2 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * cos(-current_time/7.5));
        int y3 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * cos(current_time/12.4));
        int y4 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * sin(-current_time/10));
        int y5 = (int)(0.5*HEIGHT + (0.5*HEIGHT-1) * cos(-current_time/7.9));
        int i_plasma1 = y1 * WIDTH * 2 + x1;
        int i_plasma2 = y2 * WIDTH * 2 + x2;
        int i_plasma3 = y3 * WIDTH * 2 + x3;
        int i_plasma4 = y4 * WIDTH * 2 + x4;
        int i_plasma5 = y5 * WIDTH * 2 + x5;

        // set color palette
        int r, g, b;
        int i, dst = 0;
        for (i=0; i<256; ++i) 
        {
            r = (int)(32 + 31 * cos(i * PI / 128 + current_time/7));
            g = (int)(32 + 31 * sin(i * PI / 128 + current_time/6));
            b = (int)(32 - 31 * cos(i * PI / 128 + current_time/8));

            palette[dst++] = (r << 16) | (g << 8) | b; 
        }

        // draw
        int color_val;
        for(int y = 0; y < HEIGHT; y++) { 
            for(int x = 0; x < WIDTH; x++) { 
                int color_val = (int)(u8)(plasma1[i_plasma1] + plasma3[i_plasma3] + plasma4[i_plasma4]); // cast to u8 to wrap overflow
                // TODO(shaw): is it significantly faster to write to a 
                // buffer and draw whole buffer once? and is this possible
                // in olcPixelEngine?
                //Draw(x, y, olc::Pixel(color_val, color_val, color_val));
                Draw(x, y, olc::Pixel((palette[color_val] >> 16)& 0xFF, 
                                      (palette[color_val] >> 8) & 0xFF, 
                                      palette[color_val] & 0xFF));

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
        delete [] plasma3;
        delete [] plasma4;
        delete [] plasma5;
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

