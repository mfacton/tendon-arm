#include "stepper.h"

void Stepper_SetEn(struct Stepper* motor, uint8_t state) {
    HAL_GPIO_WritePin(motor->en.pPort, motor->en.pPin, !state);
    HAL_GPIO_WritePin(motor->en.mPort, motor->en.mPin, state);
}
void Stepper_SetDir(struct Stepper* motor, uint8_t state) {
    HAL_GPIO_WritePin(motor->dir.pPort, motor->dir.pPin, !state);
    HAL_GPIO_WritePin(motor->dir.mPort, motor->dir.mPin, state);
}
void Stepper_SetPul(struct Stepper* motor, uint8_t state) {
    HAL_GPIO_WritePin(motor->pul.pPort, motor->pul.pPin, !state);
    HAL_GPIO_WritePin(motor->pul.mPort, motor->pul.mPin, state);
}