#include "mbed.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);

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


	thread.start(thread_fn);
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

	user_button.rise( []() {
		queue.call(printf, "ping\n");
		queue.call_in(2s, printf, "pong\n");
	});

	int counter = 0;
	while(true) 
	{
		printf("Hello world from Mbed CE! %d\n", counter);
		ThisThread::sleep_for(1s);
		counter++;
	}

	return 0;
}