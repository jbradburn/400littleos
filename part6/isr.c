#include "io.h"
#include "isr.h"

void interrupt_handler(reg_t reg) {
    fb_write("Interrupt: ", 11);
    char i = reg.int_no;
    fb_write_cell(i);
}