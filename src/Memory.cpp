#include "Memory.hpp"

#include <malloc.h>
#include "Utils.hpp"

#ifndef MIDIFI_ARENA_STACK_LEN
#define MIDIFI_ARENA_STACK_LEN 64
#endif

namespace MidiFi {
    /**
     * A timeline of allocation events, in the form of a linked list.
     */
    struct AllocEntry {
        size_t size;
        const char *purpose;
        void *id;

        AllocEntry *next;
        AllocEntry &operator[](size_t index);
    };

    AllocEntry &AllocEntry::operator[](size_t index) {
        if (index) return this->next->operator[](index - 1);
        else return *this;
    }

    static AllocEntry *_entries = nullptr;
    static size_t _count = 0;
    static size_t _total = 0;

    void *m_malloc(size_t size, const char *purpose) {
        AllocEntry *_entry = nullptr;
        if (!_entries) {
            _entry = (AllocEntry *) malloc(sizeof(AllocEntry));
            _entry->purpose = purpose;
            _entry->size = size;
            _count++;
            _entries = _entry;
        } else {
            _entry = (AllocEntry *) malloc(sizeof(AllocEntry));
            _entry->purpose = purpose;
            _entry->size = size;
            _entries[_count - 1].next = _entry;
            _count++;
        }

        _total += size;
        // TODO: check if addition of `size` bytes is too much
        _entry->id = malloc(size);
        return _entry->id;
    }

    void m_free(void *ptr) {
        if (!_entries) {
            m_log_warning("Attempt to free memory when none was allocated.");
            return;
        }

        if (_entries->id == ptr) {
            AllocEntry *prev = _entries;
            _entries = _entries->next;

            _count--;
            _total -= prev->size;
            free(prev);
            free(ptr);
            return;
        }

        AllocEntry *prev = nullptr;
        AllocEntry *curr = _entries;
        AllocEntry *next = _entries->next;
        while (curr->next) {
            prev = curr;
            curr = curr->next;
            next = curr->next;

            if (curr->id == ptr) {
                prev->next = next;

                _count--;
                _total -= curr->size;
                free(curr);
                free(ptr);
                return;
            }
        }

        m_log_warning("Attempt to free unallocated pointer %p. Did you use malloc() instead of m_malloc()?", ptr);
    }

    size_t m_log_memory_usage(bool verbose) {
        if (!verbose) return _total;
        if (!_entries) {
            m_log_message("No memory is allocated.");
        } else {
            AllocEntry *curr = _entries;
            while (curr) {
                m_log_message("Memory usage at %p : %s", curr->id, 
                        curr->purpose ? curr->purpose : "Usage not specified.");
                curr = curr->next;
            }
            m_log_message("Used memory: %ld.", _total);
        }
        return _total;
    }

    void initArena(Arena &a, size_t size) {
        a._base = (char *) malloc(size);
        a._cursor = a._base;
        a.size = size;

        a.stack = (Field *) malloc(MIDIFI_ARENA_STACK_LEN * sizeof(Field));
        a.stackCount = 0;
    }

    void deleteArena(Arena &a) {
        free(a._base);
        free(a.stack);

        a._base = nullptr;
        a._cursor = nullptr;
        a.size = 0;
        a.stack = nullptr;
        a.stackCount = 0;
    }

    void arenaCheck(Arena &a) {
        bool badCursor = a._cursor < a._base || a._cursor > a._base + a.size;
        m_assert(!badCursor, "Arena %p has illegal cursor position %p, with base %p and size %ld.",
                &a, a._cursor, a._base, a.size);
    }

    void *arenaPush(Arena &a, size_t size) {
        a.stack[a.stackCount] = Field{a._cursor, size};
        a.stack[a.stackCount].entries = (FieldEntry *) malloc(0);
        a._cursor += size;
        a.stackCount++;

        arenaCheck(a);
        return a._cursor - size;
    }

    void *arenaPop(Arena &a) {
        a._cursor -= a.stack[a.stackCount].size;
        a.stackCount--;

        arenaCheck(a);
        return nullptr;
    }

    void *fieldAlloc(Arena &a, size_t size) {
        // easy: enough space as is to allocate size bytes

        return nullptr;
    }

    Field Arena::operator[](size_t index) {
        m_assert(index < stackCount, "Index %ld out of bounds for Arena with %ld entries.", 
                index, stackCount);
        return stack[index];
    }
}

