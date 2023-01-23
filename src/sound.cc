/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/sound.h"

#ifdef __APPLE__
#  include <cstdlib>
#  include <cstdio>
#  include <cmath>


bool Sound::initialized_ = false;
bool Sound::playing_ = false;
unsigned int Sound::hz_, Sound::samples_;
double Sound::theta_;
const unsigned int Sound::kSampleRate_ = 44100;
const double Sound::kPi_ = 3.14159265358979323846;
AudioStreamBasicDescription  Sound::stream_basic_description_;
AURenderCallbackStruct Sound::callback_struct_;

Sound::Sound() {
  if (!initialized_) {
    AudioComponent component;
    AudioComponentDescription component_description;

    // Select component as output audio unit
    component_description.componentType = kAudioUnitType_Output;

    // Configure component as default audio output unit
    component_description.componentSubType = kAudioUnitSubType_DefaultOutput;

    // Audio units from AUComponent.h must have this as manufacturer
    component_description.componentManufacturer = kAudioUnitManufacturer_Apple;

    // Apple developer documentation on AudioComponentDescription says set to 0
    component_description.componentFlags = 0;
    component_description.componentFlagsMask = 0;

    // Search for a component that matches description
    component = AudioComponentFindNext(NULL, &component_description);

    if (!component) {
      // Don't find a suitable audo component
      std::fprintf(stderr,
        "Could not find audio component matching description: \n"
        "  componentType = %u\n"
        "  componentSubType = %u\n"
        "  componentManufacturer = %u\n",
        static_cast<unsigned int>(component_description.componentType),
        static_cast<unsigned int>(component_description.componentSubType),
        static_cast<unsigned int>(component_description.componentManufacturer));

      std::exit(EXIT_FAILURE);
    }

    // Found a suitable audio component, gain access
    AudioComponentInstanceNew(component, &audio_output_unit_);

    // Audio data format in stream
    stream_basic_description_.mFormatID = kAudioFormatLinearPCM;

    // Flags for details of format
    stream_basic_description_.mFormatFlags = (
      kAudioFormatFlagsNativeFloatPacked | kAudioFormatFlagIsNonInterleaved);

    // Nyquist-Shannon: take sampling rate > 2*4816 Hz
    stream_basic_description_.mSampleRate = kSampleRate_;

    // Number of bits for one audio sample
    stream_basic_description_.mBitsPerChannel = 32;

    // Channels per audio frame
    stream_basic_description_.mChannelsPerFrame = 1;

    // (n channels)*(bytes per channel)
    stream_basic_description_.mBytesPerFrame = (
      4*stream_basic_description_.mChannelsPerFrame);

    // Frames per audio packet
    stream_basic_description_.mFramesPerPacket = 1;

    // Bytes per audio packet
    stream_basic_description_.mBytesPerPacket = (
      stream_basic_description_ .mBytesPerFrame
      *stream_basic_description_.mFramesPerPacket);

    // Padding to force 8-byte alignment; Apple developer docs says set to 0
    stream_basic_description_.mReserved = 0;

    AudioUnitSetProperty(
      audio_output_unit_,
      kAudioUnitProperty_StreamFormat,
      kAudioUnitScope_Input,
      0,
      &stream_basic_description_,
      sizeof(stream_basic_description_));

    // Set callback
    callback_struct_.inputProc = SoundCallback;
    AudioUnitSetProperty(
      audio_output_unit_,
      kAudioUnitProperty_SetRenderCallback,
      kAudioUnitScope_Input,
      0,
      &callback_struct_,
      sizeof(callback_struct_));

    // Requirements in place, initialize and start audio unit
    AudioUnitInitialize(audio_output_unit_);

    // We will be making sounds, initialize parameter to our (sine) wave
    theta_ = 0.;

    initialized_ = true;
  }
}

Sound::~Sound() {
  if (initialized_) {
    if (playing_) Stop();
    AudioUnitUninitialize(audio_output_unit_);
    AudioComponentInstanceDispose(audio_output_unit_);
  }
}

void Sound::Start(const unsigned int hz) {
  hz_ = hz;
  if (!playing_) {
    AudioOutputUnitStart(audio_output_unit_);
    playing_ = true;
  }
}

void Sound::Stop() {
  if (playing_) {
    AudioOutputUnitStop(audio_output_unit_);
    playing_ = false;
  }
}

OSStatus Sound::SoundCallback(
  void* in_ref_con,
  AudioUnitRenderActionFlags* io_action_flags,
  const AudioTimeStamp* in_timestamp,
  UInt32 in_bus_number,
  UInt32 in_number_frames,
  AudioBufferList* io_data) {
  // Unused
  static_cast<void>(in_ref_con);
  static_cast<void>(io_action_flags);
  static_cast<void>(in_timestamp);
  static_cast<void>(in_bus_number);

  if (playing_) {
    // Wave parameters for sine wave; frequency (`hz_`) set in Sound::Start
    const double amp = 0.25;
    double dtheta = 2.0*kPi_*hz_/kSampleRate_;  // increment

    // Audio stream configured as single channel so just a single buffer to fill
    Float32* bfr = reinterpret_cast<Float32*>(io_data->mBuffers[0].mData);

    // Compute wave value at sample points
    for (UInt32 frame = 0; frame < in_number_frames; frame++) {
      // y_i = A*sin(theta_i), theta_i = 2*pi*f*i/r
      // where f is the frequency, r is the sample rate
      bfr[frame] = amp*std::sin(theta_);
      theta_ += dtheta;

      // Keep theta in [0, 2*pi]
      theta_ -= theta_ > 2.0*kPi_ ? 2.0*kPi_ : 0;
    }
  }
  return noErr;
}

#else

#  include <cstdio>


void Sound::Start(const unsigned int dummy) {
  // This produces a bell sound on some terminals
  std::printf("\a");
}

void Sound::Stop() { }

#endif  // __APPLE__
