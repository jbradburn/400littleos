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
#define FB_WHITE        7
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
unsigned short location = DEFAULT_LINE_OFFSET*DEFAULT_LINE_SIZE + DEFAULT_CHAR_OFFSET;


/** fb_move_cursor:
 * Moves the fb cursor to a given position
 * 
 * @param pos The new position of the cursor
 */
void fb_move_cursor(unsigned short pos) {
    outb(FB_COMMAND_PORT,   FB_HIGH_BYTE_COMMAND);
    outb(FB_DATA_PORT,      ((pos >> 8) & 0x00FF));
    outb(FB_COMMAND_PORT,   FB_LOW_BYTE_COMMAND); 
    outb(FB_DATA_PORT,      pos & 0x00FF);
}
/** fb_write_cell:
 *  Write a char with bg and fg to pos i in the framebuffer
 * 
 *  @param i    Framebuffer position
 *  @param c    Character to write
 *  @param fg   Foreground color
 *  @param bg   Background color
 */
void fb_write_cell(unsigned int i, char c, unsigned char fg, unsigned char bg) {
    fb[i] = c;
    fb[i + 1] = ((fg & 0x0F) << 4) | (bg & 0x0F);
}
/** fb_write:
 *  Writes contents of char* buf to screen
 * 
 *  @param buf  char buffer holding data
 *  @param len  size of buffer
 */
void fb_write(char* buf, unsigned int len) {
    unsigned int i;
    unsigned int cur = location;
    for ( i = 0; i < len-1; i++ ) {
        fb_write_cell( location, (buf[i]), FB_BLACK, FB_WHITE );
        fb_move_cursor( location );
        location += 2;
    }
    location = cur;
    location += 160;
    fb_move_cursor(location);
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