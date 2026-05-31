/*
 * SPDX-License-Identifier: MIT
 */

#include <../../../deps/modules/lib/nn-custom-driver/include/nn-custom-driver.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define NN_DRIVER_NODE DT_NODELABEL(nn_driver0)

#define DEV_USE_NN_DRIVER

#if DT_NODE_HAS_STATUS(NN_DRIVER_NODE, okay)
static const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(nn_driver0));
#else
#warning "- DEV 0525 - No enabled nn_driver node found"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

#ifdef DEV_USE_NN_DRIVER
namespace {
	/*
	void test(void) {
		int32_t rc = 0;
		struct sensor_value val;
		rc = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("back from call to nn_driver API, status = %d", rc);
	};
	*/

	void test_channel_get(void) {
		int32_t rc = 0;
		struct sensor_value val;
		// Note following call is to custom driver, not DEVICE_API() based one:
		rc = channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val, &led);
	};

	void test_sample_fetch(void) {
		int32_t rc = 0;
		// Note following call is to custom driver, not DEVICE_API() based one:
		rc = sample_fetch(driver, SENSOR_CHAN_AMBIENT_TEMP, &led);
	};
};
#endif // DEV_USE_NN_DRIVER

int main(void)
{
    bool led_state = true;
    static uint32_t call_count = 0;
    uint32_t rc = 0;

    // Note Zephyr 3.7.0 (possibly a few earlier versions) require application
    // main() to return zero.  We do so here even on a device input/output
    // error:
    if (!gpio_is_ready_dt(&led)) goto end;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) goto end;

    if (!device_is_ready(driver)) {
        LOG_ERR("LED device not ready!  Exiting early . . .");
        goto end;
    }

    rc = (uint32_t)show_settings(driver);
    // LOG_INF("- DEV 0528 - nn driver setting for active_led = %d", rc);

    LOG_INF("- DEV 0530 - setting driver id to non-zero value . . .");
    rc = (uint32_t)set_id(driver, 555);

    LOG_INF("- DEV 0530 - showing settings again:");
    rc = (uint32_t)show_settings(driver);

    while (1) {
	call_count++;
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

	if ((call_count % 3) == 0) {
		test_channel_get();
	} else {
		test_sample_fetch();
	}

        k_msleep(CONFIG_APP_HEARTBEAT_PERIOD_MS);
    }

end:
    return 0;
}
