#ifndef __GOODIXIMPL_H_
#define __GOODIXIMPL_H_

#include "QSEEComFunc.h"
#include "tz_api.h"

struct QSEECom_handle * mHandle;
int device_fd;

int goodix_init();
int goodix_device_enable();
int goodix_enable_gpio();
int goodix_open();
int goodix_enable_clk();
int goodix_disable_irq();
int goodix_enable_irq();
int goodix_hw_reset();
int goodix_device_check_reset();


int init_netlink();

//TZ app functions

int goodix_ta_GetStatus();

#define DEVICE_NAME "/dev/goodix_fp"

//goodix spi ioctls, maybe include it from kernel herader in future
#define  GF_IOC_DISABLE_IRQ 0x4700
#define  GF_IOC_ENABLE_IRQ 0x4701
#define  GF_IOC_SETSPEED 0x40044702
#define  GF_IOC_RESET 0x4703
#define  GF_IOC_COOLBOOT 0x4704
#define  GF_IOC_SENDKEY 0x4705
#define  GF_IOC_CLK_READY 0x4706
#define  GF_IOC_CLK_UNREADY 0x4707
#define  GF_IOC_PM_FBCABCK 0x4708
#define  GF_IOC_POWER_ON 0x4709
#define  GF_IOC_POWER_OFF 0x470A
#define  GF_IOC_ENABLE_GPIO 0x470B
#define  GF_IOC_RELEASE_GPIO 0x470C

#endif //__GOODIXIMPL_H_