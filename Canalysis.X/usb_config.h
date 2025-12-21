/*
 * M-Stack USB Configuration for CanalystII
 * PIC32MX795F512L - CDC ACM (Virtual COM Port)
 */

#ifndef USB_CONFIG_H__
#define USB_CONFIG_H__

/* Number of endpoint numbers besides endpoint zero.
 * CDC needs EP1 (notifications) and EP2 (data IN/OUT) */
#define NUM_ENDPOINT_NUMBERS 2

/* Endpoint 0 buffer size - 64 bytes for full-speed */
#define EP_0_LEN 64

/* Endpoint 1 - CDC ACM Notification endpoint (IN only) */
#define EP_1_OUT_LEN 1   /* Not used, but must be defined */
#define EP_1_IN_LEN 16   /* Notification packets */

/* Endpoint 2 - CDC Data endpoint (IN and OUT) */
#define EP_2_LEN 64
#define EP_2_OUT_LEN EP_2_LEN
#define EP_2_IN_LEN EP_2_LEN

/* Number of USB configurations */
#define NUMBER_OF_CONFIGURATIONS 1

/* Ping-pong buffering mode
 * PIC32MX only supports PPB_ALL */
#define PPB_MODE PPB_ALL

/* PIC32MX USB OTG - Required for D+ pullup! */
#define USE_OTG

/* Use interrupts for USB (recommended for reliable operation) */
#define USB_USE_INTERRUPTS

/* Descriptor objects from usb_descriptors.c */
#define USB_DEVICE_DESCRIPTOR this_device_descriptor
#define USB_CONFIG_DESCRIPTOR_MAP usb_application_config_descs
#define USB_STRING_DESCRIPTOR_FUNC usb_application_get_string

/* Application callbacks */
#define SET_CONFIGURATION_CALLBACK app_set_configuration_callback
#define GET_DEVICE_STATUS_CALLBACK app_get_device_status_callback
#define ENDPOINT_HALT_CALLBACK     app_endpoint_halt_callback
#define SET_INTERFACE_CALLBACK     app_set_interface_callback
#define GET_INTERFACE_CALLBACK     app_get_interface_callback
#define OUT_TRANSACTION_CALLBACK   app_out_transaction_callback
#define IN_TRANSACTION_COMPLETE_CALLBACK app_in_transaction_complete_callback
#define UNKNOWN_SETUP_REQUEST_CALLBACK app_unknown_setup_request_callback
#define UNKNOWN_GET_DESCRIPTOR_CALLBACK app_unknown_get_descriptor_callback
#define START_OF_FRAME_CALLBACK    app_start_of_frame_callback
#define USB_RESET_CALLBACK         app_usb_reset_callback

/* CDC Configuration callbacks */
#define CDC_SET_LINE_CODING_CALLBACK app_set_line_coding_callback
#define CDC_GET_LINE_CODING_CALLBACK app_get_line_coding_callback
#define CDC_SET_CONTROL_LINE_STATE_CALLBACK app_set_control_line_state_callback

#endif /* USB_CONFIG_H__ */

