#ifndef INC_APP_H_
#define INC_APP_H_

#include "main.h"

struct Differential {
    GPIO_TypeDef* pPort;
    uint16_t pPin;

    GPIO_TypeDef* mPort;
    uint16_t mPin;
};

struct Stepper {
    struct Differential pul;
    struct Differential dir;
    struct Differential en;
};

void Stepper_SetEn(struct Stepper* motor, uint8_t state);
void Stepper_SetDir(struct Stepper* motor, uint8_t state);
void Stepper_SetPul(struct Stepper* motor, uint8_t state);

#endif