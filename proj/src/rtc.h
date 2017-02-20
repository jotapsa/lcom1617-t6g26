#ifndef __RTC_H
#define __RTC_H

#define RTC_IRQ 8

#define RTC_ADDR_REG 		0x70
#define RTC_DATA_REG 		0x71

#define RTC_SECONDS 		0
#define RTC_MINUTES 		2
#define RTC_HOURS 			4
#define RTC_DAY_OF_MONTH 	7
#define RTC_MONTH 			8
#define RTC_YEAR 			9

/**
 * @brief Converts data read from the RTC from hexadecimal to decimal
 *
 * @param hex data in hex
 * @return data in decimal
 */

unsigned long hex_to_dec(unsigned long hex);

/**
 * @brief Reads date from RTC
 *
 * @return unsigned long* pointer to data
 */

unsigned long* getdate();

/**
 * @brief Reads time from RTC
 *
 * @return unsigned long* pointer to time
 */

unsigned long* gettime();

#endif
