/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/chip8.h"

#include <utility>

#include "src/display.h"


// System architecture constants
const int Chip8::kMemorySize_ = 0x1000;  // RAM
const int Chip8::kRegistersSize_ = 16;  // Registers
const int Chip8::kStackSize_ = 16;  // Interpreter return stack

// Graphics constants
const int Chip8::kRows_ = 32;  // Rows of pixel buffer
const int Chip8::kCols_ = 64;  // Columns of pixel buffer
const int Chip8::kPixelSize_ = 15;  // Size of pixel in display buffer
const int Chip8::kChannels_ =  3;  // RGB
const int Chip8::kFramesPerSecond_ = 60;  // Choose to make games playable
const double Chip8::kSecondsPerFrame_
  = static_cast<double>(1.)/static_cast<double>(kFramesPerSecond_);
clock_t Chip8::then_ = clock();

// Font sprites
const int Chip8::kFontSpritesAddress_ = 0x00;
const int Chip8::kBytesPerFontSprite_ = 5;
const int Chip8::kNumFontSprites_ = 16;
const uint8_t Chip8::kFontSprites_[kBytesPerFontSprite_*kNumFontSprites_] = {
  0xF0, 0x90, 0x90, 0x90, 0xF0,  // 0
  0x20, 0x60, 0x20, 0x20, 0x70,  // 1
  0xF0, 0x10, 0xF0, 0x80, 0xF0,  // 2
  0xF0, 0x10, 0xF0, 0x10, 0xF0,  // 3
  0x90, 0x90, 0xF0, 0x10, 0x10,  // 4
  0xF0, 0x80, 0xF0, 0x10, 0xF0,  // 5
  0xF0, 0x80, 0xF0, 0x90, 0xF0,  // 6
  0xF0, 0x10, 0x20, 0x40, 0x40,  // 7
  0xF0, 0x90, 0xF0, 0x90, 0xF0,  // 8
  0xF0, 0x90, 0xF0, 0x10, 0xF0,  // 9
  0xF0, 0x90, 0xF0, 0x90, 0x90,  // A
  0xE0, 0x90, 0xE0, 0x90, 0xE0,  // B
  0xF0, 0x80, 0x80, 0x80, 0xF0,  // C
  0xE0, 0x90, 0x90, 0x90, 0xE0,  // D
  0xF0, 0x80, 0xF0, 0x80, 0xF0,  // E
  0xF0, 0x80, 0xF0, 0x80, 0x80   // F
};

// Program
const int Chip8::kProgramAddress_ = 0x0200;
const int Chip8::kMaxProgramSize_ = kMemorySize_ - kProgramAddress_;

Chip8::Chip8() {
  // System architecture
  memory_ = new uint8_t[kMemorySize_]();
  v_ = new uint8_t[kRegistersSize_]();
  index_ = 0;
  pc_ = 0x200;
  sp_ = 0;
  stack_ = new uint16_t[kStackSize_]();
  delay_timer_ = 0;
  sound_timer_ = 0;

  // System configuration
  speed_  = 18;
  wrap_around_y_ = false;
  foreground_red_pixel_value_
    = foreground_green_pixel_value_
    = foreground_blue_pixel_value_
    = 255;
  background_red_pixel_value_
    = background_green_pixel_value_
    = background_blue_pixel_value_
    = 0;

  // Load font sprites into memory
  LoadFontSprites();

  // Keyboard
  keyboard_ = new Keyboard();

  // Sound
  sound_ = new Sound();
  sound_->Start(0);  // Warmup
  sound_->Stop();

  // Graphics
  pixel_buffer_ = new uint8_t*[kRows_]();
  for (int i = 0; i < kRows_; ++i) pixel_buffer_[i] = new uint8_t[kCols_]();
  display_buffer_ = new uint8_t[(kRows_*kPixelSize_)
                                *(kCols_*kPixelSize_)*kChannels_]();
  display_ = new Display(kRows_*kPixelSize_,
                         kCols_*kPixelSize_,
                         display_buffer_,
                         keyboard_);
}

void Chip8::Run(const std::string& path_to_rom) {
  // Load program into memory
  LoadProgram(path_to_rom);

  while (!display_->ShouldClose()) {
    Step();  // Step program, update display buffer
  }
}

Chip8::~Chip8() {
  // Cleanup
  // Memory
  delete[] memory_;

  // Register
  delete[] v_;
  delete[] stack_;

  // Keyboard
  delete keyboard_;

  // Sound
  delete sound_;

  // Display
  for (int i = 0; i < kRows_; ++i) delete[] pixel_buffer_[i];
  delete[] pixel_buffer_;
  delete[] display_buffer_;
  delete display_;
}

inline void Chip8::UnknownInstruction(const uint16_t opcode) {
  std::fprintf(stderr, "Unknown opcode: 0x%X\n", opcode);
  std::exit(EXIT_FAILURE);
}

void Chip8::InterpretInstruction(const uint16_t opcode) {
  if (DEBUG) {
    printf("[Press ENTER key to step]\n");
    getchar();
  }

  // Extract values from (16-bit) opcode
  const uint8_t  x   = (opcode >> 8) & 0x000F;  //  4 lower bits of upper byte
  const uint8_t  y   = (opcode >> 4) & 0x000F;  //  4 upper bits of lower byte
  const uint8_t  n   = opcode        & 0x000F;  //  4 lowest bits
  const uint8_t  kk  = opcode        & 0x00FF;  //  8 lowest bits
  const uint16_t nnn = opcode        & 0x0FFF;  // 12 lowest bits

  DebugMessage("      CURRENT STATE      \n"
               "    -----------------    \n"
               "    pc      |  0x%04X    \n"
               "    opcode  |  0x%04X    \n"
               "    sp      |  0x  %02X  \n"
               "    index   |  0x%04X    \n"
               "    x       |  0x %01X   \n"
               "    y       |  0x  %01X  \n"
               "    n       |  0x   %01X \n"
               "    kk      |  0x  %02X  \n"
               "    nnn     |  0x %03X   \n"
               "    Vx      |  0x  %02X  \n"
               "    Vy      |  0x  %02X  \n"
               "                         \n",
               pc_, opcode, sp_, index_, x, y, n, kk, nnn, v_[x], v_[y]);

  // opcode 0x#### is 4*4 bits = 2 bytes long;
  // increment program counter by one opcode
  pc_ += 2;

  switch (opcode & 0xF000) {
    case 0x0000: {  // 0x0kk
      switch (kk) {
        case 0x00E0: {  // 00E0: CLR
          DebugMessage(
            "[0x00E0: CLR]\n"
            "    Clear screen.\n");
          ClearPixelBuffer();
          break;
        }
        case 0x00EE: {  // 00EE: RET
          DebugMessage(
            "[0x00EE: RET]\n"
            "    Return: set sp -= 1 = 0x%02X; pc = stack[sp] = 0x%04X.\n",
            sp_ - 1,
            stack_[sp_ - 1]);
          pc_ = stack_[--sp_];
          break;
        }
        default: UnknownInstruction(opcode);
      }
      break;
    }
    case 0x1000: {  // 1nnn: JP addr
      DebugMessage(
        "[0x1nnn: JP addr]\n"
        "    Jump to address nnn: set pc = 0x%04X.\n",
        nnn);
      pc_ = nnn;
      break;
    }
    case 0x2000: {  // 2nnn: CALL addr
      DebugMessage(
        "[0x2nnn: CALL addr]\n"
        "    Call address nnn: stack[sp] = pc = 0x%04X;\n"
        "    sp += 1 = 0x%02X; pc = 0x%04X.\n",
        pc_, sp_ + 1,
        nnn);
      stack_[sp_++] = pc_;
      pc_ = nnn;
      break;
    }
    case 0x3000: {  // 3xkk: SE Vx, byte
      DebugMessage(
        "[0x3xkk: SE Vx, byte]\n"
        "    Skip next instruction if Vx == kk: set pc += %d\n"
        "    because 0x%02X %c= 0x%02X.\n",
        (v_[x] == kk) ? 2 : 0,
        v_[x], (v_[x] == kk) ? '=' : '!', kk);
      pc_ += (v_[x] == kk) ? 2 : 0;
      break;
    }
    case 0x4000: {  // 4xkk: SNE Vx, byte
      DebugMessage(
        "[0x4xkk: SNE Vx, byte]\n"
        "    Skip next instruction if Vx != kk: set pc += %d\n"
        "    because 0x%02X %c= 0x%02X.\n",
        (v_[x] != kk) ? 2 : 0,
        v_[x], (v_[x] != kk) ? '!' : '=', kk);
      pc_ += (v_[x] != kk) ? 2 : 0;
      break;
    }
    case 0x5000: {  // 5xy0: SE Vx, Vy
      DebugMessage(
        "[0x5xy0: SE Vx, Vy]\n"
        "    Skip next instruction if Vx == Vy: set pc += %d\n"
        "    because 0x%02X %c= 0x%02X.\n",
        (v_[x] == v_[y]) ? 2 : 0,
        v_[x], (v_[x] == v_[y]) ? '=' : '!', v_[y]);
      pc_ += (v_[x] == v_[y]) ? 2 : 0;
      break;
    }
    case 0x6000: {  // 6xkk: LD Vx, byte
      DebugMessage(
        "[0x6xkk: LD Vx, byte]\n"
        "    Load byte kk into register Vx: set Vx = 0x%02X.\n", kk);
      v_[x] = kk;
      break;
    }
    case 0x7000: {  // 7xkk: ADD Vx, byte
      DebugMessage(
        "[0x7xkk: ADD Vx, byte]\n"
        "    Load Vx + kk into register Vx: set Vx += 0x%02X = 0x%02X.\n",
        kk, v_[x] + kk);
      v_[x] += kk;
      break;
    }
    case 0x8000: {  // 8xyn
      switch (n) {
        case 0x0000: {  // 8xy0: LD Vx, Vy
          DebugMessage(
            "[0x8xy0: LD Vx, Vy]\n"
            "    Load Vy into register Vx: set Vx = 0x%02X.\n",
            v_[y]);
          v_[x] = v_[y];
          break;
        }
        case 0x0001: {  // 8xy1: OR Vx, Vy
          DebugMessage(
            "[0x8xy1: OR Vx, Vy]\n"
            "    Load Vx | Vy into register Vx:\n"
            "    set Vx = 0x%02X | 0x%02X = 0x%02X.\n",
            v_[x], v_[y], v_[x] | v_[y]);
          v_[x] |= v_[y];
          break;
        }
        case 0x0002: {  //  8xy2: AND Vx, Vy
          DebugMessage(
            "[0x8xy2: AND Vx, Vy]\n"
            "    Load Vx & Vy into register Vx:\n"
            "    set Vx = 0x%02X & 0x%02X = 0x%02X.\n",
            v_[x], v_[y], v_[x] & v_[y]);
          v_[x] &= v_[y];
          break;
        }
        case 0x0003: {  // 8xy3: XOR Vx, Vy
          DebugMessage(
            "[0x8xy3: XOR Vx, Vy]\n"
            "    Load Vx ^ Vy into register Vx:\n"
            "    set Vx = 0x%02X ^ 0x%02X = 0x%02X.\n",
            v_[x], v_[y], v_[x] ^ v_[y]);
          v_[x] ^= v_[y];
          break;
        }
        case 0x0004: {  // 8xy4: ADD Vx, Vy
          DebugMessage(
            "[0x8xy4: ADD Vx, Vy]\n"
            "    Store Vx + Vy into register Vx: store carry VF = %d\n"
            "    and set Vx = 0x%02X + 0x%02X = 0x%02X.\n",
            ((static_cast<int>(v_[x]) + static_cast<int>(v_[y])) > 255) ? 1 : 0,
            v_[x], v_[y], v_[x] + v_[y]);
          v_[0xF] = ((static_cast<int>(v_[x]) + static_cast<int>(v_[y])) > 255)
                    ? 1 : 0;
          v_[x] = v_[x] + v_[y];
          break;
        }
        case 0x0005: {  // 8xy5: SUB Vx, Vy
          DebugMessage(
            "[0x8xy5: SUB Vx, Vy]\n"
            "    Store Vx - Vy into register Vx: store carry VF = %d\n"
            "    because Vx %c Vy and set Vx = 0x%02X - 0x%02X = 0x%02X.\n",
            (v_[x] > v_[y]) ?  1 : 0, (v_[x] > v_[y]) ?  '>' : '<',
            v_[x], v_[y], v_[x] - v_[y]);
          v_[0xF] = (v_[x] > v_[y]) ? 1 : 0;
          v_[x] = v_[x] - v_[y];
          break;
        }
        case 0x0006: {  // 8xy6: SHR Vx {, Vy}
          DebugMessage(
            "[0x8xy6: SHR Vx{, Vy}]\n"
            "    Shift right Vx one bit: store least significant bit VF = %d\n"
            "    and set Vx = (0x%02X >> 1) = 0x%02X.\n",
            v_[x] & 0x01,
            v_[x], v_[x] >> 1);
          v_[0xF] = v_[x] & 0x01;
          v_[x] >>= 1;
          break;
        }
        case 0x0007: {  // 8xy7: SUBN Vx, Vy
          DebugMessage(
            "[0x8xy7: SUBN Vx, Vy]\n"
            "    Store Vy - Vx into register Vx: store carry VF = %d\n"
            "    because Vy %c Vx and set Vx = 0x%02X - 0x%02X = 0x%02X.\n",
            (v_[y] > v_[x]) ?  1 : 0, (v_[y] > v_[x]) ?  '>' : '<',
            v_[y], v_[x], v_[y] - v_[x]);
          v_[0xF] = (v_[y] > v_[x]) ? 1 : 0;
          v_[x] = v_[y] - v_[x];
          break;
        }
        case 0x000E: {  // 8xyE: SHL Vx{, Vy}
          DebugMessage(
            "[0x8xyE: SHL Vx{, Vy}]\n"
            "    Shift left Vx one bit: store most significant bit VF = %d\n"
            "    and set Vx = (0x%02X << 1) = 0x%02X.\n",
            v_[x] & 0x80,
            v_[x], v_[x] << 1);
          v_[0xF] = v_[x] & 0x80;
          v_[x] <<= 1;
          break;
        }
        default: UnknownInstruction(opcode);
      }
      break;
    }
    case 0x9000: {  // 9xy0: SNE Vx, Vy
      DebugMessage(
        "[0x9xy0: SNE Vx, Vy]\n"
        "    Skip next instruction if Vx != Vy: set pc += %d\n"
        "    because 0x%02X %c= 0x%02X.\n",
        (v_[x] != v_[y]) ? 2 : 0,
        v_[x], (v_[x] != v_[y]) ? '!' : '=', v_[y]);
      pc_ += (v_[x] != v_[y]) ? 2 : 0;
      break;
    }
    case 0xA000: {  // Annn: LD I, addr
      DebugMessage(
        "[0xAnnn: LD I, addr]\n"
        "    Load address nnn into register I: set index = 0x%03X.\n",
        nnn);
      index_ = nnn;
      break;
    }
    case 0xB000: {  // Bnnn: JP V0, addr
      DebugMessage(
        "[0xBnnn: JP V0, addr]\n"
        "    Jump to address nnn + V0: set pc = 0x%03X + 0x%02X = 0x%04X.\n",
        nnn, v_[0], nnn + v_[0]);
      pc_ = nnn + v_[0];
      break;
    }
    case 0xC000: {  // Cxkk: RND Vx, byte
      unsigned int seed = time(NULL);
      uint8_t res = (rand_r(&seed) % 0x00FF) & kk;
      DebugMessage(
        "[0xCxkk: RND Vx, byte]\n"
        "    Load random byte & kk into register Vx:\n"
        "    set Vx = (rand_r(&seed) % 0x00FF) & 0x%02X = 0x%02X.\n",
        kk, res);
      v_[x] = res;
      break;
    }
    case 0xD000: {  // Dxyn: DRW Vx, Vy, nibble
      uint8_t collision = DrawSpriteToPixelBuffer(v_[y], v_[x], n) ? 1 : 0;
      DebugMessage(
        "[0xDxyn: DRW Vx, Vy, nibble]\n"
        "    Draw %d-byte sprite starting at memory location I = 0x%03X\n"
        "    at (Vx, Vy) = (0x%02X, 0x%02X); set VF = %d (collision).\n",
        n,
        index_,
        v_[x], v_[y], collision);
      v_[0xF] = collision;
      break;
    }
    case 0xE000: {  // Exkk
      switch (kk) {
        case 0x009E: {  // Ex9E: SKP Vx
          DebugMessage(
            "[0xEx9E: SKP Vx]\n"
            "    Skip next instruction if key with value Vx is pressed:\n"
            "    set pc += %d because Vx = 0x%02X\n"
            "    and KeyIsPressed(0x%02X) == %s.\n",
            keyboard_->KeyIsPressed(v_[x]) ? 2 : 0, v_[x],
            v_[x], keyboard_->KeyIsPressed(v_[x]) ? "true" : "false");
          if (keyboard_->KeyIsPressed(v_[x])) pc_ += 2;
          break;
        }
        case 0x00A1: {  // ExA1: SKNP Vx
          DebugMessage(
            "[0xExA1: SKNP Vx]\n"
            "    Skip next instruction if key with value Vx is not pressed:\n"
            "    set pc += %d because Vx = 0x%02X\n"
            "    and KeyIsPressed(0x%02X) == %s.\n",
            !keyboard_->KeyIsPressed(v_[x]) ? 2 : 0, v_[x],
            v_[x], keyboard_->KeyIsPressed(v_[x]) ? "true" : "false");
          if (!keyboard_->KeyIsPressed(v_[x])) pc_ += 2;
          break;
        }
        default: UnknownInstruction(opcode);
      }
      break;
    }
    case 0xF000: {
      switch (kk) {  // Fxkk
        case 0x0007: {  // Fx07: LD Vx, DT
          DebugMessage(
            "[0xFx07: LD Vx, DT]\n"
            "    Load delay timer into register Vx:\n"
            "    set Vx = delay_timer = 0x%02X.\n",
            delay_timer_);
          v_[x] = delay_timer_;
          break;
        }
        case 0x000A: {  // Fx0A: LD Vx, K
          DebugMessage(
            "[0xFx0A: LD Vx, K]\n"
            "    Wait for any keypress and store into register Vx:\n");
          while (true) {
            DrawPixelsToDisplayBuffer();
            display_->Paint();  // So we always see the updated screen while
                                // waiting for input; Paint will poll events

            if (display_->ShouldClose()) goto get_out_of_here;

            for (std::pair<uint8_t, bool> kp : keyboard_->key_pressed_flag_) {
              if (kp.second == true) {
                v_[x] = kp.first;
                DebugMessage("    stored 0x%02X into register Vx.\n", kp.first);
                goto get_out_of_here;
              }
            }
          }
          get_out_of_here:
          break;
        }
        case 0x0015: {  // Fx15: LD DT, Vx
          DebugMessage(
            "[0xFx15: LD DT, Vx]\n"
            "    Set delay timer to value stored in register Vx:\n"
            "    set delay_timer = Vx = 0x%02X.\n",
            v_[x]);
          delay_timer_ = v_[x];
          break;
        }
        case 0x0018: {  // Fx18: LD ST, Vx
          DebugMessage(
            "[0xFx18: LD ST, Vx]\n"
            "    Set sound timer to value stored in register Vx:\n"
            "    set sound_timer = Vx = 0x%02X.\n",
            v_[x]);
          sound_timer_ = v_[x];
          break;
        }
        case 0x001E: {  // Fx1E: ADD I, Vx
          DebugMessage(
            "[0xFx1E: ADD I, Vx]\n"
            "    Add value store in Vx to register I:\n"
            "    set index += 0x%02X = 0x%03X.\n",
            v_[x], index_ + v_[x]);
          index_ += v_[x];
          break;
        }
        case 0x0029: {  // Fx29: LD F, Vx
          DebugMessage(
            "[0xFx29: LD F, Vx]\n"
            "    Set register I to location of sprite for digit Vx:\n"
            "    set index = 0x%02X*0x%02X = 0x%03X.\n",
            kBytesPerFontSprite_, v_[x], kBytesPerFontSprite_*v_[x]);
          index_ = kBytesPerFontSprite_*v_[x];
          break;
        }
        case 0x0033: {  // Fx33: LD B, Vx
          DebugMessage(
            "[0xFx33: LD B, Vx]\n"
            "    Load Binary Coded Decimal representation of Vx in memory:\n"
            "    memory[0x%03X] = 0x%02X\n"
            "    memory[0x%03X] = 0x%02X\n"
            "    memory[0x%03X] = 0x%02X.\n",
            index_    , (v_[x]/100)%10,
            index_ + 1, (v_[x]/ 10)%10,
            index_ + 2, (v_[x]/  1)%10);
          memory_[index_    ] = (v_[x]/100)%10;  // Vx hundreds digit store at I
          memory_[index_ + 1] = (v_[x]/ 10)%10;  // Vx tens digit store at I+1
          memory_[index_ + 2] = (v_[x]/  1)%10;  // Vx ones digit store at I+2
          break;
        }
        case 0x0055: {  // Fx55: LD [I], Vx
          DebugMessage(
            "[0xFx55: LD [I], Vx]\n"
            "    Store registers V0 through Vx in memory\n"
            "    starting at I = 0x%03X:",
            index_);
          for (int i = 0; i <= x; ++i) {
            DebugMessage(
              "\n    memory[0x%03X] = V%d = 0x%02X",
              index_ + i, i, v_[i]);
          }
          DebugMessage(".\n");

          for (int i = 0; i <= x; ++i) memory_[index_ + i] = v_[i];
          break;
        }
        case 0x0065: {  // Fx65: LD Vx, [I]
          DebugMessage(
            "[0xFx65: LD Vx, [I] ]\n"
            "    Load from memory into registers V0 through Vx\n"
            "    starting at I = 0x%03X:",
            index_);
          for (int i = 0; i <= x; ++i) {
            DebugMessage(
              "\n    V%d = memory[0x%03X] = 0x%02X",
              i, index_ + i, memory_[index_ + i]);
          }
          DebugMessage(".\n");

          for (int i = 0; i <= x; ++i) v_[i] = memory_[index_ + i];
          break;
        }
        default: UnknownInstruction(opcode);
      }
      break;
    }
    default: UnknownInstruction(opcode);
  }
  DebugScreen();
}

bool Chip8::TogglePixel(int i, int j) {
  // Chip-8 specification does not say whether to wrap around in y,
  // some ROMs are written assuming yes and others no
  if (!wrap_around_y_) {
    if (i < 0 || i >= kRows_) return false;
  }

  // Remember: C's % is division remainder, not modulo
  i = (kRows_ + (i%kRows_))%kRows_;
  j = (kCols_ + (j%kCols_))%kCols_;

  pixel_buffer_[i][j] ^= 1;
  return pixel_buffer_[i][j] != 1;
}

void Chip8::ClearPixelBuffer() {
  for (int i = 0; i < kRows_; ++i) {
    for (int j = 0; j < kCols_; ++j) {
      pixel_buffer_[i][j] = 0;
    }
  }
}

void Chip8::ClearDisplayBuffer() {
  for (int i = 0;
       i < (kRows_*kPixelSize_)*(kCols_*kPixelSize_);
       ++i
  ) {
    display_buffer_[i*kChannels_ + 0] = background_red_pixel_value_;
    display_buffer_[i*kChannels_ + 1] = background_green_pixel_value_;
    display_buffer_[i*kChannels_ + 2] = background_blue_pixel_value_;
  }
}

void Chip8::DrawPixelToDisplayBuffer(const int& i, const int& j) {
  // Draw pixel at (i, j) in the pixel buffer to the display buffer
  int m0 = i*kPixelSize_, n0 = j*kPixelSize_;
  if (pixel_buffer_[i][j] == 1) {
    for (int dm = 0; dm < kPixelSize_; ++dm) {
      for (int dn = 0; dn < kPixelSize_; ++dn) {
        int idx = ((m0 + dm)*kCols_*kPixelSize_*kChannels_
                   + (n0 + dn)*kChannels_);
        display_buffer_[idx + 0] = foreground_red_pixel_value_;
        display_buffer_[idx + 1] = foreground_green_pixel_value_;
        display_buffer_[idx + 2] = foreground_blue_pixel_value_;
      }
    }
  }
}

bool Chip8::DrawSpriteToPixelBuffer(
  const uint8_t& i0, const uint8_t& j0, const uint8_t& n
) {
  const int sprite_width = 8, sprite_height = n;
  bool collision_flag = false;
  for (int di = 0; di < sprite_height; ++di) {
    uint8_t byte_of_sprite = memory_[index_ + di];
    for (int dj = 0; dj < sprite_width; ++dj) {
      if ( (byte_of_sprite << dj) & 0x80 ) {
        collision_flag |= TogglePixel(i0 + di, j0 + dj);
      }
    }
  }
  return collision_flag;
}

void Chip8::DrawPixelsToDisplayBuffer() {
  ClearDisplayBuffer();
  for (int i = 0; i < kRows_; ++i) {
    for (int j = 0; j < kCols_; ++j) DrawPixelToDisplayBuffer(i, j);
  }
}

void Chip8::LoadFontSprites() {
  for (int i = 0; i < kBytesPerFontSprite_*kNumFontSprites_; ++i) {
    memory_[kFontSpritesAddress_ + i] = kFontSprites_[i];
  }
}

void Chip8::UpdateTimers() {
  delay_timer_ -= (delay_timer_ > 0) ? 1 : 0;
  sound_timer_ -= (sound_timer_ > 0) ? 1 : 0;
}

void Chip8::EmulateCycle() {
  for (int i = 0; i < speed_; ++i) {
    uint16_t opcode = (memory_[pc_] << 8 | memory_[pc_ + 1]);
    InterpretInstruction(opcode);
  }

  UpdateTimers();

  PlaySound();

  Paint();
}

void Chip8::LoadProgram(const std::string& path_to_rom) {
  FILE* program = std::fopen(path_to_rom.c_str(), "rb");
  if (!program) {
    std::fprintf(
      stderr,
      "In Chip8::LoadProgram: file does not exist: %s\n", path_to_rom.c_str());
    std::exit(EXIT_FAILURE);
  }

  std::fread(&memory_[kProgramAddress_], 1, kMaxProgramSize_, program);

  std::fclose(program);
}

void Chip8::LoadProgram(const std::string& path_to_rom,
                        unsigned char* buffer) {
  // overload reads the rom into a buffer it is given
  FILE* program = std::fopen(path_to_rom.c_str(), "rb");
  if (!program) {
    std::fprintf(
      stderr,
      "In Chip8::LoadProgram: file does not exist: %s\n", path_to_rom.c_str());
    std::exit(EXIT_FAILURE);
  }

  std::fread(&buffer[0], 1, kMaxProgramSize_, program);

  std::fclose(program);
}

void Chip8::Step() {
  clock_t now = clock();
  clock_t elapsed_cycles = now - then_;
  double elapsed_seconds
    = static_cast<double>(elapsed_cycles)/static_cast<double>(CLOCKS_PER_SEC);
  if (elapsed_seconds >= kSecondsPerFrame_) {
    EmulateCycle();
    then_ = clock();
  }
}

void Chip8::PlaySound() {
  if (sound_timer_ > 0) {
    sound_->Start(440);
  } else {
    sound_->Stop();
  }
}

void Chip8::Paint() {
  DrawPixelsToDisplayBuffer();
  display_->Paint();
}
