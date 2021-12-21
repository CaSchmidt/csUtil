#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <array>
#include <chrono>
#include <thread>

#include <csUtil/csSerial.h>

int main(int /*argc*/, char ** /*argv*/)
{
  using namespace std::chrono_literals;

  std::array<char,1024> buffer;
  constexpr rsize_t bufferSize = buffer.size() - 2;

  printf("device = ");
  buffer.fill(0);
  fscanf_s(stdin, "%s", buffer.data(), bufferSize); fflush(stdin);
  const std::u8string device = reinterpret_cast<char8_t*>(buffer.data());

  printf("rate = ");
  int rate;
  fscanf_s(stdin, "%d", &rate); fflush(stdin);

  printf("-------\n");
  printf("Device = %s\n", reinterpret_cast<const char*>(device.data()));
  printf("Rate   = %d\n", rate);
  printf("-------\n");

  csSerial serial;

  if( !serial.open(device, rate) ) {
    fprintf(stderr, "ERROR: Unable to open device!\n");
    return EXIT_FAILURE;
  }

  while( true ) {
    printf(": ");

    buffer.fill(0);
    fscanf_s(stdin, "%s", buffer.data(), bufferSize); fflush(stdin);
    if( strcmp(buffer.data(), "quit") == 0 ) {
      break;
    }

    buffer[strlen(buffer.data())] = '\n';
    serial.write(buffer.data(), strlen(buffer.data()));
    std::this_thread::sleep_for(2000ms);

    buffer.fill(0);
    serial.read(buffer.data(), bufferSize);
    printf("%s\n", buffer.data());
  }

  printf("DONE!\n");

  return EXIT_SUCCESS;
}
