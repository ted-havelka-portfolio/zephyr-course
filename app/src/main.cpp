// #include <nn-driver.h>

#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/logging/log.h>

#define NN_DRIVER_NODE DT_NODELABEL(nn_driver0)

#if DT_NODE_HAS_STATUS(NN_DRIVER_NODE, okay)
static const struct device *driver = DEVICE_DT_GET(DT_NODELABEL(nn_driver0));
#else
#warning "- DEV 0525 - No enabled nn_driver node found"
#endif

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace {
	void test(void) {
		int32_t rc = 0;
		struct sensor_value val;
		rc = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
		LOG_INF("back from call to nn_driver API, status = %d", rc);
	};

	void test_channel_get(void) {
		int32_t rc = 0;
		struct sensor_value val;
		rc = sensor_channel_get(driver, SENSOR_CHAN_AMBIENT_TEMP, &val);
	};

	void test_sample_fetch(void) {
		int32_t rc = 0;
		// TODO [ ] Determine why the build process says
		// sensor_sample_fetch() takes only one argument while its
		// prototype in sensors.h and our nn-driver.c file has two
		// parameters.  Never seen this before, but one parameter here
		// in application somehow works!
		//
		// rc = sensor_sample_fetch(driver, SENSOR_CHAN_AMBIENT_TEMP);
		rc = sensor_sample_fetch(driver);
	};
};

int main(void)
{
    bool led_state = true;
    static uint32_t call_count = 0;

    // Note Zephyr 3.7.0 (possibly a few earlier versions) require application
    // main() to return zero.  We do so here even on a device input/output
    // error:
    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    test();

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
    return 0;
}
