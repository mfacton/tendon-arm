#include "app.h"
#include "main.h"
#include "string.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

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

static struct Stepper* steppers[STEPPER_COUNT] = {&st0, &st1, &st2, &st3};

static enum State {
    IDLE = 0u,
    READY = 1u,
    MOVING = 2u,
};

static struct Trajectory {
    uint8_t frame;
    int16_t targets[STEPPER_COUNT];
};

// frame and que
static enum State state = IDLE;
static uint8_t currentFrame = 0;
static uint16_t frameStep = 0;

// que also acts as watchdog
static uint8_t queStart = 0;
static uint8_t queSize = 0;
static struct Trajectory trajectoryQue[QUE_SIZE];

static int16_t lastJoints[4] = {0};
static int16_t currentJoints[4] = {0};
static int16_t futureJoints[4] = {0};

void App_Init(void) {
    for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
        Motor_SetEn(steppers[i], 0);
        Motor_SetDir(steppers[i], 0);
    }

    HAL_TIM_Base_Start_IT(&htim1);
}

void App_Update(void) {
}

// 100khz stepper update
// 100hz path update
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    UNUSED(htim);
    if (state != MOVING) return;

    if (frameStep == 0) {
        if (!queSize) {
            // que empty
            state = IDLE;
            HAL_GPIO_Write(LED_GPIO_Port, LED_Pin, 0);
        }
        memcpy(lastJoints, currentJoints, 8);
        memcpy(futureJoints, trajectoryQue[queStart], 8);
        queStart = (queStart + 1) % QUE_SIZE;
        queSize--;
    }

    for (uint8_t i = 0; i < 4; i++) {
        
    }

    frameStep = (frameStep + 1) % 1000;
    // if (state == MOVING) {

    // }
    // Motor_SetPul(&m4, state&1);
    // if (!(state&((1<<pps)-1))) {
    //     Motor_SetDir(&m4, (state&(1<<pps))>>1);
    // }
    // state++;
}

void App_UsbHandler(uint8_t* data, uint32_t len) {
    if (len == 1 + 2*STEPPER_COUNT) {
        // trajectory received
        if (state == READY) {
            state = MOVING;
        }
        if (state == MOVING && queSize < QUE_SIZE) {
            const uint8_t index = (queStart + queSize) % QUE_SIZE;
            memcpy(&trajectoryQue[index], data, 1 + 2*STEPPER_COUNT);
            queSize++;
        }
    } else if (len == 0) { // start message
        state = READY;
        currentFrame = 0;
        queSize = 0;
        queStart = 0;
        frameStep = 0;
        HAL_GPIO_Write(LED_GPIO_Port, LED_Pin, 1);
    } else if (len == 1) {
        // enable byte xxxx3210
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            Stepper_SetEn(steppers[i], (data[0]>>i)&1);
        }
    }
}
