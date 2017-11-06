#include "goodix_imp.h"

#define LOG_TAG "GOODIX IMP"
#define LOG_NDEBUG 0
#include <cutils/log.h>
#include <string.h>
#include <errno.h>
#include <sys/ioctl.h>

static int qsee_load_trustlet(struct QSEECom_handle **clnt_handle,
                       const char *path, const char *fname,
                       uint32_t sb_size)
{
    int ret = 0;
    char* errstr;

    ALOGV("Starting app %s\n", fname);
    ret = mStartApp(clnt_handle, path, fname, sb_size);
    if (ret < 0) {
        errstr = qsee_error_strings(ret);
        ALOGE("Could not load app %s. Error: %s (%d)\n",
              fname, errstr, ret);
    } else
        ALOGI("TZ App loaded : %s\n", fname);

    return ret;
}


int send_normal_command(uint32_t cmd, uint32_t param, struct QSEECom_handle * handle)
{

    ALOGI("TA command %d , param %d\n", cmd, param);
    fpc_send_std_cmd_t* send_cmd = (fpc_send_std_cmd_t*) handle->ion_sbuffer;
    fpc_send_std_cmd_t* rec_cmd = (fpc_send_std_cmd_t*) handle->ion_sbuffer + 64;

    send_cmd->cmd_id = cmd;
    send_cmd->ret_val = param;

    int ret = send_cmd_fn(handle,send_cmd,64,rec_cmd,64);

    if(ret < 0) {
        ALOGE("TA command %d failed\n", cmd);
        return -1;
    }

    ALOGI("TA cmd %d result %d\n", cmd, rec_cmd->ret_val);

    return rec_cmd->ret_val;
}


int init_netlink()
{
    //TODO implement netlink
    return 0;
}


int goodix_init()
{

    mHandle = 0;
    device_fd=0;


    ALOGV("INIT GOODIX TZ APP\n");

    if (open_handle() < 1) {
        ALOGE("Qseecom Lib Not Open !\n");
        return -1;
    }
    ALOGV("LOADING TZ APP\n");
    if (qsee_load_trustlet(&mHandle, FP_TZAPP_PATH,
                             FP_TZAPP_NAME, 1024) < 0) {
        ALOGE("Failed to load tzapp, or already loaded\n");
        return -1;
    }

    if(goodix_device_enable()){ 
        ALOGE("fingerprint device enable failed!\n");
        mStopApp(&mHandle);
    };

    if(send_normal_command(GX_INIT,0, mHandle)) {
        ALOGE("Failed to init TA\n");
        mStopApp(&mHandle);
    }

    if(send_normal_command(GX_PREPROCESSOR_INIT,0, mHandle) < 0) {
       ALOGE("Failed preprocessor init!\n");
    }

    return 0;
}

int goodix_device_enable()
{
    ALOGD("%s", __func__);
    if(!goodix_open()) {
        ALOGE("Failed to open device!\n");
        return -1;
    }

    goodix_enable_clk();
    //set_speed not implemented

    if (goodix_device_check_reset()) {
        ALOGE("Failed to reset device!\n");
        return -1;
    }

    goodix_hw_reset();
    init_netlink();

    return 0;
}

int goodix_device_check_reset()
{
   ALOGD("%s", __func__);
   int result = 0;
   int ret = send_normal_command(GX_GET_STATUS,0, mHandle);
   if (ret) {
       ALOGE("Failed to get status 0x%x\n", ret);
       return -1;
   }

   ret = send_normal_command(GX_CLEAN_STATUS,0, mHandle);
   if (ret & 0x100) {
       ALOGE("Failed to clean status: 0x%x\n", ret);
   }

   //add mutex here
   goodix_disable_irq();
   if (goodix_hw_reset()) {
       ALOGE("Failed to reset\n");
       result = -1;
   }


   ret = send_normal_command(GX_GET_STATUS,0, mHandle);
   if(ret) {
      ALOGE("Failed to get status 0x%x", ret);
      result =-1;
   }

   goodix_enable_irq();
   //unlock mutex

   if(!(ret & 0x100)) {
      ALOGE("Failed to check: 0x%x\n", ret);
    //  result = -1; // not sure about this
   } 

   return result;
}

int goodix_enable_clk()
{
    int ret = 0;
    if(device_fd) {
        ret = ioctl(device_fd, GF_IOC_CLK_READY);
        if (ret < 0) {
             ALOGE("%s: Failed to do GF_IOC_CLK_READY, error: %d, %s\n", __func__, errno, strerror(errno));
             return -1;
        }
    } else {
        ALOGE("%s: Device handle is NULL to do IOCTL ops\n", __func__);
        return -1;
    }
    return 0;
}


int goodix_enable_gpio()
{
    int ret = 0;
    if(device_fd) {
        ret = ioctl(device_fd, GF_IOC_ENABLE_GPIO);
        if (ret < 0 ) {
             ALOGE("%s: Failed to do GF_IOC_ENABLE_GPIO, error: %d, %s\n", __func__, errno, strerror(errno));
             return -1;
        }
    } else {
        ALOGE("%s: Device handle is NULL to do IOCTL ops\n", __func__);
        return -1;
    }
    return 0;
}

int goodix_open()
{
    if (device_fd) {
       ALOGV("Device has been opened\n");
       return device_fd;
    }
    device_fd = open(DEVICE_NAME, O_RDWR, 0);
    if (device_fd < 0) {
        ALOGE("%s:%d: Failed to open file %s, error:%s\n",
                __func__, __LINE__, DEVICE_NAME, strerror(errno));
        return -errno;
    }

    if (goodix_enable_gpio()) {
       ALOGE("Failed to enable GPIO!\n");
       return -1;
    }
    return device_fd;
}

int goodix_enable_irq()
{
    int ret = 0;
    if(device_fd) {
        ret = ioctl(device_fd, GF_IOC_ENABLE_IRQ);
        if (ret < 0) {
             ALOGE("%s: Failed to do GF_IOC_ENABLE_IRQ, error: %d, %s\n", __func__, errno, strerror(errno));
             return -1;
        }
    } else {
        ALOGE("%s: Device handle is NULL to do IOCTL ops\n", __func__);
        return -1;
    }
    return 0;
}

int goodix_disable_irq()
{
    int ret = 0;
    if(device_fd) {
        ret = ioctl(device_fd, GF_IOC_DISABLE_IRQ);
        if (ret < 0) {
             ALOGE("%s: Failed to do GF_IOC_DISABLE_IRQ, error: %d, %s\n", __func__, errno, strerror(errno));
             return -1;
        }
    } else {
        ALOGE("%s: Device handle is NULL to do IOCTL ops\n", __func__);
        return -1;
    }
    return 0;
}

int goodix_hw_reset()
{
    int ret = 0;
    if(device_fd) {
        ret = ioctl(device_fd, GF_IOC_RESET);
        if (ret < 0) {
             ALOGE("%s: Failed to do GF_IOC_HW_RESET, error: %d, %s\n", __func__, errno, strerror(errno));
             return -1;
        }
    } else {
        ALOGE("%s: Device handle is NULL to do IOCTL ops\n", __func__);
        return -1;
    }
    return 0;
}


int goodix_ta_GetStatus()
{
    send_normal_command(GX_GET_STATUS,0,mHandle);
    return 0;
}