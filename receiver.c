#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 8080

int main() {
    int sockfd;
    struct sockaddr_in servaddr, cliaddr;
    socklen_t len = sizeof(cliaddr);
    char frame[2];  
    char ack;
    int expected_frame_no = 0;
    int loss_chance, damage_chance;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    memset(&servaddr, 0, sizeof(servaddr));
    memset(&cliaddr, 0, sizeof(cliaddr));
    
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        int n = recvfrom(sockfd, frame, sizeof(frame), 0, (struct sockaddr *)&cliaddr, &len);

        if (n > 0) {
            damage_chance = rand() % 4;
            if (damage_chance == 0) {
                printf("Frame %c is damaged. Discarding and sending ACK for last valid frame.\n", frame[1]);
                ack = '0' + (1 - expected_frame_no); 
            } else {
                printf("Received frame: %c\n", frame[1]);

                if (expected_frame_no == (frame[0] - '0')) {
                    printf("Frame %c is correct. Sending ACK.\n", frame[1]);
                    ack = '0' + expected_frame_no;
                    expected_frame_no = 1 - expected_frame_no;  
                } else {
                    printf("Duplicate frame %c received. Discarding and sending ACK for last valid frame.\n", frame[1]);
                    ack = '0' + (1 - expected_frame_no);  
                }
            }

            loss_chance = rand() % 4;
            if (loss_chance != 0) {
                sendto(sockfd, &ack, sizeof(ack), 0, (const struct sockaddr *)&cliaddr, len);
                printf("ACK sent: %c\n", ack);
            } else {
                printf("ACK %c lost.\n", ack);
            }
        }

    }

    close(sockfd);
    return 0;
}

