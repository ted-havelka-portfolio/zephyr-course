#ifndef NN_DRIVER_PRIVATE_H
#define NN_DRIVER_PRIVATE_H

#include <stdint.h>

enum nn_driver_led {
	NN_DRIVER_LED_NONE,
	NN_DRIVER_LED_1,
	NN_DRIVER_LED_2,
	NN_DRIVER_LED_BOTH,
};

struct nn_device_data {
	uint32_t active_led;
	uint32_t instance;
};

#endif // NN_DRIVER_PRIVATE_H
