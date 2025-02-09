#include "app.h"
#include "main.h"
#include "stepper.h"
#include "string.h"

extern TIM_HandleTypeDef htim1;
extern TIM_HandleTypeDef htim2;

static struct Stepper_Handle st1 = {
    {.pPort = M1PP_GPIO_Port, .pPin = M1PP_Pin, .mPort = M1PM_GPIO_Port, .mPin = M1PM_Pin},
    {.pPort = M1DP_GPIO_Port, .pPin = M1DP_Pin, .mPort = M1DM_GPIO_Port, .mPin = M1DM_Pin},
    {.pPort = M1EP_GPIO_Port, .pPin = M1EP_Pin, .mPort = M1EM_GPIO_Port, .mPin = M1EM_Pin},
    .ppr = 400,
};
static struct Stepper_Handle st2 = {
    {.pPort = M2PP_GPIO_Port, .pPin = M2PP_Pin, .mPort = M2PM_GPIO_Port, .mPin = M2PM_Pin},
    {.pPort = M2DP_GPIO_Port, .pPin = M2DP_Pin, .mPort = M2DM_GPIO_Port, .mPin = M2DM_Pin},
    {.pPort = M2EP_GPIO_Port, .pPin = M2EP_Pin, .mPort = M2EM_GPIO_Port, .mPin = M2EM_Pin},
    .ppr = 400,
};

static struct Stepper_Handle* steppers[STEPPER_COUNT] = {&st1, &st2};

static const float maxSpeed = 5.0;
static const float accel = maxSpeed/2.0;

static float position = 10.0;
static float lastSpeed = 0;

//- cw first then ccw
static float target = 10.0;

static float factor = 1;
static uint8_t which = 0;

void App_Init(void) {
    for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
        Stepper_Init(steppers[i]);
    }

    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim2);

    Stepper_SetEn(steppers[0], 1);
    Stepper_SetEn(steppers[1], 1);
}

void App_Update(void) {
    if (!HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
        while (HAL_GPIO_ReadPin(BTN_GPIO_Port, BTN_Pin)) {
            HAL_Delay(10);
        }
        
        if (factor > 0) {
            which = !which;
        }
        position = 0;
        factor = -factor;
        HAL_Delay(200);
    }

    const float DT = 0.01;

    float speed;
    float deltaspeed = (float)accel * DT;
	if (target - position <= lastSpeed*lastSpeed/2.0/accel) {
		speed = lastSpeed - deltaspeed;
		if (speed < 0) {
			speed = 0;
		}
	} else {
		speed = lastSpeed + deltaspeed;
		if (speed > maxSpeed) {
			speed = maxSpeed;
		}
	}

	position += DT*(speed+lastSpeed)/2.0;
	lastSpeed = speed;

    Stepper_SetSpeed(steppers[which], speed*factor);

    HAL_Delay(10);
}

// 100khz stepper update
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
    if (htim == &htim2) {
        HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
    }else{
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            Stepper_Update(steppers[i]);
        }
    }
}

void App_UsbHandler(uint8_t* data, uint32_t len) {
    if (len == 2*STEPPER_COUNT) {
        float speeds[STEPPER_COUNT];
        memcpy(speeds, data, 4*STEPPER_COUNT);
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            Stepper_SetSpeed(steppers[i], speeds[i]);
        }
    } else if (len == 1) {
        // enable byte xxxx3210
        for (uint8_t i = 0; i < STEPPER_COUNT; i++) {
            Stepper_SetEn(steppers[i], (data[0]>>i)&1);
        }
    }
}
