# Use a standard Ubuntu image (closest to the grader's environment)
FROM ubuntu:22.04

# Install C compiler, Make, and the required monitoring tools (top, iostat, taskset)
RUN apt-get update && apt-get install -y \
    build-essential \
    sysstat \
    procps \
    bc \
    nano

# Set working directory
WORKDIR /app

# Default command
CMD ["bash"]