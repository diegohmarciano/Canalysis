/**
 * @file gvret.h
 * @brief GVRET Protocol Implementation for CANalysis
 * 
 * Implements the GVRET (Generalized Vehicle Reverse Engineering Tool) protocol
 * for communication with SavvyCAN and compatible software.
 * 
 * Protocol Reference: M2RET CommProtocol.txt
 */

#ifndef GVRET_H
#define GVRET_H

#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Build Configuration
// =============================================================================

#define GVRET_BUILD_NUM         100     // Firmware build number for SavvyCAN
#define GVRET_EEPROM_VER        0x20    // EEPROM version (matches M2RET)
#define GVRET_NUM_BUSES         2       // Number of CAN buses (CAN1 and CAN2)

// Serial buffer configuration
#define GVRET_SER_BUFF_SIZE     2048    // USB serial buffer size (handles burst traffic)
#define GVRET_FLUSH_INTERVAL_US 2000    // Flush interval in microseconds

// =============================================================================
// Protocol Constants
// =============================================================================

#define GVRET_START_BYTE        0xF1    // Start of binary command
#define GVRET_BINARY_MODE       0xE7    // Switch to binary mode

// =============================================================================
// Protocol Commands
// =============================================================================

typedef enum {
    GVRET_CMD_BUILD_CAN_FRAME   = 0,    // Send/receive CAN frame
    GVRET_CMD_TIME_SYNC         = 1,    // Time synchronization
    GVRET_CMD_DIG_INPUTS        = 2,    // Get digital inputs
    GVRET_CMD_ANA_INPUTS        = 3,    // Get analog inputs
    GVRET_CMD_SET_DIG_OUT       = 4,    // Set digital outputs
    GVRET_CMD_SETUP_CANBUS      = 5,    // Configure CAN bus
    GVRET_CMD_GET_CANBUS_PARAMS = 6,    // Get CAN bus parameters
    GVRET_CMD_GET_DEV_INFO      = 7,    // Get device information
    GVRET_CMD_SET_SW_MODE       = 8,    // Set single wire mode
    GVRET_CMD_KEEPALIVE         = 9,    // Keep-alive / validation
    GVRET_CMD_SET_SYSTYPE       = 10,   // Set system type
    GVRET_CMD_ECHO_CAN_FRAME    = 11,   // Echo CAN frame back
    GVRET_CMD_GET_NUMBUSES      = 12,   // Get number of buses
    GVRET_CMD_GET_EXT_BUSES     = 13,   // Get extended bus info
    GVRET_CMD_SET_EXT_BUSES     = 14,   // Set extended bus config
} GVRET_Command;

// =============================================================================
// Protocol State Machine
// =============================================================================

typedef enum {
    GVRET_STATE_IDLE,               // Waiting for 0xF1 or 0xE7
    GVRET_STATE_GET_COMMAND,        // Got 0xF1, waiting for command byte
    GVRET_STATE_BUILD_CAN_FRAME,    // Building outgoing CAN frame
    GVRET_STATE_TIME_SYNC,          // Processing time sync
    GVRET_STATE_SETUP_CANBUS,       // Processing CAN bus setup
    GVRET_STATE_SET_DIG_OUTPUTS,    // Processing digital output set
    GVRET_STATE_SET_SYSTYPE,        // Processing system type set
    GVRET_STATE_ECHO_CAN_FRAME,     // Processing CAN frame echo
    GVRET_STATE_SETUP_EXT_BUSES,    // Processing extended bus setup
} GVRET_State;

// =============================================================================
// CAN Bus Settings
// =============================================================================

typedef struct {
    uint32_t speed;         // Baud rate in bps
    bool enabled;           // Bus enabled
    bool listenOnly;        // Listen-only mode
} GVRET_CANSettings;

// =============================================================================
// CAN Frame Structure
// =============================================================================

typedef struct {
    uint32_t id;            // CAN ID (bit 31 = extended flag)
    uint8_t length;         // Data length (0-8)
    uint8_t bus;            // Bus index (0 or 1)
    bool extended;          // Extended ID flag
    uint8_t data[8];        // Data bytes
} GVRET_CANFrame;

// =============================================================================
// GVRET Context
// =============================================================================

typedef struct {
    // State machine
    GVRET_State state;
    uint8_t step;           // Sub-step within state
    
    // Binary mode flag
    bool binaryMode;
    
    // Frame being built (for TX)
    GVRET_CANFrame buildFrame;
    uint32_t buildInt;      // Temporary 32-bit value being assembled
    
    // Serial buffer
    uint8_t txBuffer[GVRET_SER_BUFF_SIZE];
    uint16_t txBufferLen;
    uint32_t lastFlushTime; // Microseconds
    
    // CAN settings
    GVRET_CANSettings can[GVRET_NUM_BUSES];
    
    // Timestamp base
    uint32_t startTime;     // Microseconds at startup
} GVRET_Context;

// =============================================================================
// Public API
// =============================================================================

/**
 * @brief Initialize GVRET protocol handler
 * @param ctx Pointer to GVRET context structure
 */
void gvret_init(GVRET_Context *ctx);

/**
 * @brief Process a received byte through the GVRET state machine
 * @param ctx Pointer to GVRET context
 * @param byte Received byte from USB
 */
void gvret_process_byte(GVRET_Context *ctx, uint8_t byte);

/**
 * @brief Send a received CAN frame to the host
 * @param ctx Pointer to GVRET context
 * @param frame Pointer to CAN frame to send
 * @param timestamp Timestamp in microseconds
 */
void gvret_send_can_frame(GVRET_Context *ctx, const GVRET_CANFrame *frame, uint32_t timestamp);

/**
 * @brief Flush the serial buffer if needed
 * @param ctx Pointer to GVRET context
 * @param currentTime Current time in microseconds
 */
void gvret_flush_if_needed(GVRET_Context *ctx, uint32_t currentTime);

/**
 * @brief Force flush the serial buffer
 * @param ctx Pointer to GVRET context
 */
void gvret_flush(GVRET_Context *ctx);

/**
 * @brief Get current timestamp in microseconds
 * @return Timestamp since system start
 */
uint32_t gvret_get_timestamp(void);

/**
 * @brief Check if GVRET is in binary mode
 * @param ctx Pointer to GVRET context
 * @return true if binary mode is active
 */
static inline bool gvret_is_binary_mode(GVRET_Context *ctx) {
    return ctx->binaryMode;
}

#endif // GVRET_H

