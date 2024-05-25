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

    // if first byte of buffer is 0x00 then call pingCallbackFunction
    // if ((regPtr->buffer.bytes)[0] == 0x00) {
    //     if (regPtr->pingCallbackFuncPtr != NULL) {
    //         (*(regPtr->pingCallbackFuncPtr))();
    //     }
    // }

    switch((regPtr->buffer.bytes)[0]) {
        case 0x00:
            if (regPtr->pingCallbackFuncPtr != NULL) {
                (*(regPtr->pingCallbackFuncPtr))();
            }
            break;
        case 0x02:
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


void gOnPing(GReg* regPtr, void (*funcPtr)(void)) {
    regPtr->pingCallbackFuncPtr = funcPtr;
}


void gOnTranslate(GReg* regPtr, void (*funcPtr)(float x, float y)) {
    regPtr->translateCallbackFuncPtr = funcPtr;
}
