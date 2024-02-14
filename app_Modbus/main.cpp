#include "mbed.h"
#include "modbus_slave.h"

Thread thread;
Thread thread_events;
EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);
DigitalOut led3(LED3);


int main()
{
    printf("mbed-ce modbus-slave\n");
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);


	// start a thread with queue dispatcher
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

	// Enable the Modbus Protocol Stack.
    eMBErrorCode    eStatus;

	eStatus = eMBInit(MB_RTU, SLAVE_ID, DUMMY_PORT, 115200, MB_PAR_EVEN);
	eStatus = eMBEnable();

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