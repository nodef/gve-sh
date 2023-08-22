#pragma once
#include <vector>
#include <string>
#include <unordered_map>
#include <cstdint>

using std::vector;
using std::string;
using std::unordered_map;




#pragma region CLASSES
class VM {
  #pragma region ENUMS
  public:
  /** Instruction opcodes. */
  enum class Opcode {
    /** No operation. */
    NOP   = 0x0000,
    /** Define a 64-bit literal. */
    LIT64 = 0x00B0,
    /** Define a 64-bit variable (in stack). */
    VAR64 = 0x00F0,

    #pragma region LABELS
    /** Define a program label. */
    LABEL       = 0x0010,
    /** Define a program label that is a function. */
    LABEL_FUNC  = 0x0011,
    /** Define a program label that is a block. */
    LABEL_BLOCK = 0x0012,
    /** Define a program label that is a loop. */
    LABEL_LOOP  = 0x0013,
    #pragma endregion

    #pragma region STACK OPERATIONS
    /** Push an 8-bit value onto the stack. */
    PUSH08 = 0x0020,
    /** Push a 16-bit value onto the stack. */
    PUSH16 = 0x0021,
    /** Push a 32-bit value onto the stack. */
    PUSH32 = 0x0022,
    /** Push a 64-bit value onto the stack. */
    PUSH64 = 0x0023,
    /** Pop an 8-bit value from the stack. */
    POP08  = 0x0024,
    /** Pop a 16-bit value from the stack. */
    POP16  = 0x0025,
    /** Pop a 32-bit value from the stack. */
    POP32  = 0x0026,
    /** Pop a 64-bit value from the stack. */
    POP64  = 0x0027,
    /** Peek an 8-bit value from the stack. */
    PEEK08 = 0x0028,
    /** Peek a 16-bit value from the stack. */
    PEEK16 = 0x0029,
    /** Peek a 32-bit value from the stack. */
    PEEK32 = 0x002A,
    /** Peek a 64-bit value from the stack. */
    PEEK64 = 0x002B,
    /** Allocate a block of memory on the stack. */
    STACK_ALLOC = 0x002C,
    /** Free a block of memory on the stack. */
    STACK_FREE  = 0x002D,
    /** Get the size of the stack within a block. */
    STACK_INNER = 0x002E,
    /** Get the size of the stack outside a block. */
    STACK_OUTER = 0x002F,
    #pragma endregion

    #pragma region BRANCH/CALL OPERATIONS
    /** Jump to a label if value is falsy. */
    JUMP_FALSE  = 0x0030,
    /** Jump to a label if value is truthy. */
    JUMP_TRUE   = 0x0031,
    /** Call a label if value is falsy. */
    CALL_FALSE  = 0x0040,
    /** Call a label if value is truthy. */
    CALL_TRUE   = 0x0041,
    /** Return from a function if value is falsy. */
    RET_FALSE   = 0x0042,
    /** Return from a function if value is truthy. */
    RET_TRUE    = 0x0043,
    SYSCALL_QUERY = 0x0044,
    SYSCALL_LABEL = 0x0045,
    SYSCALL_REG   = 0x0048,
    #pragma endregion

    #pragma region MEMORY OPERATIONS
    /** Load an 8-bit value from memory (r1 <- r2:[r3]). */
    LOAD08  = 0x10,
    /** Load a 16-bit value from memory (r1 <- r2:[r3]). */
    LOAD16  = 0x11,
    /** Load a 32-bit value from memory (r1 <- r2:[r3]). */
    LOAD32  = 0x12,
    /** Load a 64-bit value from memory (r1 <- r2:[r3]). */
    LOAD64  = 0x13,
    /** Store an 8-bit value to memory (r1 -> r2:[r3]). */
    STORE08 = 0x14,
    /** Store a 16-bit value to memory (r1 -> r2:[r3]). */
    STORE16 = 0x15,
    /** Store a 32-bit value to memory (r1 -> r2:[r3]). */
    STORE32 = 0x16,
    /** Store a 64-bit value to memory (r1 -> r2:[r3]). */
    STORE64 = 0x17,
    /** Load from an 8-bit variable (r1 <- [label]). */
    LOAD_VAR08  = 0x18,
    /** Load from a 16-bit variable (r1 <- [label]). */
    LOAD_VAR16  = 0x19,
    /** Load from a 32-bit variable (r1 <- [label]). */
    LOAD_VAR32  = 0x1A,
    /** Load from a 64-bit variable (r1 <- [label]). */
    LOAD_VAR64  = 0x1B,
    /** Store to an 8-bit variable (r1 -> [label]). */
    STORE_VAR08 = 0x1C,
    /** Store to a 16-bit variable (r1 -> [label]). */
    STORE_VAR16 = 0x1D,
    /** Store to a 32-bit variable (r1 -> [label]). */
    STORE_VAR32 = 0x1E,
    /** Store to a 64-bit variable (r1 -> [label]). */
    STORE_VAR64 = 0x1F,
    #pragma endregion

    #pragma region LOGICAL OPERATIONS
    /** Bitwise AND two values (r1 <- r2 & r3). */
    AND  = 0x20,
    /** Bitwise OR two values (r1 <- r2 | r3). */
    OR   = 0x21,
    /** Bitwise XOR two values (r1 <- r2 ^ r3). */
    XOR  = 0x22,
    /** Bitwise XNOR two values (r1 <- r2 ^ ~r3). */
    XNOR = 0x23,
    /** Bitwise Shift left a value (r1 <- r2 << r3). */
    SHL  = 0x24,
    /** Bitwise Shift right a value (r1 <- r2 >> r3). */
    SHR  = 0x25,
    /** Bitwise Shift arithmetic right a value (r1 <- r2 >>> r3). */
    SAL  = 0x26,
    /** Bitwise Shift arithmetic right a value (r1 <- r2 >>> r3). */
    SAR  = 0x27,
    #pragma endregion

    #pragma region ARITHMETIC OPERATIONS
    /** Add two integers (r1 <- r2 + r3). */
    ADD_UINT = 0x30,
    /** Subtract two unsigned integers (r1 <- r2 - r3). */
    SUB_UINT = 0x32,
    /** Multiply two unsigned integers (r1 <- r2 * r3). */
    MUL_UINT = 0x34,
    /** Multiply two signed integers (r1 <- r2 * r3). */
    MUL_INT  = 0x35,
    /** Divide two unsigned integers (r1 <- r2 / r3). */
    DIV_UINT = 0x36,
    /** Divide two signed integers (r1 <- r2 / r3). */
    DIV_INT  = 0x37,
    /** Add two 32-bit floating point numbers (r1 <- r2 + r3). */
    ADD_F32  = 0x38,
    /** Add two 64-bit floating point numbers (r1 <- r2 + r3). */
    ADD_F64  = 0x39,
    /** Subtract two 32-bit floating point numbers (r1 <- r2 - r3). */
    SUB_F32  = 0x3A,
    /** Subtract two 64-bit floating point numbers (r1 <- r2 - r3). */
    SUB_F64  = 0x3B,
    /** Multiply two 32-bit floating point numbers (r1 <- r2 * r3). */
    MUL_F32  = 0x3C,
    /** Multiply two 64-bit floating point numbers (r1 <- r2 * r3). */
    MUL_F64  = 0x3D,
    /** Divide two 32-bit floating point numbers (r1 <- r2 / r3). */
    DIV_F32  = 0x3E,
    /** Divide two 64-bit floating point numbers (r1 <- r2 / r3). */
    DIV_F64  = 0x3F,
    #pragma endregion

    #pragma region TYPE CONVERSION
    /** Convert a 32-bit integer to a 64-bit integer (r1 <- r2). */
    I32_TO_I64 = 0x42,
    /** Convert a 64-bit integer to a 32-bit integer (r1 <- r2). */
    I64_TO_I32 = 0x43,
    /** Convert a 32-bit floating point number to a 64-bit floating point number (r1 <- r2). */
    F32_TO_F64 = 0x46,
    /** Convert a 64-bit floating point number to a 32-bit floating point number (r1 <- r2). */
    F64_TO_F32 = 0x47,
    /** Convert a 64-bit integer to a 32-bit floating point number (r1 <- r2). */
    I64_TO_F32 = 0x49,
    /** Convert a 64-bit integer to a 64-bit floating point number (r1 <- r2). */
    I64_TO_F64 = 0x4B,
    /** Convert a 32-bit floating point number to a 64-bit integer (r1 <- r2). */
    F32_TO_I64 = 0x4D,
    /** Convert a 64-bit floating point number to a 64-bit integer (r1 <- r2). */
    F64_TO_I64 = 0x4F,
    #pragma endregion

    #pragma region MATH OPERATIONS
    /** Calculate the ceiling of a 32-bit floating point number (r1 <- ceil(r2)). */
    CEIL_F32  = 0x40,
    /** Calculate the ceiling of a 64-bit floating point number (r1 <- ceil(r2)). */
    CEIL_F64  = 0x41,
    /** Calculate the floor of a 32-bit floating point number (r1 <- floor(r2)). */
    FLOOR_F32 = 0x42,
    /** Calculate the floor of a 64-bit floating point number (r1 <- floor(r2)). */
    FLOOR_F64 = 0x43,
    /** Calculate the truncation of a 32-bit floating point number (r1 <- trunc(r2)). */
    TRUNC_F32 = 0x44,
    /** Calculate the truncation of a 64-bit floating point number (r1 <- trunc(r2)). */
    TRUNC_F64 = 0x45,
    /** Calculate the round of a 32-bit floating point number (r1 <- round(r2)). */
    ROUND_F32 = 0x46,
    /** Calculate the round of a 64-bit floating point number (r1 <- round(r2)). */
    ROUND_F64 = 0x47,
    /** Calculate the absolute of a 32-bit floating point number (r1 <- abs(r2)). */
    ABS_F32   = 0x48,
    /** Calculate the absolute of a 64-bit floating point number (r1 <- abs(r2)). */
    ABS_F64   = 0x49,
    /** Calculate the power of a 32-bit floating point number (r1 <- pow(r2, r3)). */
    POW_F32   = 0x4A,
    /** Calculate the power of a 64-bit floating point number (r1 <- pow(r2, r3)). */
    POW_F64   = 0x4B,
    /** Calculate the square root of a 32-bit floating point number (r1 <- sqrt(r2)). */
    SQRT_F32  = 0x4C,
    /** Calculate the square root of a 64-bit floating point number (r1 <- sqrt(r2)). */
    SQRT_F64  = 0x4D,
    /** Calculate the cube root of a 32-bit floating point number (r1 <- cbrt(r2)). */
    CBRT_F32  = 0x4E,
    /** Calculate the cube root of a 64-bit floating point number (r1 <- cbrt(r2)). */
    CBRT_F64  = 0x4F,
    /** Calculate the natural logarithm of a 32-bit floating point number (r1 <- log(r2)). */
    LOG_F32   = 0x50,
    /** Calculate the natural logarithm of a 64-bit floating point number (r1 <- log(r2)). */
    LOG_F64   = 0x51,
    /** Calculate the base-2 logarithm of a 32-bit floating point number (r1 <- log2(r2)). */
    LOG2_F32  = 0x52,
    /** Calculate the base-2 logarithm of a 64-bit floating point number (r1 <- log2(r2)). */
    LOG2_F64  = 0x53,
    /** Calculate the base-10 logarithm of a 32-bit floating point number (r1 <- log10(r2)). */
    LOG10_F32 = 0x54,
    /** Calculate the base-10 logarithm of a 64-bit floating point number (r1 <- log10(r2)). */
    LOG10_F64 = 0x55,
    /** Calculate the exponential of a 32-bit floating point number (r1 <- exp(r2)). */
    EXP_F32   = 0x56,
    /** Calculate the exponential of a 64-bit floating point number (r1 <- exp(r2)). */
    EXP_F64   = 0x57,
    /** Calculate the sine of a 32-bit floating point number (r1 <- sin(r2)). */
    SIN_F32   = 0x58,
    /** Calculate the sine of a 64-bit floating point number (r1 <- sin(r2)). */
    SIN_F64   = 0x59,
    /** Calculate the cosine of a 32-bit floating point number (r1 <- cos(r2)). */
    COS_F32   = 0x5A,
    /** Calculate the cosine of a 64-bit floating point number (r1 <- cos(r2)). */
    COS_F64   = 0x5B,
    /** Calculate the tangent of a 32-bit floating point number (r1 <- tan(r2)). */
    TAN_F32   = 0x5C,
    /** Calculate the tangent of a 64-bit floating point number (r1 <- tan(r2)). */
    TAN_F64   = 0x5D,
    /** Calculate the arctangent of a 32-bit floating point number (r1 <- atan(r2)). */
    ATAN_F32  = 0x5E,
    /** Calculate the arctangent of a 64-bit floating point number (r1 <- atan(r2)). */
    ATAN_F64  = 0x5F,
    #pragma endregion

    #pragma region COMPARISON OPERATIONS
    /** Compare two 64-bit unsigned integers for equality (r1 <- r2 == r3). */
    CMP_EQ_UINT = 0x60,
    /** Compare two 64-bit signed integers for equality (r1 <- r2 == r3). */
    CMP_EQ_INT  = 0x61,
    /** Compare two 64-bit unsigned integers for inequality (r1 <- r2 != r3). */
    CMP_NE_UINT = 0x62,
    /** Compare two 64-bit signed integers for inequality (r1 <- r2 != r3). */
    CMP_NE_INT  = 0x63,
    /** Compare two 64-bit unsigned integers for less than (r1 <- r2 < r3). */
    CMP_LT_UINT = 0x64,
    /** Compare two 64-bit signed integers for less than (r1 <- r2 < r3). */
    CMP_LT_INT  = 0x65,
    /** Compare two 64-bit unsigned integers for less than or equal (r1 <- r2 <= r3). */
    CMP_LE_UINT = 0x66,
    /** Compare two 64-bit signed integers for less than or equal (r1 <- r2 <= r3). */
    CMP_LE_INT  = 0x67,
    /** Compare two 64-bit unsigned integers for greater than (r1 <- r2 > r3). */
    CMP_GT_UINT = 0x68,
    /** Compare two 64-bit signed integers for greater than (r1 <- r2 > r3). */
    CMP_GT_INT  = 0x69,
    /** Compare two 64-bit unsigned integers for greater than or equal (r1 <- r2 >= r3). */
    CMP_GE_UINT = 0x6A,
    /** Compare two 64-bit signed integers for greater than or equal (r1 <- r2 >= r3). */
    CMP_GE_INT  = 0x6B,
    /** Compare two 32-bit floating point numbers for equality (r1 <- r2 == r3). */
    CMP_EQ_F32  = 0x70,
    /** Compare two 64-bit floating point numbers for equality (r1 <- r2 == r3). */
    CMP_EQ_F64  = 0x71,
    /** Compare two 32-bit floating point numbers for inequality (r1 <- r2 != r3). */
    CMP_NE_F32  = 0x72,
    /** Compare two 64-bit floating point numbers for inequality (r1 <- r2 != r3). */
    CMP_NE_F64  = 0x73,
    /** Compare two 32-bit floating point numbers for less than (r1 <- r2 < r3). */
    CMP_LT_F32  = 0x74,
    /** Compare two 64-bit floating point numbers for less than (r1 <- r2 < r3). */
    CMP_LT_F64  = 0x75,
    /** Compare two 32-bit floating point numbers for less than or equal (r1 <- r2 <= r3). */
    CMP_LE_F32  = 0x76,
    /** Compare two 64-bit floating point numbers for less than or equal (r1 <- r2 <= r3). */
    CMP_LE_F64  = 0x77,
    /** Compare two 32-bit floating point numbers for greater than (r1 <- r2 > r3). */
    CMP_GT_F32  = 0x78,
    /** Compare two 64-bit floating point numbers for greater than (r1 <- r2 > r3). */
    CMP_GT_F64  = 0x79,
    /** Compare two 32-bit floating point numbers for greater than or equal (r1 <- r2 >= r3). */
    CMP_GE_F32  = 0x7A,
    /** Compare two 64-bit floating point numbers for greater than or equal (r1 <- r2 >= r3). */
    CMP_GE_F64  = 0x7B,
    #pragma endregion
  };
  #pragma endregion


  #pragma region DATA
  protected:
  /** Program consisting of 32-bit instructions. */
  vector<uint32_t> pgm;
  /** Map of labels to their addresses. */
  unordered_map<uint32_t, uint32_t> sym;
  /** Program counter. */
  uint32_t pc;
  /** 256 64-bit registers, where 0 is the zero register. */
  vector<uint64_t> reg;
  /** Stack of size determined by the program. */
  vector<uint64_t> stk;
  /** Memory of size determined by the program. */
  vector<uint8_t> mem;
  #pragma endregion


  #pragma region METHODS
  public:
  /**
   * Execute the given instruction.
   * @param inst instruction to execute
   */
  inline void execute(uint32_t inst) {
    uint8_t opcode = inst & 0xFF;
  }
  #pragma endregion
// public:
//   VM();
//   ~VM();

  // void run(const string& filename);
  // void run(const vector<string>& code);
};
#pragma endregion
