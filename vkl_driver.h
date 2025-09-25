/* 
Driver for VKLxxx series LCD driver chip
Supported chips:
- VKL060
- VKL076
- VKL092
- VKL128
- VKL144
*/

#ifndef VKL_H
#define VKL_H

#include <stdint.h> // uint8_t
#include <stddef.h> // NULL

#ifndef _VKL_port_i2c_write
#error "Please define _VKL_port_i2c_write() before including vkl_driver.h"
#endif

/*
// Example psudo code of a good _VKL_port_i2c_write() implementation

#define _VKL_port_i2c_write i2c_write

void i2c_write(uint8_t addr, uint8_t cmd, uint8_t *buf, uint8_t len) {
    i2c_start();
    i2c_send_addr(addr);
    i2c_send_byte(cmd);
    for (uint8_t i=0;i<len;i++) {
        i2c_send_byte(buf[i]);
    }
    i2c_stop();
}
*/

#define VKL_I2C_ADDR 0x7c

#define VKL_command(cmd) do { \
    _VKL_port_i2c_write(VKL_I2C_ADDR, (cmd), NULL, 0); \
} while(0)

#define VKL_MODE_SET_DISPLAY_OFF (0<<3)
#define VKL_MODE_SET_DISPLAY_ON (1<<3)
#define VKL_MODE_SET_1_3_BIAS (0<<2)
#define VKL_MODE_SET_1_2_BIAS (1<<2)

#define VKL_cmd_mode_set(display, bias) VKL_command(0b11000000|(((display)|(bias))&0b00001100))

#define VKL_DISCTL_FR_80 (0<<3)
#define VKL_DISCTL_FR_71 (1<<3)
#define VKL_DISCTL_FR_64 (2<<3)
#define VKL_DISCTL_FR_53 (3<<3)

#define VKL_DISCTL_DM_LINE_INV (0<<2)
#define VKL_DISCTL_DM_FRAME_INV (1<<2)

#define VKL_DISCTL_SR_LP1 (0<<0)
#define VKL_DISCTL_SR_LP2 (1<<0)
#define VKL_DISCTL_SR_NP (2<<0)
#define VKL_DISCTL_SR_HP (3<<0)

#define VKL_cmd_disctl(fr, dm, sr) VKL_command(0b10100000|(((fr)|(dm)|(sr))&(0b00011111)))

#define VKL_ICSET_AD_MSB_0 (0<<2)
#define VKL_ICSET_AD_MSB_1 (1<<2)
#define VKL_ICSET_NO_RESET (0<<1)
#define VKL_ICSET_SW_RESET (1<<1)
#define VKL_ICSET_OSC_INT (0<<0)
#define VKL_ICSET_OSC_EXT (1<<0)

#define VKL_cmd_icset(osc, rst, ad) VKL_command(0b11101000|(((osc)|(rst)|(ad))&(0b00000111)))

#define VKL_BLKCTL_BLINK_OFF 0
#define VKL_BLKCTL_BLINK_0_5HZ 1
#define VKL_BLKCTL_BLINK_1HZ 2
#define VKL_BLKCTL_BLINK_2HZ 3

#define VKL_cmd_blkctl(blink) VKL_command(0b11110000|((blink)&(0b00000011)))

#define VKL_APCTL_NORMAL 0
#define VKL_APCTL_ALL_PIXEL_OFF 1
#define VKL_APCTL_ALL_PIXEL_ON 2

#define VKL_cmd_apctl(all_px_on_off) VKL_command(0b11111100|((all_px_on_off)&(0b00000011)))

#define VKL_write_data(addr, p_buf, len) do { \
    uint8_t addr_msb = VKL_ICSET_AD_MSB_0; \
    if ((addr>>5)&1) { \
        addr_msb = VKL_ICSET_AD_MSB_1; \
    } \
    VKL_cmd_icset(VKL_ICSET_OSC_INT, VKL_ICSET_NO_RESET, addr_msb); \
    _VKL_port_i2c_write(VKL_I2C_ADDR, addr&0b00011111, p_buf, len); \
} while(0)

#endif
