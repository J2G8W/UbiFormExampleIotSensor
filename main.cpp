#include <iostream>
#include <UbiForm/Component.h>

#include <chrono>
#include <thread>
#include "sys/types.h"
#include "sys/sysinfo.h"



void temperaturePublisherCallback(Endpoint* endpoint, void* userData){
    auto* component = static_cast<Component*>(userData);

    DataSenderEndpoint* publisherEndpoint = component->castToDataSenderEndpoint(endpoint);
    int temperature = 13;
    int humidity = 90;
    std::chrono::milliseconds timespan(100);

    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> tempChangeDistribution(-1,1);
    std::uniform_int_distribution<int> humidityChangeDistribution(-3,3);

    SocketMessage sm;
    while(true){
        temperature += tempChangeDistribution(generator);
        humidity += humidityChangeDistribution(generator);
        try{

            sm.addMember("temperature",temperature);
            sm.addMember("humidity",humidity);
            publisherEndpoint->sendMessage(sm);
        } catch (std::logic_error &e) {
            std::cerr << "Endpoint stopped: " << e.what() << std::endl;
            break;
        }
        std::this_thread::sleep_for(timespan);
    }
}

int main() {
    Component component;
    FILE* fp = fopen("Manifest.json", "r");
    if(fp == nullptr){
        std::cerr << "Unable to find Manifest.json" << std::endl;
        exit(-1);
    }
    component.specifyManifest(fp);
    fclose(fp);

    component.registerStartupFunction("weatherPublisher",temperaturePublisherCallback, &component);


    struct sysinfo memInfo{};

    sysinfo (&memInfo);

    long long totalRAM = memInfo.totalram;
    long long freeRAM = memInfo.freeram;

    long long ramPercent = (100*freeRAM)/totalRAM;
    totalRAM *= memInfo.mem_unit;
    totalRAM /= 1024;

    std::cout << "Total RAM in machine: " << totalRAM << "kb" << std::endl;
    std::cout << "Free RAM: " << ramPercent << "%" << std::endl;

}
