#include "io.h"

/*  I/O PORTS
 *  Defining base offsets for serial ports
 */
#define SERIAL_COM1_BASE    0x3F8   /* COM1 base port */

#define SERIAL_DATA_PORT(base)          (base)
#define SERIAL_FIFO_COMMAND_PORT(base)  (base + 2)
#define SERIAL_LINE_COMMAND_PORT(base)  (base + 3)
#define SERIAL_MODEM_COMMAND_PORT(base) (base + 4)
#define SERIAL_LINE_STATUS_PORT(base)   (base + 5)

/* Value tells the serial port to expect high 8 bits first, followed by low 8 bits */
#define SERIAL_LINE_ENABLE_DLAB     0x80

/* FRAMEBUFFER VALUES */
// Framebuffer colors
#define FB_BLACK        0
#define FB_WHITE        15
// I/O ports
#define FB_COMMAND_PORT 0x3D4
#define FB_DATA_PORT    0x3D5
// I/O port commands
#define FB_HIGH_BYTE_COMMAND    14
#define FB_LOW_BYTE_COMMAND     15
// Default Framebuffer offsets
#define DEFAULT_LINE_OFFSET 7
#define DEFAULT_LINE_SIZE 160
#define DEFAULT_CHAR_OFFSET 4

// Framebuffer location, offset
char* fb = (char*) 0x000B8000;
unsigned short cur_x = 0;
unsigned short cur_y = 7;
unsigned short location = DEFAULT_LINE_OFFSET*DEFAULT_LINE_SIZE + DEFAULT_CHAR_OFFSET;


/** fb_move_cursor:
 * Moves the fb cursor to a given position
 * 
 * @param pos The new position of the cursor
 */
void fb_move_cursor() {
    unsigned short pos = cur_y*160 + (cur_x/2);
    outb(FB_COMMAND_PORT,   FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,      pos >> 8 );
    outb(FB_COMMAND_PORT,   FB_LOW_BYTE_COMMAND); 
    outb(FB_DATA_PORT,      pos );
}

// Scrolls the text on the screen up by one line.
void fb_scroll()
{

   // Get a space character with the default colour attributes.
   unsigned char attributeByte = (FB_BLACK << 4) | (FB_WHITE & 0x0F);
   unsigned short blank = 0x20 /* space */ | (attributeByte << 8);

   // Row 25 is the end, this means we need to scroll up
   if(cur_y >= 25)
   {
       // Move text on screen up one line in Framebuffer
       int i;
       for (i = 0*160; i < 24*160; i++)
       {
           fb[i] = fb[i+160];
       }
       // Write 80 spaces to last line
       for (i = 24*160; i < 25*160; i++)
       {
           fb[i] = blank;
       }
       // The cursor should now be on the last line.
       cur_y = 24;
   }
}
/** fb_write_cell:
 *  Write a char to the framebuffer
 * 
 *  @param c    Character to write
 */
void fb_write_cell(char c) {
    unsigned char fg = 15;  // Text color is white
    unsigned char bg = 0;   // Background color is blacks
    unsigned short pos;
    pos =  cur_y*160 + cur_x; 
    fb[pos] = c;
    fb[pos + 1] = ((bg) << 4) | (fg & 0x0F);
    cur_x += 2;
    // If at end of screen put new line
    if (cur_x >= 160) {
       cur_x = 0;
       cur_y ++;
    }

    /* TODO
     * Handle Backspace
     * Handle Tab
     * Handle CR
     * Handle New Line
     */
    fb_scroll();   // Scroll screen if needed
    fb_move_cursor();   // Move cursor
}
/** fb_write:
 *  Writes contents of char* buf to screen
 * 
 *  @param buf  char buffer holding data
 */
void fb_write(char* buf, unsigned short size) {
    for( int i = 0; i < size-1; i++ ) {
        fb_write_cell(buf[i]);
    }
}

void fb_clear()
{
   // Make an attribute byte for the default colours
   unsigned short blank = 0x20;

   int i;
   for (i = 0; i < 160*25; i+=2)
   {
       fb[i] = blank;
       fb[i + 1] = (0 << 4) | (15 & 0x0F);
   }

   // Move the hardware cursor back to the start.
   cur_y = 0;
   cur_x = 0;
   fb_move_cursor();
}


/*  serial_configure_baud_rate:
 *  Sets speed of the data being sent. Default speed is 115200 bits/s.
 *  Argument is a divisor, so the new speed is the default speed divided
 *  by the divisor
 * 
 *  @param com      Target COM Port
 *  @param divisor  Divisor
 */
void serial_configure_baud_rate(unsigned short com, unsigned short divisor) {
    outb( SERIAL_LINE_COMMAND_PORT(com), 
        SERIAL_LINE_ENABLE_DLAB );
    outb( SERIAL_DATA_PORT(com), 
        ( divisor >> 8 ) & 0x00FF );
    outb( SERIAL_DATA_PORT(com), 
        divisor & 0x00FF );
}
/** serial_configure_line:
 *  Configures the line of the given serial port. 
 *  length is 8 bits, no parity bits, one stop bit, and break control disabled.
 *
 *  @param com  The serial port to configure
 */
void serial_configure_line(unsigned short com)
{
    /* Bit:     | 7 | 6 | 5 4 3 | 2 | 1 0 |
     * Content: | d | b | prty  | s | dl  |
     * Value:   | 0 | 0 | 0 0 0 | 0 | 1 1 | = 0x03
     */
    outb(SERIAL_LINE_COMMAND_PORT(com), 0x03);
}
/** serial_is_transmit_fifo_empty:
 *  Checks if the transmit FIFO queue is empty or not for the given COM port.
 * 
 *  @param  com     The COM port
 *  @return 0 if FIFO queue is not empty
 *          1 if FIFO queue is empty
 */
int serial_is_transmit_fifo_empty(unsigned int com) {
    // 0x20 = 0010 0000
    return inb(SERIAL_LINE_STATUS_PORT(com)) & 0x20;
}
