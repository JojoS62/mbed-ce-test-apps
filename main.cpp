#include "mbed.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);

void thread_fn(){
	DigitalOut led(LED1);

	while(1){
		led = !led;
		ThisThread::sleep_for(50ms);
	}

}

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