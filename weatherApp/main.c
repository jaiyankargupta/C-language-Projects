#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define API_KEY "Your API key"
#define API_HOST "api.openweathermap.org"
#define API_PATH "/data/2.5/weather"
#define BUFFER_SIZE 1024

void handleError(const char *msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

void printWeatherInfo(const char *json) {
    // Parse JSON manually and extract relevant information
    char *start = strstr(json, "\"main\":\"") + strlen("\"main\":\"");
    char *end = strchr(start, '"');
    printf("Weather: %.*s\n", (int)(end - start), start);

    start = strstr(json, "\"temp\":") + strlen("\"temp\":");
    double temp = atof(start) - 273.15; // Convert temperature from Kelvin to Celsius
    printf("Temperature: %.2f°C\n", temp);

    start = strstr(json, "\"humidity\":") + strlen("\"humidity\":");
    int humidity = atoi(start);
    printf("Humidity: %d%%\n", humidity);

    start = strstr(json, "\"speed\":") + strlen("\"speed\":");
    double wind_speed = atof(start);
    printf("Wind Speed: %.2f m/s\n", wind_speed);

    start = strstr(json, "\"country\":\"") + strlen("\"country\":\"");
    end = strchr(start, '"');
    printf("Country: %.*s\n", (int)(end - start), start);
}

void parseHTTPResponse(int sockfd) {
    // Receive and display the HTTP response
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        // Check if the response contains the JSON data
        char *json_start = strstr(buffer, "{");
        if (json_start != NULL) {
            printWeatherInfo(json_start);
            break; // Stop reading further data after parsing JSON
        }
    }

    // Check for receive errors
    if (bytes_received < 0) {
        handleError("Error: Unable to receive response");
    }

    // Close the socket
    close(sockfd);
}

void getWeather(char *location) {
    // Create a TCP socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        handleError("Error: Unable to create socket");
    }

    // Resolve server address
    struct hostent *server = gethostbyname(API_HOST);
    if (server == NULL) {
        handleError("Error: Unable to resolve server address");
    }

    // Prepare server address structure
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    memcpy(&server_addr.sin_addr.s_addr, server->h_addr, server->h_length);
    server_addr.sin_port = htons(80);

    // Connect to the server
    if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        handleError("Error: Unable to connect to server");
    }

    // Prepare the HTTP request
    char request[BUFFER_SIZE];
    snprintf(request, BUFFER_SIZE, "GET %s?q=%s&appid=%s HTTP/1.1\r\nHost: %s\r\nConnection: close\r\n\r\n", API_PATH, location, API_KEY, API_HOST);

    // Send the HTTP request
    if (send(sockfd, request, strlen(request), 0) < 0) {
        handleError("Error: Unable to send request");
    }

    // Parse and display the HTTP response
    parseHTTPResponse(sockfd);
}

int main() {
    char location[100];

    // Get user input for location
    printf("Enter location: ");
    fgets(location, sizeof(location), stdin);
    location[strcspn(location, "\n")] = '\0'; // Remove newline character

    // Call function to fetch weather data
    getWeather(location);

    return 0;
}
