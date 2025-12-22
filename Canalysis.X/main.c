
// Configuration Bits for PIC32MX795F512L with 8 MHz crystal
// USB requires 48 MHz from USB PLL: 8MHz / 2 = 4MHz * 24 = 96MHz / 2 = 48MHz USB clock

#pragma config USERID = 0xFFFF          
#pragma config FSRSSEL = PRIORITY_7     
#pragma config FMIIEN = ON              
#pragma config FETHIO = ON              
#pragma config FCANIO = ON              
#pragma config FUSBIDIO = ON            
#pragma config FVBUSONIO = ON           

// System PLL: 8MHz / 2 = 4MHz * 24 = 96MHz / 2 = 48MHz SYSCLK
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (8MHz / 2 = 4MHz)
#pragma config FPLLMUL = MUL_24         // PLL Multiplier (4MHz * 24 = 96MHz)
#pragma config FPLLODIV = DIV_2         // System PLL Output Divider (96MHz / 2 = 48MHz)

// USB PLL: 8MHz / 2 = 4MHz to USB PLL -> 48MHz USB clock
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (8MHz / 2 = 4MHz)
#pragma config UPLLEN = ON              // USB PLL Enable

#pragma config FNOSC = PRIPLL           // Oscillator Selection (Primary with PLL)
#pragma config FSOSCEN = OFF            // Secondary Oscillator Disable
#pragma config IESO = OFF               // Internal/External Switch Over Disabled
#pragma config POSCMOD = HS             // Primary Oscillator (HS Crystal)
#pragma config OSCIOFNC = OFF           // CLKO Output Disabled
#pragma config FPBDIV = DIV_1           // Peripheral Bus Divisor (PBCLK = SYSCLK)
#pragma config FCKSM = CSDCMD           // Clock Switching Disabled, Clock Monitor Disabled
#pragma config WDTPS = PS1048576        
#pragma config FWDTEN = OFF             // Watchdog Timer Disabled
#pragma config DEBUG = OFF              
#pragma config ICESEL = ICS_PGx2        
#pragma config PWP = OFF                
#pragma config BWP = OFF                
#pragma config CP = OFF

#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attribs.h>
#include <sys/kmem.h>
#include <stdint.h>
#include <stdbool.h>

#include "usb.h"
#include "usb_cdc.h"
#include "CAN.h"
#include "version.h"
#include "gvret.h"
#include "can_driver.h"

// System Frequency Definitions
#define SYS_FREQ 48000000UL
#define PBCLK SYS_FREQ

// LED Definitions (active high)
// LED1 = USB power (hardware connected to VBUS)
// LED2 = Interface connected (DTR active)
// LED3 = CAN1 activity / USB data activity  
// LED4 = CAN2 activity / USB configured

#define LED_INTERFACE_TRIS  TRISGbits.TRISG14
#define LED_INTERFACE       LATGbits.LATG14

#define LED_CAN1_TRIS       TRISGbits.TRISG12
#define LED_CAN1            LATGbits.LATG12

#define LED_CAN2_TRIS       TRISGbits.TRISG13
#define LED_CAN2            LATGbits.LATG13

// Debug counters
static volatile uint32_t usb_reset_count = 0;
static volatile uint32_t usb_configured_count = 0;

// CDC state
static volatile bool cdc_connected = false;
static volatile bool send_welcome = false;

// Received command from CDC (for debug menu)
static volatile char received_cmd = 0;

// GVRET protocol context
static GVRET_Context gvret_ctx;

// DEBUG MODE: Raw CAN ID printing (bypasses GVRET)
static volatile bool debug_can_mode = false;
static volatile uint32_t debug_last_print = 0;
static volatile uint32_t debug_frame_count = 0;

// CAN Loopback test state
static volatile bool loopback_running = false;
static volatile bool loopback_stop_requested = false;
static volatile bool loopback_internal = false;  // true = internal loopback, false = external (CAN1->CAN2)
static volatile uint8_t loopback_can_module = 1; // 1 = CAN1, 2 = CAN2
static volatile uint32_t loopback_sent = 0;
static volatile uint32_t loopback_received = 0;
static volatile uint32_t loopback_timeouts = 0;
static volatile bool can_msg_received = false;
static volatile uint16_t can_rx_sid = 0;
static volatile uint8_t can_rx_dlc = 0;
static volatile uint8_t can_rx_data[8];

// CAN message buffers - must be aligned for DMA and in coherent (non-cached) memory
// The PIC32MX CAN module uses a SINGLE CONTIGUOUS buffer for ALL FIFOs!
// FIFO layout: [FIFO0 (16 bytes)] [FIFO1 (16 bytes)] [FIFO2 (16 bytes)] ...
// Hardware calculates: FIFO(n) address = FIFOBA + sum of sizes of FIFO(0..n-1)
//
// CRITICAL: Using 2KB alignment to force buffer into higher memory (away from IVT at 0x000-0x1000)
// Also making buffer much larger to ensure linker places it in a clean region
static uint8_t __attribute__((coherent, aligned(2048))) can1_fifo[32 * 16];  // 512 bytes for 32 messages
static uint8_t __attribute__((coherent, aligned(2048))) can2_fifo[32 * 16];  // 512 bytes for 32 messages

// Direct pointers into the contiguous buffer (calculated once)
// These MUST be used instead of PA_TO_KVA1(C1FIFOUAx) to avoid address mismatch!
#define CAN1_FIFO0_PTR ((volatile uint32_t *)&can1_fifo[0])    // TX buffer (offset 0)
#define CAN1_FIFO1_PTR ((volatile uint32_t *)&can1_fifo[16])   // RX buffer (offset 16)
#define CAN2_FIFO0_PTR ((volatile uint32_t *)&can2_fifo[0])    // RX buffer

// Delay Function
void delay_ms(unsigned int milliseconds) {
    while (milliseconds--) {
        unsigned int i;
        for (i = 0; i < (SYS_FREQ / 56072); i++) {
            asm volatile("nop");
        }
    }
}

// Function to blink LED
void blink_led_pin(volatile unsigned int *lat, unsigned int bit, uint8_t count, uint16_t on_ms, uint16_t off_ms) {
    for (uint8_t i = 0; i < count; i++) {
        *lat |= (1 << bit);
        delay_ms(on_ms);
        *lat &= ~(1 << bit);
        delay_ms(off_ms);
    }
}

// ============================================================================
// CDC Helper Functions
// ============================================================================

// Forward declarations
void cdc_send_string(const char *str);
void cdc_send_number(uint32_t num);
void cdc_send_hex_byte(uint8_t b);
void cdc_send_hex_sid(uint16_t sid);

// Track if endpoint 2 IN is busy
// Track if endpoint 2 IN is busy (shared with gvret.c)
volatile bool ep2_in_busy = false;

// Wait for endpoint to be ready (with timeout)
static void wait_ep2_ready(void) {
    uint32_t timeout = 50000;
    while (ep2_in_busy && timeout > 0) {
        usb_service();
        timeout--;
    }
    ep2_in_busy = false;
}

// Send a string over CDC (blocking until sent)
void cdc_send_string(const char *str) {
    if (!usb_is_configured() || !cdc_connected) return;
    
    size_t len = strlen(str);
    if (len == 0) return;
    
    // Wait for previous send to complete
    wait_ep2_ready();
    
    // Get the IN buffer for endpoint 2 (CDC data)
    unsigned char *buf = usb_get_in_buffer(2);
    if (!buf) return;
    
    // Copy string (max 64 bytes per packet)
    size_t send_len = (len > 64) ? 64 : len;
    memcpy(buf, str, send_len);
    
    // Mark as busy and send
    ep2_in_busy = true;
    usb_send_in_buffer(2, send_len);
}

// Send welcome/status message
void send_status_message(void) {
    // Send as larger chunks to improve reliability
    cdc_send_string("\r\n================================\r\n");
    cdc_send_string("   CANalysis USB-CAN Bridge\r\n");
    cdc_send_string("   Firmware " FW_VERSION_STRING "\r\n");
    cdc_send_string("   Build #");
    cdc_send_number(FW_BUILD_NUM);
    cdc_send_string("\r\n================================\r\n");
    cdc_send_string(FW_MCU " @ ");
    cdc_send_number(FW_CLOCK_MHZ);
    cdc_send_string("MHz\r\n");
    cdc_send_string("GVRET Protocol Ready\r\n");
    cdc_send_string("Build: " FW_BUILD_DATE " " FW_BUILD_TIME "\r\n");
    cdc_send_string("\r\nDebug Commands:\r\n");
    cdc_send_string("  d - DEBUG: Print raw CAN IDs (500kbps)\r\n");
    cdc_send_string("  i - Internal loopback (CAN1)\r\n");
    cdc_send_string("  l - Internal loopback (CAN2)\r\n");
    cdc_send_string("  p - External (CAN1->CAN2)\r\n");
    cdc_send_string("  q - External (CAN2->CAN1)\r\n");
    cdc_send_string("  s - Stop test\r\n");
    cdc_send_string("\r\nUse SavvyCAN for GVRET mode.\r\n\r\n");
}

// Helper to print a number
void cdc_send_number(uint32_t num) {
    char buf[12];
    int i = 0;
    if (num == 0) {
        cdc_send_string("0");
        return;
    }
    while (num > 0 && i < 11) {
        buf[i++] = '0' + (num % 10);
        num /= 10;
    }
    // Reverse and send
    char out[12];
    int j = 0;
    while (i > 0) {
        out[j++] = buf[--i];
    }
    out[j] = '\0';
    cdc_send_string(out);
}

// Helper to print hex byte
void cdc_send_hex_byte(uint8_t b) {
    const char hex[] = "0123456789ABCDEF";
    char buf[3];
    buf[0] = hex[(b >> 4) & 0x0F];
    buf[1] = hex[b & 0x0F];
    buf[2] = '\0';
    cdc_send_string(buf);
}

// Helper to print hex word (3 digits for SID)
void cdc_send_hex_sid(uint16_t sid) {
    const char hex[] = "0123456789ABCDEF";
    char buf[4];
    buf[0] = hex[(sid >> 8) & 0x0F];
    buf[1] = hex[(sid >> 4) & 0x0F];
    buf[2] = hex[sid & 0x0F];
    buf[3] = '\0';
    cdc_send_string(buf);
}

// ============================================================================
// USB M-Stack Callbacks
// ============================================================================

// Called when USB reset is received from host
void app_usb_reset_callback(void) {
    usb_reset_count++;
    // Brief flash on LED3
    LED_CAN1 = 1;
    delay_ms(50);
    LED_CAN1 = 0;
}

// Called when SET_CONFIGURATION is received
void app_set_configuration_callback(uint8_t config) {
    if (config == 1) {
        usb_configured_count++;
        // Rapid flash LED4 to indicate configured
        for (int i = 0; i < 5; i++) {
            LED_CAN2 = 1;
            delay_ms(30);
            LED_CAN2 = 0;
            delay_ms(30);
        }
    }
}

// Called to get device status (for GET_STATUS request)
uint16_t app_get_device_status_callback(void) {
    return 0; // Not self-powered, no remote wakeup
}

// Called when endpoint halt status changes
void app_endpoint_halt_callback(uint8_t ep, bool halted) {
    // Nothing to do
}

// Called for SET_INTERFACE request
int8_t app_set_interface_callback(uint8_t interface, uint8_t alt_setting) {
    return 0;
}

// Called for GET_INTERFACE request  
int8_t app_get_interface_callback(uint8_t interface) {
    return 0;
}

// Called for OUT transactions (data from host)
void app_out_transaction_callback(uint8_t ep) {
    // Flash LED3 briefly on data received
    LED_CAN1 = 1;
    
    if (ep == 2) {
        // CDC data endpoint
        const unsigned char *data;
        size_t len = usb_get_out_buffer(ep, &data);
        if (len > 0) {
            // Process each byte through GVRET protocol
            for (size_t i = 0; i < len; i++) {
                // Check for debug menu commands first (only in idle/non-binary mode)
                if (!gvret_is_binary_mode(&gvret_ctx) && 
                    gvret_ctx.state == GVRET_STATE_IDLE) {
                    // Pass to debug menu for loopback tests
                    char c = data[i];
                    if (c == 'i' || c == 'I' || c == 'l' || c == 'L' ||
                        c == 'p' || c == 'P' || c == 'q' || c == 'Q' ||
                        c == 's' || c == 'S' || c == 'd' || c == 'D') {
                        received_cmd = c;
                    }
                }
                
                // Always process through GVRET (handles 0xE7 and 0xF1)
                gvret_process_byte(&gvret_ctx, data[i]);
            }
            
            // Re-arm endpoint for next reception
            usb_arm_out_endpoint(ep);
        }
    }
    
    LED_CAN1 = 0;
}

// Called when IN transaction completes (data sent to host)
void app_in_transaction_complete_callback(uint8_t ep) {
    if (ep == 2) {
        ep2_in_busy = false;  // Clear busy flag
    }
    // Brief flash
    LED_CAN1 = 1;
    LED_CAN1 = 0;
}

// Called for unknown SETUP requests
int8_t app_unknown_setup_request_callback(const struct setup_packet *setup) {
    return process_cdc_setup_request(setup);
}

// Called for unknown GET_DESCRIPTOR requests
int16_t app_unknown_get_descriptor_callback(const struct setup_packet *pkt, const void **descriptor) {
    return -1;
}

// Called on each USB SOF (Start of Frame) - every 1ms
void app_start_of_frame_callback(void) {
    // Can be used for timing
}

// ============================================================================
// CDC-specific Callbacks
// ============================================================================

int8_t app_send_encapsulated_command(uint8_t interface, uint16_t length) {
    return 0;
}

int16_t app_get_encapsulated_response(uint8_t interface, uint16_t length, const void **response) {
    return -1;
}

int8_t app_set_comm_feature_callback(uint8_t interface, bool idle_setting, bool data_multiplexed_state) {
    return 0;
}

int8_t app_clear_comm_feature_callback(uint8_t interface, bool idle_setting, bool data_multiplexed_state) {
    return 0;
}

int8_t app_get_comm_feature_callback(uint8_t interface, bool *idle_setting, bool *data_multiplexed_state) {
    return -1;
}

int8_t app_set_line_coding_callback(uint8_t interface, const struct cdc_line_coding *coding) {
    return 0;
}

int8_t app_get_line_coding_callback(uint8_t interface, struct cdc_line_coding *coding) {
    // Return default line coding
    coding->dwDTERate = 115200;
    coding->bCharFormat = 0; // 1 stop bit
    coding->bParityType = 0; // No parity
    coding->bDataBits = 8;
    return 0;
}

int8_t app_set_control_line_state_callback(uint8_t interface, bool dtr, bool rts) {
    // DTR indicates terminal is connected
    if (dtr) {
        LED_INTERFACE = 1; // Turn on LED2 when interface connected
        if (!cdc_connected) {
            cdc_connected = true;
            send_welcome = true;  // Trigger welcome message
        }
    } else {
        LED_INTERFACE = 0; // Turn off when disconnected
        cdc_connected = false;
    }
    return 0;
}

int8_t app_send_break_callback(uint8_t interface, uint16_t duration) {
    return 0;
}

// ============================================================================
// CAN Functions
// ============================================================================

// CAN1 RX Interrupt Handler (for internal loopback ONLY - GVRET uses polling)
void __attribute__((vector(_CAN_1_VECTOR), interrupt(IPL4SOFT), nomips16)) CAN1_ISR(void) {
    LED_CAN1 ^= 1;  // Toggle LED to show ISR activity
    
    // In GVRET binary mode, we use POLLING not interrupts!
    // This prevents ISR starvation that blocks USB.
    // Just clear the flag and return - main loop will poll.
    if (gvret_is_binary_mode(&gvret_ctx)) {
        C1INTbits.RBIF = 0;  // Clear interrupt flag
        IFS1CLR = _IFS1_CAN1IF_MASK;
        return;  // Let main loop handle it via polling
    } else {
        // Legacy loopback test mode - use the old handler
        // Check FIFO0 first (used in external RX mode - q test)
        if (C1FIFOINT0bits.RXNEMPTYIF) {
            volatile uint32_t *rxBuf = CAN1_FIFO0_PTR;
            
            can_rx_sid = rxBuf[0] & 0x7FF;
            can_rx_dlc = rxBuf[1] & 0x0F;
            volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
            for (int i = 0; i < 8; i++) {
                can_rx_data[i] = dataPtr[i];
            }
            can_msg_received = true;
            
            C1FIFOCON0bits.UINC = 1;
        }
        // Check FIFO1 (used in internal loopback mode - i test)
        else if (C1FIFOINT1bits.RXNEMPTYIF) {
            volatile uint32_t *rxBuf = CAN1_FIFO1_PTR;
            
            can_rx_sid = rxBuf[0] & 0x7FF;
            can_rx_dlc = rxBuf[1] & 0x0F;
            volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
            for (int i = 0; i < 8; i++) {
                can_rx_data[i] = dataPtr[i];
            }
            can_msg_received = true;
            
            C1FIFOCON1bits.UINC = 1;
        }
    }
    
    // Clear only RBIF (bit 1), NOT enable bits (bits 16-23)
    // 0x00FF00FF was clearing RBIE which disabled further interrupts!
    C1INTbits.RBIF = 0;  // Clear receive buffer interrupt flag only
    IFS1CLR = _IFS1_CAN1IF_MASK;
}

// CAN2 RX Interrupt Handler (for external loopback ONLY - GVRET uses polling)
void __attribute__((vector(_CAN_2_VECTOR), interrupt(IPL4SOFT), nomips16)) CAN2_ISR(void) {
    LED_CAN2 ^= 1;  // Toggle LED to show ISR activity
    
    // In GVRET binary mode, we use POLLING not interrupts!
    // Just clear the flag and return - main loop will handle it.
    if (gvret_is_binary_mode(&gvret_ctx)) {
        C2INTbits.RBIF = 0;  // Clear interrupt flag
        IFS1CLR = _IFS1_CAN2IF_MASK;
        return;  // Let main loop handle it via polling
    } else {
        // Legacy loopback test mode
        if (C2FIFOINT0bits.RXNEMPTYIF) {
            volatile uint32_t *rxBuf = CAN2_FIFO0_PTR;
            
            can_rx_sid = rxBuf[0] & 0x7FF;
            can_rx_dlc = rxBuf[1] & 0x0F;
            volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
            for (int i = 0; i < 8; i++) {
                can_rx_data[i] = dataPtr[i];
            }
            can_msg_received = true;
            
            C2FIFOCON0bits.UINC = 1;
        }
    }
    
    // Clear only RBIF, NOT enable bits
    C2INTbits.RBIF = 0;
    IFS1CLR = _IFS1_CAN2IF_MASK;
}

// Wait for CAN mode change with timeout
static bool wait_can_mode(volatile uint32_t *con_reg, uint8_t target_mode, const char *name) {
    uint32_t timeout = 500000;
    while (((*con_reg >> 21) & 0x7) != target_mode && timeout > 0) {
        timeout--;
    }
    if (timeout == 0) {
        cdc_send_string("  TIMEOUT: ");
        cdc_send_string(name);
        cdc_send_string(" mode ");
        cdc_send_number(target_mode);
        cdc_send_string(" (current: ");
        cdc_send_number((*con_reg >> 21) & 0x7);
        cdc_send_string(")\r\n");
        return false;
    }
    return true;
}

// Initialize CAN1 for internal loopback (TX and RX on same module)
// Following Microchip Harmony PLIB sequence exactly
bool CAN1_Init_Internal(void) {
    // Configure CAN1 pins (RF0=RX, RF1=TX)
    TRISFbits.TRISF0 = 1;  // CAN1RX as input
    TRISFbits.TRISF1 = 0;  // CAN1TX as output
    
    cdc_send_string("  CAN1: Full module reset...\r\n");
    
    // Step 0: Full reset of CAN1 module before configuration
    C1CON = 0;              // Disable module completely
    C1CFG = 0;              // Clear config
    C1INT = 0;              // Clear all interrupt enables
    IFS1bits.CAN1IF = 0;    // Clear CAN1 interrupt flag
    C1FIFOCON0 = 0;         // Clear FIFO0 config
    C1FIFOCON1 = 0;         // Clear FIFO1 config
    C1FLTCON0 = 0;          // Clear filter config
    C1RXF0 = 0;             // Clear filter 0
    C1RXM0 = 0;             // Clear mask 0
    delay_ms(10);           // Let module settle
    
    cdc_send_string("  CAN1: Enabling module...\r\n");
    
    // Step 1: Switch the CAN module ON (Harmony does this first!)
    C1CONSET = _C1CON_ON_MASK;
    
    // Step 2: Switch to Configuration mode and WAIT
    cdc_send_string("  CAN1: Entering config mode...\r\n");
    C1CON = (C1CON & ~_C1CON_REQOP_MASK) | ((4 << _C1CON_REQOP_POSITION) & _C1CON_REQOP_MASK);
    while(((C1CON & _C1CON_OPMOD_MASK) >> _C1CON_OPMOD_POSITION) != 4) {
        // Wait for config mode
    }
    
    // Step 3: Set the Bitrate (125 kbps @ 48MHz)
    cdc_send_string("  CAN1: Configuring 125kbps...\r\n");
    C1CFG = ((11UL << _C1CFG_BRP_POSITION) & _C1CFG_BRP_MASK)      // BRP=11
          | ((1UL << _C1CFG_SJW_POSITION) & _C1CFG_SJW_MASK)       // SJW=1
          | ((5UL << _C1CFG_SEG2PH_POSITION) & _C1CFG_SEG2PH_MASK) // SEG2PH=5
          | ((5UL << _C1CFG_SEG1PH_POSITION) & _C1CFG_SEG1PH_MASK) // SEG1PH=5
          | ((2UL << _C1CFG_PRSEG_POSITION) & _C1CFG_PRSEG_MASK)   // PRSEG=2
          | _C1CFG_SEG2PHTS_MASK                                    // SEG2PHTS=1
          | _C1CFG_SAM_MASK;                                        // SAM=1
    
    // Step 4: Set FIFO base address AFTER entering config mode (critical!)
    cdc_send_string("  CAN1: Setting FIFO base...\r\n");
    memset(can1_fifo, 0, sizeof(can1_fifo));  // Zero the buffer first
    
    // Calculate physical address for hardware
    uint32_t fifo_pa = (uint32_t)KVA_TO_PA(can1_fifo);
    C1FIFOBA = fifo_pa;
    
    // Debug: Show buffer addresses to verify correct placement
    cdc_send_string("  Buffer VA=0x");
    uint32_t fifo_va = (uint32_t)can1_fifo;
    cdc_send_hex_byte((fifo_va >> 24) & 0xFF);
    cdc_send_hex_byte((fifo_va >> 16) & 0xFF);
    cdc_send_hex_byte((fifo_va >> 8) & 0xFF);
    cdc_send_hex_byte(fifo_va & 0xFF);
    cdc_send_string(" PA=0x");
    cdc_send_hex_byte((fifo_pa >> 24) & 0xFF);
    cdc_send_hex_byte((fifo_pa >> 16) & 0xFF);
    cdc_send_hex_byte((fifo_pa >> 8) & 0xFF);
    cdc_send_hex_byte(fifo_pa & 0xFF);
    cdc_send_string("\r\n");
    
    // Step 5: Configure CAN FIFOs with proper RESET
    // Per datasheet 24.7 - FRESET must be set, then poll until cleared
    
    // FIFO0 for TX (1 message)
    C1FIFOCON0 = (((1UL - 1UL) << _C1FIFOCON0_FSIZE_POSITION) & _C1FIFOCON0_FSIZE_MASK)
               | _C1FIFOCON0_TXEN_MASK;  // TX FIFO
    
    // Reset FIFO0 - SET the FRESET bit
    C1FIFOCON0SET = _C1FIFOCON0_FRESET_MASK;
    while (C1FIFOCON0 & _C1FIFOCON0_FRESET_MASK);  // Wait for reset
    cdc_send_string("  FIFO0: Reset complete\r\n");
    
    // FIFO1 for RX (1 message)
    C1FIFOCON1 = (((1UL - 1UL) << _C1FIFOCON1_FSIZE_POSITION) & _C1FIFOCON1_FSIZE_MASK);
    
    C1FIFOCON1SET = _C1FIFOCON1_FRESET_MASK;
    while (C1FIFOCON1 & _C1FIFOCON1_FRESET_MASK);  // Wait for reset
    cdc_send_string("  FIFO1: Reset complete\r\n");
    
    // Step 6: Configure CAN Filter and Mask (EXACTLY like Harmony PLIB)
    // For "accept all" standard ID messages:
    // - Filter value = 0 (don't care)
    // - Mask value = 0 (don't care about any bits, including IDE)
    C1RXF0 = 0;  // Filter value = 0
    C1RXM0 = 0;  // Mask = 0: Accept ALL messages (no MIDE, no ID filtering)
    
    // Step 7: Link filter to FIFO and enable
    C1FLTCON0SET = ((0x1UL << _C1FLTCON0_FSEL0_POSITION) & _C1FLTCON0_FSEL0_MASK)  // Filter 0 -> FIFO1
                 | ((0x0UL << _C1FLTCON0_MSEL0_POSITION) & _C1FLTCON0_MSEL0_MASK)  // Use Mask 0
                 | _C1FLTCON0_FLTEN0_MASK;  // Enable Filter 0
    
    // Step 8: Enable interrupt flags for debugging (we poll, but want to see flags)
    C1FIFOINT1SET = _C1FIFOINT1_RXNEMPTYIE_MASK;  // Enable RXNEMPTY interrupt on FIFO1
    
    // Enable BOTH TX and RX buffer interrupt enables in C1INT
    // TBIE (bit 16) and RBIE (bit 17) - needed for TBIF and RBIF flags to work!
    C1INTSET = _C1INT_TBIE_MASK | _C1INT_RBIE_MASK;
    
    // Disable global CAN1 interrupt - use polling
    IEC1bits.CAN1IE = 0;
    
    // Step 9: Switch to Loopback mode and WAIT
    cdc_send_string("  CAN1: Entering loopback mode...\r\n");
    C1CON = (C1CON & ~_C1CON_REQOP_MASK) | ((2 << _C1CON_REQOP_POSITION) & _C1CON_REQOP_MASK);
    while(((C1CON & _C1CON_OPMOD_MASK) >> _C1CON_OPMOD_POSITION) != 2) {
        // Wait for loopback mode
    }
    
    // Clear any pending flags after mode change
    C1INTbits.MODIF = 0;    // Clear mode change flag
    C1INTbits.RBIF = 0;     // Clear RX buffer flag
    C1INTbits.TBIF = 0;     // Clear TX buffer flag
    IFS1bits.CAN1IF = 0;    // Clear CAN1 interrupt flag
    
    // Longer delay after mode change to let hardware stabilize
    delay_ms(50);
    
    // Debug: show full configuration
    cdc_send_string("  --- Configuration Dump ---\r\n");
    cdc_send_string("  C1CON=0x");
    cdc_send_hex_byte((C1CON >> 24) & 0xFF);
    cdc_send_hex_byte((C1CON >> 16) & 0xFF);
    cdc_send_hex_byte((C1CON >> 8) & 0xFF);
    cdc_send_hex_byte(C1CON & 0xFF);
    cdc_send_string(" OPMOD=");
    cdc_send_number(C1CONbits.OPMOD);
    cdc_send_string("\r\n  FIFOBA=0x");
    cdc_send_hex_byte((C1FIFOBA >> 24) & 0xFF);
    cdc_send_hex_byte((C1FIFOBA >> 16) & 0xFF);
    cdc_send_hex_byte((C1FIFOBA >> 8) & 0xFF);
    cdc_send_hex_byte(C1FIFOBA & 0xFF);
    cdc_send_string("\r\n  FIFO0: CON=0x");
    cdc_send_hex_byte((C1FIFOCON0 >> 24) & 0xFF);
    cdc_send_hex_byte((C1FIFOCON0 >> 16) & 0xFF);
    cdc_send_hex_byte((C1FIFOCON0 >> 8) & 0xFF);
    cdc_send_hex_byte(C1FIFOCON0 & 0xFF);
    cdc_send_string(" TXEN=");
    cdc_send_number(C1FIFOCON0bits.TXEN);
    cdc_send_string(" UA=0x");
    cdc_send_hex_byte((C1FIFOUA0 >> 8) & 0xFF);
    cdc_send_hex_byte(C1FIFOUA0 & 0xFF);
    cdc_send_string("\r\n  FIFO1: CON=0x");
    cdc_send_hex_byte((C1FIFOCON1 >> 24) & 0xFF);
    cdc_send_hex_byte((C1FIFOCON1 >> 16) & 0xFF);
    cdc_send_hex_byte((C1FIFOCON1 >> 8) & 0xFF);
    cdc_send_hex_byte(C1FIFOCON1 & 0xFF);
    cdc_send_string(" TXEN=");
    cdc_send_number(C1FIFOCON1bits.TXEN);
    cdc_send_string(" UA=0x");
    cdc_send_hex_byte((C1FIFOUA1 >> 8) & 0xFF);
    cdc_send_hex_byte(C1FIFOUA1 & 0xFF);
    cdc_send_string("\r\n  FLTCON0=0x");
    cdc_send_hex_byte((C1FLTCON0 >> 24) & 0xFF);
    cdc_send_hex_byte((C1FLTCON0 >> 16) & 0xFF);
    cdc_send_hex_byte((C1FLTCON0 >> 8) & 0xFF);
    cdc_send_hex_byte(C1FLTCON0 & 0xFF);
    cdc_send_string(" FSEL0=");
    cdc_send_number(C1FLTCON0bits.FSEL0);
    cdc_send_string(" FLTEN0=");
    cdc_send_number(C1FLTCON0bits.FLTEN0);
    cdc_send_string("\r\n  RXF0=0x");
    cdc_send_hex_byte((C1RXF0 >> 24) & 0xFF);
    cdc_send_hex_byte((C1RXF0 >> 16) & 0xFF);
    cdc_send_hex_byte((C1RXF0 >> 8) & 0xFF);
    cdc_send_hex_byte(C1RXF0 & 0xFF);
    cdc_send_string(" RXM0=0x");
    cdc_send_hex_byte((C1RXM0 >> 24) & 0xFF);
    cdc_send_hex_byte((C1RXM0 >> 16) & 0xFF);
    cdc_send_hex_byte((C1RXM0 >> 8) & 0xFF);
    cdc_send_hex_byte(C1RXM0 & 0xFF);
    cdc_send_string("\r\n  C1CFG=0x");
    cdc_send_hex_byte((C1CFG >> 24) & 0xFF);
    cdc_send_hex_byte((C1CFG >> 16) & 0xFF);
    cdc_send_hex_byte((C1CFG >> 8) & 0xFF);
    cdc_send_hex_byte(C1CFG & 0xFF);
    cdc_send_string("\r\n  C1INT=0x");
    cdc_send_hex_byte((C1INT >> 24) & 0xFF);
    cdc_send_hex_byte((C1INT >> 16) & 0xFF);
    cdc_send_hex_byte((C1INT >> 8) & 0xFF);
    cdc_send_hex_byte(C1INT & 0xFF);
    cdc_send_string(" C1TREC=0x");
    cdc_send_hex_byte((C1TREC >> 8) & 0xFF);
    cdc_send_hex_byte(C1TREC & 0xFF);
    cdc_send_string("\r\n");
    
    cdc_send_string("  CAN1: OK (internal loopback)\r\n");
    return true;
}

// Initialize CAN2 for internal loopback (TX and RX on same module)
bool CAN2_Init_Internal(void) {
    // Configure CAN2 pins (RG0=RX, RG1=TX per hardware spec)
    TRISGbits.TRISG0 = 1;  // CAN2RX as input
    TRISGbits.TRISG1 = 0;  // CAN2TX as output
    
    cdc_send_string("  CAN2: Full module reset...\r\n");
    
    // Full reset of CAN2 module
    C2CON = 0;
    C2CFG = 0;
    C2INT = 0;
    IFS1bits.CAN2IF = 0;
    C2FIFOCON0 = 0;
    C2FIFOCON1 = 0;
    C2FLTCON0 = 0;
    C2RXF0 = 0;
    C2RXM0 = 0;
    delay_ms(10);
    
    cdc_send_string("  CAN2: Enabling module...\r\n");
    C2CONSET = _C2CON_ON_MASK;
    
    cdc_send_string("  CAN2: Entering config mode...\r\n");
    C2CON = (C2CON & ~_C2CON_REQOP_MASK) | ((4 << _C2CON_REQOP_POSITION) & _C2CON_REQOP_MASK);
    while(((C2CON & _C2CON_OPMOD_MASK) >> _C2CON_OPMOD_POSITION) != 4) {
        // Wait for config mode
    }
    
    cdc_send_string("  CAN2: Configuring 125kbps...\r\n");
    C2CFG = ((11UL << _C2CFG_BRP_POSITION) & _C2CFG_BRP_MASK)
          | ((1UL << _C2CFG_SJW_POSITION) & _C2CFG_SJW_MASK)
          | ((5UL << _C2CFG_SEG2PH_POSITION) & _C2CFG_SEG2PH_MASK)
          | ((5UL << _C2CFG_SEG1PH_POSITION) & _C2CFG_SEG1PH_MASK)
          | ((2UL << _C2CFG_PRSEG_POSITION) & _C2CFG_PRSEG_MASK)
          | _C2CFG_SEG2PHTS_MASK
          | _C2CFG_SAM_MASK;
    
    cdc_send_string("  CAN2: Setting FIFO base...\r\n");
    memset(can2_fifo, 0, sizeof(can2_fifo));
    uint32_t fifo_pa = (uint32_t)KVA_TO_PA(can2_fifo);
    C2FIFOBA = fifo_pa;
    
    cdc_send_string("  Buffer VA=0x");
    uint32_t fifo_va = (uint32_t)can2_fifo;
    cdc_send_hex_byte((fifo_va >> 24) & 0xFF);
    cdc_send_hex_byte((fifo_va >> 16) & 0xFF);
    cdc_send_hex_byte((fifo_va >> 8) & 0xFF);
    cdc_send_hex_byte(fifo_va & 0xFF);
    cdc_send_string(" PA=0x");
    cdc_send_hex_byte((fifo_pa >> 24) & 0xFF);
    cdc_send_hex_byte((fifo_pa >> 16) & 0xFF);
    cdc_send_hex_byte((fifo_pa >> 8) & 0xFF);
    cdc_send_hex_byte(fifo_pa & 0xFF);
    cdc_send_string("\r\n");
    
    // FIFO0 for TX
    C2FIFOCON0 = _C2FIFOCON0_TXEN_MASK;
    C2FIFOCON0SET = _C2FIFOCON0_FRESET_MASK;
    while (C2FIFOCON0 & _C2FIFOCON0_FRESET_MASK);
    cdc_send_string("  FIFO0: Reset complete\r\n");
    
    // FIFO1 for RX
    C2FIFOCON1 = 0;
    C2FIFOCON1SET = _C2FIFOCON1_FRESET_MASK;
    while (C2FIFOCON1 & _C2FIFOCON1_FRESET_MASK);
    cdc_send_string("  FIFO1: Reset complete\r\n");
    
    // Filter/mask - accept all
    C2RXF0 = 0;
    C2RXM0 = 0;
    C2FLTCON0SET = ((0x1UL << _C2FLTCON0_FSEL0_POSITION) & _C2FLTCON0_FSEL0_MASK)
                 | _C2FLTCON0_FLTEN0_MASK;
    
    // Enable interrupt flags
    C2FIFOINT1SET = _C2FIFOINT1_RXNEMPTYIE_MASK;
    C2INTSET = _C2INT_TBIE_MASK | _C2INT_RBIE_MASK;
    IEC1bits.CAN2IE = 0;  // Polling mode
    
    // Enter loopback mode
    cdc_send_string("  CAN2: Entering loopback mode...\r\n");
    C2CON = (C2CON & ~_C2CON_REQOP_MASK) | ((2 << _C2CON_REQOP_POSITION) & _C2CON_REQOP_MASK);
    while(((C2CON & _C2CON_OPMOD_MASK) >> _C2CON_OPMOD_POSITION) != 2);
    
    C2INTbits.MODIF = 0;
    C2INTbits.RBIF = 0;
    C2INTbits.TBIF = 0;
    IFS1bits.CAN2IF = 0;
    delay_ms(50);
    
    cdc_send_string("  CAN2: OK (internal loopback)\r\n");
    return true;
}

// Initialize CAN1 for external TX (normal mode)
bool CAN1_Init_External(void) {
    // Full module reset first
    cdc_send_string("  CAN1: Full reset...\r\n");
    C1CONbits.ON = 0;
    delay_ms(1);
    
    // Disable all CAN1 interrupts
    IEC1bits.CAN1IE = 0;
    IFS1bits.CAN1IF = 0;
    C1INT = 0;
    
    // Clear all filter enables
    C1FLTCON0 = 0;
    C1FLTCON1 = 0;
    C1FLTCON2 = 0;
    C1FLTCON3 = 0;
    
    cdc_send_string("  CAN1: Setting FIFO base...\r\n");
    C1FIFOBA = KVA_TO_PA(can1_fifo);
    
    cdc_send_string("  CAN1: Entering config mode...\r\n");
    C1CONbits.ON = 1;
    C1CONbits.REQOP = 4;
    if (!wait_can_mode(&C1CON, 4, "CAN1 config")) return false;
    
    cdc_send_string("  CAN1: Configuring 125kbps...\r\n");
    C1CFGbits.BRP = 11;
    C1CFGbits.SJW = 1;
    C1CFGbits.PRSEG = 2;
    C1CFGbits.SEG1PH = 5;
    C1CFGbits.SEG2PHTS = 1;
    C1CFGbits.SEG2PH = 5;
    C1CFGbits.SAM = 1;
    
    // FIFO0 for TX only - reset it first
    C1FIFOCON0bits.FRESET = 1;
    while (C1FIFOCON0bits.FRESET);
    C1FIFOCON0bits.FSIZE = 0;  // 1 message
    C1FIFOCON0bits.TXEN = 1;   // TX mode
    
    cdc_send_string("  CAN1: Entering normal mode...\r\n");
    cdc_send_string("  (Requires 11 recessive bits on bus)\r\n");
    C1CONbits.REQOP = 0;  // Normal mode
    if (!wait_can_mode(&C1CON, 0, "CAN1 normal")) {
        cdc_send_string("  Check: termination, wiring\r\n");
        return false;
    }
    
    cdc_send_string("  CAN1: OK (normal mode TX)\r\n");
    return true;
}

// Initialize CAN2 for external RX (normal mode)
bool CAN2_Init_External(void) {
    // Full module reset first
    cdc_send_string("  CAN2: Full reset...\r\n");
    C2CONbits.ON = 0;
    delay_ms(1);
    
    // Disable all CAN2 interrupts
    IEC1bits.CAN2IE = 0;
    IFS1bits.CAN2IF = 0;
    C2INT = 0;
    
    // Clear all filter enables
    C2FLTCON0 = 0;
    C2FLTCON1 = 0;
    C2FLTCON2 = 0;
    C2FLTCON3 = 0;
    
    cdc_send_string("  CAN2: Setting FIFO base...\r\n");
    C2FIFOBA = KVA_TO_PA(can2_fifo);
    
    cdc_send_string("  CAN2: Entering config mode...\r\n");
    C2CONbits.ON = 1;
    C2CONbits.REQOP = 4;
    if (!wait_can_mode(&C2CON, 4, "CAN2 config")) return false;
    
    cdc_send_string("  CAN2: Configuring 125kbps...\r\n");
    C2CFGbits.BRP = 11;
    C2CFGbits.SJW = 1;
    C2CFGbits.PRSEG = 2;
    C2CFGbits.SEG1PH = 5;
    C2CFGbits.SEG2PHTS = 1;
    C2CFGbits.SEG2PH = 5;
    C2CFGbits.SAM = 1;
    
    // FIFO0 for RX - reset it first
    C2FIFOCON0bits.FRESET = 1;
    while (C2FIFOCON0bits.FRESET);  // Wait for reset
    C2FIFOCON0bits.FSIZE = 0;  // 1 message
    C2FIFOCON0bits.TXEN = 0;   // RX mode
    
    // Filter 0 accepts all into FIFO0
    C2FLTCON0bits.FSEL0 = 0;   // Route to FIFO0
    C2FLTCON0bits.MSEL0 = 0;   // Use mask 0
    C2RXF0 = 0;                // Match any SID (cleared)
    C2RXM0 = 0;                // Don't care about any bits
    C2FLTCON0bits.FLTEN0 = 1;  // Enable filter 0
    
    // Enable RX interrupt on FIFO0
    C2FIFOINT0bits.RXNEMPTYIE = 1;
    IPC11bits.CAN2IP = 4;
    IPC11bits.CAN2IS = 0;
    IFS1bits.CAN2IF = 0;
    C2INTCLR = 0xFFFFFFFF;     // Clear all CAN2 interrupt flags
    IEC1bits.CAN2IE = 1;       // Enable CAN2 system interrupt
    C2INTbits.RBIE = 1;        // Enable RX buffer interrupt
    
    cdc_send_string("  CAN2: Entering normal mode...\r\n");
    C2CONbits.REQOP = 0;  // Normal mode
    if (!wait_can_mode(&C2CON, 0, "CAN2 normal")) {
        cdc_send_string("  Check: termination, wiring\r\n");
        return false;
    }
    
    cdc_send_string("  CAN2: OK (normal mode RX)\r\n");
    return true;
}

// Initialize CAN1 for external RX (used in reverse loopback q)
bool CAN1_Init_External_RX(void) {
    cdc_send_string("  CAN1: Full reset...\r\n");
    C1CON = 0;
    C1INT = 0;
    C1INTCLR = 0xFFFFFFFF;
    IEC1bits.CAN1IE = 0;
    IFS1bits.CAN1IF = 0;
    
    // Clear filter config (disable filters before reconfiguring)
    C1FLTCON0 = 0;
    C1FLTCON1 = 0;
    C1FLTCON2 = 0;
    C1FLTCON3 = 0;
    
    cdc_send_string("  CAN1: Setting FIFO base...\r\n");
    C1FIFOBA = KVA_TO_PA((uint32_t)can1_fifo);
    
    cdc_send_string("  CAN1: Entering config mode...\r\n");
    C1CONbits.ON = 1;
    C1CONbits.REQOP = 4;  // Config mode
    if (!wait_can_mode(&C1CON, 4, "CAN1 config")) return false;
    
    cdc_send_string("  CAN1: Configuring 125kbps...\r\n");
    C1CFG = 0;
    C1CFGbits.BRP = 11;
    C1CFGbits.SJW = 3;
    C1CFGbits.PRSEG = 2;
    C1CFGbits.SEG1PH = 5;
    C1CFGbits.SEG2PHTS = 1;
    C1CFGbits.SEG2PH = 5;
    C1CFGbits.SAM = 1;
    
    // Reset BOTH FIFOs to clear any stale data from previous tests
    C1FIFOCON0bits.FRESET = 1;
    while (C1FIFOCON0bits.FRESET);
    C1FIFOCON1bits.FRESET = 1;
    while (C1FIFOCON1bits.FRESET);
    
    // FIFO0 for RX
    C1FIFOCON0bits.FSIZE = 0;  // 1 message
    C1FIFOCON0bits.TXEN = 0;   // RX mode
    
    // FIFO1 also RX (not used but clear it)
    C1FIFOCON1bits.FSIZE = 0;
    C1FIFOCON1bits.TXEN = 0;
    
    // Filter 0 accepts all into FIFO0
    C1FLTCON0bits.FLTEN0 = 0;  // Disable filter first
    C1FLTCON0bits.FSEL0 = 0;   // Route to FIFO0
    C1FLTCON0bits.MSEL0 = 0;   // Use mask 0
    C1RXF0 = 0;                // Match any SID
    C1RXM0 = 0;                // Don't care about any bits
    C1FLTCON0bits.FLTEN0 = 1;  // Enable filter 0
    
    // Enable RX interrupt on FIFO0
    C1FIFOINT0bits.RXNEMPTYIE = 1;
    C1FIFOINT1bits.RXNEMPTYIE = 0;  // Disable FIFO1 interrupt
    IPC11bits.CAN1IP = 4;
    IPC11bits.CAN1IS = 0;
    IFS1bits.CAN1IF = 0;
    C1INTCLR = 0xFFFFFFFF;
    IEC1bits.CAN1IE = 1;       // Enable CAN1 system interrupt
    C1INTbits.RBIE = 1;        // Enable RX buffer interrupt
    
    cdc_send_string("  CAN1: Entering normal mode...\r\n");
    C1CONbits.REQOP = 0;  // Normal mode
    if (!wait_can_mode(&C1CON, 0, "CAN1 normal")) {
        cdc_send_string("  Check: termination, wiring\r\n");
        return false;
    }
    
    cdc_send_string("  CAN1: OK (normal mode RX)\r\n");
    return true;
}

// Initialize CAN2 for external TX (used in reverse loopback q)
bool CAN2_Init_External_TX(void) {
    cdc_send_string("  CAN2: Full reset...\r\n");
    C2CON = 0;
    C2INT = 0;
    C2INTCLR = 0xFFFFFFFF;
    IEC1bits.CAN2IE = 0;
    IFS1bits.CAN2IF = 0;
    
    cdc_send_string("  CAN2: Setting FIFO base...\r\n");
    C2FIFOBA = KVA_TO_PA((uint32_t)can2_fifo);
    
    cdc_send_string("  CAN2: Entering config mode...\r\n");
    C2CONbits.ON = 1;
    C2CONbits.REQOP = 4;  // Config mode
    if (!wait_can_mode(&C2CON, 4, "CAN2 config")) return false;
    
    cdc_send_string("  CAN2: Configuring 125kbps...\r\n");
    C2CFG = 0;
    C2CFGbits.BRP = 11;
    C2CFGbits.SJW = 3;
    C2CFGbits.PRSEG = 2;
    C2CFGbits.SEG1PH = 5;
    C2CFGbits.SEG2PHTS = 1;
    C2CFGbits.SEG2PH = 5;
    C2CFGbits.SAM = 1;
    
    // FIFO0 for TX - reset it first
    C2FIFOCON0bits.FRESET = 1;
    while (C2FIFOCON0bits.FRESET);
    C2FIFOCON0bits.FSIZE = 0;  // 1 message
    C2FIFOCON0bits.TXEN = 1;   // TX mode
    
    cdc_send_string("  CAN2: Entering normal mode...\r\n");
    cdc_send_string("  (Requires 11 recessive bits on bus)\r\n");
    C2CONbits.REQOP = 0;  // Normal mode
    if (!wait_can_mode(&C2CON, 0, "CAN2 normal")) {
        cdc_send_string("  Check: termination, wiring\r\n");
        return false;
    }
    
    cdc_send_string("  CAN2: OK (normal mode TX)\r\n");
    return true;
}

// Send a CAN message on CAN1
bool CAN1_SendMessage(uint16_t sid, uint8_t *data, uint8_t dlc) {
    // Wait for FIFO0 to be ready
    if (C1FIFOCON0bits.TXREQ) {
        return false;  // Previous TX still pending
    }
    
    // Ensure CAN module is not busy
    if (C1CONbits.CANBUSY) {
        return false;
    }
    
    // Use direct pointer into our contiguous buffer - NOT PA_TO_KVA1!
    // FIFO0 is at offset 0 in our buffer
    volatile uint32_t *buffer = CAN1_FIFO0_PTR;
    
    // Clear buffer
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    
    // PIC32 CAN TX message buffer format (from CAN.h CAN_TX_MSG_SID / CAN_MSG_EID):
    // Word 0 (CAN_TX_MSG_SID):
    //   Bits 10:0  = SID<10:0>  (NOT shifted like filter registers!)
    //   Bits 31:11 = Reserved
    buffer[0] = (uint32_t)(sid & 0x7FF);  // SID in bits 10:0
    
    // Word 1 (CAN_MSG_EID):
    //   Bits 3:0   = DLC
    //   Bit 4      = RB0 (reserved, 0)
    //   Bits 7:5   = Reserved
    //   Bit 8      = RB1 (reserved, 0)
    //   Bit 9      = RTR (0 for data frame)
    //   Bits 17:10 = EID<7:0>
    //   Bits 27:18 = EID<17:8>
    //   Bit 28     = IDE (0 for standard ID, 1 for extended)
    //   Bit 29     = SRR
    //   Bits 31:30 = Reserved
    buffer[1] = (uint32_t)(dlc & 0x0F);  // DLC in bits 3:0, IDE=0 (standard)
    
    // Words 2-3: Data bytes (little-endian)
    uint8_t *dataPtr = (uint8_t *)&buffer[2];
    for (uint8_t i = 0; i < dlc && i < 8; i++) {
        dataPtr[i] = data[i];
    }
    
    LED_CAN1 = 1;
    
    // Clear TBIF and set UINC to hand buffer to hardware
    C1INTCLR = 0x01;
    C1FIFOCON0SET = _C1FIFOCON0_UINC_MASK;
    delay_ms(1);  // Brief delay for UINC
    
    // Set TXREQ to start transmission
    C1FIFOCON0SET = _C1FIFOCON0_TXREQ_MASK;
    
    // Wait for TX to complete (TXREQ clears when done)
    // Use CP0 COUNT for accurate 1-second timeout
    uint32_t start = _CP0_GET_COUNT();
    uint32_t timeout_ticks = (SYS_FREQ / 2);  // 1 second (COUNT runs at SYSCLK/2)
    
    while (C1FIFOCON0bits.TXREQ) {
        // Service USB to stay responsive
        usb_service();
        
        // Check for user abort
        if (received_cmd == 's' || received_cmd == 'S') {
            C1CONbits.ABAT = 1;
            received_cmd = 0;
            loopback_stop_requested = true;
            LED_CAN1 = 0;
            return false;
        }
        
        // Check timeout
        if ((_CP0_GET_COUNT() - start) > timeout_ticks) {
            C1CONbits.ABAT = 1;  // Abort TX
            LED_CAN1 = 0;
            return false;  // Timeout
        }
    }
    
    LED_CAN1 = 0;
    return true;  // TX completed successfully
}

// Send a CAN message on CAN2 (for CAN2 loopback test)
bool CAN2_SendMessage(uint16_t sid, uint8_t *data, uint8_t dlc) {
    if (C2FIFOCON0bits.TXREQ) {
        return false;  // Previous TX still pending
    }
    
    if (C2CONbits.CANBUSY) {
        return false;
    }
    
    // Use direct pointer into CAN2 FIFO buffer
    volatile uint32_t *buffer = (volatile uint32_t *)&can2_fifo[0];
    
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    
    buffer[0] = (uint32_t)(sid & 0x7FF);
    buffer[1] = (uint32_t)(dlc & 0x0F);
    
    uint8_t *dataPtr = (uint8_t *)&buffer[2];
    for (uint8_t i = 0; i < dlc && i < 8; i++) {
        dataPtr[i] = data[i];
    }
    
    LED_CAN2 = 1;
    
    // Clear TBIF and set UINC to hand buffer to hardware
    C2INTCLR = 0x01;
    C2FIFOCON0SET = _C2FIFOCON0_UINC_MASK;
    delay_ms(1);  // Brief delay for UINC
    
    // Set TXREQ to start transmission
    C2FIFOCON0SET = _C2FIFOCON0_TXREQ_MASK;
    
    // Wait for TX to complete (TXREQ clears when done)
    uint32_t start = _CP0_GET_COUNT();
    uint32_t timeout_ticks = (SYS_FREQ / 2);  // 1 second
    
    while (C2FIFOCON0bits.TXREQ) {
        usb_service();
        
        // Check for user abort
        if (received_cmd == 's' || received_cmd == 'S') {
            C2CONbits.ABAT = 1;
            received_cmd = 0;
            loopback_stop_requested = true;
            LED_CAN2 = 0;
            return false;
        }
        
        if ((_CP0_GET_COUNT() - start) > timeout_ticks) {
            C2CONbits.ABAT = 1;
            LED_CAN2 = 0;
            return false;
        }
    }
    
    LED_CAN2 = 0;
    return true;
}


// Run the loopback test
void run_loopback_test(void) {
    static uint8_t test_counter = 0;
    uint8_t test_data[8];
    const char *tx_name = (loopback_can_module == 2) ? "CAN2" : "CAN1";
    const char *rx_name;
    if (loopback_internal) {
        rx_name = tx_name;  // Same module receives
    } else if (loopback_can_module == 1) {
        rx_name = "CAN2";   // p: CAN1 TX, CAN2 RX
    } else {
        rx_name = "CAN1";   // q: CAN2 TX, CAN1 RX
    }
    
    // Prepare test message
    test_data[0] = test_counter;
    test_data[1] = 0xCA;
    test_data[2] = 0xFE;
    test_data[3] = 0xBA;
    test_data[4] = 0xBE;
    test_data[5] = test_counter ^ 0xFF;
    test_data[6] = 0x55;
    test_data[7] = 0xAA;
    
    // Send message
    cdc_send_string(tx_name);
    cdc_send_string(" TX: SID=0x123 Data=[");
    for (int i = 0; i < 8; i++) {
        cdc_send_hex_byte(test_data[i]);
        if (i < 7) cdc_send_string(" ");
    }
    cdc_send_string("]\r\n");
    
    can_msg_received = false;
    LED_CAN1 = 0;
    LED_CAN2 = 0;
    
    // Send using appropriate CAN module
    bool tx_ok = false;
    if (loopback_can_module == 2) {
        tx_ok = CAN2_SendMessage(0x123, test_data, 8);
        if (!tx_ok && !loopback_stop_requested) {
            cdc_send_string("  ERROR: CAN2 TX failed!\r\n");
        }
    } else {
        tx_ok = CAN1_SendMessage(0x123, test_data, 8);
        if (!tx_ok && !loopback_stop_requested) {
            cdc_send_string("  ERROR: CAN1 TX failed!\r\n");
        }
    }
    
    if (!tx_ok) {
        // Only count as failure if not a user-requested stop
        if (!loopback_stop_requested) {
            loopback_timeouts++;
            loopback_sent++;
        }
        return;
    }
    
    loopback_sent++;
    
    // Small delay after TX for loopback to complete
    delay_ms(10);
    
    // Poll for RX (no interrupt, direct polling)
    uint32_t wait_count = 0;
    while (wait_count < 100) {  // 100ms timeout
        bool rx_ready = false;
        volatile uint32_t *rxBuf = NULL;
        
        // Check appropriate FIFO based on module
        if (loopback_can_module == 2 && loopback_internal) {
            // CAN2 internal loopback - check C2FIFO1
            if (C2FIFOINT1bits.RXNEMPTYIF) {
                rxBuf = (volatile uint32_t *)PA_TO_KVA1(C2FIFOUA1);
                rx_ready = true;
            }
        } else if (loopback_can_module == 1 && loopback_internal) {
            // CAN1 internal loopback - check C1FIFO1
            if (C1FIFOINT1bits.RXNEMPTYIF) {
                rxBuf = (volatile uint32_t *)PA_TO_KVA1(C1FIFOUA1);
                rx_ready = true;
            }
        } else if (loopback_can_module == 1) {
            // External loopback p - CAN1 TX, CAN2 receives on FIFO0
            if (C2FIFOINT0bits.RXNEMPTYIF) {
                rxBuf = (volatile uint32_t *)PA_TO_KVA1(C2FIFOUA0);
                rx_ready = true;
            }
        } else {
            // External loopback q - CAN2 TX, CAN1 receives on FIFO0
            if (C1FIFOINT0bits.RXNEMPTYIF) {
                rxBuf = (volatile uint32_t *)PA_TO_KVA1(C1FIFOUA0);
                rx_ready = true;
            }
        }
        
        if (rx_ready && rxBuf != NULL) {
            // Extract SID from bits 10:0 of Word 0
            can_rx_sid = rxBuf[0] & 0x7FF;
            // Extract DLC from bits 3:0 of Word 1
            can_rx_dlc = rxBuf[1] & 0x0F;
            // Copy data from Words 2-3
            volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
            for (int i = 0; i < 8; i++) {
                can_rx_data[i] = dataPtr[i];
            }
            can_msg_received = true;
            
            // Advance FIFO pointer based on which module/FIFO
            if (loopback_can_module == 2 && loopback_internal) {
                // CAN2 internal loopback - FIFO1
                C2FIFOCON1SET = _C2FIFOCON1_UINC_MASK;
                while (C2FIFOCON1 & _C2FIFOCON1_UINC_MASK);
                LED_CAN2 = 1;
            } else if (loopback_can_module == 1 && loopback_internal) {
                // CAN1 internal loopback - FIFO1
                C1FIFOCON1SET = _C1FIFOCON1_UINC_MASK;
                while (C1FIFOCON1 & _C1FIFOCON1_UINC_MASK);
                LED_CAN1 = 1;
            } else if (loopback_can_module == 1) {
                // External p - CAN2 receives on FIFO0
                C2FIFOCON0SET = _C2FIFOCON0_UINC_MASK;
                while (C2FIFOCON0 & _C2FIFOCON0_UINC_MASK);
                LED_CAN2 = 1;
            } else {
                // External q - CAN1 receives on FIFO0
                C1FIFOCON0SET = _C1FIFOCON0_UINC_MASK;
                while (C1FIFOCON0 & _C1FIFOCON0_UINC_MASK);
                LED_CAN1 = 1;
            }
            break;
        }
        
        delay_ms(1);
        usb_service();  // Keep USB alive
        wait_count++;
        
        // Check for stop command
        if (received_cmd == 's' || received_cmd == 'S') {
            loopback_stop_requested = true;
            received_cmd = 0;
            return;
        }
    }
    
    // If still not received, show debug info
    if (!can_msg_received) {
        cdc_send_string("  TIMEOUT - checking FIFO status...\r\n");
        if (loopback_can_module == 2) {
            cdc_send_string("  C2FIFOINT1=0x");
            cdc_send_hex_byte((C2FIFOINT1 >> 8) & 0xFF);
            cdc_send_hex_byte(C2FIFOINT1 & 0xFF);
        } else {
            cdc_send_string("  C1FIFOINT1=0x");
            cdc_send_hex_byte((C1FIFOINT1 >> 8) & 0xFF);
            cdc_send_hex_byte(C1FIFOINT1 & 0xFF);
        }
        cdc_send_string("\r\n");
    }
    
    LED_CAN1 = 0;
    LED_CAN2 = 0;
    
    if (can_msg_received) {
        loopback_received++;
        cdc_send_string(rx_name);
        cdc_send_string(" RX: SID=0x");
        cdc_send_hex_sid(can_rx_sid);
        cdc_send_string(" Data=[");
        for (int i = 0; i < can_rx_dlc; i++) {
            cdc_send_hex_byte(can_rx_data[i]);
            if (i < can_rx_dlc - 1) cdc_send_string(" ");
        }
        cdc_send_string("]\r\n");
        
        // Verify data matches
        bool match = (can_rx_sid == 0x123) && (can_rx_dlc == 8);
        for (int i = 0; i < 8 && match; i++) {
            if (can_rx_data[i] != test_data[i]) match = false;
        }
        if (match) {
            cdc_send_string("  OK - Data verified!\r\n");
        } else {
            cdc_send_string("  WARNING: Data mismatch!\r\n");
        }
    } else {
        loopback_timeouts++;
        cdc_send_string("  TIMEOUT - No response! Retrying...\r\n");
    }
    
    cdc_send_string("\r\n");
    test_counter++;
}

// Print loopback test summary
void print_loopback_summary(void) {
    uint32_t lost = loopback_sent - loopback_received;
    uint32_t loss_percent = (loopback_sent > 0) ? (lost * 100 / loopback_sent) : 0;
    
    cdc_send_string("\r\n--- CAN Bus loopback test ---\r\n");
    cdc_send_string("    Packets: Sent = ");
    cdc_send_number(loopback_sent);
    cdc_send_string(", Received = ");
    cdc_send_number(loopback_received);
    cdc_send_string(", Lost = ");
    cdc_send_number(lost);
    cdc_send_string(" (");
    cdc_send_number(loss_percent);
    cdc_send_string("% loss)\r\n");
    cdc_send_string("    Timeouts = ");
    cdc_send_number(loopback_timeouts);
    cdc_send_string("\r\n\r\n");
}

// ============================================================================
// Main
// ============================================================================

int main(void) {
    // Configure LED pins as outputs
    LED_INTERFACE_TRIS = 0;
    LED_CAN1_TRIS = 0;
    LED_CAN2_TRIS = 0;
    
    LED_INTERFACE = 0;
    LED_CAN1 = 0;
    LED_CAN2 = 0;

    // Startup indication - flash all LEDs
    for (int i = 0; i < 3; i++) {
        LED_INTERFACE = 1;
        LED_CAN1 = 1;
        LED_CAN2 = 1;
        delay_ms(100);
        LED_INTERFACE = 0;
        LED_CAN1 = 0;
        LED_CAN2 = 0;
        delay_ms(100);
    }

    // Initialize CAN driver
    can_driver_init();
    
    // Initialize GVRET protocol
    gvret_init(&gvret_ctx);
    
    // DON'T enable CAN at startup - wait for SavvyCAN to send config command
    // This prevents ISR flooding before USB/GVRET is ready
    // Just set default values in GVRET context (hardware stays disabled)
    gvret_ctx.can[0].speed = 500000;
    gvret_ctx.can[0].enabled = false;  // Disabled until SavvyCAN configures
    gvret_ctx.can[0].listenOnly = false;
    
    // Initialize USB
    usb_init();
    
    // Enable multi-vector interrupts
    INTCONbits.MVEC = 1;
    __builtin_enable_interrupts();

    // Main loop
    uint32_t loop_count = 0;
    
    while (1) {
        // Service USB
        usb_service();
        
        // Flush GVRET buffer if needed
        gvret_flush_if_needed(&gvret_ctx, gvret_get_timestamp());
        
        // Stream CAN RX frames to GVRET (when in binary mode)
        // Using POLLING approach (like M2RET) instead of ISR to prevent USB starvation
        if (gvret_is_binary_mode(&gvret_ctx)) {
            CAN_Frame rxFrame;
            GVRET_CANFrame gvFrame;
            uint8_t processed = 0;
            const uint8_t MAX_PER_LOOP = 4;  // Process up to 4 frames per main loop
            
            // Poll CAN1 hardware FIFO directly
            while (C1FIFOINT1bits.RXNEMPTYIF && processed < MAX_PER_LOOP) {
                can_process_rx(CAN_BUS_0);  // Move from HW FIFO to SW queue
                processed++;
            }
            
            // Poll CAN2 hardware FIFO directly  
            while (C2FIFOINT1bits.RXNEMPTYIF && processed < MAX_PER_LOOP) {
                can_process_rx(CAN_BUS_1);
                processed++;
            }
            
            // Now send any queued frames to USB
            for (uint8_t bus = 0; bus < CAN_NUM_BUSES; bus++) {
                while (can_rx_available(bus) > 0) {
                    if (can_rx_get(bus, &rxFrame)) {
                        LED_CAN1 ^= 1;  // Toggle LED to show activity
                        
                        // Convert to GVRET format
                        gvFrame.id = rxFrame.id;
                        gvFrame.length = rxFrame.length;
                        gvFrame.bus = rxFrame.bus;
                        gvFrame.extended = rxFrame.extended;
                        for (int i = 0; i < 8; i++) {
                            gvFrame.data[i] = rxFrame.data[i];
                        }
                        
                        // Send to host
                        gvret_send_can_frame(&gvret_ctx, &gvFrame, rxFrame.timestamp);
                    }
                }
            }
        }
        
        loop_count++;
        
        // Send welcome message when terminal connects (but not in binary GVRET mode)
        if (send_welcome && usb_is_configured() && !gvret_is_binary_mode(&gvret_ctx)) {
            delay_ms(100);  // Brief delay for terminal to be ready
            send_status_message();
            send_welcome = false;
        }
        // If SavvyCAN connected (binary mode), just clear the flag
        if (send_welcome && gvret_is_binary_mode(&gvret_ctx)) {
            send_welcome = false;
        }
        
        // Handle commands
        if (received_cmd == 'i' || received_cmd == 'I') {
            received_cmd = 0;
            
            if (!loopback_running) {
                cdc_send_string("\r\n=== CAN1 INTERNAL LOOPBACK TEST ===\r\n");
                cdc_send_string("(CAN1 TX -> CAN1 RX, no bus needed)\r\n\r\n");
                
                if (!CAN1_Init_Internal()) {
                    cdc_send_string("ERROR: CAN1 init failed!\r\n\r\n");
                    continue;
                }
                
                cdc_send_string("\r\nStarting test... Press 's' to stop.\r\n\r\n");
                
                loopback_sent = 0;
                loopback_received = 0;
                loopback_timeouts = 0;
                loopback_internal = true;
                loopback_can_module = 1;
                loopback_running = true;
                loopback_stop_requested = false;
            }
        }
        
        if (received_cmd == 'l' || received_cmd == 'L') {
            received_cmd = 0;
            
            if (!loopback_running) {
                cdc_send_string("\r\n=== CAN2 INTERNAL LOOPBACK TEST ===\r\n");
                cdc_send_string("(CAN2 TX -> CAN2 RX, no bus needed)\r\n\r\n");
                
                if (!CAN2_Init_Internal()) {
                    cdc_send_string("ERROR: CAN2 init failed!\r\n\r\n");
                    continue;
                }
                
                cdc_send_string("\r\nStarting test... Press 's' to stop.\r\n\r\n");
                
                loopback_sent = 0;
                loopback_received = 0;
                loopback_timeouts = 0;
                loopback_internal = true;
                loopback_can_module = 2;
                loopback_running = true;
                loopback_stop_requested = false;
            }
        }
        
        if (received_cmd == 'p' || received_cmd == 'P') {
            received_cmd = 0;
            
            if (!loopback_running) {
                cdc_send_string("\r\n=== EXTERNAL LOOPBACK TEST ===\r\n");
                cdc_send_string("(CAN1 TX -> CAN2 RX via bus)\r\n");
                cdc_send_string("Requires: CAN1<->CAN2 wired, 120ohm term\r\n\r\n");
                
                // Initialize CAN2 first (RX)
                if (!CAN2_Init_External()) {
                    cdc_send_string("ERROR: CAN2 init failed!\r\n\r\n");
                    continue;
                }
                
                // Then CAN1 (TX)
                if (!CAN1_Init_External()) {
                    cdc_send_string("ERROR: CAN1 init failed!\r\n\r\n");
                    continue;
                }
                
                cdc_send_string("\r\nStarting test... Press 's' to stop.\r\n\r\n");
                
                loopback_sent = 0;
                loopback_received = 0;
                loopback_timeouts = 0;
                loopback_internal = false;
                loopback_can_module = 1;  // CAN1 transmits
                loopback_running = true;
                loopback_stop_requested = false;
            }
        }
        
        if (received_cmd == 'q' || received_cmd == 'Q') {
            received_cmd = 0;
            
            if (!loopback_running) {
                cdc_send_string("\r\n=== REVERSE EXTERNAL LOOPBACK TEST ===\r\n");
                cdc_send_string("(CAN2 TX -> CAN1 RX via bus)\r\n");
                cdc_send_string("Requires: CAN1<->CAN2 wired, 120ohm term\r\n\r\n");
                
                // Initialize CAN1 first (RX) - need new init function
                if (!CAN1_Init_External_RX()) {
                    cdc_send_string("ERROR: CAN1 init failed!\r\n\r\n");
                    continue;
                }
                
                // Then CAN2 (TX)
                if (!CAN2_Init_External_TX()) {
                    cdc_send_string("ERROR: CAN2 init failed!\r\n\r\n");
                    continue;
                }
                
                cdc_send_string("\r\nStarting test... Press 's' to stop.\r\n\r\n");
                
                loopback_sent = 0;
                loopback_received = 0;
                loopback_timeouts = 0;
                loopback_internal = false;
                loopback_can_module = 2;  // CAN2 transmits
                loopback_running = true;
                loopback_stop_requested = false;
            }
        }
        
        if (received_cmd == 'd' || received_cmd == 'D') {
            received_cmd = 0;
            
            if (!debug_can_mode && !loopback_running) {
                cdc_send_string("\r\n=== DEBUG CAN RX MODE ===\r\n");
                cdc_send_string("Enabling CAN0 at 500kbps...\r\n");
                cdc_send_string("Will print raw FIFO data for each frame.\r\n");
                cdc_send_string("Press 's' to stop.\r\n\r\n");
                
                // Configure and enable CAN0
                CAN_Config cfg;
                cfg.baudRate = 500000;
                cfg.enabled = true;
                cfg.listenOnly = false;  // Normal mode to ACK frames
                
                if (can_configure(CAN_BUS_0, &cfg)) {
                    cdc_send_string("CAN0 enabled.\r\n");
                    
                    // Print FIFO configuration for debugging
                    uint32_t fifoba = C1FIFOBA;
                    cdc_send_string("C1FIFOBA = 0x");
                    cdc_send_hex_byte((fifoba >> 24) & 0xFF);
                    cdc_send_hex_byte((fifoba >> 16) & 0xFF);
                    cdc_send_hex_byte((fifoba >> 8) & 0xFF);
                    cdc_send_hex_byte(fifoba & 0xFF);
                    cdc_send_string("\r\n");
                    
                    cdc_send_string("C1FIFOCON0 = 0x");
                    cdc_send_hex_byte((C1FIFOCON0 >> 24) & 0xFF);
                    cdc_send_hex_byte((C1FIFOCON0 >> 16) & 0xFF);
                    cdc_send_hex_byte((C1FIFOCON0 >> 8) & 0xFF);
                    cdc_send_hex_byte(C1FIFOCON0 & 0xFF);
                    cdc_send_string("\r\n");
                    
                    cdc_send_string("C1FIFOCON1 = 0x");
                    cdc_send_hex_byte((C1FIFOCON1 >> 24) & 0xFF);
                    cdc_send_hex_byte((C1FIFOCON1 >> 16) & 0xFF);
                    cdc_send_hex_byte((C1FIFOCON1 >> 8) & 0xFF);
                    cdc_send_hex_byte(C1FIFOCON1 & 0xFF);
                    cdc_send_string("\r\n");
                    
                    cdc_send_string("Waiting for frames...\r\n\r\n");
                    debug_can_mode = true;
                    debug_frame_count = 0;
                } else {
                    cdc_send_string("ERROR: CAN0 init failed!\r\n\r\n");
                }
            }
        }
        
        if (received_cmd == 's' || received_cmd == 'S') {
            received_cmd = 0;
            if (debug_can_mode) {
                debug_can_mode = false;
                can_disable(CAN_BUS_0);
                cdc_send_string("\r\n--- Debug mode stopped ---\r\n");
                cdc_send_string("Total frames: ");
                cdc_send_number(debug_frame_count);
                cdc_send_string("\r\n\r\n");
            }
            if (loopback_running) {
                loopback_stop_requested = true;
            }
        }
        
        // DEBUG MODE: Poll CAN FIFO and print raw data
        if (debug_can_mode) {
            // Check if FIFO1 has a message
            if (C1FIFOINT1bits.RXNEMPTYIF) {
                LED_CAN1 = 1;
                
                // Get the FIFO address (physical -> virtual)
                uint32_t fifoPA = C1FIFOUA1;
                volatile uint32_t *rxBuf = (volatile uint32_t *)PA_TO_KVA1(fifoPA);
                
                // Read raw words
                uint32_t word0 = rxBuf[0];
                uint32_t word1 = rxBuf[1];
                uint32_t word2 = rxBuf[2];  // Data bytes 0-3
                uint32_t word3 = rxBuf[3];  // Data bytes 4-7
                
                // Parse the ID
                uint32_t id;
                if (word0 & (1 << 19)) {
                    // Extended
                    id = ((word0 >> 11) & 0x3FFFF) | ((word0 & 0x7FF) << 18);
                } else {
                    // Standard
                    id = word0 & 0x7FF;
                }
                
                uint8_t dlc = word1 & 0x0F;
                
                // Print: Frame# | FIFO_PA | Word0 | ID | DLC
                cdc_send_string("F");
                cdc_send_number(debug_frame_count);
                cdc_send_string(" PA=0x");
                cdc_send_hex_byte((fifoPA >> 24) & 0xFF);
                cdc_send_hex_byte((fifoPA >> 16) & 0xFF);
                cdc_send_hex_byte((fifoPA >> 8) & 0xFF);
                cdc_send_hex_byte(fifoPA & 0xFF);
                cdc_send_string(" W0=0x");
                cdc_send_hex_byte((word0 >> 24) & 0xFF);
                cdc_send_hex_byte((word0 >> 16) & 0xFF);
                cdc_send_hex_byte((word0 >> 8) & 0xFF);
                cdc_send_hex_byte(word0 & 0xFF);
                cdc_send_string(" ID=0x");
                cdc_send_hex_byte((id >> 8) & 0xFF);
                cdc_send_hex_byte(id & 0xFF);
                cdc_send_string(" L=");
                cdc_send_number(dlc);
                cdc_send_string("\r\n");
                
                debug_frame_count++;
                
                // CRITICAL: Increment FIFO pointer to release this slot
                // MUST use atomic SET register - bit-field write doesn't work for write-only bits!
                C1FIFOCON1SET = _C1FIFOCON1_UINC_MASK;
                
                LED_CAN1 = 0;
            }
            
            // Clear any overflow
            if (C1FIFOINT1bits.RXOVFLIF) {
                C1FIFOINT1bits.RXOVFLIF = 0;
                cdc_send_string("!OVERFLOW!\r\n");
            }
        }
        
        // Run loopback test if active
        if (loopback_running && !loopback_stop_requested) {
            run_loopback_test();
            delay_ms(500);  // Delay between messages
        }
        
        // Stop loopback if requested
        if (loopback_running && loopback_stop_requested) {
            loopback_running = false;
            print_loopback_summary();
            cdc_send_string("Ready.\r\n\r\n");
        }
        
        // Slow blink LED_CAN2 while waiting for USB configuration
        if (!usb_is_configured()) {
            if ((loop_count % 100000) == 0) {
                LED_CAN2 ^= 1;  // Toggle LED4
            }
        }
    }
    
    return 0;
}
