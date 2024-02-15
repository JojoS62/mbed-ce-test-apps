#include "mqtt.h"

#include "MQTTThreadedClient.h"

using namespace MQTT;

float temperature1;

const char* mqtt_listener_name = "mqtt_listener";
const char* mqtt_sender_name = "mqtt_sender";
static Thread thread_mqtt_listener(osPriorityNormal, 8192, nullptr, mqtt_listener_name);
static Thread thread_mqtt_sender(osPriorityNormal, 8192, nullptr, mqtt_sender_name);

static MQTTThreadedClient *mqtt;

static const char *clientID = "HeaterController";
static const char *topicCmd = "cmd";
static const char *topicCurrentValues = "currentValues";

static int arrivedcount = 0;

void messageArrived(MessageData& md)
{
    Message &message = md.message;
    printf("Arrived Callback cmd : qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload [%.*s]\n", message.payloadlen, (char*)message.payload);
    char s[64];
    memset(s, 0, sizeof(s));
    memcpy(s, message.payload, min(message.payloadlen, sizeof(s)-1));
    
    if (strcmp(s, "reset") == 0) {
        printf("reset counter\n");
    } else if (strcmp(s, "master") == 0) {
        printf("mode:master\n");
    } else if (strcmp(s, "slave") == 0) {
        printf("mode:slave\n");
    } else if (strstr(s, "fbh")) {
        printf("set fbh: ");
        const char *pch = strchr(s, ' ');
        if (pch) {
            float temp = atoi(pch);
            if (temp > 2 && temp < 60) {
                printf(" %f °C\n", temp);
            } else {
                printf(" err: value out range\n");
            }

        }
    } else {
        printf("unknown cmd\n");
    }
    ++arrivedcount;
}

void mqtt_sender_fn(){
    auto cycleTime = 1s;
    printf("starting mqtt sender thread\n");

    while(true)
    {
        auto nextTime = Kernel::Clock::now() + cycleTime;
        printf("send mqtt\n");

        PubMessage message;
        message.qos = QOS0;
        message.id = 123;
        
        strcpy(&message.topic[0], topicCurrentValues);
        sprintf(&message.payload[0], "%7.1f", temperature1);
        message.payloadlen = strlen((const char *) &message.payload[0]);
        mqtt->publish(message);

        ThisThread::sleep_until(nextTime);
    }
}

void mqtt_init(){
    float version = 0.6;

    printf("starting mqtt version %.2f\n", version);
    mqtt = new MQTTThreadedClient(network);

    // const char* hostname = "jojosRPi3-1";
    // const char* hostname = "192.168.32.1";
    // const char* hostname = "172.19.11.26";
    const char* hostname = "192.168.100.108";
    // const char* hostname = "192.168.1.140";       // Daniel
    int port = 1883;

    MQTTPacket_connectData logindata = MQTTPacket_connectData_initializer;
    logindata.MQTTVersion = 3;
    logindata.clientID.cstring = (char *) clientID;
    //logindata.username.cstring = (char *) userID;
    //logindata.password.cstring = (char *) password;
    
    mqtt->setConnectionParameters(hostname, port, logindata);
    // subscribe topics
    mqtt->addTopicHandler(topicCmd, messageArrived);
    // mqtt->addTopicHandler(topic_2, callback(&testcb, &CallbackTest::messageArrived));

    // Start the listener
    thread_mqtt_listener.start(mbed::callback(mqtt, &MQTTThreadedClient::startListener));
    thread_mqtt_sender.start(mqtt_sender_fn);
}