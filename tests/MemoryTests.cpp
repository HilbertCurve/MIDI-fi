#include <Utils.hpp>
#include <Memory.hpp>
#include <cfloat>

int main() {
    using namespace MidiFi;
    int *d = (int *) m_malloc(3 * sizeof(int), "lmao");
    float *f = (float *) m_malloc(4 * sizeof(float), nullptr);

    d[2] = 5;
    f[3] = 3.4;
    d[0] = 33333;
    f[0] = FLT_MAX;

    //m_log_message("d[2]: %d ", d[0]);
    m_assert(d[0] == 33333, "Function `m_malloc(size_t, const char *)` failed assertion.");
    
    // the real test is to ensure no segmentation faults / corrupted malloc stuff

    m_assert(m_log_memory_usage(false) == 3 * sizeof(int) + 4 * sizeof(float),
            "Function `m_malloc(size_t, const char *)` failed assertion.");

    m_free(d);
    m_free(f);
    size_t size = m_log_memory_usage(false);

    m_assert(size == 0, 
            "Function `m_free(void *)` failed assertion.");

    d = (int *) m_malloc((10), "Large allocation");
    float *a = (float *) m_malloc((10), "Second large allocation");
    float *b = (float *) m_malloc((10), "Third large allocation");
    float *c = (float *) m_malloc((10), "Fourth large allocation");
    m_free(a);
    m_free(b);
    m_free(c);
    m_free(d);
}

