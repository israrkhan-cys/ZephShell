FROM ubuntu:22.04
WORKDIR app/
COPY . .
RUN apt-get update && apt-get install -y gcc 
RUN gcc main.c -o main
CMD ["./main"]	
