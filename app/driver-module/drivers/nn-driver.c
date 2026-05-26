/**
 * Iomico 2026 Zephyr Course sample driver source file
 */

#define DT_DRV_COMPAT nn_driver

#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/pm/device.h>

#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(nn_driver, LOG_LEVEL_INF);

static int nn_driver_channel_get(const struct device *dev,
                                 enum sensor_channel chan,
                                 struct sensor_value *val)
{
	LOG_INF("Hello from nn_driver_channel_get()");
	return 0;
}

static DEVICE_API(sensor, api_nn_driver) = {
	.channel_get = nn_driver_channel_get,
	.sample_fetch = NULL,
};

static int init(const struct device *dev)
{
	LOG_INF("nn_driver init() function called");
	return 0;
}

DEVICE_DT_INST_DEFINE(0, init, NULL, NULL, NULL, POST_KERNEL, 80, &api_nn_driver);
