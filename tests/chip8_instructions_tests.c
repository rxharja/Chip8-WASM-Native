#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "../include/config.h"
#include "../include/chip8.h"

// Test:
// 00E0 CLS
// clear screen
static void test_chip8_cls(struct Chip8* chip8)
{
  printf("00E0 ");
  chip8_init(chip8);
  memset(chip8->screen.pixels, 1, sizeof(chip8->screen.pixels));
  chip8_exec(chip8, 0x00E0);
  for(int x = 0; x < CHIP8_WIDTH; x++) {
    for(int y = 0; y < CHIP8_HEIGHT; y++) {
      assert(chip8->screen.pixels[y][x] == false);
    }
  }
  printf("Success\n");
}

// Test:
// 1NNN JP ADDR
// skip next instruction if Vx == KK
static void test_chip8_3xkk(struct Chip8* chip8)
{
  printf("3xkk ");
  int originalPC = 0x00;
  chip8->registers.PC = originalPC;
  chip8->registers.V[0x00] = 0x22;
  chip8_exec(chip8, 0x3022);
  assert(chip8->registers.PC == originalPC + 2);
  printf("Success\n");
}

// Test:
// 4XKK SNE Vx, byte
// skip next instruction if Vx != KK
static void test_chip8_4xkk(struct Chip8* chip8)
{
  printf("4xkk ");
  int originalPC = 0x00;
  chip8->registers.PC = originalPC;
  chip8->registers.V[0x00] = 0x22;
  chip8_exec(chip8, 0x4022);
  assert(chip8->registers.PC != originalPC + 2);
  printf("Success\n");
}

// Test:
// 5xy0, SE Vx Vy
// skip next instruction if Vx == Vy
static void test_chip8_5xy0(struct Chip8* chip8)
{
  printf("5xy0 ");
  int originalPC = 0x00;

  chip8->registers.PC = originalPC;
  chip8->registers.V[2] = 0x05;
  chip8->registers.V[3] = 0x10;
  chip8_exec(chip8, 0x5230);
  assert(chip8->registers.PC == originalPC);

  chip8->registers.V[2] = 0x10;
  chip8->registers.V[3] = 0x10;
  chip8_exec(chip8, 0x5230);
  assert(chip8->registers.PC == originalPC + 2);
  printf("Success\n");
}

// Test:
// 8xy0, SE Vx Vy
// skip next instruction if Vx == Vy
static void test_chip8_8xy0(struct Chip8* chip8)
{
  printf("8xy0 ");
  chip8->registers.V[0] = 0x05;
  chip8->registers.V[1] = 0x10;
  chip8_exec(chip8, 0x8010);
  assert(chip8->registers.V[0] == 0x10);
  assert(chip8->registers.V[1] == 0x10);
  assert(chip8->registers.V[0] == chip8->registers.V[3]);
  printf("Success\n");
}

// Test:
// 8xy1, OR Vx, Vy
// bitwise OR on Vx, Vy, result stored in Vx
static void test_chip8_8xy1(struct Chip8* chip8)
{
  printf("8xy1 ");
  chip8->registers.V[0] = 0b10;
  chip8->registers.V[1] = 0b01;
  chip8_exec(chip8, 0x8011);
  assert(chip8->registers.V[0] == 0b11);
  printf("Success\n");
}

// Test:
// 8xy2, AND Vx, Vy
// bitwise AND on Vx, Vy, result stored in Vx
static void test_chip8_8xy2(struct Chip8* chip8)
{
  printf("8xy2 ");
  chip8->registers.V[0] = 0b10;
  chip8->registers.V[1] = 0b01;
  chip8_exec(chip8, 0x8012);
  assert(chip8->registers.V[0] == 0b00);
  printf("Success\n");
}

// Test:
// 8xy3, XOR Vx, Vy
// bitwise XOR on Vx, Vy, result stored in Vx
static void test_chip8_8xy3(struct Chip8* chip8)
{
  printf("8xy3 ");
  chip8->registers.V[0] = 0b10;
  chip8->registers.V[1] = 0b01;
  chip8_exec(chip8, 0x8013);
  assert(chip8->registers.V[0] == 0b11);

  chip8->registers.V[0] = 0b11;
  chip8->registers.V[1] = 0b11;
  chip8_exec(chip8, 0x8013);
  assert(chip8->registers.V[0] == 0b00);
  printf("Success\n");
}

// Test:
// 8xy4, ADD Vx, Vy
// ADD on Vx, Vy, result stored in Vx, Vf 1 if Vx + Vy > 0xff;
static void test_chip8_8xy4(struct Chip8* chip8)
{
  printf("8xy4 ");
  chip8->registers.V[0] = 0xff;
  chip8->registers.V[1] = 0x02;
  chip8_exec(chip8, 0x8014);
  assert(chip8->registers.V[0] == 1);
  assert(chip8->registers.V[0x0F] == 1);

  chip8->registers.V[0] = 0x01;
  chip8->registers.V[1] = 0x01;
  chip8_exec(chip8, 0x8014);
  assert(chip8->registers.V[0] == 2);
  assert(chip8->registers.V[0x0F] == 0);
  printf("Success\n");
}

// Test:
// 8xy5, SUB Vx, Vy
// SUB on Vx, Vy, result stored in Vx, Vf 1 if V[x] > V[y]
static void test_chip8_8xy5(struct Chip8* chip8)
{
  printf("8xy5 ");
  chip8->registers.V[0] = 0xff;
  chip8->registers.V[1] = 0x02;
  chip8_exec(chip8, 0x8015);
  assert(chip8->registers.V[0] == 0xfd);
  assert(chip8->registers.V[0x0F] == 1);

  chip8->registers.V[0] = 0x01;
  chip8->registers.V[1] = 0x02;
  chip8_exec(chip8, 0x8015);
  assert(chip8->registers.V[0] == 0xff);
  assert(chip8->registers.V[0x0F] == 0);
  printf("Success\n");
}

// Test:
// 8xy6, SUB Vx, Vy
// SUB on Vx, Vy, result stored in Vx, Vf 1 if V[x] > V[y]
static void test_chip8_8xy6(struct Chip8* chip8)
{
  printf("8xy6 ");
  chip8->registers.V[0] = 0xff;
  chip8_exec(chip8, 0x8016);
  assert(chip8->registers.V[0x0F] == 1);
  assert(chip8->registers.V[0] == 0x7f);

  chip8->registers.V[0] = 0x00;
  chip8_exec(chip8, 0x8016);
  assert(chip8->registers.V[0x0F] == 0);
  assert(chip8->registers.V[0] == 0);
  printf("Success\n");
}

// Test:
// 8xy7, SUB Vx, Vy
// SUB on Vx, Vy, result stored in Vx, Vf 1 if V[x] > V[y]
static void test_chip8_8xy7(struct Chip8* chip8)
{
  printf("8xy7 ");
  chip8->registers.V[0] = 0x01;
  chip8->registers.V[1] = 0x02;
  chip8_exec(chip8, 0x8017);
  assert(chip8->registers.V[0x0F] == 1);
  assert(chip8->registers.V[0] == 0x01);
  printf("Success\n");
}

// Test:
// 8xyE, SHL Vx, Vy
// SUB on Vx, Vy, result stored in Vx, Vf 1 if V[x] > V[y]
static void test_chip8_8xye(struct Chip8* chip8)
{
  printf("8xyE ");
  chip8->registers.V[0] = 0x80;
  chip8_exec(chip8, 0x801E);
  assert(chip8->registers.V[0x0F] == 1);
  assert(chip8->registers.V[0] == 0x0); // 0x80 * 2 == 0x100, loops around to 0
  printf("Success\n");
}

int main(void) {
  struct Chip8 chip8;
  test_chip8_cls(&chip8);
  test_chip8_3xkk(&chip8);
  test_chip8_4xkk(&chip8);
  test_chip8_5xy0(&chip8);
  test_chip8_8xy0(&chip8);
  test_chip8_8xy1(&chip8);
  test_chip8_8xy2(&chip8);
  test_chip8_8xy3(&chip8);
  test_chip8_8xy4(&chip8);
  test_chip8_8xy5(&chip8);
  test_chip8_8xy6(&chip8);
  test_chip8_8xy7(&chip8);
  test_chip8_8xye(&chip8);
  printf("CHIP8 Instructions: Success\n");
}
