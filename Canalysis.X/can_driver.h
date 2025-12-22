/**
 * @file can_driver.h
 * @brief CAN Bus Driver for CANalysis
 * 
 * Provides a clean interface to both CAN modules for GVRET integration.
 * Supports configurable baud rates and normal/listen-only modes.
 */

#ifndef CAN_DRIVER_H
#define CAN_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

// =============================================================================
// Constants
// =============================================================================

#define CAN_NUM_BUSES       2
#define CAN_MAX_DATA_LEN    8
#define CAN_RX_QUEUE_SIZE   32  // Messages per bus

// CAN Bus indices
#define CAN_BUS_0           0   // CAN1 module (RF0/RF1)
#define CAN_BUS_1           1   // CAN2 module (RG0/RG1)

// =============================================================================
// CAN Frame Structure
// =============================================================================

typedef struct {
    uint32_t id;            // CAN ID (11-bit or 29-bit)
    uint32_t timestamp;     // Microseconds since boot
    uint8_t  length;        // Data length (0-8)
    uint8_t  bus;           // Bus index (0 or 1)
    uint8_t  extended;      // 1 = 29-bit ID, 0 = 11-bit ID
    uint8_t  rtr;           // Remote transmission request
    uint8_t  data[CAN_MAX_DATA_LEN];
} CAN_Frame;

// =============================================================================
// CAN Bus Configuration
// =============================================================================

typedef struct {
    uint32_t baudRate;      // Baud rate in bps (125000, 250000, 500000, 1000000)
    bool     enabled;       // Bus enabled
    bool     listenOnly;    // Listen-only mode (no ACK, no TX)
} CAN_Config;

// =============================================================================
// RX Queue (circular buffer)
// =============================================================================

typedef struct {
    CAN_Frame frames[CAN_RX_QUEUE_SIZE];
    volatile uint8_t head;      // Write index
    volatile uint8_t tail;      // Read index
    volatile uint8_t count;     // Number of frames in queue
} CAN_RxQueue;

// =============================================================================
// Public API
// =============================================================================

/**
 * @brief Initialize CAN driver (both buses disabled)
 */
void can_driver_init(void);

/**
 * @brief Configure a CAN bus
 * @param bus Bus index (0 or 1)
 * @param config Configuration settings
 * @return true on success
 */
bool can_configure(uint8_t bus, const CAN_Config *config);

/**
 * @brief Enable a CAN bus
 * @param bus Bus index (0 or 1)
 * @return true on success
 */
bool can_enable(uint8_t bus);

/**
 * @brief Disable a CAN bus
 * @param bus Bus index (0 or 1)
 */
void can_disable(uint8_t bus);

/**
 * @brief Check if frames are available in RX queue
 * @param bus Bus index (0 or 1)
 * @return Number of frames available
 */
uint8_t can_rx_available(uint8_t bus);

/**
 * @brief Get a received frame from the RX queue
 * @param bus Bus index (0 or 1)
 * @param frame Pointer to store the frame
 * @return true if frame was available
 */
bool can_rx_get(uint8_t bus, CAN_Frame *frame);

/**
 * @brief Transmit a CAN frame
 * @param bus Bus index (0 or 1)
 * @param frame Pointer to frame to transmit
 * @return true on success
 */
bool can_tx(uint8_t bus, const CAN_Frame *frame);

/**
 * @brief Get current bus configuration
 * @param bus Bus index (0 or 1)
 * @param config Pointer to store configuration
 */
void can_get_config(uint8_t bus, CAN_Config *config);

/**
 * @brief Get current timestamp in microseconds
 * @return Timestamp
 */
uint32_t can_get_timestamp(void);

/**
 * @brief Process CAN RX (call from ISR or polling loop)
 * Called internally by ISRs
 */
void can_process_rx(uint8_t bus);

#endif // CAN_DRIVER_H

