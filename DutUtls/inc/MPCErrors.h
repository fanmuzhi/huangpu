#ifndef __MPCErrors__
#define __MPCErrors__


////////////////////////////////////////////////////////////////////////////////
//Copyright (c) 2012 Synaptics Inc. All Rights Reserved
//
//Errors.h
//
//All errors that the MPC04 application can generate.
//
//Version   When      Why
//--------  --------  --------------------------------------------------------
//01.00.00  02/10/12  Initial release.
//
////////////////////////////////////////////////////////////////////////////////

#if  SPCFirmware
  #define ERROR_BASE 0x2001
#elif  SPCBootLoader
  #define ERROR_BASE 0x1001
#elif  MPCFirmware
  #define ERROR_BASE 0x2001
#elif  MPCBootLoader
  #define ERROR_BASE 0x1001
#elif  MPCHostApp
  #define ERROR_BASE 0x0001
#else
  #error The firmware type must be defined
#endif

class Errors
{
public:
  enum  { NO_MPC_ERROR = 0,                         //MPC04's error indicator to host.
          UNKNOWN_COMMAND = ERROR_BASE,
          INSUFFICIENT_FLASH,
          INSUFFICIENT_RAM,
          FLASH_WRITE_FAILED,
          FLASH_ERASE_FAILED,
          FLASH_ERASE_ON_ADR_ZERO,
          NOT_IN_BOOT_MODE,
          ALREADY_IN_BOOT_MODE,
          NUM_BYTES_NOT_MULT_4,
          ADDRESS_NOT_MULT_4,
          SPECIFIED_MEM_OUT_OF_BOUNDS,
          ADDRESS_ERROR,
          SIZE_ERROR,
          UNKNOWN_FILE_INDEX,
          UNKNOWN_FILE_ID,
          INVALID_DATA_RANGE,               //0x2010
          FILE_IS_ACTIVE,
          FILE_IS_NOT_SCRIPT,
          FILE_ALREADY_EXISTS,
          FILE_NOT_FOUND,
          MAX_FILES_EXCEEDED,
          I2C_0_FAULT,
          I2C_1_FAULT,
          ASIC_COULD_NOT_ENTER_BOOT_MODE,
          REFLASH_ERR_SUCCESS,              //The reflash errors must be in
          REFLASH_ERR_UNUSED,               //sequential order.
          REFLASH_ERR_NOT_ENABLED,
          REFLASH_ERR_INVALID_BLOCK_NUM,
          REFLASH_ERR_BLOCK_NOT_ERASED,
          REFLASH_ERR_INVALID_ERASE_KEY,
          REFLASH_ERR_UKNOWN,
          REFLASH_ERR_DEVICE_RESET,         //0x2020
          INVALID_FXN_OR_REG_SPECIFIER,
          LIMIT_OF_4K_DATA,
          INVALID_PORT_ID,
          INVALID_GPIO_PORT_ID,
          INVALID_PIN_TYPE,
          EXTERNAL_PORT_NOT_SET,
          LOCKDOWN_NOT_ALLOWED_ON_LOCKED_MODULE,
          ARG_OUT_OF_RANGE,
          TIMEOUT_ON_HID_RMI_READ,
          FLASH_WRITE_DID_NOT_STICK,
          NUM_BYTES_MUST_BE_MULT_2,
          VIRTUAL_PORT_NOT_SET,
          INVALID_ARG_INDEX,
          INVALID_ARG_VALUE,
          SDA_DID_NOT_FALL_DURING_OSC_CAL,
          SCL_DID_NOT_TOGGLE_DURING_OSC_CAL,  //0x2030
          WRITE_VALIDATION_FAILURE,
          ASIC_CLOCK_PROBLEMS_DURING_OSC_CAL,
          ACTIVE_SCRIPT,
          SCRIPT_TOO_BIG,
          IO_ERROR,
          PLL_DID_NOT_STABILIZE,
          FS_USB_DEV_NOT_ATTACHED,
          INVALID_MAX3421E_VERSION,
          USB_ENUMERATION_FAILURE,
          I2C_ADR_NOT_FOUND,
          USB_RMI_WRITE_FAILURE,
          USB_RMI_READ_FAILURE,
          SCRIPT_FAILURE,
          INVALID_TXN_ID,
          TIMEOUT_WAITING_ATTN,
          APP_IMG_LOC_WOULD_OVERWRITE_BOOTLOADER,       //0x2040
          BOOTLOADER_NOT_FOUND,
          FREQ_MEASUREMENT_TIMEOUT
        };
};

#endif //__MPCErrors__
