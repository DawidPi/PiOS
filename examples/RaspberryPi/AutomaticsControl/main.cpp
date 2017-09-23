#include <Uart/Uart.hpp>
#include <Timer/Timer.hpp>
#include <VectorTableManagement.hpp>
#include <Pi.hpp>
#include <RealTimeTask.hpp>
#include <cassert>
#include <sstream>
#include <cstdio>
#include <Uart/c_uart.hpp>
#include "ImageData.hpp"
#include <iterator>


using namespace uart;
using namespace Time;

namespace {
    float input=0.0f;
    float controlOutput=0.0f;
    float objectOutput=0.0f;
    float iOutput=0.0f;
    PiOS::Time lastProbeControl = PiOS::Time(0);
    PiOS::Time lastProbeObject = PiOS::Time(0);

    constexpr uint32_t samplesNo = ImageData::IMAGE_WIDTH-(ImageData::MAX_BAR_X_OFFSET+2);
    constexpr uint32_t samplesStart = ImageData::MAX_BAR_X_OFFSET+2;
    int intOutputs[samplesNo]={0};
    int boolSecondElapsed[samplesNo]={false};
    bool outputGenerated = false;
}

void backgroundTask();

void controlTask();

void objectTask();

bool secondElapsed();

void drawSecondsLine(uint32_t sampleNo);

PiOS::RealTimeTask *systemControlTask;
PiOS::RealTimeTask *systemObjectTask;

constexpr int timeQuantumMillis = 10;
const auto millisPerSecond = 1000.0f;

ImageData newImageData;

void backgroundTask() {
    while (1) {
        while(not outputGenerated);
        outputGenerated=false;
        int savedObjectOutput = static_cast<int>(objectOutput);

        for(auto currentSample = std::begin(intOutputs); currentSample < std::end(intOutputs); ++currentSample){
            auto nextSample = currentSample;
            std::advance(nextSample, 1);
            if(nextSample == std::end(intOutputs)){
                *currentSample = savedObjectOutput;
            }else{
                *currentSample = *nextSample;
            }
        }

        for(auto currentSample = std::begin(boolSecondElapsed); currentSample < std::end(boolSecondElapsed); ++currentSample){
            auto nextSample = currentSample;
            std::advance(nextSample, 1);
            if(nextSample == std::end(boolSecondElapsed)){
                *currentSample = secondElapsed();
            }else{
                *currentSample = *nextSample;
            }
        }

        newImageData.fill(false);
        newImageData.setInput(static_cast<int>(input));

        for(uint32_t currentSample=0; currentSample <samplesNo; ++currentSample){
            auto sampleValue = intOutputs[currentSample];
            auto secondElapsedValue = boolSecondElapsed[currentSample];
            if(sampleValue > ImageData::MAX_VALUE)
                sampleValue = ImageData::MAX_VALUE;
            else if(sampleValue < ImageData::MIN_VALUE){
                sampleValue = ImageData::MIN_VALUE;
            }
            newImageData.setPixel(samplesStart + currentSample, -sampleValue + ImageData::IMAGE_HEIGHT / 2);
            if(secondElapsedValue){
                drawSecondsLine(currentSample);
            }
        }

        char* begin = reinterpret_cast<char *>(newImageData.data());
        auto end = begin + ImageData::IMAGE_SIZE_BYTES;

        while(begin != end){
            uart_send(*begin);
            begin++;
        }

        if(uart_dataToRead()){
            int newInput;
            uart::cin>>newInput;
            input = static_cast<int>(newInput);
        }

    }
}

void drawSecondsLine(uint32_t sampleNo) {
//    if(sampleNo <= ImageData::MAX_BAR_X_OFFSET){
//        return;
//    }
    newImageData.setPixel(samplesStart+sampleNo, ImageData::IMAGE_HEIGHT-1);
    newImageData.setPixel(samplesStart+sampleNo, ImageData::IMAGE_HEIGHT-2);
    newImageData.setPixel(samplesStart+sampleNo, ImageData::IMAGE_HEIGHT-3);
    newImageData.setPixel(samplesStart+sampleNo, 0);
    newImageData.setPixel(samplesStart+sampleNo, 1);
    newImageData.setPixel(samplesStart+sampleNo, 2);
}

bool secondElapsed() {
    static PiOS::Time lastTimeStamp(0);
    auto currentTime = PiOS::PiOSHolder::getInstance()->currentTime();

    if((currentTime - lastTimeStamp).getRawValue() * timeQuantumMillis >= millisPerSecond){
        lastTimeStamp = currentTime;
        return true;
    }

    return false;
}

void controlTask() {
    auto Pi = PiOS::PiOSHolder::getInstance();
    assert(Pi != nullptr);
    
    float e = input - objectOutput;

    float k =3.0f;
    float ki =1.0f;

    PiOS::Time currentTime = Pi->currentTime();
    float timeDiff = (currentTime - lastProbeControl).getRawValue() * timeQuantumMillis / millisPerSecond;
    lastProbeControl = currentTime;

    iOutput += timeDiff * e * ki;
    uart::cout << "[I_CTRL_OUTPUT]: " << (int)iOutput << uart::endl;
    float kOutput = e*k;
    uart::cout << "[K_CTRL_OUTPUT]: " << (int)kOutput << uart::endl;

    controlOutput = iOutput + kOutput;
    uart::cout << "[CTRL_OUTPUT]: " << (int)controlOutput << uart::endl;

    systemControlTask->releaseTime() = Pi->currentTime() + PiOS::Time(20);
    systemControlTask->deadline() = Pi->currentTime() + PiOS::Time(23);

    Pi->addTask(systemControlTask);
}


void objectTask() {
    auto Pi = PiOS::PiOSHolder::getInstance();
    assert(Pi != nullptr);

    float k =.30f;

    PiOS::Time currentTime = Pi->currentTime();
    float timeDiff = (currentTime - lastProbeObject).getRawValue() * (timeQuantumMillis / millisPerSecond);
    lastProbeObject = currentTime;

    objectOutput += controlOutput * k * timeDiff;

    outputGenerated=true;

    uart::cout << "[OUTPUT]: " << (int)objectOutput << uart::endl;

    systemObjectTask->releaseTime() = Pi->currentTime() + PiOS::Time(3);
    systemObjectTask->deadline() = Pi->currentTime() + PiOS::Time(5);
    Pi->addTask(systemObjectTask);
}

int main() {
    VectorTable::setUp();

    auto Pi = PiOS::PiOSHolder::getInstance();
    assert(Pi != nullptr);

    systemControlTask = new PiOS::RealTimeTask(controlTask, PiOS::Time(0), PiOS::Time(0), 1024 * 1024);
    systemObjectTask = new PiOS::RealTimeTask(objectTask, PiOS::Time(0), PiOS::Time(0), 1024 * 1024 * 20);

    systemControlTask->releaseTime() = Pi->currentTime() + PiOS::Time(20);
    systemControlTask->deadline() = Pi->currentTime() + PiOS::Time(23);

    systemObjectTask->releaseTime() = Pi->currentTime() + PiOS::Time(3);
    systemObjectTask->deadline() = Pi->currentTime() + PiOS::Time(5);

    Pi->addTask(systemControlTask);
    Pi->addTask(systemObjectTask);
    Pi->setBackgroundTask(new PiOS::Task(backgroundTask, 1024));

    Timer timer;
    timer.init(TimePeriod::millis(timeQuantumMillis));

    return 0;
}