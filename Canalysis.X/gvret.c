/**
 * @file gvret.c
 * @brief GVRET Protocol Implementation for CANalysis
 */

#include "gvret.h"
#include "usb.h"
#include "version.h"
#include "can_driver.h"
#include <xc.h>
#include <string.h>

// =============================================================================
// System Frequency (must match main.c)
// =============================================================================

#define SYS_FREQ 48000000UL

// =============================================================================
// External Variables (shared with main.c)
// =============================================================================

// Track if endpoint 2 IN is busy (shared with main.c)
extern volatile bool ep2_in_busy;

// =============================================================================
// Internal Functions
// =============================================================================

static void gvret_buffer_byte(GVRET_Context *ctx, uint8_t byte);
static void gvret_buffer_response(GVRET_Context *ctx, const uint8_t *data, uint16_t len);
static void gvret_handle_command(GVRET_Context *ctx, uint8_t cmd);
static void gvret_handle_build_frame(GVRET_Context *ctx, uint8_t byte);
static void gvret_handle_setup_canbus(GVRET_Context *ctx, uint8_t byte);

// =============================================================================
// Timestamp Functions
// =============================================================================

uint32_t gvret_get_timestamp(void) {
    // Core Timer runs at SYSCLK/2 = 24 MHz
    // To get microseconds: count / 24
    return _CP0_GET_COUNT() / 24;
}

// =============================================================================
// Initialization
// =============================================================================

void gvret_init(GVRET_Context *ctx) {
    memset(ctx, 0, sizeof(GVRET_Context));
    
    ctx->state = GVRET_STATE_IDLE;
    ctx->binaryMode = false;
    ctx->startTime = gvret_get_timestamp();
    
    // Default CAN settings: 500000 bps, enabled, not listen-only
    ctx->can[0].speed = 500000;
    ctx->can[0].enabled = true;
    ctx->can[0].listenOnly = false;
    
    ctx->can[1].speed = 500000;
    ctx->can[1].enabled = false;
    ctx->can[1].listenOnly = false;
}

// =============================================================================
// Buffer Management
// =============================================================================

static void gvret_buffer_byte(GVRET_Context *ctx, uint8_t byte) {
    if (ctx->txBufferLen < GVRET_SER_BUFF_SIZE) {
        ctx->txBuffer[ctx->txBufferLen++] = byte;
    }
}

void gvret_flush(GVRET_Context *ctx) {
    if (ctx->txBufferLen == 0) return;
    if (!usb_is_configured()) return;
    
    // Don't try to send if endpoint is busy
    if (ep2_in_busy) return;
    
    // Get the IN buffer and copy data
    unsigned char *buf = usb_get_in_buffer(2);
    if (!buf) return;
    
    // Send up to 64 bytes at a time
    uint16_t chunk = (ctx->txBufferLen > 64) ? 64 : ctx->txBufferLen;
    
    memcpy(buf, ctx->txBuffer, chunk);
    ep2_in_busy = true;
    usb_send_in_buffer(2, chunk);
    
    // Shift remaining data to front of buffer
    if (chunk < ctx->txBufferLen) {
        memmove(ctx->txBuffer, &ctx->txBuffer[chunk], ctx->txBufferLen - chunk);
    }
    ctx->txBufferLen -= chunk;
    ctx->lastFlushTime = gvret_get_timestamp();
}

void gvret_flush_if_needed(GVRET_Context *ctx, uint32_t currentTime) {
    if (ctx->txBufferLen > 0) {
        uint32_t elapsed = currentTime - ctx->lastFlushTime;
        // Flush every 1ms or when buffer is getting full
        if (elapsed >= 1000 || ctx->txBufferLen >= GVRET_SER_BUFF_SIZE - 64) {
            gvret_flush(ctx);
        }
    }
}

// Buffer data for sending (will be flushed by main loop)
static void gvret_buffer_response(GVRET_Context *ctx, const uint8_t *data, uint16_t len) {
    for (uint16_t i = 0; i < len; i++) {
        gvret_buffer_byte(ctx, data[i]);
    }
}

// =============================================================================
// State Machine - Main Entry Point
// =============================================================================

void gvret_process_byte(GVRET_Context *ctx, uint8_t byte) {
    switch (ctx->state) {
        case GVRET_STATE_IDLE:
            if (byte == GVRET_START_BYTE) {
                ctx->state = GVRET_STATE_GET_COMMAND;
            } else if (byte == GVRET_BINARY_MODE) {
                // Enable binary mode
                if (!ctx->binaryMode) {
                    ctx->binaryMode = true;
                    
                    // Apply CAN configuration to hardware when entering binary mode
                    for (int bus = 0; bus < GVRET_NUM_BUSES; bus++) {
                        if (ctx->can[bus].enabled) {
                            CAN_Config config;
                            config.baudRate = ctx->can[bus].speed;
                            config.enabled = true;
                            config.listenOnly = ctx->can[bus].listenOnly;
                            can_configure(bus, &config);
                        }
                    }
                }
            }
            // Other bytes are ignored (could be ASCII console)
            break;
            
        case GVRET_STATE_GET_COMMAND:
            gvret_handle_command(ctx, byte);
            break;
            
        case GVRET_STATE_BUILD_CAN_FRAME:
            gvret_handle_build_frame(ctx, byte);
            break;
            
        case GVRET_STATE_SETUP_CANBUS:
            gvret_handle_setup_canbus(ctx, byte);
            break;
            
        case GVRET_STATE_SET_DIG_OUTPUTS:
            // We don't have digital outputs, just consume the byte
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_STATE_SET_SYSTYPE:
            // Consume system type byte, ignore it
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_STATE_ECHO_CAN_FRAME:
            gvret_handle_build_frame(ctx, byte);  // Same parsing as build
            break;
            
        default:
            ctx->state = GVRET_STATE_IDLE;
            break;
    }
}

// =============================================================================
// Command Handler
// =============================================================================

static void gvret_handle_command(GVRET_Context *ctx, uint8_t cmd) {
    uint8_t response[20];
    uint32_t timestamp;
    
    switch (cmd) {
        case GVRET_CMD_BUILD_CAN_FRAME:
            // Start building a CAN frame to transmit
            ctx->state = GVRET_STATE_BUILD_CAN_FRAME;
            ctx->step = 0;
            memset(&ctx->buildFrame, 0, sizeof(ctx->buildFrame));
            break;
            
        case GVRET_CMD_TIME_SYNC:
            // Return current timestamp
            timestamp = gvret_get_timestamp();
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_TIME_SYNC;
            response[2] = (uint8_t)(timestamp & 0xFF);
            response[3] = (uint8_t)((timestamp >> 8) & 0xFF);
            response[4] = (uint8_t)((timestamp >> 16) & 0xFF);
            response[5] = (uint8_t)((timestamp >> 24) & 0xFF);
            gvret_buffer_response(ctx, response, 6);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_DIG_INPUTS:
            // Return digital inputs (we don't have any, return 0)
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_DIG_INPUTS;
            response[2] = 0x00;  // All inputs low
            response[3] = 0x00;  // Checksum
            gvret_buffer_response(ctx, response, 4);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_ANA_INPUTS:
            // Return analog inputs (we don't have any, return 0)
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_ANA_INPUTS;
            // 6 analog inputs, 2 bytes each = 12 bytes + checksum
            memset(&response[2], 0, 14);
            response[16] = 0x00;  // Checksum
            gvret_buffer_response(ctx, response, 17);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_SET_DIG_OUT:
            // Wait for output byte
            ctx->state = GVRET_STATE_SET_DIG_OUTPUTS;
            break;
            
        case GVRET_CMD_SETUP_CANBUS:
            // Start receiving CAN configuration (8 bytes)
            ctx->state = GVRET_STATE_SETUP_CANBUS;
            ctx->step = 0;
            ctx->buildInt = 0;
            break;
            
        case GVRET_CMD_GET_CANBUS_PARAMS:
            // Return CAN bus parameters
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_GET_CANBUS_PARAMS;
            
            // CAN0 (our CAN1) - Byte 2: enabled + listen-only flags
            response[2] = (ctx->can[0].enabled ? 0x01 : 0x00) | 
                         (ctx->can[0].listenOnly ? 0x10 : 0x00);
            // CAN0 speed (bytes 3-6, LSB first)
            response[3] = (uint8_t)(ctx->can[0].speed & 0xFF);
            response[4] = (uint8_t)((ctx->can[0].speed >> 8) & 0xFF);
            response[5] = (uint8_t)((ctx->can[0].speed >> 16) & 0xFF);
            response[6] = (uint8_t)((ctx->can[0].speed >> 24) & 0xFF);
            
            // CAN1 (our CAN2) - Byte 7: enabled + listen-only flags
            response[7] = (ctx->can[1].enabled ? 0x01 : 0x00) | 
                         (ctx->can[1].listenOnly ? 0x10 : 0x00);
            // CAN1 speed (bytes 8-11, LSB first)
            response[8] = (uint8_t)(ctx->can[1].speed & 0xFF);
            response[9] = (uint8_t)((ctx->can[1].speed >> 8) & 0xFF);
            response[10] = (uint8_t)((ctx->can[1].speed >> 16) & 0xFF);
            response[11] = (uint8_t)((ctx->can[1].speed >> 24) & 0xFF);
            
            gvret_buffer_response(ctx, response, 12);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_GET_DEV_INFO:
            // Return device information
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_GET_DEV_INFO;
            response[2] = (uint8_t)(GVRET_BUILD_NUM & 0xFF);      // Build number LSB
            response[3] = (uint8_t)((GVRET_BUILD_NUM >> 8) & 0xFF); // Build number MSB
            response[4] = GVRET_EEPROM_VER;  // EEPROM version
            response[5] = 0;                  // File output type (NONE)
            response[6] = 0;                  // Auto start logging (NO)
            response[7] = 0;                  // Single wire mode (NO)
            gvret_buffer_response(ctx, response, 8);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_SET_SW_MODE:
            // Just consume the next byte
            ctx->state = GVRET_STATE_SET_SYSTYPE;  // Reuse, just consumes 1 byte
            break;
            
        case GVRET_CMD_KEEPALIVE:
            // Respond with 0xDEAD
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_KEEPALIVE;
            response[2] = 0xDE;
            response[3] = 0xAD;
            gvret_buffer_response(ctx, response, 4);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_SET_SYSTYPE:
            // Wait for system type byte
            ctx->state = GVRET_STATE_SET_SYSTYPE;
            break;
            
        case GVRET_CMD_ECHO_CAN_FRAME:
            // Start building a CAN frame to echo back
            ctx->state = GVRET_STATE_ECHO_CAN_FRAME;
            ctx->step = 0;
            memset(&ctx->buildFrame, 0, sizeof(ctx->buildFrame));
            break;
            
        case GVRET_CMD_GET_NUMBUSES:
            // Return number of buses
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_GET_NUMBUSES;
            response[2] = GVRET_NUM_BUSES;
            gvret_buffer_response(ctx, response, 3);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_GET_EXT_BUSES:
            // Return extended bus info (SWCAN, LIN - we don't have these)
            response[0] = GVRET_START_BYTE;
            response[1] = GVRET_CMD_GET_EXT_BUSES;
            // SWCAN disabled, speed 0
            response[2] = 0x00;  // Not enabled
            response[3] = 0x00; response[4] = 0x00; response[5] = 0x00; response[6] = 0x00;
            // LIN1 disabled
            response[7] = 0x00;
            response[8] = 0x00; response[9] = 0x00; response[10] = 0x00; response[11] = 0x00;
            // LIN2 disabled
            response[12] = 0x00;
            response[13] = 0x00; response[14] = 0x00; response[15] = 0x00; response[16] = 0x00;
            gvret_buffer_response(ctx, response, 17);
            ctx->state = GVRET_STATE_IDLE;
            break;
            
        case GVRET_CMD_SET_EXT_BUSES:
            // Start receiving extended bus config
            ctx->state = GVRET_STATE_SETUP_EXT_BUSES;
            ctx->step = 0;
            break;
            
        default:
            // Unknown command, return to idle
            ctx->state = GVRET_STATE_IDLE;
            break;
    }
}

// =============================================================================
// Build CAN Frame Handler (for TX and Echo)
// =============================================================================

static void gvret_handle_build_frame(GVRET_Context *ctx, uint8_t byte) {
    bool isEcho = (ctx->state == GVRET_STATE_ECHO_CAN_FRAME);
    
    switch (ctx->step) {
        case 0: // ID byte 0 (LSB)
            ctx->buildFrame.id = byte;
            break;
        case 1: // ID byte 1
            ctx->buildFrame.id |= ((uint32_t)byte << 8);
            break;
        case 2: // ID byte 2
            ctx->buildFrame.id |= ((uint32_t)byte << 16);
            break;
        case 3: // ID byte 3 (MSB) - bit 31 = extended
            ctx->buildFrame.id |= ((uint32_t)byte << 24);
            if (ctx->buildFrame.id & 0x80000000) {
                ctx->buildFrame.id &= 0x7FFFFFFF;
                ctx->buildFrame.extended = true;
            } else {
                ctx->buildFrame.extended = false;
            }
            break;
        case 4: // Bus number
            ctx->buildFrame.bus = byte & 0x03;  // 0 or 1
            break;
        case 5: // Frame length
            ctx->buildFrame.length = byte & 0x0F;
            if (ctx->buildFrame.length > 8) {
                ctx->buildFrame.length = 8;
            }
            break;
        default: // Data bytes
            if (ctx->step < (uint8_t)(6 + ctx->buildFrame.length)) {
                ctx->buildFrame.data[ctx->step - 6] = byte;
            } else {
                // This is the checksum byte - frame is complete
                if (isEcho) {
                    // Echo the frame back as if it was received
                    gvret_send_can_frame(ctx, &ctx->buildFrame, gvret_get_timestamp());
                } else {
                    // Transmit the frame on the CAN bus
                    CAN_Frame txFrame;
                    txFrame.id = ctx->buildFrame.id;
                    txFrame.length = ctx->buildFrame.length;
                    txFrame.extended = ctx->buildFrame.extended ? 1 : 0;
                    txFrame.rtr = 0;
                    for (int i = 0; i < 8; i++) {
                        txFrame.data[i] = ctx->buildFrame.data[i];
                    }
                    can_tx(ctx->buildFrame.bus, &txFrame);
                }
                ctx->state = GVRET_STATE_IDLE;
                return;
            }
            break;
    }
    ctx->step++;
}

// =============================================================================
// Setup CAN Bus Handler
// =============================================================================

static void gvret_handle_setup_canbus(GVRET_Context *ctx, uint8_t byte) {
    switch (ctx->step) {
        case 0: // CAN0 speed byte 0 (LSB)
            ctx->buildInt = byte;
            break;
        case 1:
            ctx->buildInt |= ((uint32_t)byte << 8);
            break;
        case 2:
            ctx->buildInt |= ((uint32_t)byte << 16);
            break;
        case 3: // CAN0 speed byte 3 (MSB) - high bits are flags
            ctx->buildInt |= ((uint32_t)byte << 24);
            if (ctx->buildInt > 0) {
                if (ctx->buildInt & 0x80000000) {
                    ctx->can[0].enabled = (ctx->buildInt & 0x40000000) ? true : false;
                    ctx->can[0].listenOnly = (ctx->buildInt & 0x20000000) ? true : false;
                } else {
                    ctx->can[0].enabled = true;
                }
                ctx->can[0].speed = ctx->buildInt & 0x0FFFFF;  // Lower 20 bits
                if (ctx->can[0].speed > 1000000) {
                    ctx->can[0].speed = 1000000;
                }
            } else {
                ctx->can[0].enabled = false;
            }
            break;
        case 4: // CAN1 speed byte 0 (LSB)
            ctx->buildInt = byte;
            break;
        case 5:
            ctx->buildInt |= ((uint32_t)byte << 8);
            break;
        case 6:
            ctx->buildInt |= ((uint32_t)byte << 16);
            break;
        case 7: // CAN1 speed byte 3 (MSB)
            ctx->buildInt |= ((uint32_t)byte << 24);
            if (ctx->buildInt > 0) {
                if (ctx->buildInt & 0x80000000) {
                    ctx->can[1].enabled = (ctx->buildInt & 0x40000000) ? true : false;
                    ctx->can[1].listenOnly = (ctx->buildInt & 0x20000000) ? true : false;
                } else {
                    ctx->can[1].enabled = true;
                }
                ctx->can[1].speed = ctx->buildInt & 0x0FFFFF;
                if (ctx->can[1].speed > 1000000) {
                    ctx->can[1].speed = 1000000;
                }
            } else {
                ctx->can[1].enabled = false;
            }
            ctx->state = GVRET_STATE_IDLE;
            
            // Apply CAN configuration to hardware
            for (int bus = 0; bus < GVRET_NUM_BUSES; bus++) {
                CAN_Config config;
                config.baudRate = ctx->can[bus].speed;
                config.enabled = ctx->can[bus].enabled;
                config.listenOnly = ctx->can[bus].listenOnly;
                can_configure(bus, &config);
            }
            return;
        default:
            break;
    }
    ctx->step++;
}

// =============================================================================
// Send CAN Frame to Host
// =============================================================================

void gvret_send_can_frame(GVRET_Context *ctx, const GVRET_CANFrame *frame, uint32_t timestamp) {
    if (!ctx->binaryMode) return;
    
    // Safety check: ensure we have space for a full frame (max 20 bytes: 2+4+4+1+8+1)
    // If buffer is nearly full, drop this frame to prevent corruption
    if (ctx->txBufferLen > GVRET_SER_BUFF_SIZE - 20) {
        return; // Drop frame - buffer full
    }
    
    uint32_t id = frame->id;
    if (frame->extended) {
        id |= 0x80000000;
    }
    
    // Build the frame packet
    gvret_buffer_byte(ctx, GVRET_START_BYTE);
    gvret_buffer_byte(ctx, GVRET_CMD_BUILD_CAN_FRAME);  // 0x00 = CAN frame
    
    // Timestamp (4 bytes, LSB first)
    gvret_buffer_byte(ctx, (uint8_t)(timestamp & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((timestamp >> 8) & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((timestamp >> 16) & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((timestamp >> 24) & 0xFF));
    
    // ID (4 bytes, LSB first)
    gvret_buffer_byte(ctx, (uint8_t)(id & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((id >> 8) & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((id >> 16) & 0xFF));
    gvret_buffer_byte(ctx, (uint8_t)((id >> 24) & 0xFF));
    
    // Length + Bus (length in low nibble, bus in high nibble)
    gvret_buffer_byte(ctx, (frame->length & 0x0F) | ((frame->bus & 0x0F) << 4));
    
    // Data bytes
    for (uint8_t i = 0; i < frame->length; i++) {
        gvret_buffer_byte(ctx, frame->data[i]);
    }
    
    // Trailing zero (checksum placeholder)
    gvret_buffer_byte(ctx, 0x00);
}

