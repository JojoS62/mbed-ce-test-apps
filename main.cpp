#include "mbed.h"
#include "modbus_slave.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);


void thread_fn() {
	DigitalOut led(LED1);

	while(1) {
		led = !led;
		ThisThread::sleep_for(500ms);
	}
}

int main()
{
    printf("mbed-ce modbus-slave\n");
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

	// Enable the Modbus Protocol Stack.
    eMBErrorCode    eStatus;

	eStatus = eMBInit(MB_RTU, SLAVE_ID, DUMMY_PORT, 115200, MB_PAR_EVEN);
	eStatus = eMBEnable();

	    // Initialise some registers
    usRegInputBuf[1] = 0x1234;
    usRegInputBuf[2] = 0x5678;
    usRegInputBuf[3] = 0x9abc;        

	while(true) 
	{
		// Call the main polling loop of the Modbus protocol stack.
		eStatus = eMBPoll();
 
       // Here we simply count the number of poll cycles.
        usRegInputBuf[0]++;
 
		//ThisThread::sleep_for(100ms);
	}


	return 0;
}