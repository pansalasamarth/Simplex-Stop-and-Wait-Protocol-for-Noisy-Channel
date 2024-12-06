# Simplex Stop and Wait Protocol for Noisy Channel

This GitHub repository demonstrates a UDP-based communication system between a **Sender** and a **Receiver** simulating the Stop-and-Wait protocol with frame loss, damage, and acknowledgment loss scenarios.

## Project Overview

The project consists of two main programs:
- **Sender** (`sender.c`): Simulates the sending of frames to the receiver.
- **Receiver** (`receiver.c`): Handles received frames, simulates frame damage, and responds with acknowledgments.

Both programs utilize the UDP protocol for communication and include mechanisms to simulate network anomalies.

## Features
- Implementation of the Stop-and-Wait ARQ protocol.
- Randomized simulation of frame loss, acknowledgment loss, and frame corruption.
- Proper handling of duplicate and damaged frames.
- Timeout and retransmission for lost frames.

## File Descriptions

### `sender.c`
The sender program:
- Sends frames sequentially with a frame number.
- Waits for an acknowledgment for the last sent frame.
- Retransmits the frame if the acknowledgment is not received within a timeout period.

### `receiver.c`
The receiver program:
- Listens for incoming frames.
- Checks frame integrity and order.
- Sends acknowledgments for valid frames.
- Simulates acknowledgment loss and frame corruption.

## How to Run

### Prerequisites
- A C compiler (e.g., GCC).
- A Unix-based environment for testing (e.g., Linux, macOS).

### Steps
1. Compile the programs:
   ```bash
   gcc sender.c -o sender
   gcc receiver.c -o receiver
   ```
2. Open two terminals:
   - In the first terminal, run the receiver:
     ```bash
     ./receiver
     ```
   - In the second terminal, run the sender:
     ```bash
     ./sender
     ```
3. Observe the communication between the sender and receiver in the terminal outputs.

## Notes
- Random behavior is seeded by the program's runtime; results may vary for each run.
- Modify the `PORT` and `TIMEOUT` values in the source code if needed.

## Example Output

### Receiver:
```
Received frame: A
Frame A is correct. Sending ACK.
ACK sent: 0
ACK 0 lost.
```

### Sender:
```
Sending frame 0: A
Timeout, resending frame 0.
Received correct ACK for frame 0.
```
