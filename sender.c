#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080
#define TIMEOUT 15

int main() {
    int sockfd;
    struct sockaddr_in servaddr;
    socklen_t len = sizeof(servaddr);
    char frame[2];  
    char ack;
    int frame_no = 0;
    int loss_chance;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    while (1) {
    
        frame[0] = '0' + frame_no;  
        frame[1] = 'A' + frame_no;  

        loss_chance = rand() % 4;
        if (loss_chance != 0) {
            printf("Sending frame %d: %c\n", frame_no, frame[1]);
            sendto(sockfd, frame, sizeof(frame), 0, (const struct sockaddr *)&servaddr, len);
        } else {
            printf("Frame %d lost.\n", frame_no);
        }

        struct timeval tv;
        tv.tv_sec = TIMEOUT;
        tv.tv_usec = 0;
        setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

        int n = recvfrom(sockfd, &ack, sizeof(ack), 0, (struct sockaddr *)&servaddr, &len);
        if (n < 0) {
            printf("Timeout, resending frame %d.\n", frame_no);
        } else {
            if (ack == '0' + frame_no) {
                printf("Received correct ACK for frame %d.\n", frame_no);
                frame_no = 1 - frame_no;
            } else {
                printf("Received ACK for previous frame. Resending frame %d.\n", frame_no);
            }
        }

        sleep(4); 
    }

    close(sockfd);
    return 0;
}

