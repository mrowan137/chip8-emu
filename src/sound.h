/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#ifndef SRC_SOUND_H_
#define SRC_SOUND_H_

#ifdef __APPLE__
#  include <AudioUnit/AudioUnit.h>


class Sound {
 public:
  void Start(const unsigned int hz);
  void Stop();
  Sound();
  ~Sound();

 private:
  AudioUnit audio_output_unit_;
  static bool initialized_;
  static bool playing_;
  static AudioStreamBasicDescription stream_basic_description_;
  static AURenderCallbackStruct callback_struct_;
  static unsigned int hz_, samples_;
  static double theta_;
  static const unsigned int kSampleRate_;
  static const double kPi_;
  static OSStatus SoundCallback(
    void* in_ref_con,
    AudioUnitRenderActionFlags* io_action_flags,
    const AudioTimeStamp* in_timestamp,
    UInt32 in_bus_number,
    UInt32 in_number_frames,
    AudioBufferList * io_data);
};

#else

class Sound {
 public:
  void Start(const unsigned int dummy);
  void Stop();
};

#endif  // __APPLE__
#endif  // SRC_SOUND_H_
