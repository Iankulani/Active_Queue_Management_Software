#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_QUEUE_SIZE 10
#define THRESHOLD 8
#define DROP_PROBABILITY 0.2

// Structure to represent a packet with IP address
typedef struct {
    char packet_id[50];
    char ip_address[20];
} Packet;

// Queue structure to hold packets
typedef struct {
    Packet queue[MAX_QUEUE_SIZE];
    int front;
    int rear;
} Queue;

// Function prototypes
void enqueue(Queue* q, char* packet, char* ip_address);
void apply_aqm(Queue* q, char* ip_address);
void dequeue(Queue* q);
void show_queue(Queue* q);
int is_full(Queue* q);
int is_empty(Queue* q);
int validate_ip(char* ip_address);

int main() {
    Queue q = {{0}, 0, 0};
    int max_queue_size = MAX_QUEUE_SIZE;
    int threshold = THRESHOLD;
    float drop_probability = DROP_PROBABILITY;
    char packet[50];
    char ip_address[20];
    int action;

    printf("Welcome to the Active Queue Management (AQM) Simulator!\n");
    printf("This educational tool demonstrates the concept of AQM in networking with IP addresses.\n");

    printf("Enter the maximum size of the packet queue (default 10): ");
    scanf("%d", &max_queue_size);
    printf("Enter the threshold value for AQM (default 8): ");
    scanf("%d", &threshold);
    printf("Enter the packet drop probability (between 0 and 1, default 0.2): ");
    scanf("%f", &drop_probability);

    while (1) {
        printf("\nSelect an action:\n");
        printf("1. Enqueue a packet\n");
        printf("2. Dequeue a packet\n");
        printf("3. View the current queue\n");
        printf("4. Exit\n");
        printf("Enter the number corresponding to your choice: ");
        scanf("%d", &action);

        if (action == 1) {
            printf("Enter packet identifier (e.g., 'packet1', 'packet2'): ");
            scanf("%s", packet);
            printf("Enter the IP address of the packet (e.g., '192.168.1.1'): ");
            scanf("%s", ip_address);

            // Validate IP address format
            if (validate_ip(ip_address)) {
                enqueue(&q, packet, ip_address);
            } else {
                printf("Invalid IP address format: %s. Please try again.\n", ip_address);
            }

        } else if (action == 2) {
            dequeue(&q);

        } else if (action == 3) {
            show_queue(&q);

        } else if (action == 4) {
            printf("Exiting the simulator. Goodbye!\n");
            break;

        } else {
            printf("Invalid option. Please try again.\n");
        }

        sleep(1);  // Simulate delay in real networking environments
    }

    return 0;
}

// Enqueue function to add a packet to the queue
void enqueue(Queue* q, char* packet, char* ip_address) {
    if (is_full(q)) {
        printf("Queue is full! Applying AQM for IP %s...\n", ip_address);
        apply_aqm(q, ip_address);
    } else {
        // Add packet to queue
        strcpy(q->queue[q->rear].packet_id, packet);
        strcpy(q->queue[q->rear].ip_address, ip_address);
        q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
        printf("Packet %s from IP %s added to the queue.\n", packet, ip_address);
    }
}

// Apply AQM function to handle congestion
void apply_aqm(Queue* q, char* ip_address) {
    if (q->rear >= THRESHOLD) {
        printf("Queue size exceeds threshold (%d). Dropping packet from IP %s...\n", THRESHOLD, ip_address);
        // Drop packet with a probability
        if ((float)rand() / RAND_MAX < DROP_PROBABILITY) {
            printf("Packet %s from IP %s dropped to manage congestion.\n", q->queue[q->front].packet_id, q->queue[q->front].ip_address);
            q->front = (q->front + 1) % MAX_QUEUE_SIZE;
        } else {
            printf("No packet dropped for IP %s. Adjusting drop probability.\n", ip_address);
        }
    } else {
        printf("Queue size is within limits. No packet dropped for IP %s.\n", ip_address);
    }
}

// Dequeue function to remove a packet from the queue
void dequeue(Queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty. No packets to dequeue.\n");
    } else {
        printf("Packet %s from IP %s dequeued.\n", q->queue[q->front].packet_id, q->queue[q->front].ip_address);
        q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    }
}

// Show the current state of the queue
void show_queue(Queue* q) {
    if (is_empty(q)) {
        printf("Queue is empty.\n");
    } else {
        printf("Current queue state:\n");
        for (int i = q->front; i != q->rear; i = (i + 1) % MAX_QUEUE_SIZE) {
            printf("Packet %s from IP %s\n", q->queue[i].packet_id, q->queue[i].ip_address);
        }
    }
}

// Check if the queue is full
int is_full(Queue* q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// Check if the queue is empty
int is_empty(Queue* q) {
    return q->front == q->rear;
}

// Validate IP address format
int validate_ip(char* ip_address) {
    int parts = 0;
    char* token = strtok(ip_address, ".");
    while (token != NULL) {
        if (strlen(token) > 3 || atoi(token) < 0 || atoi(token) > 255) {
            return 0;
        }
        parts++;
        token = strtok(NULL, ".");
    }
    return parts == 4;
}
