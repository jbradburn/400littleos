#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

/* GDT struct to hold start and size of segment descriptor table */
struct gdt {
    unsigned int address;
    unsigned short size;
} __attribute__((packed));

void fb_write(char*, unsigned short);
void fb_write_cell(char);
void fb_clear();
void fb_scroll();


/** outb:
 *  Sends given data to given I/O port, defined in io.s
 * 
 * @param port The I/O port to send data to
 * @param data The data to send
 */
void outb(unsigned short port, unsigned char data);

/** inb:
 *  Reads a byte from an I/O port.
 * 
 *  @param port Address of I/O port
 *  @return     The read byte
 */
unsigned char inb(unsigned short port);

#endif