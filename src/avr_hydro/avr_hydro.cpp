// Do not remove the include below
#include "avr_hydro.h"
//#include <Wire.h>
#include "DHT.h"
#include <avr/wdt.h>
#include <ModbusRtu.h>

#define SLAVE_ID   1
#define LIGHTPIN A7     // what pin we're connected to
#define DHTPIN 4     // what pin we're connected to
#define LEDPIN 13     // what pin we're connected to
#define DHTTYPE DHT11   // DHT 11

int16_t valT = NAN;
int16_t valH = NAN;
int16_t valLight = NAN;
int16_t valUptime = NAN;

DHT dht(DHTPIN, DHTTYPE);

Modbus slave(SLAVE_ID, 0, 0); // this is slave ID and RS-232 or USB-FTDI

// data array for modbus network sharing
uint16_t au16data[20];
int8_t state = 0;

// Определяем количество значений в массиве.
// Чем больше это количество, тем более сглаженным будет результат,
// и тем больше будет задержка между входными и выходными данными.
// Использование константы вместо переменной позволяет задать размер для массива.
const int numReadings = 20;
unsigned long int dht_time = 0;

int readings[numReadings];  // данные, считанные с входного аналогового контакта
int index = 0;       // индекс для значения, которое считывается в данный момент
int total = 0;                  // суммарное значение
int average = 0;                // среднее значение

void setup() {


	// включаем вачдог
	wdt_enable(WDTO_8S);

	// выставляем все считываемые значения на ноль:
	for (int thisReading = 0; thisReading < numReadings; thisReading++)
		readings[thisReading] = 0;

	pinMode(LEDPIN, OUTPUT);
	digitalWrite(LEDPIN, LOW);

	// start communication
	slave.begin(19200);
	digitalWrite(LEDPIN, LOW);

}

/**
 *  Link between the Arduino pins and the Modbus array
 */
void io_poll() {

	digitalWrite( LEDPIN, bitRead(au16data[9], 1));

	au16data[0] = slave.getInCnt();
	au16data[1] = slave.getOutCnt();
	au16data[2] = slave.getErrCnt();
	au16data[3] = valT;
	au16data[4] = valH;
	au16data[5] = valLight;
	au16data[6] = valUptime;
}

void loop() {


	int sensorValue = analogRead(LIGHTPIN);
	sensorValue = 100 - map(sensorValue, 0, 585, 0, 99);
	// 0 - макс яркость, 585 - мин яркость

	// берем последнее значение...
	total = total - readings[index];
	// ...которое было считано от сенсора:
	readings[index] = sensorValue;
	// добавляем его к общей сумме:
	total = total + readings[index];
	// продвигаемся к следующему значению в массиве:
	index = index + 1;

	// если мы в конце массива...
	if (index >= numReadings)
		// ...возвращаемся к началу:
		index = 0;

	// вычисляем среднее значение:
	average = total / numReadings;

	valLight = average;

	// Опрос раз в 10 сек (дорогая операция)
	if (dht_time == 0 || dht_time < millis() - 10000) {
		float h = dht.readHumidity();
		float t = dht.readTemperature();

		if (!isnan(t)) {
			valT = (int) t;
		}

		if (!isnan(h)) {
			valH = (int) h;
		}
		dht_time = millis();
	}

	valUptime = millis() / 1000;

	// poll messages
	state = slave.poll(au16data, 20);

	// link the Arduino pins to the Modbus array
	io_poll();


	// сброс вачдога
	wdt_reset();

}

