#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
#include <stdio.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb/stb_image_write.h>

unsigned char ttf_buffer[1<<20];
unsigned char temp_bitmap[512*512];
stbtt_bakedchar cdata[96]; // ASCII 32..126 is 95 glyphs

void my_stbtt_initfont()
{
    fread(ttf_buffer, 1, 1<<20, fopen("assets/fonts/JetBrainsMono-Medium.ttf", "rb"));
    stbtt_BakeFontBitmap(ttf_buffer,0, 32.0, temp_bitmap,512,512, 32,96, cdata); // no guarantee this fits!
    // can free ttf_buffer at this point
    
    // stb image write not included in repo because it's not used (at least not right now)
    // stbi_write_png("assets/fonts/bin/test.png", 512, 512, 1, temp_bitmap, 512);
}

int main()
{
    my_stbtt_initfont();


    return 0;
}