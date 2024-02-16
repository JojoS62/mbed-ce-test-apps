#include "mbed.h"
#include "Adafruit_Fingerprint.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);
BufferedSerial	comR503(PC_6, PC_7, 57600);
Adafruit_Fingerprint finger(&comR503);

int main()
{
    printf("mbed-ce Test Fingerprintsensor R503\n");
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

	// start a thread with queue dispatcher
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

	// add interrupt callback for button1, write messages to serial
	user_button.rise( []() {
		queue.call(printf, "ping\n");
		queue.call_in(2s, printf, "pong\n");
	});

	// add a cyclic function call to queue
	queue.call_every(200ms, []() {
		led2 = !led2;
	});

	finger.begin(57600);
	finger.getParameters();

	printf("Status  : 0x%0x\n", finger.status_reg);
	printf("Sys ID  : 0x%0x\n",finger.system_id);
	printf("Capacity: %0d\n", finger.capacity);
    printf("Security level: %d\n", finger.security_level);
    printf("Device address: 0x%0x\n", finger.device_addr);
    printf("Packet len: %d\n", finger.packet_len);
    printf("Baud rate: %d\n", finger.baud_rate);

	// main loop, print message with counter
	int counter = 0;
	while(true) 
	{
		counter++;

		// LED fully on
		finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
		ThisThread::sleep_for(250ms);
		finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
		ThisThread::sleep_for(250ms);
		finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_PURPLE);
		ThisThread::sleep_for(250ms);

		// flash red LED
		finger.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 10);
		ThisThread::sleep_for(2s);
		// Breathe blue LED till we say to stop
		finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_BLUE);
		ThisThread::sleep_for(3s);
		finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_ON, 200, FINGERPRINT_LED_PURPLE);
		ThisThread::sleep_for(2s);
		finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 200, FINGERPRINT_LED_PURPLE);
		ThisThread::sleep_for(2s);

	}

	return 0;
}