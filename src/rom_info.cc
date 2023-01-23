/* Copyright 2022 Michael E. Rowan
 *
 * This file is part of Chip8-Emu.
 *
 * License: MIT
 */
#include "src/rom_info.h"


std::map<std::string, std::string> RomInfo::info_(
  {
    {
      "ca4545bc229cf4b20d15556f53a995eaa40ee466",
      "  Title: 15 Puzzle\n"
      "  Author: Roger Ivie\n"
    },
    {
      "a4cf28364424ec41d42de81b15812684b611c4ae",
      "  Title: 1D Cellular Automata\n"
      "  Author: SharpenedSpoon\n"
    },
    {
      "a02a2cd142bfde54897ba1036c58927e2b62eb37",
      "  Title: 8CE Attourny - Disc 1\n"
      "  Author: SystemLogoff\n"
    },
    {
      "5697e00978837f57e8ec1dce8084c60bdde36843",
      "  Title: 8CE Attourny - Disc 2\n"
      "  Author: SystemLogoff\n"
    },
    {
      "4d0b047231d8fc8484cf9cac24be1afb4b7252a4",
      "  Title: 8CE Attourny - Disc 3\n"
      "  Author: SystemLogoff\n"
    },
    {
      "253f14d11b8ba09aea2941b97ba7cb12c2950f6a",
      "  Title: Bad Kaiju Ju\n"
      "  Author: MattBooth\n"
    },
    {
      "5776bd8043a91ea637e3bf76be0c38e02d72c94e",
      "  Title: Blinky\n"
      "  Author: Hans Christian Egeberg\n"
    },
    {
      "9362b529288fc38205bcf653f41b5a30238f2cb9",
      "  Title: Blitz\n"
      "  Author: David Winter\n"
    },
    {
      "fc9b7da43ab9eca090bcef7bad03f35c72e11844",
      "  Title: Br8kout\n"
      "  Author: SharpenedSpoon\n"
    },
    {
      "8ae409448b4639886be83373a09b2494d1bd134a",
      "  Title: Brix\n"
      "  Author: Andreas Gustafsson\n"
    },
    {
      "ada2b31ca146b0f15c1fa88aa8b7618b89bebd56",
      "  Title: Carbon8\n"
      "  Author: Chromatophore\n"
    },
    {
      "d840ff389e4801741893d3a2ccd2665bf43fd1ac",
      "  Title: Cave Explorer\n"
      "  Author: John Earnest\n"
    },
    {
      "d8c6f2a6243cc5f7e922091f98e24c8cdb8af630",
      "  Title: chip8-test-rom\n"
      "  Author: corax89\n"
    },
    {
      "f9335110fa6c33a5d34989a77c7df2f26d08761d",
      "  Title: Chipquarium\n"
      "  Author: mattmik\n"
    },
    {
      "5a8a87bc2f4774f6ea7ed239e485973e06e8be7e",
      "  Title: ChipWar\n"
      "  Author: John Earnest\n"
    },
    {
      "8c7560cd0b5f54e5356e927c6bff7bdbf754331f",
      "  Title: Connect 4\n"
      "  Author: David Winter\n"
    },
    {
      "93f328f2d428fce08239825616d429dfbfbd4c1a",
      "  Title: Danm8ku\n"
      "  Author: buffi\n"
    },
    {
      "1818d0bf5242e3fb6bf0a4faedcb850430a00166",
      "  Title: down8\n"
      "  Author: tinaun\n"
    },
    {
      "a9fa3e5cb9b194645ddf18f6f8dff82baa26c0d4",
      "  Title: Flight Runner\n"
      "  Author: TodPunk\n"
    },
    {
      "2873ae9b8b9c8f49c0b7b9c3d2bdc3cead618f32",
      "  Title: Fuse\n"
      "  Author: John Earnest\n"
    },
    {
      "301ada55eeb83dfbfa3954cf2f7c3e0a7666951d",
      "  Title: Ghost Escape\n"
      "  Author: TomRintjema\n"
    },
    {
      "d1e03a902dfad10f0ca1b33b6a7f372be27bc908",
      "  Title: Glitch Ghost\n"
      "  Author: JackieKircher\n"
    },
    {
      "01992f0410657974298ae6f03e9646c4c99a4d1c",
      "  Title: Guess\n"
      "  Author: David Winter\n"
    },
    {
      "355c9be05b0b7bba670398677ea257d469e1adc7",
      "  Title: Hidden\n"
      "  Author: David Winter\n"
    },
    {
      "35b0e7caf03c3350838a291cdf7b6b3c072421ad",
      "  Title: Horse World Online\n"
      "  Author: TomRintjema\n"
    },
    {
      "6b438c78c7cdf65d38a2beb8d9da21c12fbaf384",
      "  Title: IBM\n"
      "  Author: Unknown\n"
    },
    {
      "a3c6e05bee1f492f3ffa20dd85c8204ba5a45144",
      "  Title: Kaleidoscope\n"
      "  Author: Joseph Weisbecker\n"
    },
    {
      "10cf5223063036d8634064d128a4fe4d39e63d06",
      "  Title: Masquer8\n"
      "  Author: Chromatophore\n"
    },
    {
      "48cf322d670d9fb2daf7fa8950fa5823c4f3a3a3",
      "  Title: Mastermind\n"
      "  Author: WilliamDonnelly\n"
    },
    {
      "dabf4212823da92592519c6ee354d22db0721f8a",
      "  Title: Maze\n"
      "  Author: David Winter\n"
    },
    {
      "1a111f83c9f153c0a82f03b03369289369c08b01",
      "  Title: Merlin\n"
      "  Author: David Winter\n"
    },
    {
      "e05fa0fa2cd8db87996c95ad71caca1276366c67",
      "  Title: Mini Lights Out\n"
      "  Author: tobiasvl\n"
    },
    {
      "1cb4ef121d6c593dd9156db3b681b1926fdc1bb2",
      "  Title: Missile\n"
      "  Author: David Winter\n"
    },
    {
      "bc62fd6ee42c9474d9e0b08bfc928de9ad1a15f8",
      "  Title: Octo: a Chip 8 Story\n"
      "  Author: SystemLogoff\n"
    },
    {
      "3efdf686916039d695a37f70d2680ecab1d6dad6",
      "  Title: Octo Rancher\n"
      "  Author: SystemLogoff\n"
    },
    {
      "98f5842823856ecbacdc81900018bcf04f9b9975",
      "  Title: Octojam 1 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "2c5fc20c6de22356f86faf3906ff848c6e833a19",
      "  Title: Octojam 2 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "9c218c68b5d97dda113398cdedc9a16f176e56f9",
      "  Title: Octojam 3 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "bb23e4b870cfcba26c63959b8fed54fdf9409c8c",
      "  Title: Octojam 4 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "8145ff0f124c3b51a0029d484ef800373e5e3990",
      "  Title: Octojam 5 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "a524540b94ff9de99a3856d1c285ae06bf783a45",
      "  Title: Octojam 6 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "ca7ee8b6ab65e924581d30dc5cc37c07e162c349",
      "  Title: Octojam 7 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "1ca061362d0eabb8a0c0384f94661e54e2576db5",
      "  Title: Octojam 8 Title\n"
      "  Author: John Earnest\n"
    },
    {
      "25c16189e84c26424633f9ab422271bbb9cd8be8",
      "  Title: Outlaw\n"
      "  Author: John Earnest\n"
    },
    {
      "751c94a5d9b64b913e9a8412b1286c212e5ea51f",
      "  Title: Pet Dog\n"
      "  Author: SystemLogoff\n"
    },
    {
      "7258ad7d910e51ed68c95b7607c95691929b8edf",
      "  Title: Piper\n"
      "  Author: Aeris, JordanMecom, LillianWang\n"
    },
    {
      "bae68342c534df821f3073778408e60ad80834d4",
      "  Title: Pong\n"
      "  Author: Paul Vervalin\n"
    },
    {
      "84d9923374e2d2d5bd2b86f21a4477ce14e5a617",
      "  Title: Pong 2\n"
      "  Author: David Winter\n"
    },
    {
      "d3f402f3b9480238ad337fb8bfa9f825736078f6",
      "  Title: Pumpkin \"Dreess\" Up\n"
      "  Author: SystemLogoff\n"
    },
    {
      "1d1f79c985556ac95a16aa2c5cd60c4aa1e053b0",
      "  Title: Puzzle\n"
      "  Author: Unknown\n"
    },
    {
      "5df381e3e83f2574887ad362ded942f283d90b63",
      "  Title: RPS\n"
      "  Author: SystemLogoff\n"
    },
    {
      "5f954335373e45a3b54ff41fe4ecee7896ba4b32",
      "  Title: Slippery Slope\n"
      "  Author: John Earnest\n"
    },
    {
      "90b221c726ac95c54f369f05565658d3dedbeacd",
      "  Title: Snek\n"
      "  Author: JohnEarnest\n"
    },
    {
      "a19d1416995409a06610fe8f3551b5df353f5d6c",
      "  Title: Space Invaders\n"
      "  Author: David Winter\n"
    },
    {
      "7e4cf12cbceb343968e45323771a7271e2f93c14",
      "  Title: Space Racer\n"
      "  Author: WilliamDonnelly\n"
    },
    {
      "174d4074d3689a694e77697cf528b1cbb80206db",
      "  Title: Spacejam!\n"
      "  Author: WilliamDonnelly\n"
    },
    {
      "6d1240991261deff0a9f026bacb1d4007fef6614",
      "  Title: Spock Paper Scissors\n"
      "  Author: fontz\n"
    },
    {
      "1a38cf1a4550aeff5e1201735c188431ce662b0b",
      "  Title: Super Pong\n"
      "  Author: offstatic\n"
    },
    {
      "4df1f34b74fa50de97328f1ea751e7f2980df787",
      "  Title: Syzygy\n"
      "  Author: Roy Trevino\n"
    },
    {
      "cd877a77b140fb60827f6d96b6f2fad7030214cf",
      "  Title: Tank\n"
      "  Author: Unknown\n"
    },
    {
      "32190b480c6d078fa1e968a9da57acd3f9d3f9f7",
      "  Title: Tank!\n"
      "  Author: Rectus\n"
    },
    {
      "489dfa2ea502e6bb8009bf8806955099760d006d",
      "  Title: Tetris\n"
      "  Author: Fran Dachille\n"
    },
    {
      "3dcf6f3544e36f4d4d69871e51e51f1eeefeacfb",
      "  Title: Tic-Tac-Toe\n"
      "  Author: David Winter\n"
    },
    {
      "4af33340d7b2443fb7f1b109d15951f17b6da00b",
      "  Title: TOMB STON TIPP\n"
      "  Author: TomRintjema\n"
    },
    {
      "e51a3befdf3d441287c0023880315bb3ed60b736",
      "  Title: UFO\n"
      "  Author: Lutz V\n"
    },
    {
      "f6f803907cfd003b466b25e5289ac9921fd172ce",
      "  Title: Vers\n"
      "  Author: JMN\n"
    },
    {
      "f6cf1f714581f0fdc755844f4b883886809b7c52",
      "  Title: Vertical Brix\n"
      "  Author: Paul Robson\n"
    },
    {
      "d74cf6a1b5f17156abf159d8e8bf6b0a03db57fe",
      "  Title: Wipe Off\n"
      "  Author: Joseph Weisbecker\n"
    }
  }
);
