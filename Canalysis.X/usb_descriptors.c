/*
 * USB Descriptors for CANalysis - CDC ACM (Virtual COM Port)
 * Based on M-Stack CDC ACM example
 */

#include "usb_config.h"
#include "usb/usb.h"
#include "usb/usb_ch9.h"
#include "usb/usb_cdc.h"

/* Configuration Packet structure
 * Contains all descriptors for this configuration */
struct configuration_1_packet {
    struct configuration_descriptor  config;
    struct interface_association_descriptor iad;
    
    /* CDC Communication Interface */
    struct interface_descriptor      cdc_class_interface;
    struct cdc_functional_descriptor_header cdc_func_header;
    struct cdc_acm_functional_descriptor cdc_acm;
    struct cdc_union_functional_descriptor cdc_union;
    struct endpoint_descriptor       cdc_ep;
    
    /* CDC Data Interface */
    struct interface_descriptor      cdc_data_interface;
    struct endpoint_descriptor       data_ep_in;
    struct endpoint_descriptor       data_ep_out;
};

/* Device Descriptor
 * VID: 0x04D8 (Microchip)
 * PID: 0x000A (CDC RS-232 Emulation Demo) */
const struct device_descriptor this_device_descriptor = {
    sizeof(struct device_descriptor),   // bLength
    DESC_DEVICE,                        // bDescriptorType
    0x0200,                             // bcdUSB (USB 2.0)
    DEVICE_CLASS_MISC,                  // bDeviceClass (0xEF for IAD)
    0x02,                               // bDeviceSubClass (Common Class)
    0x01,                               // bDeviceProtocol (Interface Association)
    EP_0_LEN,                           // bMaxPacketSize0
    0x04D8,                             // idVendor (Microchip)
    0x000A,                             // idProduct (CDC RS-232 Demo)
    0x0100,                             // bcdDevice (1.0)
    1,                                  // iManufacturer (string index)
    2,                                  // iProduct (string index)
    3,                                  // iSerialNumber (string index)
    NUMBER_OF_CONFIGURATIONS            // bNumConfigurations
};

/* Configuration Descriptor */
static const struct configuration_1_packet configuration_1 = {
    /* Configuration Descriptor */
    {
        sizeof(struct configuration_descriptor),
        DESC_CONFIGURATION,
        sizeof(configuration_1),        // wTotalLength
        2,                              // bNumInterfaces
        1,                              // bConfigurationValue
        0,                              // iConfiguration
        0x80,                           // bmAttributes (bus powered)
        100/2,                          // bMaxPower (100mA)
    },
    
    /* Interface Association Descriptor (for composite device) */
    {
        sizeof(struct interface_association_descriptor),
        DESC_INTERFACE_ASSOCIATION,
        0,                              // bFirstInterface
        2,                              // bInterfaceCount
        CDC_COMMUNICATION_INTERFACE_CLASS,
        CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS,
        0,                              // bFunctionProtocol
        2,                              // iFunction
    },
    
    /* Interface 0: CDC Communication Class */
    {
        sizeof(struct interface_descriptor),
        DESC_INTERFACE,
        0x00,                           // bInterfaceNumber
        0x00,                           // bAlternateSetting
        0x01,                           // bNumEndpoints
        CDC_COMMUNICATION_INTERFACE_CLASS,    // bInterfaceClass (0x02)
        CDC_COMMUNICATION_INTERFACE_CLASS_ACM_SUBCLASS, // bInterfaceSubClass (0x02)
        0x00,                           // bInterfaceProtocol
        0,                              // iInterface
    },
    
    /* CDC Header Functional Descriptor */
    {
        sizeof(struct cdc_functional_descriptor_header),
        DESC_CS_INTERFACE,
        CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_HEADER,
        0x0110,                         // bcdCDC (CDC 1.10)
    },
    
    /* CDC ACM Functional Descriptor */
    {
        sizeof(struct cdc_acm_functional_descriptor),
        DESC_CS_INTERFACE,
        CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_ACM,
        CDC_ACM_CAPABILITY_LINE_CODINGS,  // bmCapabilities
    },
    
    /* CDC Union Functional Descriptor */
    {
        sizeof(struct cdc_union_functional_descriptor),
        DESC_CS_INTERFACE,
        CDC_FUNCTIONAL_DESCRIPTOR_SUBTYPE_UNION,
        0,                              // bMasterInterface (interface 0)
        1,                              // bSlaveInterface0 (interface 1)
    },
    
    /* Endpoint 1 IN - CDC Notification */
    {
        sizeof(struct endpoint_descriptor),
        DESC_ENDPOINT,
        0x81,                           // bEndpointAddress (EP1 IN)
        EP_INTERRUPT,                   // bmAttributes
        EP_1_IN_LEN,                    // wMaxPacketSize
        16,                             // bInterval (16ms)
    },
    
    /* Interface 1: CDC Data Class */
    {
        sizeof(struct interface_descriptor),
        DESC_INTERFACE,
        0x01,                           // bInterfaceNumber
        0x00,                           // bAlternateSetting
        0x02,                           // bNumEndpoints
        CDC_DATA_INTERFACE_CLASS,       // bInterfaceClass (0x0A)
        0x00,                           // bInterfaceSubClass
        CDC_DATA_INTERFACE_CLASS_PROTOCOL_NONE, // bInterfaceProtocol
        0,                              // iInterface
    },
    
    /* Endpoint 2 IN - Bulk Data */
    {
        sizeof(struct endpoint_descriptor),
        DESC_ENDPOINT,
        0x82,                           // bEndpointAddress (EP2 IN)
        EP_BULK,                        // bmAttributes
        EP_2_IN_LEN,                    // wMaxPacketSize
        0,                              // bInterval
    },
    
    /* Endpoint 2 OUT - Bulk Data */
    {
        sizeof(struct endpoint_descriptor),
        DESC_ENDPOINT,
        0x02,                           // bEndpointAddress (EP2 OUT)
        EP_BULK,                        // bmAttributes
        EP_2_OUT_LEN,                   // wMaxPacketSize
        0,                              // bInterval
    },
};

/* String Descriptors */

/* String 0: Supported Languages (US English) */
static const struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t lang; } str00 = {
    sizeof(str00),
    DESC_STRING,
    0x0409  // US English
};

/* String 1: Manufacturer */
static const struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[9]; } vendor_string = {
    sizeof(vendor_string),
    DESC_STRING,
    {'C','A','N','a','l','y','s','i','s'}
};

/* String 2: Product */
static const struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[17]; } product_string = {
    sizeof(product_string),
    DESC_STRING,
    {'C','A','N','a','l','y','s','i','s',' ','U','S','B',' ','C','D','C'}
};

/* String 3: Serial Number */
static const struct { uint8_t bLength; uint8_t bDescriptorType; uint16_t chars[8]; } serial_string = {
    sizeof(serial_string),
    DESC_STRING,
    {'0','0','0','0','0','0','0','1'}
};

/* String Descriptor Function - called by M-Stack */
int16_t usb_application_get_string(uint8_t string_number, const void **ptr)
{
    if (string_number == 0) {
        *ptr = &str00;
        return sizeof(str00);
    }
    else if (string_number == 1) {
        *ptr = &vendor_string;
        return sizeof(vendor_string);
    }
    else if (string_number == 2) {
        *ptr = &product_string;
        return sizeof(product_string);
    }
    else if (string_number == 3) {
        *ptr = &serial_string;
        return sizeof(serial_string);
    }
    
    return -1;
}

/* Configuration Descriptor List */
const struct configuration_descriptor *usb_application_config_descs[] = {
    (struct configuration_descriptor*) &configuration_1,
};

