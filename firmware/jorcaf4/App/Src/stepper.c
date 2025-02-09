#include "stepper.h"

void Stepper_Init(struct Stepper_Handle* stepper) {
    stepper->pos = 0;
    stepper->move = 0;
    stepper->counter = 0;
    stepper->dirState = 1;
    stepper->pulState = 0;

    Stepper_SetEn(stepper, 0);
    Stepper_SetDir(stepper, 1);
    Stepper_SetPul(stepper, 0);
}

void Stepper_Update(struct Stepper_Handle* stepper) {
    if (!stepper->move) return;
    if (stepper->counter >= stepper->div) {
        Stepper_SetPul(stepper, 1);
        stepper->pulState = 1;
        stepper->counter = 0;
        if (stepper->dirState) {
            stepper->pos++;
        }else{
            stepper->pos--;
        }
    }else if (stepper->pulState) {
        Stepper_SetPul(stepper, 0);
        stepper->pulState = 0;
    }
    stepper->counter++;
}

void Stepper_SetSpeed(struct Stepper_Handle* stepper, float speed) {
    if (speed == 0.0) {
        stepper->move = 0;
        return;
    }
    stepper->move = 1;
    stepper->dirState = speed >= 0;
    float absSpeed = speed;
    if (!stepper->dirState) {
        absSpeed = -speed;
    }
    Stepper_SetDir(stepper, stepper->dirState);
    stepper->div = 100000.0 / (absSpeed * stepper->ppr);
}

// inverted because low is enable
void Stepper_SetEn(struct Stepper_Handle* stepper, uint8_t state) {
    HAL_GPIO_WritePin(stepper->en.pPort, stepper->en.pPin, !state);
    HAL_GPIO_WritePin(stepper->en.mPort, stepper->en.mPin, state);
}

void Stepper_SetDir(struct Stepper_Handle* stepper, uint8_t state) {
    HAL_GPIO_WritePin(stepper->dir.pPort, stepper->dir.pPin, state);
    HAL_GPIO_WritePin(stepper->dir.mPort, stepper->dir.mPin, !state);
}

void Stepper_SetPul(struct Stepper_Handle* stepper, uint8_t state) {
    HAL_GPIO_WritePin(stepper->pul.pPort, stepper->pul.pPin, state);
    HAL_GPIO_WritePin(stepper->pul.mPort, stepper->pul.mPin, !state);
}