/* date = March 26th 2022 4:00 pm */

#ifndef MIDIFI_MEMORY_HPP
#define MIDIFI_MEMORY_HPP

namespace MidiFi {
    typedef unsigned char u8;
    typedef unsigned short u16;
    typedef unsigned int u32;
    typedef unsigned long u64;
    typedef long unsigned int size_t;

    typedef signed char i8;
    typedef signed short i16;
    typedef signed int i32;
    typedef signed long i64;

    typedef float f32;
    typedef double f64;

    /**
     * Allocates `size` bytes of memory.
     * @return Memory address of allocated bytes.
     */
    void *m_malloc(size_t size, const char *purpose);
    /**
     * Frees a region of memory at location `ptr`. If no memory was
     * allocated there, it prints a warning message.
     */
    void m_free(void *ptr);
    /**
     * If verbose is true, prints current memory status; all instances of 
     * memory allocation, if there are any, and how much memory is used.
     * 
     * Note: total usage might be slightly inaccurate. This function only
     * measures how much memory was allocated by m_malloc(), and does not
     * account for static variables, function pointers, etc.
     *
     * @return Current total bytes allocated.
     */
    size_t m_log_memory_usage(bool verbose);

    // TODO: arenas, eventually.
    struct FieldEntry {
        void *loc;
        size_t len;
    };

    /**
     * A descriptor for a subsection of memory inside an Arena block.
     */
    struct Field {
        /**
         * Beginning of this entry in memory.
         */
        char *base;
        /**
         * Size, in bytes, of this entry.
         */
        size_t size;

        FieldEntry *entries;
    };

    /**
     * An `Arena` is a region of pre-allocated memory with internal
     * information and sectioning.
     */
    struct Arena {
        /**
         * Minimum value for cursor. As the start of an arena allocation
         * block, it is used in conjunction with `malloc` and `free`.
         */
        char *_base;
        /**
         * Current position in memory being used; (1 byte after) the maximum
         * pointer value in the most recent stack entry.
         */
        char *_cursor;
        /**
         * Size, in bytes, allocated to this arena.
         */
        size_t size;

        /**
         * A list of stack entries. The maximum number of stack
         * entries possible is the macro `MIDIFI_ARENA_STACK_LEN`,
         * which can be defined at build time.
         */
        Field *stack;
        /**
         * Number of stack entries, minus one.
         */
        size_t stackCount;
        /**
         * Gets a stack entry.
         */
        Field operator[](size_t index);
    };
    
    inline size_t Kilobytes(size_t b) { return b << 10; }
    inline size_t Megabytes(size_t b) { return b << 20; }
    inline size_t Gigabytes(size_t b) { return b << 30; }

    /**
     * Allocates an arena `size` bytes large.
     */
    void initArena(Arena &a, size_t size);
    /**
     * Deallocates an arena, freeing any memory used.
     */
    void deleteArena(Arena &a);
    /**
     * Checks to see if an arena is in an illegal state; namely:
     * if _cursor < _base or _cursor > _base + size.
     *
     * More arena checks might be added, so this function is not
     * marked with `inline`.
     *
     * This function can possibly crash the program, with the hopes of
     * zero tolerance for potential BO exploits.
     */
    void arenaCheck(Arena &a);
    /**
     * Add new field to an arena `size` bytes large.
     * @return A pointer to the beginning of the partitioned region.
     */
    void *arenaPush(Arena &a, size_t size);
    /**
     * Remove most recently added field from an arena.
     * @return nullptr, to assign to whatever variable `arenaPush` assigned to.
     */
    void *arenaPop(Arena &a);
    void *arenaAlloc(Arena &a, size_t size);
    void *arenaFree(Arena &a, void *ptr);
}

#endif // MIDIFI_MEMORY_HPP

