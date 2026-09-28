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

int isValidFrame(const unsigned char *frame, unsigned char control)
{
    return frame[0] == FLAG &&
           frame[1] == A_TX &&
           frame[2] == control &&
           frame[3] == (frame[1] ^ frame[2]) &&
           frame[4] == FLAG;
}

int readFrame(unsigned char *frame)
{
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
        }
        else if (r == 0)
        {
            continue;
        }
        else
        {
            frame[i] = byte;
            i++;
        }
    }

    return 0;
}

int writeFrame(const unsigned char *frame)
{
    int bytes = writeBytesSerialPort(frame, FRAME_SIZE);
    printf("%d bytes written to serial port\n", bytes);

    if (bytes != FRAME_SIZE)
    {
        perror("writeBytesSerialPort");
        return -1;
    }

    return 0;
}

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

    if (writeFrame(set) < 0)
    {
        closeSerialPort();
        return -1;
    }

    unsigned char received_ua[FRAME_SIZE];
    if (readFrame(received_ua) < 0)
    {
        closeSerialPort();
        return -1;
    }

    if (!isValidFrame(received_ua, C_UA))
    {
        printf("Invalid UA frame\n");
        closeSerialPort();
        return -1;
    }

    printf("UA received, connection established\n");

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

    unsigned char received_set[FRAME_SIZE];
    if (readFrame(received_set) < 0)
    {
        closeSerialPort();
        return -1;
    }

    for (int j = 0; j < FRAME_SIZE; j++)
    {
        printf("0x%02X ", received_set[j]);
    }

    printf("\nTotal bytes received: %d\n", FRAME_SIZE);

    if (!isValidFrame(received_set, C_SET))
    {

        printf("Invalid SET frame\n");
        closeSerialPort();
        return -1;
    }

    if (writeFrame(ua) < 0)
    {
        closeSerialPort();
        return -1;
    }

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
