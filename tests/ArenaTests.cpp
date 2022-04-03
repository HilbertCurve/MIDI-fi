#include <Memory.hpp>
#include <Utils.hpp>

int main() {
    using namespace MidiFi;
    m_assert(Kilobytes(20) == 20480l, "Function `Kilobytes(size_t)` failed assertion.");
    m_assert(Megabytes(20) == 20971520l, "Function `Megabytes(size_t)` failed assertion.");
    m_assert(Gigabytes(20) == 21474836480l, "Function `Gigabytes(size_t)` failed assertion.");
    
    Arena tmp;
    initArena(tmp, Kilobytes(20));
    bool correctSize = tmp.size == 20480;
    m_assert(correctSize, "Function `initArena(Arena &, size_t)` failed assertion.");

    arenaPush(tmp, Kilobytes(1));
    i32 correctStackCount = (tmp.stackCount == 1) << 0;
    i32 correctEntry = (tmp.stack[0].size == 1024) << 1;
    i32 correctPointers = (tmp._cursor == tmp._base + tmp.stack[0].size) << 2;
    i32 arenaPush_code = correctStackCount | correctEntry | correctPointers;
    m_assert(arenaPush_code == 0b111, 
            "Function `arenaPush(Arena &, size_t)` failed assertion with code %d", arenaPush_code);

    arenaPush(tmp, Kilobytes(2));
    arenaPush(tmp, Kilobytes(3));

    correctStackCount = (tmp.stackCount == 3) << 0;
    correctEntry = (tmp.stack[2].size == 1024 * 3) << 1;
    correctPointers = (tmp._cursor == tmp._base + 
            tmp.stack[0].size + tmp.stack[1].size + tmp.stack[2].size) << 2;
    arenaPush_code = correctStackCount | correctEntry | correctPointers;
    m_assert(arenaPush_code == 0b111, 
            "Function `arenaPush(Arena &, size_t)` failed 2nd assertion with code %d", 
            arenaPush_code);

    return 0;
}

