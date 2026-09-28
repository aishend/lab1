// RCOM 2026/2027
//
// Link layer protocol implementation

#include "link_layer.h"
#include "serial_port.h"

#include <stdio.h>
#include <unistd.h>

// MISC
#define _POSIX_SOURCE 1 // POSIX compliant source
#define BUF_SIZE 256

// FLAG
#define FLAG 0x7E
#define A_TX 0x03
#define C_SET 0x03
#define C_UA 0x07

#define FRAME_SIZE 5

unsigned char set[FRAME_SIZE] = {FLAG, A_TX, C_SET, A_TX ^ C_SET, FLAG};
unsigned char ua[FRAME_SIZE] = {FLAG, A_TX, C_UA, A_TX ^ C_UA, FLAG};

////////////////////////////////////////////////
// LLOPEN
////////////////////////////////////////////////
int llOpenTx(LinkLayer llParameters)
{

    if (openSerialPort(llParameters.serialPort, llParameters.baudRate) < 0)
    {
        perror("openSerialPort");
        return -1;
    }

    printf("Serial port %s opened\n", llParameters.serialPort);

    int bytes = writeBytesSerialPort(set, sizeof(set));
    printf("%d bytes written to serial port\n", bytes);

    // Wait until all bytes have been written to the serial port
    sleep(1);

    // Close serial port
    if (closeSerialPort() < 0)
    {
        perror("closeSerialPort");
        return -1;
    }

    printf("Serial port %s closed\n", llParameters.serialPort);

    return 0;
}

int llOpenRx(LinkLayer llParameters)
{

    if (openSerialPort(llParameters.serialPort, llParameters.baudRate) < 0)
    {
        perror("openSerialPort");
        return -1;
    }

    printf("Serial port %s opened\n", llParameters.serialPort);

    unsigned char frame[FRAME_SIZE];
    int i = 0;
    while (i < FRAME_SIZE)
    {
        unsigned char byte;

        // retorna o numero de bytes lidos
        int r = readByteSerialPort(&byte);

        // caso de erro
        if (r < 0)
        {
            perror("readByteSerialPort");
            return -1;
        
        } else if (r == 0) {
            continue;

        } else {
            frame[i] = byte;
            i++;
        }
    }

    for (int j = 0; j < FRAME_SIZE; j++) {
        printf("0x%02X ", frame[k]);
    }
    printf("\n");


    printf("Total bytes received: %d\n", i);

    // Close serial port
    if (closeSerialPort() < 0)
    {
        perror("closeSerialPort");
        return -1;
    }

    printf("Serial port %s closed\n", llParameters.serialPort);

    return 0;
}

////////////////////////////////////////////////
// LLSEND
////////////////////////////////////////////////
int llSend(const unsigned char *buf, int bufSize)
{
    // TODO: Implement this function

    return 0;
}

////////////////////////////////////////////////
// LLRECEIVE
////////////////////////////////////////////////
int llReceive(unsigned char *packet)
{
    // TODO: Implement this function

    return 0;
}

////////////////////////////////////////////////
// LLCLOSE
////////////////////////////////////////////////
int llCloseTx()
{
    // TODO: Implement this function

    return 0;
}

int llCloseRx()
{
    // TODO: Implement this function

    return 0;
}
