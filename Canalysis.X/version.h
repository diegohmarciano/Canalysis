/**
 * @file version.h
 * @brief CANalysis firmware version constants
 */

#ifndef VERSION_H
#define VERSION_H

// Firmware version
#define FW_VERSION_MAJOR    0
#define FW_VERSION_MINOR    1
#define FW_VERSION_STRING   "v0.1"
#define FW_RELEASE_NAME     "Initial Test"

// Build date/time
#define FW_BUILD_DATE       __DATE__
#define FW_BUILD_TIME       __TIME__

// Device identification
#define FW_DEVICE_NAME      "CANalysis USB-CAN Bridge"
#define FW_MCU              "PIC32MX795F512L"
#define FW_CLOCK_MHZ        48

#endif // VERSION_H
