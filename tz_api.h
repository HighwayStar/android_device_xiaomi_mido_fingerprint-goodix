/*
 * Copyright (C) 2016 Shane Francis / Jens Andersen
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __TZAPI_H_
#define __TZAPI_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FP_TZAPP_PATH "/firmware/image/"
#define FP_TZAPP_NAME "goodixfp"

#define BUFFER_SIZE 64

//enumerate tz app command ID's
enum fingerprint_cmd_t {
	GX_INIT = 0x1,
	GX_RESET = 0x2,
	GX_SET_MODE = 0x3,
	GX_GET_MODE = 0x4,
	GX_REGISTER = 0x5,
	GX_CANCEL_REGISTER = 0x6,
	GX_SAVE_REGISTERED = 0x7,
	GX_GET_FP_BY_NAME = 0x8,
	GX_CHANGE_FP_BY_NAME = 0x9,
	GX_RECOGNIZE = 0xA,
	GX_DEL_FP_TEMPLATE = 0xC,
	GX_GET_FP_TEMPLATE_LIST = 0xD,
	GX_CHANGE_PASSWORD = 0xE,
	GX_VERIFY_PASSWORD = 0xF,
	GX_LOAD_ALOG_PARAMS = 0x10,
	GX_DRIVER_TEST = 0x11,
	GX_GET_STATUS = 0x12,
	GX_CLEAN_STATUS = 0x13,
	GX_CALIB = 0x14,
	GX_ESD_CHECK = 0x15,
	GX_FW_ISUPDATE = 0x16,
	GX_FW_UPDATEPRE = 0x17,
	GX_FW_UPDATE = 0x18,
	GX_DOWNLOAD_CFG = 0x19,
	GX_GET_LB_STATUS = 0x1A,
	GX_CLEAN_LB_STATUS = 0x1B,
	GX_GET_FORCE_VALUE = 0x1C,
	GX_GET_BITMAP = 0x1D,
	GX_REG_FROM_BMP = 0x1E,
	GX_REG_SAVE = 0x1F,
	GX_VERIFY_BMP = 0x20,
	GX_REG_FROM_BMP_CANCEL = 0x21,
	GX_DEL_BMP_TEMPLATE = 0x22,
	GX_GET_HW_INFO = 0x23,
	GX_MP_TEST = 0x24,
	GX_CONFIRM_STATUS = 0x26,
	GX_UPDATE_TEMPLATE = 0x27,
	GX_PREPROCESSOR_INIT = 0x29,
	GX_GET_VERSION = 0x2A,
	GX_SET_ENROLL_COUNT = 0x2B,
	GX_GET_ENROLL_COUNT = 0x2C,
	GX_PAUSE_CAPTURE = 0x2D,
	GX_SET_SESSION_ID = 0x2F,
	GX_GET_SESSION_ID = 0x30,
	GX_SET_ACTIVE_GROUP = 0x31,
	GX_SET_ACTIVE_CLIENT = 0x32,
	GX_GEN_AUTH_TOKEN_HMAC = 0x33,
	GX_ENROLL_VERIFY = 0x34,
	GX_START_DATA_MODIFIED = 0x36,
	GX_END_DATA_MODIFIED = 0x36,
	GX_SET_USER_ID = 0x38,
	GX_LOAD_ALL_FPDATA = 0x39,
	GX_SAVE_REGISTERED_FP_WITH_ID = 0x3A,
	GX_SET_FP_DB = 0x3B,
	GX_CFG_FDT_DOWN = 0x3C,
	GX_REVERSE_CHIP = 0x3D,
	GX_RESET_CHIP = 0x3E,
	GX_UPDATE_FDT_DOWN_UP = 0x3F,
	GX_UPDATE_FDT_UP_REG = 0x40,
	GX_MF_KEY_FDT_TOUCHED = 0x46,
	GX_SET_LCD_STATUS = 0x47,
	GX_NAV_GET_STATUS = 0x50,
	GX_NAV_CLEAN_STATUS = 0x51,
	GX_NAV = 0x52,
	GX_NAV_GET_BASE = 0x53,
	GX_NAV_CAPTURE = 0x54,
	GX_NAV_GET_FRAME = 0x55,
	GX_NAV_GET_DEBUG_FRAME = 0x56,
	GX_SET_SAFE_CLASS = 0x64
};   


typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t length; //Some length of data supplied by previous modified command
    uint32_t extra; //Some length of data supplied by previous modified command
} fpc_send_std_cmd_t;



typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t length; //Some length of data supplied by previous modified command
    uint32_t id; //Some length of data supplied by previous modified command
} fpc_send_auth_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t ret_val; //Some cases this is used for return value of the command
    uint32_t na1; // always 0x45 ?
    uint32_t na2; //???
    uint32_t na3; //???
    uint32_t na4; //???
    uint32_t print_index;
} fpc_send_enroll_start_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t p1; //Some cases this is used for return value of the command
    uint32_t p2; // always 0x45 ?
    uint32_t p3; //???
    uint32_t p4; //???
    uint32_t p5; //???
    uint32_t print_count;
} fpc_get_pint_index_cmd_t;

typedef struct {
    uint32_t cmd_id;
    uint32_t v_addr; //Virtual address of ion mmap buffer
    uint32_t length; //Length of data on ion buffer
    uint32_t extra; //???
} fpc_send_mod_cmd_t;


typedef struct {
    uint32_t cmd_id;
    uint32_t v_addr;
    uint64_t ret_val; //64bit int
} fpc_send_int64_cmd_t;

#ifdef __cplusplus
}
#endif
#endif
