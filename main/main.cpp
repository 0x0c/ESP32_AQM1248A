#include <FreeRTOS-cpp_task.h>
#include <esp_log.h>
#include <esp_system.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <string>

#include <AQM1248A.h>

#include "bitmap.h"

extern "C" {
void app_main();
}

#define PIN_NUM_MOSI GPIO_NUM_13
#define PIN_NUM_CLK GPIO_NUM_14
#define PIN_NUM_CS GPIO_NUM_15
#define PIN_NUM_RESET GPIO_NUM_25
#define PIN_NUM_RS GPIO_NUM_4

using namespace m2d::ESP32;

void app_main()
{
	static m2d::FreeRTOS::Task lcd_task("lcd task", 10, 1024 * 4, [&] {
		AQM1248A::LCD lcd(PIN_NUM_CLK, PIN_NUM_MOSI, PIN_NUM_CS, PIN_NUM_RS, PIN_NUM_RESET);
		while (1) {
			lcd.flush();
			vTaskDelay(1000 / portTICK_PERIOD_MS);

			lcd.set_data(m2d::bitmap::SwitchScience, sizeof(m2d::bitmap::SwitchScience));
			lcd.draw();
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	});
	lcd_task.run();
}
