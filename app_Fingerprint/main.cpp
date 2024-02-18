#include "mbed.h"
#include "Adafruit_Fingerprint.h"
#include "storage.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);
BufferedSerial	comR503(PC_6, PC_7, 57600);

Adafruit_Fingerprint finger(&comR503);
InterruptIn wakeupFP(PA_0);

void getImage() {
	uint8_t fp_result = finger.getImage();
	fp_result = finger.uploadImage();
}

int main()
{
    printf("Mbed-CE Test Fingerprintsensor R503\n");
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    print_dir(&fs, "/");
    printf("\n"); 

	// start a thread with queue dispatcher
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));


	user_button.fall([]() {
		queue.call(&getImage);
	});

#if 0
	// fingerprint detect action
	wakeupFP.fall( []() {
		queue.call(&finger, static_cast<uint8_t(Adafruit_Fingerprint::*)(uint8_t, uint8_t, uint8_t, uint8_t)>(&Adafruit_Fingerprint::LEDcontrol), 
			(uint8_t)FINGERPRINT_LED_ON, (uint8_t)0, (uint8_t)FINGERPRINT_LED_BLUE, (uint8_t)0);
	});


	// fingerprint release action
	wakeupFP.rise( []() {
		queue.call(&finger, static_cast<uint8_t(Adafruit_Fingerprint::*)(uint8_t, uint8_t, uint8_t, uint8_t)>(&Adafruit_Fingerprint::LEDcontrol), 
			(uint8_t)FINGERPRINT_LED_OFF, (uint8_t)0, (uint8_t)FINGERPRINT_LED_BLUE, (uint8_t)0);
	});
#endif

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
    printf("Device address: %ld\n", finger.device_addr);
    printf("Packet len: %d\n", finger.packet_len);
    printf("Baud rate: %d\n", finger.baud_rate);

	// main loop, print message with counter
	int counter = 0;
	while(true) 
	{
		counter++;

		// // LED fully on
		// finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
		// ThisThread::sleep_for(250ms);
		// finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
		// ThisThread::sleep_for(250ms);
		// finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_PURPLE);
		// ThisThread::sleep_for(250ms);

		// // flash red LED
		// finger.LEDcontrol(FINGERPRINT_LED_FLASHING, 25, FINGERPRINT_LED_RED, 10);
		// ThisThread::sleep_for(2s);
		// // Breathe blue LED till we say to stop
		// finger.LEDcontrol(FINGERPRINT_LED_BREATHING, 100, FINGERPRINT_LED_BLUE);
		// ThisThread::sleep_for(3s);
		// finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_ON, 200, FINGERPRINT_LED_PURPLE);
		// ThisThread::sleep_for(2s);
		// finger.LEDcontrol(FINGERPRINT_LED_GRADUAL_OFF, 200, FINGERPRINT_LED_PURPLE);
		ThisThread::sleep_for(2s);

	}

	return 0;
}