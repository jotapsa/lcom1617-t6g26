#ifndef __I8042_H
#define __I8042_H

#define BIT(n) (0x01<<(n))

#define KEY_ESC_BREAK 0x81
#define SPECIAL_KEY 0xE0

#define KBD_IRQ 1

/* KBC Registers */
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define IN_BUF 0x60
#define OUT_BUF 0x60
#define OBF BIT(0)
#define IBF BIT(1)

/*KBD Comands*/
#define CMD_LED 0xED
#define CMD_RATE_DELAY 0xF3

/*KBC Command Responses*/
#define RSP_ACK 0xFA
#define RSP_RESEND 0xFE
#define RSP_ERROR 0xFC

/*LEDS*/
#define SCROLL_LOCK BIT(0)
#define NUM_LOCK BIT(1)
#define CAPS_LOCK BIT(2)


#define PAR_ERR BIT(7)
#define TO_ERR BIT(6)

#define DELAY_US  20000

#define KBD_LED_CMD 0xED

#define ERROR -1
#define OK 0

#endif /* __I8042_H */
