#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H

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