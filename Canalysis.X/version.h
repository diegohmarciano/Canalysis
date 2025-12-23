/**
 * @file version.h
 * @brief CANalysis firmware version constants
 */

#ifndef VERSION_H
#define VERSION_H

// Firmware version
#define FW_VERSION_MAJOR    1
#define FW_VERSION_MINOR    0
#define FW_VERSION_STRING   "v1.0"
#define FW_RELEASE_NAME     "SavvyCAN Integration"

// Build number for GVRET protocol (SavvyCAN compatibility)
#define FW_BUILD_NUM        110

// Build date/time
#define FW_BUILD_DATE       __DATE__
#define FW_BUILD_TIME       __TIME__

// Device identification
#define FW_DEVICE_NAME      "CANalysis USB-CAN Bridge"
#define FW_MCU              "PIC32MX795F512L"
#define FW_CLOCK_MHZ        48

#endif // VERSION_H
