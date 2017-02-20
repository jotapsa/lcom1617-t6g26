#ifndef __I8042_H
#define __I8042_H

#define SPECIAL_KEY 0xE0

/* KBC Registers */
#define STAT_REG 0x64
#define KBC_CMD_REG 0x64
#define IN_BUF 0x60
#define OUT_BUF 0x60

#define OBF BIT(0)
#define IBF BIT(1)
#define AUX BIT(5)
#define TO_ERR BIT(6)
#define PAR_ERR BIT(7)

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

/*MOUSE*/
#define MOUSE_WRITE_CMD 0xD4
#define ENABLE_MOUSE 0xA8
#define MOUSE_ENB 0xF4
#define MOUSE_DISB 0xF5
#define MOUSE_STREAM_MODE 0xEA
#define MOUSE_STATUS_REQ 0xE9

#define DELAY_US  20000

#define KBD_LED_CMD 0xED

#endif /* __I8042_H */
