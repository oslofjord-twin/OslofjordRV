FROM ubuntu:24.04
# Set the working directory
WORKDIR /monitor

# Copy the current directory contents into the container at /monitor
COPY . /monitor

# Install needed packages
RUN apt-get update && \
apt-get install -y libcjson-dev unzip default-jre python3 pipx python3-venv wget python3-pip
RUN pipx install gql[all]
RUN wget https://www.tessla.io/logging.zip && unzip logging.zip && rm logging.zip && mv liblogging.a /usr/lib && mv logging.h /usr/include
RUN pip install requests flask flask_socketio gql[all] --break-system-packages

# Execute the command
CMD python3 request_listener.py
