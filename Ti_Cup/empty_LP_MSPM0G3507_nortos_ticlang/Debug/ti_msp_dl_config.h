/*
 * Copyright (c) 2023, Texas Instruments Incorporated - http://www.ti.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ============ ti_msp_dl_config.h =============
 *  Configured MSPM0 DriverLib module declarations
 *
 *  DO NOT EDIT - This file is generated for the MSPM0G350X
 *  by the SysConfig tool.
 */
#ifndef ti_msp_dl_config_h
#define ti_msp_dl_config_h

#define CONFIG_MSPM0G350X

#if defined(__ti_version__) || defined(__TI_COMPILER_VERSION__)
#define SYSCONFIG_WEAK __attribute__((weak))
#elif defined(__IAR_SYSTEMS_ICC__)
#define SYSCONFIG_WEAK __weak
#elif defined(__GNUC__)
#define SYSCONFIG_WEAK __attribute__((weak))
#endif

#include <ti/devices/msp/msp.h>
#include <ti/driverlib/driverlib.h>
#include <ti/driverlib/m0p/dl_core.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  ======== SYSCFG_DL_init ========
 *  Perform all required MSP DL initialization
 *
 *  This function should be called once at a point before any use of
 *  MSP DL.
 */


/* clang-format off */

#define POWER_STARTUP_DELAY                                                (16)


#define CPUCLK_FREQ                                                     32000000



/* Defines for PWM_0 */
#define PWM_0_INST                                                         TIMA0
#define PWM_0_INST_IRQHandler                                   TIMA0_IRQHandler
#define PWM_0_INST_INT_IRQN                                     (TIMA0_INT_IRQn)
#define PWM_0_INST_CLK_FREQ                                               125000
/* GPIO defines for channel 0 */
#define GPIO_PWM_0_C0_PORT                                                 GPIOA
#define GPIO_PWM_0_C0_PIN                                          DL_GPIO_PIN_0
#define GPIO_PWM_0_C0_IOMUX                                       (IOMUX_PINCM1)
#define GPIO_PWM_0_C0_IOMUX_FUNC                      IOMUX_PINCM1_PF_TIMA0_CCP0
#define GPIO_PWM_0_C0_IDX                                    DL_TIMER_CC_0_INDEX
/* GPIO defines for channel 1 */
#define GPIO_PWM_0_C1_PORT                                                 GPIOB
#define GPIO_PWM_0_C1_PIN                                          DL_GPIO_PIN_9
#define GPIO_PWM_0_C1_IOMUX                                      (IOMUX_PINCM26)
#define GPIO_PWM_0_C1_IOMUX_FUNC                     IOMUX_PINCM26_PF_TIMA0_CCP1
#define GPIO_PWM_0_C1_IDX                                    DL_TIMER_CC_1_INDEX
/* GPIO defines for channel 2 */
#define GPIO_PWM_0_C2_PORT                                                 GPIOA
#define GPIO_PWM_0_C2_PIN                                          DL_GPIO_PIN_7
#define GPIO_PWM_0_C2_IOMUX                                      (IOMUX_PINCM14)
#define GPIO_PWM_0_C2_IOMUX_FUNC                     IOMUX_PINCM14_PF_TIMA0_CCP2
#define GPIO_PWM_0_C2_IDX                                    DL_TIMER_CC_2_INDEX
/* GPIO defines for channel 3 */
#define GPIO_PWM_0_C3_PORT                                                 GPIOA
#define GPIO_PWM_0_C3_PIN                                         DL_GPIO_PIN_12
#define GPIO_PWM_0_C3_IOMUX                                      (IOMUX_PINCM34)
#define GPIO_PWM_0_C3_IOMUX_FUNC                     IOMUX_PINCM34_PF_TIMA0_CCP3
#define GPIO_PWM_0_C3_IDX                                    DL_TIMER_CC_3_INDEX




/* Defines for QEI_0 */
#define QEI_0_INST                                                         TIMG8
#define QEI_0_INST_IRQHandler                                   TIMG8_IRQHandler
#define QEI_0_INST_INT_IRQN                                     (TIMG8_INT_IRQn)
/* Pin configuration defines for QEI_0 PHA Pin */
#define GPIO_QEI_0_PHA_PORT                                                GPIOA
#define GPIO_QEI_0_PHA_PIN                                        DL_GPIO_PIN_26
#define GPIO_QEI_0_PHA_IOMUX                                     (IOMUX_PINCM59)
#define GPIO_QEI_0_PHA_IOMUX_FUNC                    IOMUX_PINCM59_PF_TIMG8_CCP0
/* Pin configuration defines for QEI_0 PHB Pin */
#define GPIO_QEI_0_PHB_PORT                                                GPIOA
#define GPIO_QEI_0_PHB_PIN                                        DL_GPIO_PIN_22
#define GPIO_QEI_0_PHB_IOMUX                                     (IOMUX_PINCM47)
#define GPIO_QEI_0_PHB_IOMUX_FUNC                    IOMUX_PINCM47_PF_TIMG8_CCP1


/* Defines for CAPTURE_0 */
#define CAPTURE_0_INST                                                   (TIMG7)
#define CAPTURE_0_INST_IRQHandler                               TIMG7_IRQHandler
#define CAPTURE_0_INST_INT_IRQN                                 (TIMG7_INT_IRQn)
#define CAPTURE_0_INST_LOAD_VALUE                                         (124U)
/* GPIO defines for channel 0 */
#define GPIO_CAPTURE_0_C0_PORT                                             GPIOA
#define GPIO_CAPTURE_0_C0_PIN                                     DL_GPIO_PIN_28
#define GPIO_CAPTURE_0_C0_IOMUX                                   (IOMUX_PINCM3)
#define GPIO_CAPTURE_0_C0_IOMUX_FUNC                  IOMUX_PINCM3_PF_TIMG7_CCP0

/* Defines for CAPTURE_1 */
#define CAPTURE_1_INST                                                   (TIMA1)
#define CAPTURE_1_INST_IRQHandler                               TIMA1_IRQHandler
#define CAPTURE_1_INST_INT_IRQN                                 (TIMA1_INT_IRQn)
#define CAPTURE_1_INST_LOAD_VALUE                                         (124U)
/* GPIO defines for channel 0 */
#define GPIO_CAPTURE_1_C0_PORT                                             GPIOB
#define GPIO_CAPTURE_1_C0_PIN                                      DL_GPIO_PIN_0
#define GPIO_CAPTURE_1_C0_IOMUX                                  (IOMUX_PINCM12)
#define GPIO_CAPTURE_1_C0_IOMUX_FUNC                 IOMUX_PINCM12_PF_TIMA1_CCP0





/* Defines for TIMER_0 */
#define TIMER_0_INST                                                     (TIMG6)
#define TIMER_0_INST_IRQHandler                                 TIMG6_IRQHandler
#define TIMER_0_INST_INT_IRQN                                   (TIMG6_INT_IRQn)
#define TIMER_0_INST_LOAD_VALUE                                          (1249U)



/* Defines for UART_0 */
#define UART_0_INST                                                        UART0
#define UART_0_INST_IRQHandler                                  UART0_IRQHandler
#define UART_0_INST_INT_IRQN                                      UART0_INT_IRQn
#define GPIO_UART_0_RX_PORT                                                GPIOA
#define GPIO_UART_0_TX_PORT                                                GPIOA
#define GPIO_UART_0_RX_PIN                                        DL_GPIO_PIN_11
#define GPIO_UART_0_TX_PIN                                        DL_GPIO_PIN_10
#define GPIO_UART_0_IOMUX_RX                                     (IOMUX_PINCM22)
#define GPIO_UART_0_IOMUX_TX                                     (IOMUX_PINCM21)
#define GPIO_UART_0_IOMUX_RX_FUNC                      IOMUX_PINCM22_PF_UART0_RX
#define GPIO_UART_0_IOMUX_TX_FUNC                      IOMUX_PINCM21_PF_UART0_TX
#define UART_0_BAUD_RATE                                                  (9600)
#define UART_0_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_0_FBRD_32_MHZ_9600_BAUD                                        (21)
/* Defines for UART_1 */
#define UART_1_INST                                                        UART1
#define UART_1_INST_IRQHandler                                  UART1_IRQHandler
#define UART_1_INST_INT_IRQN                                      UART1_INT_IRQn
#define GPIO_UART_1_RX_PORT                                                GPIOA
#define GPIO_UART_1_TX_PORT                                                GPIOA
#define GPIO_UART_1_RX_PIN                                        DL_GPIO_PIN_18
#define GPIO_UART_1_TX_PIN                                        DL_GPIO_PIN_17
#define GPIO_UART_1_IOMUX_RX                                     (IOMUX_PINCM40)
#define GPIO_UART_1_IOMUX_TX                                     (IOMUX_PINCM39)
#define GPIO_UART_1_IOMUX_RX_FUNC                      IOMUX_PINCM40_PF_UART1_RX
#define GPIO_UART_1_IOMUX_TX_FUNC                      IOMUX_PINCM39_PF_UART1_TX
#define UART_1_BAUD_RATE                                                  (9600)
#define UART_1_IBRD_32_MHZ_9600_BAUD                                       (208)
#define UART_1_FBRD_32_MHZ_9600_BAUD                                        (21)
/* Defines for UART_2 */
#define UART_2_INST                                                        UART3
#define UART_2_INST_IRQHandler                                  UART3_IRQHandler
#define UART_2_INST_INT_IRQN                                      UART3_INT_IRQn
#define GPIO_UART_2_RX_PORT                                                GPIOB
#define GPIO_UART_2_TX_PORT                                                GPIOB
#define GPIO_UART_2_RX_PIN                                         DL_GPIO_PIN_3
#define GPIO_UART_2_TX_PIN                                         DL_GPIO_PIN_2
#define GPIO_UART_2_IOMUX_RX                                     (IOMUX_PINCM16)
#define GPIO_UART_2_IOMUX_TX                                     (IOMUX_PINCM15)
#define GPIO_UART_2_IOMUX_RX_FUNC                      IOMUX_PINCM16_PF_UART3_RX
#define GPIO_UART_2_IOMUX_TX_FUNC                      IOMUX_PINCM15_PF_UART3_TX
#define UART_2_BAUD_RATE                                                (115200)
#define UART_2_IBRD_32_MHZ_115200_BAUD                                      (17)
#define UART_2_FBRD_32_MHZ_115200_BAUD                                      (23)





/* Port definition for Pin Group GPIO_LED */
#define GPIO_LED_PORT                                                    (GPIOB)

/* Defines for PIN_LED_1: GPIOB.1 with pinCMx 13 on package pin 48 */
#define GPIO_LED_PIN_LED_1_PIN                                   (DL_GPIO_PIN_1)
#define GPIO_LED_PIN_LED_1_IOMUX                                 (IOMUX_PINCM13)
/* Defines for PIN_LED_2: GPIOB.20 with pinCMx 48 on package pin 19 */
#define GPIO_LED_PIN_LED_2_PIN                                  (DL_GPIO_PIN_20)
#define GPIO_LED_PIN_LED_2_IOMUX                                 (IOMUX_PINCM48)
/* Defines for PIN_BEEP: GPIOB.24 with pinCMx 52 on package pin 23 */
#define GPIO_LED_PIN_BEEP_PIN                                   (DL_GPIO_PIN_24)
#define GPIO_LED_PIN_BEEP_IOMUX                                  (IOMUX_PINCM52)
/* Port definition for Pin Group GPIO_Encoder */
#define GPIO_Encoder_PORT                                                (GPIOA)

/* Defines for PIN_ENCODER_LEFT: GPIOA.1 with pinCMx 2 on package pin 34 */
#define GPIO_Encoder_PIN_ENCODER_LEFT_PIN                        (DL_GPIO_PIN_1)
#define GPIO_Encoder_PIN_ENCODER_LEFT_IOMUX                       (IOMUX_PINCM2)
/* Defines for PIN_ENCODER_RIGHT: GPIOA.2 with pinCMx 7 on package pin 42 */
#define GPIO_Encoder_PIN_ENCODER_RIGHT_PIN                       (DL_GPIO_PIN_2)
#define GPIO_Encoder_PIN_ENCODER_RIGHT_IOMUX                      (IOMUX_PINCM7)
/* Defines for PIN_LINE_1: GPIOB.27 with pinCMx 58 on package pin 29 */
#define GPIO_LINE_PIN_LINE_1_PORT                                        (GPIOB)
#define GPIO_LINE_PIN_LINE_1_PIN                                (DL_GPIO_PIN_27)
#define GPIO_LINE_PIN_LINE_1_IOMUX                               (IOMUX_PINCM58)
/* Defines for PIN_LINE_5: GPIOB.10 with pinCMx 27 on package pin 62 */
#define GPIO_LINE_PIN_LINE_5_PORT                                        (GPIOB)
#define GPIO_LINE_PIN_LINE_5_PIN                                (DL_GPIO_PIN_10)
#define GPIO_LINE_PIN_LINE_5_IOMUX                               (IOMUX_PINCM27)
/* Defines for PIN_LINE_3: GPIOB.23 with pinCMx 51 on package pin 22 */
#define GPIO_LINE_PIN_LINE_3_PORT                                        (GPIOB)
#define GPIO_LINE_PIN_LINE_3_PIN                                (DL_GPIO_PIN_23)
#define GPIO_LINE_PIN_LINE_3_IOMUX                               (IOMUX_PINCM51)
/* Defines for PIN_LINE_4: GPIOB.21 with pinCMx 49 on package pin 20 */
#define GPIO_LINE_PIN_LINE_4_PORT                                        (GPIOB)
#define GPIO_LINE_PIN_LINE_4_PIN                                (DL_GPIO_PIN_21)
#define GPIO_LINE_PIN_LINE_4_IOMUX                               (IOMUX_PINCM49)
/* Defines for PIN_LINE_2: GPIOB.26 with pinCMx 57 on package pin 28 */
#define GPIO_LINE_PIN_LINE_2_PORT                                        (GPIOB)
#define GPIO_LINE_PIN_LINE_2_PIN                                (DL_GPIO_PIN_26)
#define GPIO_LINE_PIN_LINE_2_IOMUX                               (IOMUX_PINCM57)
/* Defines for PIN_LINE_6: GPIOA.23 with pinCMx 53 on package pin 24 */
#define GPIO_LINE_PIN_LINE_6_PORT                                        (GPIOA)
#define GPIO_LINE_PIN_LINE_6_PIN                                (DL_GPIO_PIN_23)
#define GPIO_LINE_PIN_LINE_6_IOMUX                               (IOMUX_PINCM53)
/* Defines for PIN_LINE_7: GPIOA.21 with pinCMx 46 on package pin 17 */
#define GPIO_LINE_PIN_LINE_7_PORT                                        (GPIOA)
#define GPIO_LINE_PIN_LINE_7_PIN                                (DL_GPIO_PIN_21)
#define GPIO_LINE_PIN_LINE_7_IOMUX                               (IOMUX_PINCM46)
/* Defines for PIN_LINE_8: GPIOA.5 with pinCMx 10 on package pin 45 */
#define GPIO_LINE_PIN_LINE_8_PORT                                        (GPIOA)
#define GPIO_LINE_PIN_LINE_8_PIN                                 (DL_GPIO_PIN_5)
#define GPIO_LINE_PIN_LINE_8_IOMUX                               (IOMUX_PINCM10)

/* clang-format on */

void SYSCFG_DL_init(void);
void SYSCFG_DL_initPower(void);
void SYSCFG_DL_GPIO_init(void);
void SYSCFG_DL_SYSCTL_init(void);
void SYSCFG_DL_PWM_0_init(void);
void SYSCFG_DL_QEI_0_init(void);
void SYSCFG_DL_CAPTURE_0_init(void);
void SYSCFG_DL_CAPTURE_1_init(void);
void SYSCFG_DL_TIMER_0_init(void);
void SYSCFG_DL_UART_0_init(void);
void SYSCFG_DL_UART_1_init(void);
void SYSCFG_DL_UART_2_init(void);


bool SYSCFG_DL_saveConfiguration(void);
bool SYSCFG_DL_restoreConfiguration(void);

#ifdef __cplusplus
}
#endif

#endif /* ti_msp_dl_config_h */
