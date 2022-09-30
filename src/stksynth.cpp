// crtsine.cpp STK tutorial program
//#include "SineWave.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <filesystem>
#include <unistd.h>
#include <RtAudio.h>
#include <RtMidi.h>
#include <ADSR.h>
#include "Config.h"
#include "Synth.h"


namespace fs = std::filesystem;
using namespace stk;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  Synth* synth = (Synth*) dataPointer;
  StkFloat *samples = (StkFloat *) outputBuffer;
  synth->tick(samples, 2, nBufferFrames);
  return 0;
}

void printAudioDetails(RtAudio* dac) {
  int nDevices = dac->getDeviceCount();
  std::cout << "The following devices were available:\n";
  for (int i = 0; i < nDevices; i++) {
    RtAudio::DeviceInfo deviceInfo = dac->getDeviceInfo(i);
    std::cout << "  device " << i << ": " << deviceInfo.name << "\n";
    std::cout << "    supported sample rates: ";
    for(unsigned int sampleRate : deviceInfo.sampleRates) {
      std::cout << sampleRate << " ";
    }
    std::cout << "\n";
    std::cout << "    preferred sample rate: " << deviceInfo.preferredSampleRate << "\n";
  }
  std::cout << "Sample rate of the device: " << dac->getStreamSampleRate() << "\n";
}

void task_user_input(int* flag, std::string configFilename, RtAudio* dac, Synth* synth) {
  std::string text;
  while (*flag) {
    std::cout << "Enter \"quit\" to quit:\n";
    std::cin >> text;
    if (text.compare("quit") == 0) {
      synth->stopRecording();
      *flag = 0;
    }
    if (text.compare("record") == 0) {
      synth->startRecording();
    }
    if (text.compare("stop") == 0) {
      synth->stopRecording();
    }
  }
  std::cout << "Quit stksynth.\n";
}

void task_watch_file(int* flag, std::string configFilename, RtAudio* dac, Synth* synth) {
  fs::path p = configFilename;
  auto lastWriteTime = std::filesystem::last_write_time(p);
  Config* config = new Config(configFilename);
  Config* newConfig = config;
  while (*flag) {
    usleep(1000000);
    auto newWriteTime = std::filesystem::last_write_time(p);
    if (newWriteTime != lastWriteTime) {
      int recording = synth->isRecording();
      if (recording) {
        synth->stopRecording();
      }
      dac->stopStream();
      try {
        newConfig = new Config(configFilename);
        synth->reload(newConfig);
        dac->startStream();
        delete config;
        config = newConfig;
      } catch (std::invalid_argument const&) {
        std::cout << "Reload failed. Use old config.\n";
        synth->reload(config);
        dac->startStream();
      }
      if (recording) {
        synth->startRecording();
      }
    } 
    lastWriteTime = newWriteTime;
  }
  delete newConfig;
}

void task_midi_buisiness(int* flag, RtMidiIn* midiin, Synth* synth) {
  std::vector<unsigned char> message;
  int nBytes;
  while (*flag) {
    usleep(1000);
    message.clear();
    midiin->getMessage(&message);
    nBytes = message.size();
    if (nBytes > 0) {
      std::cout << "midi: ";
      for (unsigned char byte : message) {
        std::cout << (int)byte << " ";
      }
      std::cout << "\n";
    }
    if (nBytes >= 3) {
      if ((message[0] >= 144) && (message[0] < 144 + 16)) {
          int channel = message[0] - 144;
          int midicode = message[1];
          int velocity = message[2];
          if (message[2] == 0) {
            message[0] = 128 + channel;
          } else {
            synth->setMidicode(channel, midicode);
            synth->setVelocity(channel, velocity);
            synth->noteOn(channel);
          }
      }
      if ((message[0] >= 128) && (message[0] < 128 + 16)) {
          int channel = message[0] - 128;
          synth->noteOff(channel);
      }
      if ((message[0] >= 224) && (message[0] < 224 + 16)) {
          int channel = message[0] - 224;
          stk::StkFloat bending = (stk::StkFloat)(((int)message[2]) * 127 + message[1] - 8192) / 8192.0;
          synth->setBending(channel, bending);
      }
    }
  }
}


int main(int argc, char** argv)
{
  std::srand(std::time(0));
  Config config;
  std::string configFilename;
//  fs::path p = configFilename;
//  std::chrono::time_point lastWriteTime = std::filesystem::last_write_time(p);
  if (argc == 2) {
    configFilename.assign(argv[1]);
    config = Config(configFilename);
    std::cout << "Configuration filename: " << configFilename << "\n";
  } else {
    std::cout << "No parameter file given.\n";
    return 0;
  }

  // Set the global sample rate before creating class instances.
  if (!config.name_occurs("sample-rate")) {
    throw std::invalid_argument("Parameter sample-rate not defined.");
  }
  unsigned int sampleRate = config.get_int("sample-rate");
  Stk::setSampleRate( sampleRate );
  std::cout << "STK sample rate: " << Stk::sampleRate() << "\n";
  RtAudio* dac = new RtAudio();
  RtMidiIn* midiin;
  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  RtAudio::StreamParameters parameters;
  if (!config.name_occurs("device-id")) {
    throw std::invalid_argument("No device-id specified.");
  }
  parameters.deviceId = config.get_int("device-id");
  parameters.nChannels = 2;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  Synth* synth = new Synth(&config);
  try {
    dac->openStream( &parameters, NULL, format, sampleRate, &bufferFrames, &tick, (void*) synth );
    printAudioDetails(dac);
  }
  catch ( RtAudioError &error ) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
 try {
    midiin = new RtMidiIn(RtMidi::UNSPECIFIED, "stksynth in");
    midiin->openVirtualPort();
  } catch (RtMidiError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  try {
    dac->startStream();
  }
  catch ( RtAudioError &error ) {
    error.printMessage();
    exit(EXIT_FAILURE);
  } 
 
  int flag = 1;

  std::thread thread_user_input(task_user_input, &flag, configFilename, dac, synth);
  std::thread thread_watch_file(task_watch_file, &flag, configFilename, dac, synth);
  std::thread thread_midi_buisiness(task_midi_buisiness, &flag, midiin, synth);
  thread_midi_buisiness.join();
  thread_watch_file.join();
  thread_user_input.join();

  try {
    dac->closeStream();
    delete dac;
  } catch ( RtAudioError &error ) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }


  try {
    midiin->closePort();
    delete midiin;
  } catch (RtMidiError &error) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }

  delete synth;

  return 0;
}

