/**
 * Iomico 2026 Zephyr Course sample driver source file
 */

#define DT_DRV_COMPAT nn_driver

#include "../include/nn-driver/nn-driver-private.h"

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/pm/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nn_driver, LOG_LEVEL_INF);

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(DT_ALIAS(driver_led1), gpios);

static int nn_driver_channel_get(const struct device *dev,
                                 enum sensor_channel chan,
                                 struct sensor_value *val)
{
	LOG_INF("Hello from nn_driver_channel_get()");
	int rc = gpio_pin_set_dt(&led, 1);
	return rc;
}

static int nn_driver_sample_fetch(const struct device *dev,
                                  enum sensor_channel chan)
{
	int rc = gpio_pin_set_dt(&led, 0);
	return rc;
}

static DEVICE_API(sensor, api_nn_driver) = {
	.channel_get = nn_driver_channel_get,
	.sample_fetch = nn_driver_sample_fetch,
};

static int init(const struct device *dev)
{
	LOG_INF("nn_driver init() function called");

	if (!gpio_is_ready_dt(&led)) {
		return -EIO;
	}

	if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
		return -EIO;
	}

	return 0;
}

// DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_nn_driver);

#define NN_DEVICE_DATA(inst) \
	static struct nn_device_data nn_data_##inst = { 0 };

#define NN_DEVICE_INIT(inst)                                            \
	NN_DEVICE_DATA(inst)                                            \
	DEVICE_DT_INST_DEFINE(                                          \
			0,               /* device instance          */ \
			init,            /* driver init function     */ \
			NULL,            /* power management context */ \
			&nn_data_##inst, /* device data              */ \
			NULL,            /* device config            */ \
			POST_KERNEL,     /* initialization level     */ \
			80,              /* initialization priority  */ \
			&api_nn_driver   /* driver API               */ \
		 );

#define NN_DEFINE(inst) \
	NN_DEVICE_INIT(inst)

DT_INST_FOREACH_STATUS_OKAY(NN_DEFINE)
