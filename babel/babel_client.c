#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<fcntl.h>
#include<string.h>
#include<unistd.h>

#define BUFFER_LENGTH 256
#define DEVICE_NAME "/dev/babel"
static char buffer[BUFFER_LENGTH];

int main() {
    int ret, fd;
    char message[BUFFER_LENGTH];

    //   Open the device.
    printf("Opening to %s...", DEVICE_NAME);
    fd = open(DEVICE_NAME, O_RDWR);
    if (fd < 0) {
        perror("Failed to open device...");
        return errno;
    }

    printf("Connected! Enter a message:");
    scanf("%[^\n]%*c", message);

    //   Write to the device.
    printf("Sending: [%s]\n", message);
    ret = write(fd, message, strlen(message));
    if (ret < 0) {
        perror("Failed to write the message to the device.");
        return errno;
    }

    printf("Sent! Press ENTER to read back from the device...\n");
    getchar();

    printf("Reading from the device...\n");
    ret = read(fd, buffer, BUFFER_LENGTH);
    if (ret < 0){
        perror("Failed to read the message from the device.");
        return errno;
    }
    printf("The received message is: [%s]\n", buffer);

    return 0;
}
