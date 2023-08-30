/***********************************************************************************************************************
 * Copyright [2020-2023] Renesas Electronics Corporation and/or its affiliates.  All Rights Reserved.
 *
 * This software and documentation are supplied by Renesas Electronics America Inc. and may only be used with products
 * of Renesas Electronics Corp. and its affiliates ("Renesas").  No other uses are authorized.  Renesas products are
 * sold pursuant to Renesas terms and conditions of sale.  Purchasers are solely responsible for the selection and use
 * of Renesas products and Renesas assumes no liability.  No license, express or implied, to any intellectual property
 * right is granted by Renesas. This software is protected under all applicable laws, including copyright laws. Renesas
 * reserves the right to change or discontinue this software and/or this documentation. THE SOFTWARE AND DOCUMENTATION
 * IS DELIVERED TO YOU "AS IS," AND RENESAS MAKES NO REPRESENTATIONS OR WARRANTIES, AND TO THE FULLEST EXTENT
 * PERMISSIBLE UNDER APPLICABLE LAW, DISCLAIMS ALL WARRANTIES, WHETHER EXPLICITLY OR IMPLICITLY, INCLUDING WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, AND NONINFRINGEMENT, WITH RESPECT TO THE SOFTWARE OR
 * DOCUMENTATION.  RENESAS SHALL HAVE NO LIABILITY ARISING OUT OF ANY SECURITY VULNERABILITY OR BREACH.  TO THE MAXIMUM
 * EXTENT PERMITTED BY LAW, IN NO EVENT WILL RENESAS BE LIABLE TO YOU IN CONNECTION WITH THE SOFTWARE OR DOCUMENTATION
 * (OR ANY PERSON OR ENTITY CLAIMING RIGHTS DERIVED FROM YOU) FOR ANY LOSS, DAMAGES, OR CLAIMS WHATSOEVER, INCLUDING,
 * WITHOUT LIMITATION, ANY DIRECT, CONSEQUENTIAL, SPECIAL, INDIRECT, PUNITIVE, OR INCIDENTAL DAMAGES; ANY LOST PROFITS,
 * OTHER ECONOMIC DAMAGE, PROPERTY DAMAGE, OR PERSONAL INJURY; AND EVEN IF RENESAS HAS BEEN ADVISED OF THE POSSIBILITY
 * OF SUCH LOSS, DAMAGES, CLAIMS OR COSTS.
 **********************************************************************************************************************/

/*******************************************************************************************************************//**
 * @ingroup RENESAS_INTERFACES
 * @defgroup SMCI_API SMCI Interface
 * @brief Interface for SMCI communications.
 *
 * @section SMCI_INTERFACE_SUMMARY Summary
 * The SMCI interface provides common APIs for SMCI HAL drivers. The SMCI interface supports the following features:
 * - Interrupt driven transmit/receive processing
 * - Callback function with returned event code
 * - Runtime baud-rate change (baud = 1/ETU)
 * - Hardware resource locking during a transaction
 *
 * Implemented by:
 * - @ref SCI_SMCI
 *
 * @{
 **********************************************************************************************************************/

#ifndef R_SMCI_API_H
#define R_SMCI_API_H

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/

/* Includes board and MCU related header files. */
#include "bsp_api.h"

/* Common macro for FSP header files. There is also a corresponding FSP_FOOTER macro at the end of this file. */
FSP_HEADER

/**********************************************************************************************************************
 * Macro definitions
 **********************************************************************************************************************/

/**********************************************************************************************************************
 * Typedef definitions
 **********************************************************************************************************************/

typedef enum e_smci_state
{
    SMCI_STATE_IDLE_CLOCK_OFF = 0U,    ///< SMCI idle state with no clock output
    SMCI_STATE_TX_RX_IDLE     = 1U,    ///< SMCI is in idle state, clock is active
    SMCI_STATE_TX_PROGRESSING = 2U,    ///< Transmission is in progress
    SMCI_STATE_RX_PROGRESSING = 3U,    ///< Reception is in progress
} smci_state_t;

/** SMCI Event codes */
typedef enum e_smci_event
{
    SMCI_EVENT_RX_COMPLETE    = (1UL << 0), ///< Receive complete event
    SMCI_EVENT_TX_COMPLETE    = (1UL << 1), ///< Transmit complete event
    SMCI_EVENT_RX_CHAR        = (1UL << 2), ///< Character transfer is completed
    SMCI_EVENT_ERR_PARITY     = (1UL << 3), ///< Parity error event
    SMCI_EVENT_ERR_LOW_SIGNAL = (1UL << 4), ///< Low error signal response occurred event
    SMCI_EVENT_ERR_OVERRUN    = (1UL << 5)  ///< Overrun error event
} smci_event_t;

typedef enum e_smci_convention_type
{
    SMCI_CONVENTION_TYPE_DIRECT  = 0U, ///< Direct convention type (LSB First, High=1)
    SMCI_CONVENTION_TYPE_INVERSE = 1U, ///< Inverse convention type (MSB First, Low=1)
} smci_convention_type_t;

/* This table matches Table 8 from ISO/IEC7816-3 Third edition 2006-11-01 */
typedef enum e_smci_clock_conversion_integer
{
    SMCI_CLOCK_CONVERSION_INTEGER_372_4         = 0U,  ///< 372 base cycles for 1-bit period, max freq = 4Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_372_5         = 1U,  ///< 372 base cycles for 1-bit period, max freq = 5Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_558_6         = 2U,  ///< 558 base cycles for 1-bit period, max freq = 6Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_744_8         = 3U,  ///< 744 base cycles for 1-bit period, max freq = 8Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_1116_12       = 4U,  ///< 1116 base cycles for 1-bit period, max freq = 12Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_1488_16       = 5U,  ///< 1488 base cycles for 1-bit period, max freq = 16Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_1860_20       = 6U,  ///< 1860 base cycles for 1-bit period, max freq = 20Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_UNSUPPORTED7  = 7U,  ///< Unsupported Clock Cycles
    SMCI_CLOCK_CONVERSION_INTEGER_UNSUPPORTED8  = 8U,  ///< Unsupported Clock Cycles
    SMCI_CLOCK_CONVERSION_INTEGER_512_5         = 9U,  ///< 512 base cycles for 1-bit period, max freq = 5Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_768_75        = 10U, ///< 768 base cycles for 1-bit period, max freq = 7.5Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_1024_10       = 11U, ///< 1024 base cycles for 1-bit period, max freq = 10Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_1536_15       = 12U, ///< 1536 base cycles for 1-bit period, max freq = 15Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_2048_20       = 13U, ///< 2048 base cycles for 1-bit period, max freq = 20Mhz
    SMCI_CLOCK_CONVERSION_INTEGER_UNSUPPORTED14 = 14U, ///< Unsupported Clock Cycles
    SMCI_CLOCK_CONVERSION_INTEGER_UNSUPPORTED15 = 15U, ///< Unsupported Clock Cycles
    SMCI_CLOCK_CONVERSION_INTEGER_MAX           = 16U
} smci_clock_conversion_integer_t;

/* This table matches Table 7 from ISO/IEC7816-3 Third edition 2006-11-01 */
typedef enum e_smci_baudrate_adjustment_integer
{
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU0  = 0U,  ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_1     = 1U,  ///< Di=1
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_2     = 2U,  ///< Di=2
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_4     = 3U,  ///< Di=4
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_8     = 4U,  ///< Di=8
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_16    = 5U,  ///< Di=16
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_32    = 6U,  ///< Di=32
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_64    = 7U,  ///< Di=64
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_12    = 8U,  ///< Di=12
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_20    = 9U,  ///< Di=20
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU10 = 10U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU11 = 11U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU12 = 12U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU13 = 13U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU14 = 14U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_RFU15 = 15U, ///< RESERVED
    SMCI_BAUDRATE_ADJUSTMENT_INTEGER_MAX   = 16U
} smci_baudrate_adjustment_integer_t;

/** SMCI Protocol Type according to ISO7816-3 */
typedef enum e_smci_protocol_type
{
    SMCI_PROTOCOL_TYPE_T0 = 0U,        ///< Normal mode operation (Protocol T = 0)
    SMCI_PROTOCOL_TYPE_T1 = 1U,        ///< Block transfer mode operation (Protocol T = 1)
} smci_protocol_type_t;

/** SMCI driver specific information */
typedef struct st_smci_status
{
    smci_state_t smci_state;           ///< State ot the smci state machine
    uint32_t     bytes_recvd;          ///< Bytes read into receive buffer since read was called
} smci_status_t;

/** SMCI Transfer Mode settings */
typedef struct st_smci_transfer_mode
{
    smci_protocol_type_t   protocol;   ///< Protocol  (Normal t=0, or Block t=1)
    smci_convention_type_t convention; ///< Convention Direct or Inverse
    bool gsm_mode;                     ///< True=GMS Mode, false=Normal
} smci_transfer_mode_t;

/** SMCI settings that are used as inputs to register setting calculations */
typedef struct st_smci_speed_params
{
    uint32_t baudrate;                     ///< Bits per second requested, 1/ETU
    smci_baudrate_adjustment_integer_t di; ///< Referred to as D in ISO spec (from Table 8 in ISO7816-3 3rd Edition)
    smci_clock_conversion_integer_t    fi; ///< Index of in ISO spec (from Table 8 in ISO7816-3 3rd Edition)
} smci_speed_params_t;

/** SMCI Callback parameter definition */
typedef struct st_smci_callback_args
{
    uint32_t     channel;              ///< Device channel number
    smci_event_t event;                ///< Event code

    /** Contains the next character received for the events SMCI_EVENT_RX_CHAR, SMCI_EVENT_ERR_PARITY,
     * SMCI_EVENT_ERR_LOW_SIGNAL, or SMCI_EVENT_ERR_OVERRUN.  Otherwise unused. */
    uint8_t      data;                 ///< Data Byte to process
    void const * p_context;            ///< Context provided to user during callback
} smci_callback_args_t;

/** Configuration Structure for SMCI */
typedef struct st_smci_cfg
{
    /* SMCI configuration */
    uint8_t   channel;                                  ///< Channel number of the hardware.
    uint8_t   rxi_ipl;                                  ///< Receive interrupt priority
    uint8_t   txi_ipl;                                  ///< Transmit interrupt priority
    uint8_t   eri_ipl;                                  ///< Error interrupt priority
    IRQn_Type rxi_irq;                                  ///< Receive interrupt IRQ number
    IRQn_Type txi_irq;                                  ///< Transmit interrupt IRQ number
    IRQn_Type eri_irq;                                  ///< Error interrupt IRQ number

    /* Configuration for SMCI Event processing */
    void (* p_callback)(smci_callback_args_t * p_args); ///< Pointer to callback function
    void const * p_context;                             ///< User defined context passed into callback function

    /* Pointer to SMCI peripheral specific configuration */
    void const * p_extend;                              ///< SMCI hardware dependent configuration
} smci_cfg_t;

/** Smart Card Interface control block.  Allocate an instance specific control block to pass into the SMCI API calls.
 * @par Implemented as
 * - smci_instance_ctrl_t
 */
typedef void smci_ctrl_t;

/** Shared Interface definition for SMCI */
typedef struct st_smci_api
{
    /** Open Smart Card Interface Mode (SMCI)
     * @par Implemented as
     * - @ref R_SCI_SMCI_Open()
     *
     * @param[in,out]  p_ctrl     Pointer to the SMCI control block. Must be declared by user. Value set here.
     * @param[in]      smci_cfg_t Pointer to SMCI configuration structure. All elements of this structure must be set by
     *                            user.
     */
    fsp_err_t (* open)(smci_ctrl_t * const p_ctrl, smci_cfg_t const * const p_cfg);

    /** Read from Smart Card device.  The read buffer is used until the read is complete.  When a transfer is complete,
     * the callback is called with event SMCI_EVENT_RX_COMPLETE.  Bytes received outside an active transfer are received
     * in the callback function with event SMCI_EVENT_RX_CHAR.
     * @par Implemented as
     * - @ref  R_SCI_SMCI_Read()
     *
     * @param[in]   p_ctrl     Pointer to the SMCI control block for the channel.
     * @param[in]   p_dest     Destination address to read data from.
     * @param[in]   bytes      Read data length.
     */
    fsp_err_t (* read)(smci_ctrl_t * const p_ctrl, uint8_t * const p_dest, uint32_t const bytes);

    /** Write to Smart Card device.  The write buffer is used until write is complete.  Do not overwrite write buffer
     * contents until the write is finished.  When the write is complete (all bytes are fully transmitted on the wire),
     * the callback called with event SMCI_EVENT_TX_COMPLETE.
     * @par Implemented as
     * - @ref  R_SCI_SMCI_Write()
     *
     * @param[in]   p_ctrl     Pointer to the SMCI control block.
     * @param[in]   p_src      Source address to write data to.
     * @param[in]   bytes      Write data length.
     */
    fsp_err_t (* write)(smci_ctrl_t * const p_ctrl, uint8_t const * const p_src, uint32_t const bytes);

    /** Change the peripheral settings based on provided transfer mode and data convention type
     * @par Implemented as
     * - @ref R_SCI_SMCI_TransferModeSet()
     *
     * @param[in]   p_ctrl          Pointer to the SMCI control block.
     * @param[in]   p_transfer_mode_params    Pointer to SMCI setting like protocol, convention, and gsm_mode
     */
    fsp_err_t (* transferModeSet)(smci_ctrl_t * const                p_ctrl,
                                  smci_transfer_mode_t const * const p_transfer_mode_params);

    /** Change baud rate.
     * @warning Calling this API aborts any in-progress transmission and disables reception until the new baud
     * settings have been applied.
     *
     * @par Implemented as
     * - @ref  R_SCI_SMCI_BaudSet()
     *
     * @param[in]   p_ctrl          Pointer to the SMCI control block.
     * @param[in]   p_baud_setting  Pointer to module specific setting for configuring baud rate.
     */
    fsp_err_t (* baudSet)(smci_ctrl_t * const p_ctrl, void const * const p_baud_setting);

    /** Get the driver specific information.
     * @par Implemented as
     * - @ref  R_SCI_SMCI_StatusGet()
     *
     * @param[in]   p_ctrl     Pointer to the SMCI control block.
     * @param[out]  p_status   State info for the driver.
     */
    fsp_err_t (* statusGet)(smci_ctrl_t * const p_ctrl, smci_status_t * const p_status);

    /**
     * Enable or disable the SMCI clock to control the start of the activation or de-activation
     * @par Implemented as
     * - @ref  R_SCI_SMCI_ClockControl()
     *
     * @param[in]   p_ctrl          Pointer to the SMCI control block.
     * @param[in]   clock_enable    True: enables clock output, False disables it
     */
    fsp_err_t (* clockControl)(smci_ctrl_t * const p_ctrl, bool clock_enable);

    /**
     * Specify callback function and optional context pointer and callback memory pointer.
     * @par Implemented as
     * - SMCI_CallbackSet()
     *
     * @param[in]   p_ctrl                   Pointer to the SMCI control block.
     * @param[in]   p_callback               Callback function
     * @param[in]   p_context                Pointer to send to callback function
     * @param[in]   p_callback_memory         Pointer to volatile memory where callback structure can be allocated.
     *                                       Callback arguments allocated here are only valid during the callback.
     */
    fsp_err_t (* callbackSet)(smci_ctrl_t * const p_api_ctrl, void (* p_callback)(smci_callback_args_t *),
                              void const * const p_context, smci_callback_args_t * const p_callback_memory);

    /** Close SMCI device.
     * @par Implemented as
     * - @ref  R_SCI_SMCI_Close()
     *
     * @param[in]   p_ctrl     Pointer to the SMCI control block.
     */
    fsp_err_t (* close)(smci_ctrl_t * const p_ctrl);
} smci_api_t;

/** This structure encompasses everything that is needed to use an instance of this interface. */
typedef struct st_smci_instance
{
    smci_ctrl_t      * p_ctrl;         ///< Pointer to the control structure for this instance
    smci_cfg_t const * p_cfg;          ///< Pointer to the configuration structure for this instance
    smci_api_t const * p_api;          ///< Pointer to the API structure for this instance
} smci_instance_t;

/** @} (end defgroup SMCI_API) */

/* Common macro for FSP header files. There is also a corresponding FSP_HEADER macro at the top of this file. */
FSP_FOOTER

#endif                                 /* R_SMCI_API_H */
