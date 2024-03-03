#include "mbed.h"
#include "Adafruit_Fingerprint.h"
#include "storage.h"
#include "network.h"
#include "HttpServer.h"
#include "HttpResponseBuilder.h"
#include "WebsocketHandlers.h"
#include "HTTPHandlers.h" 
#include "threadTFTPServer.h"

#define USE_HTTPSERVER

Thread thread_events(osPriorityAboveNormal1, 4*1024UL, nullptr, "eventThread");
ThreadTFTPServer  threadTFTPpServer; 
HttpServer *server;

EventQueue queue;
InterruptIn user_button(BUTTON1);
DigitalOut led2(LED2);
BufferedSerial	comR503(PC_6, PC_7, 57600);

Adafruit_Fingerprint finger(&comR503);
InterruptIn wakeupFP(PA_0);

// bitmap stuff
#pragma pack(push, 2)
typedef struct tagBITMAPFILEHEADER {
  uint16_t  bfType;
  uint32_t bfSize;
  uint16_t  bfReserved1;
  uint16_t  bfReserved2;
  uint32_t bfOffBits;
} BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  uint32_t biSize;
  int32_t  biWidth;
  int32_t  biHeight;
  uint16_t  biPlanes;
  uint16_t  biBitCount;
  uint32_t biCompression;
  uint32_t biSizeImage;
  int32_t  biXPelsPerMeter;
  int32_t  biYPelsPerMeter;
  uint32_t biClrUsed;
  uint32_t biClrImportant;
} BITMAPINFOHEADER;
#pragma pack(pop)

BITMAPFILEHEADER bmpHeader;
BITMAPINFOHEADER bmpInfo;

uint32_t colorTab[256];
uint8_t imageBuffer[18432];
uint8_t lineBuffer[192];


void getImage() {
	finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_BLUE);
	server->wsSendTextAll("/ws/", "{ \"finger\" : \"on sensor\" }");
	ThisThread::sleep_for(50ms);

	uint8_t fp_result = finger.getImage();
	printf("genImage result: 0x%0x\n", fp_result);

	if (fp_result != FINGERPRINT_OK) {
		finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_RED);
		server->wsSendTextAll("/ws/", "{ \"finger\" : \"detect failed\" }");

		queue.call_in(1s, &finger, static_cast<uint8_t(Adafruit_Fingerprint::*)(uint8_t, uint8_t, uint8_t, uint8_t)>(&Adafruit_Fingerprint::LEDcontrol), 
			(uint8_t)FINGERPRINT_LED_OFF, (uint8_t)0, (uint8_t)FINGERPRINT_LED_RED, (uint8_t)0);
		return;
	}	

	finger.LEDcontrol(FINGERPRINT_LED_ON, 0, FINGERPRINT_LED_GREEN);
	server->wsSendTextAll("/ws/", "{ \"finger\" : \"detected, uploading img\" }");


	fp_result = finger.uploadImage(imageBuffer, sizeof(imageBuffer));
	printf("uploadImage result: 0x%0x\n block count: %d\n", fp_result, finger.packetCount);
	server->wsSendTextAll("/ws/", "{ \"finger\" : \"image read\" }");


	bmpHeader.bfType = 0x4d42; // magic number "BM"
	bmpHeader.bfOffBits = sizeof(bmpHeader) + sizeof(bmpInfo) + sizeof(colorTab);
	bmpHeader.bfSize = 0; // bmpHeader.bfOffBits + 192*192;

	bmpInfo.biSize = sizeof(bmpInfo);
	bmpInfo.biWidth = 192;
	bmpInfo.biHeight = 192;
	bmpInfo.biPlanes = 1;
	bmpInfo.biBitCount = 8;
	bmpInfo.biCompression = 0;
	bmpInfo.biSizeImage = 192 * 192 * 1;
	bmpInfo.biXPelsPerMeter = 20000;		// 508 DPI
	bmpInfo.biYPelsPerMeter = 20000;		// 508 DPI
	bmpInfo.biClrUsed = 0;					// max. no. of colors
	bmpInfo.biClrImportant = 0;				// all colors used


	FILE *imageFile = fopen("/sda/R503-Image.bmp", "w+b");
	if (imageFile == nullptr) {
		finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_GREEN);
		server->wsSendTextAll("/ws/", "{ \"finger\" : \"image file open failed\" }");
		return;
	}

	// headers
	fwrite(&bmpHeader, sizeof(bmpHeader), 1, imageFile);
	fwrite(&bmpInfo, sizeof(bmpInfo), 1, imageFile);

	// colortable
	uint32_t colorItem = 0x0ff000000UL;
	for(int i=0; i<256; i++) {
		colorTab[i] = colorItem;
		colorItem += 0x00010101UL;
	}
	fwrite(colorTab, sizeof(colorTab), 1, imageFile);

	// image data 4 Bit -> 8 Bit
	for(int y=0; y < 192; y++) {
		for(int x=0; x < 192/2; x++) {
			uint8_t val1 = (imageBuffer[x + y*96] >> 4) * 16;
			uint8_t val2 = (imageBuffer[x + y*96] & 0xf) * 16;
			lineBuffer[2*x] = val1;
			lineBuffer[2*x + 1] = val2;
		}
		fwrite(lineBuffer, sizeof(lineBuffer), 1, imageFile);
	}

	fclose(imageFile);

	finger.LEDcontrol(FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_GREEN);
	server->wsSendTextAll("/ws/", "{ \"finger\" : \"image saved\" }");
}

int main()
{
    printf("Mbed-CE Test Fingerprintsensor R503\n");
    printf("Hello from "  MBED_STRINGIFY(TARGET_NAME) "\n");
    printf("Mbed OS version: %d.%d.%d\n\n", MBED_MAJOR_VERSION, MBED_MINOR_VERSION, MBED_PATCH_VERSION);

    // print_dir(&fs, "/");
    // printf("\n"); 

	// start a thread with queue dispatcher
	thread_events.start(callback(&queue, &EventQueue::dispatch_forever));

	// add a cyclic function call to queue
	queue.call_every(200ms, []() {
		led2 = !led2;
	});

    
#ifdef USE_HTTPSERVER	
	nsapi_error_t connect_status =  network_init();
	if (connect_status != NSAPI_ERROR_OK) {
		while(1) {
			led2 = !led2;
			ThisThread::sleep_for(50ms);
		}
	}

    server = new HttpServer(network, 6, 2);               // max 3 threads, 2 websockets

    server->addStandardHeader("Server", "JojoS_Mbed_Server");
    server->addStandardHeader("DNT", "1");

    server->setHTTPHandler("/", &request_handler);
    server->setHTTPHandler("/stats/", &request_handler_getStatus);
    
    server->setWSHandler("/ws/", WSHandler::createHandler);

    nsapi_error_t res = server->start(8080);

    if (res == NSAPI_ERROR_OK) {
        SocketAddress socketAddress;
        network->get_ip_address(&socketAddress);
        printf("Server is listening at http://%s:8080\n", socketAddress.get_ip_address());
    }
    else {
        printf("Server could not be started... %d\n", res);
    }

    threadTFTPpServer.start(network); 
#endif 

	finger.begin(57600);
	finger.getParameters();
	finger.LEDcontrol((uint8_t)FINGERPRINT_LED_OFF, 0, FINGERPRINT_LED_BLUE);

	printf("Status  : 0x%0x\n", finger.status_reg);
	printf("Sys ID  : 0x%0x\n",finger.system_id);
	printf("Capacity: %0d\n", finger.capacity);
    printf("Security level: %d\n", finger.security_level);
    printf("Device address: %ld\n", finger.device_addr);
    printf("Packet len: %d\n", finger.packet_len);
    printf("Baud rate: %d\n\n", finger.baud_rate);

	ProductInfo pi;
	finger.readProductInfo(pi);

	printf("Module Type: %s\n", pi.module_type);
	printf("Hardware Version: %02d.%02d\n", pi.hardware_version[0], pi.hardware_version[1]);
	printf("Module Batch Number: %.4s\n", pi.module_batch_number);
	printf("Module Serial Number: %.8s\n", pi.module_serial_number);
	printf("Database size: %d\n", pi.database_size);
	printf("Template size: %d\n", pi.template_size);
	printf("Sensor Type: %.8s\n", pi.sensor_type);
	printf("Sensor width/height : %d x %d\n\n", pi.sensor_width, pi.sensor_height);

	// fingerprint detect action
	wakeupFP.fall( []() {
		queue.call(&getImage);
	});

	// main loop, print message with counter
	int counter = 0;
	while(true) 
	{
		counter++;
		ThisThread::sleep_for(50ms);
	}

	return 0;
}