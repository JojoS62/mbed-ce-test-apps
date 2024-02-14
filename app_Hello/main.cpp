#include "mbed.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);

void thread_fn() {
	DigitalOut led(LED1);

	while(1) {
		led = !led;
		ThisThread::sleep_for(500ms);
	}
}

int main()
{
    printf("mbed-ce hello-world\n");
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

	// start a thread with blinking LED1
	thread.start(thread_fn);

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

	// main loop, print message with counter
	int counter = 0;
	while(true) 
	{
		printf("Hello world from Mbed CE! %d\n", counter);
		ThisThread::sleep_for(1s);
		counter++;
	}

	return 0;
}