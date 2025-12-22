/**
 * @file can_driver.c
 * @brief CAN Bus Driver Implementation for CANalysis
 */

#include "can_driver.h"
#include <xc.h>
#include <sys/attribs.h>
#include <sys/kmem.h>
#include <string.h>

// =============================================================================
// System Configuration
// =============================================================================

#define SYS_FREQ    48000000UL
#define CAN_FREQ    48000000UL  // Peripheral clock for CAN

// =============================================================================
// CAN FIFO Buffers
// =============================================================================

// Must be aligned and in coherent memory for DMA
// Alignment requirement: 2^(FSIZE+1) * 16 bytes. For FSIZE=7: 4096 bytes
static uint8_t __attribute__((coherent, aligned(4096))) can1_fifo_buf[32 * 16];
static uint8_t __attribute__((coherent, aligned(4096))) can2_fifo_buf[32 * 16];

// Direct pointers into buffers
#define CAN1_TX_BUF     ((volatile uint32_t *)&can1_fifo_buf[0])     // FIFO0 = TX
#define CAN1_RX_BUF     ((volatile uint32_t *)&can1_fifo_buf[16])    // FIFO1 = RX
#define CAN2_TX_BUF     ((volatile uint32_t *)&can2_fifo_buf[0])     // FIFO0 = TX
#define CAN2_RX_BUF     ((volatile uint32_t *)&can2_fifo_buf[16])    // FIFO1 = RX

// =============================================================================
// RX Queues
// =============================================================================

static CAN_RxQueue rx_queue[CAN_NUM_BUSES];

// =============================================================================
// Bus Configuration State
// =============================================================================

static CAN_Config bus_config[CAN_NUM_BUSES];
static bool bus_initialized[CAN_NUM_BUSES] = {false, false};

// =============================================================================
// Timestamp
// =============================================================================

uint32_t can_get_timestamp(void) {
    // Core Timer runs at SYSCLK/2 = 24 MHz
    return _CP0_GET_COUNT() / 24;
}

// =============================================================================
// Baud Rate Calculation
// =============================================================================

// Calculate timing parameters for a given baud rate
// PIC32MX CAN timing: Tq = (BRP + 1) * 2 / Fcan
// Bit time = Tq * (SYNC + PRSEG + SEG1PH + SEG2PH)
// SYNC is always 1 Tq

typedef struct {
    uint8_t brp;        // Baud rate prescaler (0-63)
    uint8_t sjw;        // Sync jump width (0-3)
    uint8_t prseg;      // Propagation segment (0-7)
    uint8_t seg1ph;     // Phase segment 1 (0-7)
    uint8_t seg2ph;     // Phase segment 2 (0-7)
} CAN_Timing;

static bool calculate_timing(uint32_t baudRate, CAN_Timing *timing) {
    // Target sample point around 75-80%
    // Common configurations for 48MHz clock:
    
    switch (baudRate) {
        case 1000000:  // 1 Mbps: 48MHz / 3 = 16 Tq
            timing->brp = 0;       // Tq = 2/48MHz = 41.67ns
            timing->sjw = 0;       // SJW = 1 Tq
            timing->prseg = 7;     // PRSEG = 8 Tq
            timing->seg1ph = 5;    // SEG1PH = 6 Tq
            timing->seg2ph = 1;    // SEG2PH = 2 Tq
            // Total = 1 + 8 + 6 + 2 = 17 Tq, but hardware adds sync
            // Actually: Nominal = SYNC(1) + PRSEG+1 + SEG1PH+1 + SEG2PH+1 = 1+8+6+2 = 17? 
            // Hmm, need to verify. For now use tested config.
            timing->brp = 2;       // Tq = 6/48MHz = 125ns, 8 Tq = 1us = 1Mbps
            timing->prseg = 0;
            timing->seg1ph = 2;
            timing->seg2ph = 2;
            break;
            
        case 500000:  // 500 kbps
            timing->brp = 5;       // Tq = 12/48MHz = 250ns
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            // Total = 1 + 3 + 6 + 6 = 16 Tq = 4us = 250kbps? No wait...
            // 16 Tq * 250ns = 4us, 1/4us = 250kbps. Need 2us for 500kbps = 8 Tq
            timing->brp = 2;       // Tq = 6/48MHz = 125ns
            timing->prseg = 2;     // 3 Tq
            timing->seg1ph = 5;    // 6 Tq  
            timing->seg2ph = 5;    // 6 Tq
            // Total = 1+3+6+6 = 16 Tq * 125ns = 2us = 500kbps ✓
            break;
            
        case 250000:  // 250 kbps
            timing->brp = 5;       // Tq = 12/48MHz = 250ns
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            // Total = 16 Tq * 250ns = 4us = 250kbps ✓
            break;
            
        case 125000:  // 125 kbps (default, tested working)
            timing->brp = 11;      // Tq = 24/48MHz = 500ns
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            // Total = 16 Tq * 500ns = 8us = 125kbps ✓
            break;
            
        case 100000:  // 100 kbps
            timing->brp = 14;      // Tq = 30/48MHz = 625ns
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            // Total = 16 Tq * 625ns = 10us = 100kbps ✓
            break;
            
        case 50000:   // 50 kbps
            timing->brp = 29;      // Tq = 60/48MHz = 1.25us
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            // Total = 16 Tq * 1.25us = 20us = 50kbps ✓
            break;
            
        case 33333:   // 33.3 kbps (some vehicles)
            timing->brp = 44;      // Tq = 90/48MHz = 1.875us
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            break;
            
        case 20000:   // 20 kbps
            timing->brp = 59;      // Tq = 120/48MHz = 2.5us
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            break;
            
        default:
            // Unsupported baud rate - use 500k as default
            timing->brp = 2;
            timing->sjw = 1;
            timing->prseg = 2;
            timing->seg1ph = 5;
            timing->seg2ph = 5;
            return false;
    }
    return true;
}

// =============================================================================
// Initialize Driver
// =============================================================================

void can_driver_init(void) {
    // Clear RX queues
    memset(rx_queue, 0, sizeof(rx_queue));
    
    // Default configuration: 500kbps, disabled
    for (int i = 0; i < CAN_NUM_BUSES; i++) {
        bus_config[i].baudRate = 500000;
        bus_config[i].enabled = false;
        bus_config[i].listenOnly = false;
        bus_initialized[i] = false;
    }
    
    // Configure GPIO for CAN
    // CAN1: RF0=RX, RF1=TX
    TRISFbits.TRISF0 = 1;  // Input
    TRISFbits.TRISF1 = 0;  // Output
    
    // CAN2: RG0=RX, RG1=TX
    TRISGbits.TRISG0 = 1;  // Input
    TRISGbits.TRISG1 = 0;  // Output
}

// =============================================================================
// Configure CAN Bus
// =============================================================================

bool can_configure(uint8_t bus, const CAN_Config *config) {
    if (bus >= CAN_NUM_BUSES || config == NULL) return false;
    
    // Store configuration
    bus_config[bus].baudRate = config->baudRate;
    bus_config[bus].enabled = config->enabled;
    bus_config[bus].listenOnly = config->listenOnly;
    
    // If enabled, apply configuration
    if (config->enabled) {
        return can_enable(bus);
    } else {
        can_disable(bus);
        return true;
    }
}

// =============================================================================
// Enable CAN Bus
// =============================================================================

bool can_enable(uint8_t bus) {
    if (bus >= CAN_NUM_BUSES) return false;
    
    CAN_Timing timing;
    if (!calculate_timing(bus_config[bus].baudRate, &timing)) {
        // Use defaults if calculation failed
    }
    
    uint32_t timeout;
    
    if (bus == CAN_BUS_0) {
        // === CAN1 Configuration ===
        
        // Disable module first
        C1CONbits.ON = 0;
        IEC1bits.CAN1IE = 0;
        IFS1bits.CAN1IF = 0;
        C1INT = 0;
        C1INTCLR = 0xFFFFFFFF;
        
        // Clear FIFO buffer
        memset(can1_fifo_buf, 0, sizeof(can1_fifo_buf));
        
        // Clear all filter enables
        C1FLTCON0 = 0;
        C1FLTCON1 = 0;
        C1FLTCON2 = 0;
        C1FLTCON3 = 0;
        
        // Set FIFO base address
        C1FIFOBA = KVA_TO_PA(can1_fifo_buf);
        
        // Enable and enter config mode
        C1CONbits.ON = 1;
        C1CONbits.REQOP = 4;  // Config mode
        timeout = 100000;
        while (C1CONbits.OPMOD != 4 && timeout > 0) timeout--;
        if (timeout == 0) return false;
        
        // Configure bit timing
        C1CFGbits.BRP = timing.brp;
        C1CFGbits.SJW = timing.sjw;
        C1CFGbits.PRSEG = timing.prseg;
        C1CFGbits.SEG1PH = timing.seg1ph;
        C1CFGbits.SEG2PHTS = 1;  // SEG2PH is freely programmable
        C1CFGbits.SEG2PH = timing.seg2ph;
        C1CFGbits.SAM = 1;  // Sample 3 times
        
        // FIFO0 = TX (1 message, offset 0, bytes 0-15)
        C1FIFOCON0bits.FRESET = 1;
        while (C1FIFOCON0bits.FRESET);
        C1FIFOCON0bits.FSIZE = 0;  // 1 message = 16 bytes
        C1FIFOCON0bits.TXEN = 1;
        
        // FIFO1 = RX (8 messages to buffer traffic bursts)
        // FSIZE=7 requires 4096-byte alignment (which we have)
        C1FIFOCON1bits.FRESET = 1;
        while (C1FIFOCON1bits.FRESET);
        C1FIFOCON1bits.FSIZE = 7;  // 8 messages = 128 bytes
        C1FIFOCON1bits.TXEN = 0;
        
        // Filter 0: Accept all standard frames into FIFO1
        C1RXF0 = 0;
        C1RXM0 = 0;  // Don't care about any bits
        C1FLTCON0bits.FSEL0 = 1;   // Route to FIFO1
        C1FLTCON0bits.MSEL0 = 0;   // Use mask 0
        C1FLTCON0bits.FLTEN0 = 1;  // Enable filter
        
        // Enable RX notification on FIFO1 (but NOT interrupt - we use polling)
        // RXNEMPTYIE sets the RXNEMPTYIF flag when data available
        // Polling in main loop checks RXNEMPTYIF directly
        C1FIFOINT1bits.RXNEMPTYIE = 1;
        // DO NOT enable RBIE - we use polling to avoid ISR starvation
        // C1INTbits.RBIE = 1;
        
        // Configure system interrupt
        IPC11bits.CAN1IP = 4;
        IPC11bits.CAN1IS = 0;
        IFS1bits.CAN1IF = 0;
        IEC1bits.CAN1IE = 1;
        
        // Enter operational mode
        if (bus_config[bus].listenOnly) {
            C1CONbits.REQOP = 3;  // Listen-only mode
            timeout = 100000;
            while (C1CONbits.OPMOD != 3 && timeout > 0) timeout--;
        } else {
            C1CONbits.REQOP = 0;  // Normal mode
            timeout = 500000;  // Longer timeout - needs 11 recessive bits
            while (C1CONbits.OPMOD != 0 && timeout > 0) timeout--;
        }
        if (timeout == 0) return false;
        
    } else {
        // === CAN2 Configuration ===
        
        // Disable module first
        C2CONbits.ON = 0;
        IEC1bits.CAN2IE = 0;
        IFS1bits.CAN2IF = 0;
        C2INT = 0;
        C2INTCLR = 0xFFFFFFFF;
        
        // Clear FIFO buffer
        memset(can2_fifo_buf, 0, sizeof(can2_fifo_buf));
        
        // Clear all filter enables
        C2FLTCON0 = 0;
        C2FLTCON1 = 0;
        C2FLTCON2 = 0;
        C2FLTCON3 = 0;
        
        // Set FIFO base address
        C2FIFOBA = KVA_TO_PA(can2_fifo_buf);
        
        // Enable and enter config mode
        C2CONbits.ON = 1;
        C2CONbits.REQOP = 4;
        timeout = 100000;
        while (C2CONbits.OPMOD != 4 && timeout > 0) timeout--;
        if (timeout == 0) return false;
        
        // Configure bit timing
        C2CFGbits.BRP = timing.brp;
        C2CFGbits.SJW = timing.sjw;
        C2CFGbits.PRSEG = timing.prseg;
        C2CFGbits.SEG1PH = timing.seg1ph;
        C2CFGbits.SEG2PHTS = 1;
        C2CFGbits.SEG2PH = timing.seg2ph;
        C2CFGbits.SAM = 1;
        
        // FIFO0 = TX (1 message, offset 0, bytes 0-15)
        C2FIFOCON0bits.FRESET = 1;
        while (C2FIFOCON0bits.FRESET);
        C2FIFOCON0bits.FSIZE = 0;  // 1 message = 16 bytes
        C2FIFOCON0bits.TXEN = 1;
        
        // FIFO1 = RX (8 messages to buffer traffic bursts)
        C2FIFOCON1bits.FRESET = 1;
        while (C2FIFOCON1bits.FRESET);
        C2FIFOCON1bits.FSIZE = 7;  // 8 messages = 128 bytes
        C2FIFOCON1bits.TXEN = 0;
        
        // Filter 0: Accept all into FIFO1
        C2RXF0 = 0;
        C2RXM0 = 0;
        C2FLTCON0bits.FSEL0 = 1;
        C2FLTCON0bits.MSEL0 = 0;
        C2FLTCON0bits.FLTEN0 = 1;
        
        // Enable RX notification (but NOT interrupt - we use polling)
        C2FIFOINT1bits.RXNEMPTYIE = 1;
        // DO NOT enable RBIE - we use polling to avoid ISR starvation
        // C2INTbits.RBIE = 1;
        
        IPC11bits.CAN2IP = 4;
        IPC11bits.CAN2IS = 0;
        IFS1bits.CAN2IF = 0;
        IEC1bits.CAN2IE = 1;
        
        // Enter operational mode
        if (bus_config[bus].listenOnly) {
            C2CONbits.REQOP = 3;
            timeout = 100000;
            while (C2CONbits.OPMOD != 3 && timeout > 0) timeout--;
        } else {
            C2CONbits.REQOP = 0;
            timeout = 500000;
            while (C2CONbits.OPMOD != 0 && timeout > 0) timeout--;
        }
        if (timeout == 0) return false;
    }
    
    bus_initialized[bus] = true;
    bus_config[bus].enabled = true;
    return true;
}

// =============================================================================
// Disable CAN Bus
// =============================================================================

void can_disable(uint8_t bus) {
    if (bus >= CAN_NUM_BUSES) return;
    
    if (bus == CAN_BUS_0) {
        IEC1bits.CAN1IE = 0;
        C1CONbits.ON = 0;
    } else {
        IEC1bits.CAN2IE = 0;
        C2CONbits.ON = 0;
    }
    
    bus_initialized[bus] = false;
    bus_config[bus].enabled = false;
    
    // Clear RX queue
    rx_queue[bus].head = 0;
    rx_queue[bus].tail = 0;
    rx_queue[bus].count = 0;
}

// =============================================================================
// Get Configuration
// =============================================================================

void can_get_config(uint8_t bus, CAN_Config *config) {
    if (bus >= CAN_NUM_BUSES || config == NULL) return;
    *config = bus_config[bus];
}

// =============================================================================
// RX Queue Management
// =============================================================================

uint8_t can_rx_available(uint8_t bus) {
    if (bus >= CAN_NUM_BUSES) return 0;
    return rx_queue[bus].count;
}

bool can_rx_get(uint8_t bus, CAN_Frame *frame) {
    if (bus >= CAN_NUM_BUSES || frame == NULL) return false;
    
    CAN_RxQueue *q = &rx_queue[bus];
    if (q->count == 0) return false;
    
    // Disable interrupts briefly to prevent race
    uint32_t ie = __builtin_disable_interrupts();
    
    *frame = q->frames[q->tail];
    q->tail = (q->tail + 1) % CAN_RX_QUEUE_SIZE;
    q->count--;
    
    __builtin_mtc0(12, 0, ie);  // Restore interrupts
    return true;
}

// Add frame to RX queue (called from ISR)
static void rx_queue_push(uint8_t bus, const CAN_Frame *frame) {
    CAN_RxQueue *q = &rx_queue[bus];
    
    if (q->count >= CAN_RX_QUEUE_SIZE) {
        // Queue full - drop oldest frame
        q->tail = (q->tail + 1) % CAN_RX_QUEUE_SIZE;
        q->count--;
    }
    
    q->frames[q->head] = *frame;
    q->head = (q->head + 1) % CAN_RX_QUEUE_SIZE;
    q->count++;
}

// =============================================================================
// Transmit
// =============================================================================

bool can_tx(uint8_t bus, const CAN_Frame *frame) {
    if (bus >= CAN_NUM_BUSES || frame == NULL) return false;
    if (!bus_initialized[bus] || !bus_config[bus].enabled) return false;
    if (bus_config[bus].listenOnly) return false;  // Can't TX in listen-only
    
    volatile uint32_t *txBuf;
    
    if (bus == CAN_BUS_0) {
        // Check if TX FIFO is not full (TXNFULL bit = 1 means there's space)
        if ((C1FIFOINT0 & _C1FIFOINT0_TXNFULLIF_MASK) == 0) return false;
        
        // Get pointer to TX buffer slot
        txBuf = CAN1_TX_BUF;
        
        // Word 0: SID (bits 10:0) or EID (bits 28:0 with SID in 10:0)
        if (frame->extended) {
            // Extended ID: EID in bits 28:18, SID in 10:0, IDE bit 19
            txBuf[0] = ((frame->id >> 18) & 0x7FF)  // SID = upper 11 bits
                     | ((frame->id & 0x3FFFF) << 11) // EID = lower 18 bits
                     | (1 << 19);                    // IDE = extended
        } else {
            txBuf[0] = frame->id & 0x7FF;  // Standard ID only
        }
        
        // Word 1: DLC + RTR
        txBuf[1] = (frame->length & 0x0F) | (frame->rtr ? 0x200 : 0);
        
        // Words 2-3: Data
        volatile uint8_t *dataPtr = (volatile uint8_t *)&txBuf[2];
        for (int i = 0; i < 8; i++) {
            dataPtr[i] = (i < frame->length) ? frame->data[i] : 0;
        }
        
        // Request transmission - use SET register for write-only bits!
        C1FIFOCON0SET = _C1FIFOCON0_UINC_MASK | _C1FIFOCON0_TXREQ_MASK;
        
    } else {
        if ((C2FIFOINT0 & _C2FIFOINT0_TXNFULLIF_MASK) == 0) return false;
        
        txBuf = CAN2_TX_BUF;
        
        if (frame->extended) {
            txBuf[0] = ((frame->id >> 18) & 0x7FF)
                     | ((frame->id & 0x3FFFF) << 11)
                     | (1 << 19);
        } else {
            txBuf[0] = frame->id & 0x7FF;
        }
        
        txBuf[1] = (frame->length & 0x0F) | (frame->rtr ? 0x200 : 0);
        
        volatile uint8_t *dataPtr = (volatile uint8_t *)&txBuf[2];
        for (int i = 0; i < 8; i++) {
            dataPtr[i] = (i < frame->length) ? frame->data[i] : 0;
        }
        
        // Use SET register for write-only bits!
        C2FIFOCON0SET = _C2FIFOCON0_UINC_MASK | _C2FIFOCON0_TXREQ_MASK;
    }
    
    return true;
}

// =============================================================================
// Process RX (called from ISR)
// =============================================================================

// PA_TO_KVA1 is already defined in <sys/kmem.h> - converts physical to virtual address

void can_process_rx(uint8_t bus) {
    CAN_Frame frame;
    volatile uint32_t *rxBuf;
    
    frame.bus = bus;
    
    if (bus == CAN_BUS_0) {
        // Clear overflow flag if set (allows reception to continue)
        if (C1FIFOINT1bits.RXOVFLIF) {
            C1FIFOINT1bits.RXOVFLIF = 0;
        }
        
        // Check if there's a message waiting
        if (!C1FIFOINT1bits.RXNEMPTYIF) {
            return;  // No message
        }
        
        // C1FIFOUA1 contains the PHYSICAL ADDRESS of the current message
        // Must convert to virtual address for CPU access
        uint32_t fifoAddr = C1FIFOUA1;
        rxBuf = (volatile uint32_t *)PA_TO_KVA1(fifoAddr);
        
        // Timestamp when we actually read the message
        frame.timestamp = can_get_timestamp();
        
        // Parse Word 0: ID
        uint32_t word0 = rxBuf[0];
        if (word0 & (1 << 19)) {
            // Extended frame (bit 19 = IDE)
            frame.extended = 1;
            frame.id = ((word0 >> 11) & 0x3FFFF)  // EID bits 17:0
                     | ((word0 & 0x7FF) << 18);    // SID as upper bits
        } else {
            frame.extended = 0;
            frame.id = word0 & 0x7FF;
        }
        
        // Parse Word 1: DLC, RTR
        uint32_t word1 = rxBuf[1];
        frame.length = word1 & 0x0F;
        frame.rtr = (word1 & 0x200) ? 1 : 0;
        
        // Copy data
        volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
        for (int i = 0; i < 8; i++) {
            frame.data[i] = dataPtr[i];
        }
        
        // Add to software queue
        rx_queue_push(bus, &frame);
        
        // Increment hardware FIFO pointer to release this slot
        // MUST use SET register - bit-field write doesn't work for write-only bits!
        C1FIFOCON1SET = _C1FIFOCON1_UINC_MASK;
        
        // Note: We only process ONE message per ISR call.
        // If there are more messages, RBIF will remain set (or get set again)
        // and the ISR will fire again after we exit.
        // This gives the hardware time to update C1FIFOUA1 between calls.
        
    } else {
        // Clear overflow flag if set (allows reception to continue)
        if (C2FIFOINT1bits.RXOVFLIF) {
            C2FIFOINT1bits.RXOVFLIF = 0;
        }
        
        // Check if there's a message waiting
        if (!C2FIFOINT1bits.RXNEMPTYIF) {
            return;  // No message
        }
        
        // C2FIFOUA1 contains the PHYSICAL ADDRESS of the current message
        uint32_t fifoAddr = C2FIFOUA1;
        rxBuf = (volatile uint32_t *)PA_TO_KVA1(fifoAddr);
        
        // Timestamp when we actually read the message
        frame.timestamp = can_get_timestamp();
        
        // Parse Word 0: ID
        uint32_t word0 = rxBuf[0];
        if (word0 & (1 << 19)) {
            frame.extended = 1;
            frame.id = ((word0 >> 11) & 0x3FFFF)
                     | ((word0 & 0x7FF) << 18);
        } else {
            frame.extended = 0;
            frame.id = word0 & 0x7FF;
        }
        
        // Parse Word 1: DLC, RTR
        uint32_t word1 = rxBuf[1];
        frame.length = word1 & 0x0F;
        frame.rtr = (word1 & 0x200) ? 1 : 0;
        
        // Copy data
        volatile uint8_t *dataPtr = (volatile uint8_t *)&rxBuf[2];
        for (int i = 0; i < 8; i++) {
            frame.data[i] = dataPtr[i];
        }
        
        // Add to software queue
        rx_queue_push(bus, &frame);
        
        // Increment hardware FIFO pointer to release this slot
        // MUST use SET register for write-only bits!
        C2FIFOCON1SET = _C2FIFOCON1_UINC_MASK;
        
        // Only process ONE message per ISR call
    }
}

// =============================================================================
// ISR Note: CAN ISRs are defined in main.c to handle both loopback tests
// and GVRET mode. The ISRs call can_process_rx() when in GVRET mode.
// =============================================================================

