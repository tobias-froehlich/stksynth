// crtsine.cpp STK tutorial program
//#include "SineWave.h"
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <thread>
#include <unistd.h>
#include "RtAudio.h"
#include "RtMidi.h"
#include "ADSR.h"
#include "Synth.h"

using namespace stk;

// This tick() function handles sample computation only.  It will be
// called automatically when the system needs a new buffer of audio
// samples.
int tick( void *outputBuffer, void *inputBuffer, unsigned int nBufferFrames,
         double streamTime, RtAudioStreamStatus status, void *dataPointer )
{
  Synth* synth = (Synth*) dataPointer;
  register StkFloat *samples = (StkFloat *) outputBuffer;
  for ( unsigned int i=0; i<nBufferFrames; i++ ) {
    *samples = synth->tick();
    *(samples + 1) = *samples;
    samples += 2;
  }

  return 0;
}

void task_user_input(int* flag) {
  std::string text;
  while (*flag) {
    std::cin >> text;
    std::cout << text << *flag << "\n";
    if (text.compare("quit") == 0) {
      *flag = 0;
    }  
  }
  std::cout << "zuende!";
}

void task_midi_buisiness(int* flag, RtMidiIn* midiin, Synth* synth) {
  std::vector<unsigned char> message;
  int nBytes;
  while (*flag) {
    usleep(1000);
    message.clear();
    midiin->getMessage(&message);
    nBytes = message.size();
    if (nBytes >= 3) {
      if ((message[0] >= 144) && (message[0] < 144 + 16)) {
          int channel = message[0] - 144;
          int midicode = message[1];
          if (message[2] == 0) {
            message[0] = 128 + channel;
          } else {
            synth->setMidicode(channel, midicode);
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
          synth->setBending(channel, bending * 12);
      }
    }
  }
}

int main(int argc, char** argv)
{
  // Set the global sample rate before creating class instances.
  Stk::setSampleRate( 44100.0 );
  RtAudio dac;
  RtMidiIn* midiin;
  // Figure out how many bytes in an StkFloat and setup the RtAudio stream.
  RtAudio::StreamParameters parameters;
  parameters.deviceId = 2; //dac.getDefaultOutputDevice();
  std::cout << parameters.deviceId; 
  parameters.nChannels = 2;
  RtAudioFormat format = ( sizeof(StkFloat) == 8 ) ? RTAUDIO_FLOAT64 : RTAUDIO_FLOAT32;
  unsigned int bufferFrames = RT_BUFFER_SIZE;
  Synth synth;
  try {
    dac.openStream( &parameters, NULL, format, (unsigned int)Stk::sampleRate(), &bufferFrames, &tick, (void*) &synth );
  }
  catch ( RtAudioError &error ) {
    error.printMessage();
    exit(EXIT_FAILURE);
  }
  try {
    dac.startStream();
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
  // Block waiting here.
  int flag = 1;

  std::thread thread_user_input(task_user_input, &flag);
  std::thread thread_midi_buisiness(task_midi_buisiness, &flag, midiin, &synth);
  thread_user_input.join();
  thread_midi_buisiness.join();
  

  try {
    dac.closeStream();
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

  return 0;
}

