# Weather App

The Weather App is a command-line application that retrieves and displays weather information for a given location using the OpenWeatherMap API.

## Features

- Fetches weather data for a specified location.
- Displays weather information including temperature, humidity, wind speed, and weather condition.

## Installation

### Prerequisites

- C Compiler (gcc)
- Standard C libraries
- Internet connection (for fetching weather data from the OpenWeatherMap API)

### Instructions

1. Clone the repository:

```
git clone <repository_url>
```

2. Compile the program:

```
gcc -o weatherApp main.c
```

## Usage

1. Run the program:

```
./weatherApp
```

2. Enter the location: You will be prompted to enter the location for which you want to fetch the weather information.

3. View the weather information: The program will display the weather information for the specified location.

## API Key

To use the OpenWeatherMap API, you need to sign up for an API key. Once you have obtained the API key, replace the placeholder API_KEY in the main.c file with your actual API key.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.


