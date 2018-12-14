#define FB_GREEN        2
#define FB_DARK_GREY    8
#define FB_BLACK        0
#define FB_WHITE        15

int sum_of_three(int arg1, int arg2, int arg3) {
    return arg1 + arg2 + arg3;
}
/** fb_write_cell:
 *  Writes a character with the given foreground and background to position i
 *  in the framebuffer.
 *
 *  @param i  The location in the framebuffer
 *  @param c  The character
 *  @param fg The foreground color
 *  @param bg The background color
 */
char* fb = (char*) 0x000B8010;
//fb[0] = 'A';
//fb[1] = 0x28;
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg)
{
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}

int main() {
    fb_write_cell(0, 'A', FB_GREEN, FB_DARK_GREY);
    return 8;
}
