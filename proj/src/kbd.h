#ifndef __KBD_H
#define __KBD_H

#define KBD_IRQ 1

#define KEY_ESC_BREAK 0x81

#define KEY_W_MAKE 0x11
#define KEY_W_BREAK 0x91
#define KEY_A_MAKE 0x1E
#define KEY_A_BREAK 0x9E
#define KEY_S_MAKE 0x1F
#define KEY_S_BREAK 0x9F
#define KEY_D_MAKE 0x20
#define KEY_D_BREAK 0xA0

#define KEY_ARROWUP_MAKE 0xE048
#define KEY_ARROWUP_BREAK 0xE0C8
#define KEY_ARROWLEFT_MAKE 0xE04B
#define KEY_ARROWLEFT_BREAK 0xE0CB
#define KEY_ARROWDOWN_MAKE 0xE050
#define KEY_ARROWDOWN_BREAK 0xE0D0
#define KEY_ARROWRIGHT_MAKE 0xE04D
#define KEY_ARROWRIGHT_BREAK 0xE0CD

#define KEY_ENTER_MAKE 0x1C

/**
 * @brief Subscribes to keyboard interrupts
 * @return ERROR or OKAY values, as defined
 */

int kbd_subscribe_int();

/**
 * @brief Unsubscribes to keyboard interrupts
 * @return ERROR or OKAY values, as defined
 */

int kbd_unsubscribe_int();

/**
 * @brief Reads data from OUT_BUF
 * @param data data to read from adress
 * @return int ERROR or OKAY values, as defined
 */

int kbc_read(unsigned long *data);

/**
 * @brief Reads scancode
 * @return 2 byte Scancode
 */

unsigned short kbd_int_handler ();


#endif /* end of __KBD_H */
