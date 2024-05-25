#ifndef GraffitiCOM_h
#define GraffitiCOM_h

#include <Arduino.h>
#include <SoftwareSerial.h>
#define BUFFER_SIZE 0x14 // buffer size is 20 in decimal

typedef struct __GBuffer{
    int length;
    char bytes[BUFFER_SIZE];
} GBuffer;


typedef struct __GReg{
    GBuffer buffer;
    HardwareSerial* hardSerialPtr;
    SoftwareSerial* softSerialPtr;

    void (*pingCallbackFuncPtr) (void);
} GReg;


// initializes the registry
void gInit(GReg* regPtr, HardwareSerial* serialPtr);
// void gInit(GReg* regPtr, SoftwareSerial* serialPtr);

// This function return a buffer adress having the data and keeping the buffer size to BUFFER_SIZE
void gBufferUpdate(GBuffer* bufferPtr, HardwareSerial* serialPtr);
void gBufferUpdate(GBuffer* bufferPtr, SoftwareSerial* serialPtr);

int gGetBufferLength(GReg reg);


void gOnPing(GReg* regPtr, void (*funcPtr)(void));

void gUpdate(GReg* regPtr);

#endif
