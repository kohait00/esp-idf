/*
 * SPDX-FileCopyrightText: 2022-2024 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/*
 ************************ ESP32C2 Root Clock Source ***************************
 * 1) Internal 17.5MHz RC Oscillator: RC_FAST (usually referred as FOSC or CK8M/CLK8M in TRM and reg. description)
 *
 *    This RC oscillator generates a ~17.5MHz clock signal output as the RC_FAST_CLK.
 *    The ~17.5MHz signal output is also passed into a configurable divider, which by default divides the input clock
 *    frequency by 256, to generate a RC_FAST_D256_CLK (usually referred as 8md256 or simply d256 in reg. description).
 *
 *    The exact frequency of RC_FAST_CLK can be computed in runtime through calibration on the RC_FAST_D256_CLK.
 *
 * 2) External 26/40MHz Crystal Clock: XTAL
 *
 * 3) Internal 136kHz RC Oscillator: RC_SLOW (usually referrred as RTC in TRM or reg. description)
 *
 *    This RC oscillator generates a ~136kHz clock signal output as the RC_SLOW_CLK. The exact frequency of this clock
 *    can be computed in runtime through calibration.
 *
 * 4) External Slow Clock (optional): OSC_SLOW
 *
 *    A clock signal generated by an external circuit with frequency no more than 136kHz can be connected to pin0
 *    to be the clock source for the RTC_SLOW_CLK.
 *
 *    OSC_SLOW_CLK can also be calibrated to get its exact frequency.
 */

/* With the default value of CK8M_DFREQ = 100, RC_FAST clock frequency is 17.5 MHz +/- 7% */
#define SOC_CLK_RC_FAST_FREQ_APPROX         17500000                            /*!< Approximate RC_FAST_CLK frequency in Hz */
#define SOC_CLK_RC_SLOW_FREQ_APPROX         136000                              /*!< Approximate RC_SLOW_CLK frequency in Hz */
#define SOC_CLK_RC_FAST_D256_FREQ_APPROX    (SOC_CLK_RC_FAST_FREQ_APPROX / 256) /*!< Approximate RC_FAST_D256_CLK frequency in Hz */
#define SOC_CLK_OSC_SLOW_FREQ_APPROX        32768                               /*!< Approximate OSC_SLOW_CLK (external slow clock) frequency in Hz */

// Naming convention: SOC_ROOT_CLK_{loc}_{type}_[attr]
// {loc}: EXT, INT
// {type}: XTAL, RC
// [attr] - optional: [frequency], FAST, SLOW
/**
 * @brief Root clock
 */
typedef enum {
    SOC_ROOT_CLK_INT_RC_FAST,          /*!< Internal 17.5MHz RC oscillator */
    SOC_ROOT_CLK_INT_RC_SLOW,          /*!< Internal 136kHz RC oscillator */
    SOC_ROOT_CLK_EXT_XTAL,             /*!< External 26/40MHz crystal */
    SOC_ROOT_CLK_EXT_OSC_SLOW,         /*!< External slow clock signal at pin0, only support 32.768 KHz currently */
} soc_root_clk_t;

/**
 * @brief CPU_CLK mux inputs, which are the supported clock sources for the CPU_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_CPU_CLK_SRC_XTAL = 0,              /*!< Select XTAL_CLK as CPU_CLK source */
    SOC_CPU_CLK_SRC_PLL = 1,               /*!< Select PLL_CLK as CPU_CLK source (PLL_CLK is the output of 26/40MHz crystal oscillator frequency multiplier, 480MHz) */
    SOC_CPU_CLK_SRC_RC_FAST = 2,           /*!< Select RC_FAST_CLK as CPU_CLK source */
    SOC_CPU_CLK_SRC_INVALID,               /*!< Invalid CPU_CLK source */
} soc_cpu_clk_src_t;

/**
 * @brief RTC_SLOW_CLK mux inputs, which are the supported clock sources for the RTC_SLOW_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_RTC_SLOW_CLK_SRC_RC_SLOW = 0,      /*!< Select RC_SLOW_CLK as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_OSC_SLOW = 1,     /*!< Select OSC_SLOW_CLK (external slow clock) as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_RC_FAST_D256 = 2, /*!< Select RC_FAST_D256_CLK (referred as FOSC_DIV or 8m_d256/8md256 in TRM and reg. description) as RTC_SLOW_CLK source */
    SOC_RTC_SLOW_CLK_SRC_INVALID,          /*!< Invalid RTC_SLOW_CLK source */
} soc_rtc_slow_clk_src_t;

/**
 * @brief RTC_FAST_CLK mux inputs, which are the supported clock sources for the RTC_FAST_CLK
 * @note Enum values are matched with the register field values on purpose
 */
typedef enum {
    SOC_RTC_FAST_CLK_SRC_XTAL_D2 = 0,      /*!< Select XTAL_D2_CLK (may referred as XTAL_CLK_DIV_2) as RTC_FAST_CLK source */
    SOC_RTC_FAST_CLK_SRC_XTAL_DIV = SOC_RTC_FAST_CLK_SRC_XTAL_D2, /*!< Alias name for `SOC_RTC_FAST_CLK_SRC_XTAL_D2` */
    SOC_RTC_FAST_CLK_SRC_RC_FAST = 1,      /*!< Select RC_FAST_CLK as RTC_FAST_CLK source */
    SOC_RTC_FAST_CLK_SRC_INVALID,          /*!< Invalid RTC_FAST_CLK source */
} soc_rtc_fast_clk_src_t;

/**
 * @brief Possible main XTAL frequency options on the target
 * @note Enum values equal to the frequency value in MHz
 * @note Not all frequency values listed here are supported in IDF. Please check SOC_XTAL_SUPPORT_XXX in soc_caps.h for
 *       the supported ones.
 */
typedef enum {
    SOC_XTAL_FREQ_26M = 26,                /*!< 26MHz XTAL */
    SOC_XTAL_FREQ_32M = 32,                /*!< 32MHz XTAL */
    SOC_XTAL_FREQ_40M = 40,                /*!< 40MHz XTAL */
} soc_xtal_freq_t;

// Naming convention: SOC_MOD_CLK_{[upstream]clock_name}_[attr]
// {[upstream]clock_name}: (BB)PLL etc.
// [attr] - optional: FAST, SLOW, D<divider>, F<freq>
/**
 * @brief Supported clock sources for modules (CPU, peripherals, RTC, etc.)
 *
 * @note enum starts from 1, to save 0 for special purpose
 */
typedef enum {
    // For CPU domain
    SOC_MOD_CLK_CPU = 1,                       /*!< CPU_CLK can be sourced from XTAL, PLL, or RC_FAST by configuring soc_cpu_clk_src_t */
    // For RTC domain
    SOC_MOD_CLK_RTC_FAST,                      /*!< RTC_FAST_CLK can be sourced from XTAL_D2 or RC_FAST by configuring soc_rtc_fast_clk_src_t */
    SOC_MOD_CLK_RTC_SLOW,                      /*!< RTC_SLOW_CLK can be sourced from RC_SLOW, XTAL32K, or RC_FAST_D256 by configuring soc_rtc_slow_clk_src_t */
    // For digital domain: peripherals, WIFI, BLE
    SOC_MOD_CLK_APB,                           /*!< APB_CLK is always 40MHz no matter it derives from XTAL or PLL */
    SOC_MOD_CLK_PLL_F40M,                      /*!< PLL_F40M_CLK is derived from PLL, and has a fixed frequency of 40MHz */
    SOC_MOD_CLK_PLL_F60M,                      /*!< PLL_F60M_CLK is derived from PLL, and has a fixed frequency of 60MHz */
    SOC_MOD_CLK_PLL_F80M,                      /*!< PLL_F80M_CLK is derived from PLL, and has a fixed frequency of 80MHz */
    SOC_MOD_CLK_OSC_SLOW,                      /*!< OSC_SLOW_CLK comes from an external slow clock signal, passing a clock gating to the peripherals */
    SOC_MOD_CLK_RC_FAST,                       /*!< RC_FAST_CLK comes from the internal 20MHz rc oscillator, passing a clock gating to the peripherals */
    SOC_MOD_CLK_RC_FAST_D256,                  /*!< RC_FAST_D256_CLK comes from the internal 20MHz rc oscillator, divided by 256, and passing a clock gating to the peripherals */
    SOC_MOD_CLK_XTAL,                          /*!< XTAL_CLK comes from the external 26/40MHz crystal */
    SOC_MOD_CLK_INVALID,                       /*!< Indication of the end of the available module clock sources */
} soc_module_clk_t;

//////////////////////////////////////////////////SYSTIMER///////////////////////////////////////////////////////////////

/**
 * @brief Type of SYSTIMER clock source
 */
typedef enum {
    SYSTIMER_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,       /*!< SYSTIMER source clock is XTAL */
    SYSTIMER_CLK_SRC_DEFAULT = SOC_MOD_CLK_XTAL,    /*!< SYSTIMER source clock default choice is XTAL */
} soc_periph_systimer_clk_src_t;

//////////////////////////////////////////////////GPTimer///////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of GPTimer
 *
 * The following code can be used to iterate all possible clocks:
 * @code{c}
 * soc_periph_gptimer_clk_src_t gptimer_clks[] = (soc_periph_gptimer_clk_src_t)SOC_GPTIMER_CLKS;
 * for (size_t i = 0; i< sizeof(gptimer_clks) / sizeof(gptimer_clks[0]); i++) {
 *     soc_periph_gptimer_clk_src_t clk = gptimer_clks[i];
 *     // Test GPTimer with the clock `clk`
 * }
 * @endcode
 */
#define SOC_GPTIMER_CLKS {SOC_MOD_CLK_PLL_F40M, SOC_MOD_CLK_XTAL}

/**
 * @brief Type of GPTimer clock source
 */
typedef enum {
    GPTIMER_CLK_SRC_PLL_F40M = SOC_MOD_CLK_PLL_F40M, /*!< Select PLL_F40M as the source clock */
    GPTIMER_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,         /*!< Select XTAL as the source clock */
    GPTIMER_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_F40M,  /*!< Select PLL_F40M as the default choice */
} soc_periph_gptimer_clk_src_t;

/**
 * @brief Type of Timer Group clock source, reserved for the legacy timer group driver
 */
typedef enum {
    TIMER_SRC_CLK_PLL_F40M = SOC_MOD_CLK_PLL_F40M, /*!< Timer group clock source is PLL_F40M */
    TIMER_SRC_CLK_XTAL = SOC_MOD_CLK_XTAL,         /*!< Timer group clock source is XTAL */
    TIMER_SRC_CLK_DEFAULT = SOC_MOD_CLK_PLL_F40M,  /*!< Timer group clock source default choice is PLL_F40M */
} soc_periph_tg_clk_src_legacy_t;

//////////////////////////////////////////////////Temp Sensor///////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of Temperature Sensor
 */
#define SOC_TEMP_SENSOR_CLKS {SOC_MOD_CLK_XTAL, SOC_MOD_CLK_RC_FAST}

/**
 * @brief Type of Temp Sensor clock source
 */
typedef enum {
    TEMPERATURE_SENSOR_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,       /*!< Select XTAL as the source clock */
    TEMPERATURE_SENSOR_CLK_SRC_RC_FAST = SOC_MOD_CLK_RC_FAST, /*!< Select RC_FAST as the source clock */
    TEMPERATURE_SENSOR_CLK_SRC_DEFAULT = SOC_MOD_CLK_XTAL,    /*!< Select XTAL as the default choice */
} soc_periph_temperature_sensor_clk_src_t;

///////////////////////////////////////////////////UART/////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of UART
 */
#define SOC_UART_CLKS {SOC_MOD_CLK_PLL_F40M, SOC_MOD_CLK_XTAL, SOC_MOD_CLK_RC_FAST}

/**
 * @brief Type of UART clock source, reserved for the legacy UART driver
 */
typedef enum {
    UART_SCLK_PLL_F40M = SOC_MOD_CLK_PLL_F40M, /*!< UART source clock is PLL_F40M CLK */
    UART_SCLK_RTC = SOC_MOD_CLK_RC_FAST,       /*!< UART source clock is RC_FAST */
    UART_SCLK_XTAL = SOC_MOD_CLK_XTAL,         /*!< UART source clock is XTAL */
    UART_SCLK_DEFAULT = SOC_MOD_CLK_PLL_F40M,  /*!< UART source clock default choice is PLL_F40M */
} soc_periph_uart_clk_src_legacy_t;

/////////////////////////////////////////////////I2C////////////////////////////////////////////////////////////////////


/////////////////////////////////////////////////SPI////////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of SPI
 */
#define SOC_SPI_CLKS {SOC_MOD_CLK_PLL_F40M, SOC_MOD_CLK_XTAL}

/**
 * @brief Type of SPI clock source.
 */
typedef enum {
    SPI_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_F40M,     /*!< Select PLL_40M as SPI source clock */
    SPI_CLK_SRC_PLL_F40M = SOC_MOD_CLK_PLL_F40M,     /*!< Select PLL_40M as SPI source clock */
    SPI_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,            /*!< Select XTAL as SPI source clock */
} soc_periph_spi_clk_src_t;

/**
 * @brief Array initializer for all supported clock sources of I2C
 */
#define SOC_I2C_CLKS {SOC_MOD_CLK_XTAL, SOC_MOD_CLK_RC_FAST}

/**
 * @brief Type of I2C clock source.
 */
typedef enum {
    I2C_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,            /*!< Select XTAL as the source clock */
    I2C_CLK_SRC_RC_FAST = SOC_MOD_CLK_RC_FAST,      /*!< Select RC_FAST as the source clock */
    I2C_CLK_SRC_DEFAULT = SOC_MOD_CLK_XTAL,         /*!< Select XTAL as the default clock choice */
} soc_periph_i2c_clk_src_t;

//////////////////////////////////////////////////ADC///////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of ADC digital controller
 */
#define SOC_ADC_DIGI_CLKS {SOC_MOD_CLK_XTAL, SOC_MOD_CLK_PLL_F80M}

/**
 * @brief ADC digital controller clock source
 */
typedef enum {
    ADC_DIGI_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,           /*!< Select XTAL as the source clock */
    ADC_DIGI_CLK_SRC_PLL_F80M = SOC_MOD_CLK_PLL_F80M,   /*!< Select PLL_F80M as the source clock */
    ADC_DIGI_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_F80M,    /*!< Select PLL_F80M as the default clock choice */
} soc_periph_adc_digi_clk_src_t;

//////////////////////////////////////////////////GPIO Glitch Filter////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of Glitch Filter
 */
#define SOC_GLITCH_FILTER_CLKS {SOC_MOD_CLK_APB}

/**
 * @brief Glitch filter clock source
 */
typedef enum {
    GLITCH_FILTER_CLK_SRC_APB = SOC_MOD_CLK_APB,     /*!< Select APB clock as the source clock */
    GLITCH_FILTER_CLK_SRC_DEFAULT = SOC_MOD_CLK_APB, /*!< Select APB clock as the default clock choice */
} soc_periph_glitch_filter_clk_src_t;


//////////////////////////////////////////////////MWDT/////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of MWDT
 */
#define SOC_MWDT_CLKS {SOC_MOD_CLK_XTAL, SOC_MOD_CLK_PLL_F40M}

/**
 * @brief MWDT clock source
 */
typedef enum {
    MWDT_CLK_SRC_XTAL = SOC_MOD_CLK_XTAL,           /*!< Select XTAL as the source clock */
    MWDT_CLK_SRC_PLL_F40M = SOC_MOD_CLK_PLL_F40M,     /*!< Select PLL 40 Mhz as the source clock */
    MWDT_CLK_SRC_DEFAULT = SOC_MOD_CLK_PLL_F40M,     /*!< Select PLL 40 Mhz as the default clock choice */
} soc_periph_mwdt_clk_src_t;

//////////////////////////////////////////////////LEDC/////////////////////////////////////////////////////////////////

/**
 * @brief Array initializer for all supported clock sources of LEDC
 */
#define SOC_LEDC_CLKS {SOC_MOD_CLK_XTAL, SOC_MOD_CLK_PLL_F60M, SOC_MOD_CLK_RC_FAST}

/**
 * @brief Type of LEDC clock source, reserved for the legacy LEDC driver
 */
typedef enum {
    LEDC_AUTO_CLK = 0,                              /*!< LEDC source clock will be automatically selected based on the giving resolution and duty parameter when init the timer*/
    LEDC_USE_PLL_DIV_CLK = SOC_MOD_CLK_PLL_F60M,    /*!< Select PLL_F60M as the source clock */
    LEDC_USE_RC_FAST_CLK = SOC_MOD_CLK_RC_FAST,     /*!< Select RC_FAST as the source clock */
    LEDC_USE_XTAL_CLK = SOC_MOD_CLK_XTAL,           /*!< Select XTAL as the source clock */

    LEDC_USE_RTC8M_CLK __attribute__((deprecated("please use 'LEDC_USE_RC_FAST_CLK' instead"))) = LEDC_USE_RC_FAST_CLK,   /*!< Alias of 'LEDC_USE_RC_FAST_CLK' */
} soc_periph_ledc_clk_src_legacy_t;

//////////////////////////////////////////////CLOCK OUTPUT///////////////////////////////////////////////////////////
typedef enum {
    CLKOUT_SIG_PLL      = 1,    /*!< PLL_CLK is the output of crystal oscillator frequency multiplier */
    CLKOUT_SIG_RC_SLOW  = 4,    /*!< RC slow clock, depends on the RTC_CLK_SRC configuration */
    CLKOUT_SIG_XTAL     = 5,    /*!< Main crystal oscillator clock */
    CLKOUT_SIG_PLL_F80M = 13,   /*!< From PLL, usually be 80MHz */
    CLKOUT_SIG_RC_FAST  = 14,   /*!< RC fast clock, about 8MHz */
    CLKOUT_SIG_INVALID  = 0xFF,
} soc_clkout_sig_id_t;

#ifdef __cplusplus
}
#endif
