#ifndef _TUSB_CONFIG_H_
#define _TUSB_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

//--------------------------------------------------------------------
// COMMON CONFIGURATION
//--------------------------------------------------------------------
#define CFG_TUSB_MCU OPT_MCU_RP2040

// RHPort number used for Host settings
#define BOARD_TUSB_RHPORT 0

//--------------------------------------------------------------------
// HOST CONFIGURATION
//--------------------------------------------------------------------
#define CFG_TUH_ENABLED 1

// Size of the buffer to hold HID reports
#define CFG_TUH_HID_EP_BUFSIZE 64

// Number of HID instances to support (Composite Keyboard + Mouse = 2 at least)
// Setting to 4 for safety (Hubs etc)
#define CFG_TUH_HID 4

#ifdef __cplusplus
}
#endif

#endif // _TUSB_CONFIG_H_
