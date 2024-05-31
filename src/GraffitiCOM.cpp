#include "GraffitiCOM.h"

void gInit(GReg* regPtr, HardwareSerial* serialPtr) {
    regPtr->hardSerialPtr = serialPtr;
    regPtr->softSerialPtr = NULL;
    regPtr->buffer.length = 0;
    regPtr->buffer.bytes[0] = '\0';

    regPtr->pingCallbackFuncPtr = NULL;
}

void gBufferUpdate(GBuffer* bufferPtr, HardwareSerial* serialPtr) {
    bufferPtr->length = serialPtr->readBytes(bufferPtr->bytes, BUFFER_SIZE);
    bufferPtr->bytes[bufferPtr->length] = '\0';
}

void gBufferUpdate(GBuffer* bufferPtr, SoftwareSerial* serialPtr) {
    // pending implementation
}

int gGetBufferLength(GReg reg) {
    return reg.buffer.length;
}

char gGetBufferBytes(GReg reg) {
    return reg.buffer.bytes;
}

void gUpdate(GReg* regPtr) {
    if (regPtr->softSerialPtr == NULL) {
        gBufferUpdate(&(regPtr->buffer), regPtr->hardSerialPtr);
    } else {
        gBufferUpdate(&(regPtr->buffer), regPtr->softSerialPtr);
    }

    switch((regPtr->buffer.bytes)[0]) {
        case 0x00:
            if (regPtr->pingCallbackFuncPtr != NULL) {
                (*(regPtr->pingCallbackFuncPtr))();
            }
            break;

        case 0x01:
            if (regPtr->setRpmXCallbackFuncPtr == NULL) break;
            union {
                float number;
                char bytes[sizeof(float)];
            } rpmX;

            for (int i=0; i<4; i++) {
                rpmX.bytes[i] = (regPtr->buffer.bytes)[i+1];
            }

            (*(regPtr->setRpmXCallbackFuncPtr))(rpmX.number);
            break;

        case 0x02:
            if (regPtr->setRpmYCallbackFuncPtr == NULL) break;
            union {
                float number;
                char bytes[sizeof(float)];
            } rpmY;

            for (int i=0; i<4; i++) {
                rpmY.bytes[i] = (regPtr->buffer.bytes)[i+1];
            }

            (*(regPtr->setRpmYCallbackFuncPtr))(rpmY.number);
            break;

        case 0x03:
            if (regPtr->translateCallbackFuncPtr != NULL) {
                
                char asciiX[8];
                for (int i=1; i<8; i++) {
                    asciiX[i-1] = (regPtr->buffer.bytes)[i];
                }
                asciiX[7] = '\0';

                char asciiY[8];
                for (int i=8; i<16; i++) {
                    asciiY[i-8] = (regPtr->buffer.bytes)[i];
                }
                asciiY[7] = '\0';

                float x = atof(asciiX);
                float y = atof(asciiY);

                (*(regPtr->translateCallbackFuncPtr))(x, y);

            }
            break;
    }
}


//calback register functions

void gOnPing(GReg* regPtr, void (*funcPtr)(void)) {
    regPtr->pingCallbackFuncPtr = funcPtr;
}


void gOnTranslate(GReg* regPtr, void (*funcPtr)(float x, float y)) {
    regPtr->translateCallbackFuncPtr = funcPtr;
}


void gOnSetRpmX(GReg* regPtr, void (*funcPtr)(float rpm)) {
    regPtr->setRpmXCallbackFuncPtr = funcPtr;
}


void gOnSetRpmY(GReg* regPtr, void (*funcPtr)(float rpm)) {
    regPtr->setRpmYCallbackFuncPtr = funcPtr;
}
