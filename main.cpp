#include <iostream>
#include <UbiForm/Component.h>

#include <chrono>
#include <thread>
#ifdef  __linux__
#include "sys/types.h"
#include "sys/sysinfo.h"
#endif



void temperaturePublisherCallback(Endpoint* endpoint, void* userData){
    auto* component = static_cast<Component*>(userData);

    DataSenderEndpoint* publisherEndpoint = component->castToDataSenderEndpoint(endpoint);
    int temperature = 13;
    int humidity = 90;
    std::chrono::milliseconds timespan(100);

    std::default_random_engine generator(std::random_device{}());
    std::uniform_int_distribution<int> tempChangeDistribution(-1,1);
    std::uniform_int_distribution<int> humidityChangeDistribution(-3,3);

    EndpointMessage endpointMessage;
    while(true){
        temperature += tempChangeDistribution(generator);
        humidity += humidityChangeDistribution(generator);
        try{
            endpointMessage.addMember("temperature", temperature);
            endpointMessage.addMember("humidity", humidity);
            publisherEndpoint->sendMessage(endpointMessage);
        } catch (std::logic_error &e) {
            std::cerr << "Endpoint stopped: " << e.what() << std::endl;
            break;
        }
        std::this_thread::sleep_for(timespan);
    }
}

void systemStatusReply(Endpoint* endpoint, void* userData){
    auto* component = static_cast<Component*>(userData);

    DataSenderEndpoint* senderEndpoint = component->castToDataSenderEndpoint(endpoint);
    DataReceiverEndpoint* receiverEndpoint = component->castToDataReceiverEndpoint(endpoint);

    while(true){
        bool error;
        try{
            auto msg = receiverEndpoint->receiveMessage();
            error = false;
        }catch (ValidationError &e){
            std::cerr << "Received erroneous request" << std::endl;
            error = true;
        } catch (NngError &e){
            std::cerr << "Endpoint stopped: " << e.what() << std::endl;
            break;
        }
#ifdef __linux__
        struct sysinfo memInfo{};
        sysinfo (&memInfo);
        long long totalRAM = memInfo.totalram;
        long long freeRAM = memInfo.freeram;
        long long ramPercent = (100*freeRAM)/totalRAM;

        EndpointMessage endpointMessage;
        endpointMessage.addMember("freeRAM", (int) ramPercent);
        endpointMessage.addMember("error", error);
        endpointMessage.addMember("boardName", "JULIAN's board");
#else
        EndpointMessage endpointMessage;
        endpointMessage.addMember("error",true);
        endpointMessage.addMember("errorMsg", "Wrong platform for requesting");
#endif
        try {
            senderEndpoint->asyncSendMessage(endpointMessage);
        } catch (NngError &e){
            std::cerr << "Serious error: " << e.what() << std::endl;
            break;
        }
    }
}


int main(int argc, char **argv) {
    Component component;
    FILE* fp = fopen("Manifest.json", "r");
    if(fp == nullptr){
        std::cerr << "Unable to find Manifest.json" << std::endl;
        exit(-1);
    }
    component.specifyManifest(fp);
    fclose(fp);

    component.registerStartupFunction("weatherPublisher",temperaturePublisherCallback, &component);
    component.registerStartupFunction("boardStats", systemStatusReply, &component);

    component.startBackgroundListen();

    if (argc >= 2){
        component.getResourceDiscoveryConnectionEndpoint().registerWithHub(argv[1]);
    }
    if(component.getResourceDiscoveryConnectionEndpoint().getResourceDiscoveryHubs().empty()){
        std::cout << "Searching for hubs" << std::endl;
        component.getResourceDiscoveryConnectionEndpoint().searchForResourceDiscoveryHubs();
    }

    while(true){
        std::string userInput;
        std::cin >> userInput;
        if(userInput == "end"){
            break;
        }
    }
    component.getResourceDiscoveryConnectionEndpoint().deRegisterFromAllHubs();
}
