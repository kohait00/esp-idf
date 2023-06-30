/*
 * SPDX-FileCopyrightText: 2015-2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "soc/sdmmc_periph.h"
// ESP32P4-TODO: need new iomux and sig map
const sdmmc_slot_info_t sdmmc_slot_info[SOC_SDMMC_NUM_SLOTS] = {
    {
        .width = 8,
        .card_detect = 0,
        .write_protect = 0,
        .card_int = 0,
    },
    {
        .width = 4,
        .card_detect = 0,
        .write_protect = 0,
        .card_int = 0,
    }
};

const sdmmc_slot_io_info_t sdmmc_slot_gpio_num[SOC_SDMMC_NUM_SLOTS] = {
    {
        .clk = SDMMC_SLOT0_IOMUX_PIN_NUM_CLK,
        .cmd = SDMMC_SLOT0_IOMUX_PIN_NUM_CMD,
        .d0 = SDMMC_SLOT0_IOMUX_PIN_NUM_D0,
        .d1 = SDMMC_SLOT0_IOMUX_PIN_NUM_D1,
        .d2 = SDMMC_SLOT0_IOMUX_PIN_NUM_D2,
        .d3 = SDMMC_SLOT0_IOMUX_PIN_NUM_D3,
        .d4 = SDMMC_SLOT0_IOMUX_PIN_NUM_D4,
        .d5 = SDMMC_SLOT0_IOMUX_PIN_NUM_D5,
        .d6 = SDMMC_SLOT0_IOMUX_PIN_NUM_D6,
        .d7 = SDMMC_SLOT0_IOMUX_PIN_NUM_D7,
    },
    {
        .clk = SDMMC_SLOT1_IOMUX_PIN_NUM_CLK,
        .cmd = SDMMC_SLOT1_IOMUX_PIN_NUM_CMD,
        .d0 = SDMMC_SLOT1_IOMUX_PIN_NUM_D0,
        .d1 = SDMMC_SLOT1_IOMUX_PIN_NUM_D1,
        .d2 = SDMMC_SLOT1_IOMUX_PIN_NUM_D2,
        .d3 = SDMMC_SLOT1_IOMUX_PIN_NUM_D3,
    }
};

const sdmmc_slot_io_info_t sdmmc_slot_gpio_sig[SOC_SDMMC_NUM_SLOTS] = {
    {

    },
    {
        .clk = SD_CARD_CCLK_2_PAD_OUT_IDX,
        .cmd = SD_CARD_CCMD_2_PAD_OUT_IDX,
        .d0 = SD_CARD_CDATA0_2_PAD_OUT_IDX,
        .d1 = SD_CARD_CDATA1_2_PAD_OUT_IDX,
        .d2 = SD_CARD_CDATA2_2_PAD_OUT_IDX,
        .d3 = SD_CARD_CDATA3_2_PAD_OUT_IDX,
        .d4 = SD_CARD_CDATA4_2_PAD_OUT_IDX,
        .d5 = SD_CARD_CDATA5_2_PAD_OUT_IDX,
        .d6 = SD_CARD_CDATA6_2_PAD_OUT_IDX,
        .d7 = SD_CARD_CDATA7_2_PAD_OUT_IDX,
    },
};
