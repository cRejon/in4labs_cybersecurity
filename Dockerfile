# syntax=docker/dockerfile:1
FROM python:3.10-slim-bullseye

# Install Arduino CLI
RUN apt-get update && apt-get install -y curl uhubctl
WORKDIR /usr/local
RUN curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | ARDUINO_UPDATER_ENABLE_NOTIFICATION=false sh -s 0.34.2
RUN arduino-cli core update-index
RUN arduino-cli core install arduino:avr
RUN arduino-cli lib install "LiquidCrystal I2C" 
RUN arduino-cli lib install "DHT sensor library"
RUN arduino-cli lib install "MQUnifiedsensor" 
RUN arduino-cli lib install "Grove Barometer Sensor"
RUN arduino-cli lib install "Accelerometer ADXL335"
RUN arduino-cli lib install "RTC"

# Python dependencies are installed here to ensure they will be cached.
WORKDIR /app
COPY requirements.txt /app/requirements.txt
RUN pip3 install --trusted-host pypi.python.org -r requirements.txt

# Copy the current directory contents into the container at /app
COPY in4labs_perception_app /app/in4labs_perception_app
COPY arduino /app/arduino

# Make port 8000 available outside this container
EXPOSE 8000

# Run lab when the container launches
CMD ["flask", "--app", "in4labs_perception_app", "run", "--host", "0.0.0.0", "--port", "8000"]