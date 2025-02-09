#ifndef INC_STEPPER_H_
#define INC_STEPPER_H_

#include "main.h"

struct Differential {
    GPIO_TypeDef* pPort;
    uint16_t pPin;

    GPIO_TypeDef* mPort;
    uint16_t mPin;
};

struct Stepper_Handle {
    // configuration
    struct Differential pul;
    struct Differential dir;
    struct Differential en;
    uint16_t ppr;

    // internal
    int32_t pos;

    uint8_t move;
    uint16_t div;
    uint8_t dirState;
    uint16_t counter;
    uint8_t pulState;
};

void Stepper_Init(struct Stepper_Handle* stepper);
void Stepper_Update(struct Stepper_Handle* stepper);

void Stepper_SetSpeed(struct Stepper_Handle* stepper, float speed);

void Stepper_SetEn(struct Stepper_Handle* stepper, uint8_t state);
void Stepper_SetDir(struct Stepper_Handle* stepper, uint8_t state);
void Stepper_SetPul(struct Stepper_Handle* stepper, uint8_t state);

#endif