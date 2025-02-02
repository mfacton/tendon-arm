#include "app.h"
#include "main.h"
#include "stepper.h"
#include "string.h"

extern TIM_HandleTypeDef htim1;

static struct Stepper st1 = {
    {.pPort = M1PP_GPIO_Port, .pPin = M1PP_Pin, .mPort = M1PM_GPIO_Port, .mPin = M1PM_Pin},
    {.pPort = M1DP_GPIO_Port, .pPin = M1DP_Pin, .mPort = M1DM_GPIO_Port, .mPin = M1DM_Pin},
    {.pPort = M1EP_GPIO_Port, .pPin = M1EP_Pin, .mPort = M1EM_GPIO_Port, .mPin = M1EM_Pin},
};
static struct Stepper st2 = {
    {.pPort = M2PP_GPIO_Port, .pPin = M2PP_Pin, .mPort = M2PM_GPIO_Port, .mPin = M2PM_Pin},
    {.pPort = M2DP_GPIO_Port, .pPin = M2DP_Pin, .mPort = M2DM_GPIO_Port, .mPin = M2DM_Pin},
    {.pPort = M2EP_GPIO_Port, .pPin = M2EP_Pin, .mPort = M2EM_GPIO_Port, .mPin = M2EM_Pin},
};
static struct Stepper st3 = {
    {.pPort = M3PP_GPIO_Port, .pPin = M3PP_Pin, .mPort = M3PM_GPIO_Port, .mPin = M3PM_Pin},
    {.pPort = M3DP_GPIO_Port, .pPin = M3DP_Pin, .mPort = M3DM_GPIO_Port, .mPin = M3DM_Pin},
    {.pPort = M3EP_GPIO_Port, .pPin = M3EP_Pin, .mPort = M3EM_GPIO_Port, .mPin = M3EM_Pin},
};
static struct Stepper st4 = {
    {.pPort = M4PP_GPIO_Port, .pPin = M4PP_Pin, .mPort = M4PM_GPIO_Port, .mPin = M4PM_Pin},
    {.pPort = M4DP_GPIO_Port, .pPin = M4DP_Pin, .mPort = M4DM_GPIO_Port, .mPin = M4DM_Pin},
    {.pPort = M4EP_GPIO_Port, .pPin = M4EP_Pin, .mPort = M4EM_GPIO_Port, .mPin = M4EM_Pin},
};

static struct Stepper* steppers[STEPPER_COUNT] = {&st1, &st2, &st3, &st4};

static uint16_t counters[STEPPER_COUNT] = {0, 0, 0, 0};
static uint16_t invSpeeds[STEPPER_COUNT] = {600, 600, 600, 600};

static int16_t positions[STEPPER_COUNT] = {0, 0, 0, 0};
static uint8_t directions[STEPPER_COUNT] = {0, 0, 0, 0};

void App_Init(void) {
    for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
        Stepper_SetEn(steppers[i], 1);
        Stepper_SetDir(steppers[i], 1);
        Stepper_SetPul(steppers[i], 0);
    }

    HAL_TIM_Base_Start_IT(&htim1);
}

void App_Update(void) {
}

// 100hz path update

// 100khz stepper update
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    UNUSED(htim);

    for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
        if (!counters[i]) {
            Stepper_SetPul(steppers[i], 1);
            if (directions[i]) {
                positions[i]++;
            }else{
                positions[i]--;
            }
        }else if (counters[i] == invSpeeds[i]/2){
            Stepper_SetPul(steppers[i], 0);
        }
        counters[i] = (counters[i] + 1) % invSpeeds[i];
    }
}

void App_UsbHandler(uint8_t* data, uint32_t len) {
    if (len == 2*STEPPER_COUNT) {
        int16_t speeds[4];
        memcpy(speeds, data, 8);
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            if (speeds[i] < 0) {
                speeds[i] = -speeds[i];
                directions[i] = 0;
                Stepper_SetDir(steppers[i], 0);
            }else{
                directions[i] = 1;
                Stepper_SetDir(steppers[i], 1);
            }
            invSpeeds[i] = speeds[i];
        }
    } else if (len == 1) {
        // enable byte xxxx3210
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            Stepper_SetEn(steppers[i], (data[0]>>i)&1);
        }
    }
}
