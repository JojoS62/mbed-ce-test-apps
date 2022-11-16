#include "mbed.h"

DigitalOut led(LED1);

int main()
{
	thread.start(thread_fn);
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

	user_button.rise( []() {
		queue.call(printf, "ping\n");
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