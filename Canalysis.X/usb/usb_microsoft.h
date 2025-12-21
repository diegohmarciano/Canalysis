/*
 * M-Stack Microsoft OS Descriptor Support - Stub Header
 * Not used in Canalysis - only needed for WinUSB
 */

#ifndef USB_MICROSOFT_H__
#define USB_MICROSOFT_H__

#include <stdint.h>

#pragma pack(push, 1)

struct microsoft_os_descriptor {
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint16_t qwSignature[7];
    uint8_t bMS_VendorCode;
    uint8_t bPad;
};

struct microsoft_extended_compat_header {
    uint32_t dwLength;
    uint16_t bcdVersion;
    uint16_t wIndex;
    uint8_t bCount;
    uint8_t reserved[7];
};

struct microsoft_extended_compat_function {
    uint8_t bFirstInterfaceNumber;
    uint8_t reserved1;
    uint8_t compatibleID[8];
    uint8_t subCompatibleID[8];
	uint8_t reserved2[6];
};

struct microsoft_extended_properties_header {
    uint32_t dwLength;
    uint16_t bcdVersion;
    uint16_t wIndex;
    uint16_t wCount;
};

struct microsoft_extended_property_section_header {
    uint32_t dwSize;
    uint32_t dwPropertyDataType;
};

#pragma pack(pop)

#endif /* USB_MICROSOFT_H__ */
