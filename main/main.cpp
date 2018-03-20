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

#define PIN_NUM_HSPI_MOSI GPIO_NUM_13
#define PIN_NUM_HSPI_CLK GPIO_NUM_14
#define PIN_NUM_HSPI_CS GPIO_NUM_15
#define PIN_NUM_HSPI_RESET GPIO_NUM_25
#define PIN_NUM_HSPI_RS GPIO_NUM_4

#define PIN_NUM_VSPI_MOSI GPIO_NUM_23
#define PIN_NUM_VSPI_CLK GPIO_NUM_18
#define PIN_NUM_VSPI_CS GPIO_NUM_5
#define PIN_NUM_VSPI_RESET GPIO_NUM_0
#define PIN_NUM_VSPI_RS GPIO_NUM_2

using namespace m2d::ESP32;

void app_main()
{
	static m2d::FreeRTOS::Task lcd_task("lcd task", 10, 1024 * 4, [] {
		AQM1248A::LCD lcd(PIN_NUM_HSPI_CLK, PIN_NUM_HSPI_MOSI, PIN_NUM_HSPI_CS, PIN_NUM_HSPI_RS, PIN_NUM_HSPI_RESET);
		lcd.flush();
		while (1) {
			ESP_LOGI("lcd1", "flush");
			lcd.flush();
			vTaskDelay(1200 / portTICK_PERIOD_MS);

			ESP_LOGI("lcd1", "draw");
			lcd.set_data(m2d::bitmap::SwitchScience, sizeof(m2d::bitmap::SwitchScience));
			lcd.draw();
			vTaskDelay(1200 / portTICK_PERIOD_MS);
		}
	});
	lcd_task.run();

	static m2d::FreeRTOS::Task lcd_task2("lcd task2", 10, 1024 * 4, [] {
		AQM1248A::LCD lcd(PIN_NUM_VSPI_CLK, PIN_NUM_VSPI_MOSI, PIN_NUM_VSPI_CS, PIN_NUM_VSPI_RS, PIN_NUM_VSPI_RESET, VSPI_HOST);
		lcd.flush();
		while (1) {
			ESP_LOGI("lcd2", "draw");
			lcd.set_data(m2d::bitmap::SwitchScience, sizeof(m2d::bitmap::SwitchScience));
			lcd.draw();
			vTaskDelay(1000 / portTICK_PERIOD_MS);

			ESP_LOGI("lcd2", "flush");
			lcd.flush();
			vTaskDelay(1000 / portTICK_PERIOD_MS);
		}
	});
	lcd_task2.run();
}
