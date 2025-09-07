#include "platform/pstdint.h"

#include "platform.h"
#include "rnbench.h"

#define TRUE 1
#define FALSE 0

typedef unsigned char bool;

typedef union {
    double double_value;
    uint32_t uint32_value;
    int int_value;
} bench_result_t;

typedef enum {
    BENCH_TYPE_RAND = 0,
    BENCH_TYPE_WC,
    BENCH_TYPE_CRC32,
    BENCH_TYPE_RLE,
    BENCH_TYPE_QSORT,

    BENCH_TYPE_MAX
} bench_type;

#define BENCH_RND_ITERATIONS 1000
#define BENCH_WC_ITERATIONS 500
#define BENCH_CRC32_ITERATIONS 1000
#define BENCH_RLE_ITERATIONS 250
#define BENCH_QSORT_ITERATIONS 100

#define BENCH_RND_SEED 2137

/* utilities */

void rnb_strzero(char* dst, size_t size) {
    while(size-- > 0) {
        *dst++ = '\0';
    }
}

unsigned rnb_strlen(const char* text) {
    unsigned len = 0;
    int i = 0;

    for(i=0; text[i] != 0; ++i) {
        len++;
    }

    return len;
}

bool rnb_iswhitespace(char c) {
    if(c == '\n' || c == ' ' || c == '\t' || c == '\r') {
        return TRUE;
    } else {
        return FALSE;
    }
}

char* rnb_strcat(char* dst, const char* txt) {
    char* save_dst = dst;

    /* find end of string */
    while(*dst != '\0') {
        ++dst;
    } 

    /* add our source text */
    while((*dst++ = *txt++) != '\0');

    return save_dst;
}

int rnb_strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }

    return (unsigned char)*str1 - (unsigned char)*str2;
}

/* random numbers */

static int rnd_state = 0;
static const int rnd_max = ((1U << 15) - 1);

static void rnd_init(int seed) {
    rnd_state = seed;
}

static int rnd_get_int() {
    rnd_state = (rnd_state * 214013 + 2531011) & rnd_max;
	return rnd_state;
}

static int rnd_get_int_range(int from, int to) {
    int range = to - from;

	return from + rnd_get_int() % range;
}

static bench_result_t bench_random_numbers() {
    const int iterations = BENCH_RND_ITERATIONS;
    
    int i = 0;
    double avg;
    bench_result_t result;
    
    avg = 0.0;
    for(i = 0; i < iterations; ++i) {
        avg += rnd_get_int();
        avg /= 2.0;
    }
    
    result.double_value = avg;
    return result;
}

/* word count */

#define WC_TEST_WORDS_NUMBER 32
#define WC_MAX_TEST_DATA_SIZE 512 /* check if the longest word times max words number doesn't exceed this value! */

static const char* test_words[WC_TEST_WORDS_NUMBER] = {
    "lorem",
    "ipsum",
    "dolor",
    "sit", 
    "amet", 
    "consectetur", 
    "adipiscing", 
    "elit", 
    "sed", 
    "tristique", 
    "ante", 
    "libero", 
    "eu", 
    "sodales", 
    "arcu", 
    "euismod", 
    "sit", 
    "amet", 
    "duis",
    "nec",
    "consequat",
    "metus", 
    "donec", 
    "ut", 
    "lectus", 
    "vel", 
    "dui", 
    "blandit", 
    "blandit", 
    "nam", 
    "luctus", 
    "scelerisque"
};

static unsigned wc_count(const char* text) {
    char c;
    int i;
    unsigned count = 0;
    bool in_word = FALSE;
    
    if(text == 0) {
        return 0;
    }

    if(rnb_strlen(text) == 0) {
        return 0;
    }

    for(i=0; text[i] != 0; ++i) {
        c = text[i];

        /* exit early if we have character zero */
        if(c == '\0') {
            /* if we were previously in word, increase word count */
            if(in_word) {
                count++;
            }

            break;
        }

        if(!rnb_iswhitespace(c)) {
            in_word = TRUE;
        } else {
            /* if we were previously in word, increase word count */
            if(in_word) {
                count++;
            }

            in_word = FALSE;
        }
    }

    return count;
}

static void wc_create_test_text(char* text, int number_of_words, size_t max_size) {
    const char* current_word;

    /* zero test text */
    rnb_strzero(text, max_size);

    /* add random words */
    while(--number_of_words > 0) {
        current_word = test_words[rnd_get_int_range(0, WC_TEST_WORDS_NUMBER)];
        text = rnb_strcat(text, current_word);

        /* don't append space to the last one */
        if(number_of_words >= 1) {
            text = rnb_strcat(text, " ");
        }
    }
}

static bench_result_t bench_word_count() {
    const int iterations = BENCH_WC_ITERATIONS;
    
    int number_of_words;
    char test_text[WC_MAX_TEST_DATA_SIZE] = { 0 };
    int total = 0;
    int i;
    bench_result_t result;
    
    for(i = 0; i < iterations; ++i) {
        number_of_words = rnd_get_int_range(16, 32);
        wc_create_test_text(test_text, number_of_words, WC_MAX_TEST_DATA_SIZE);
        total += wc_count(test_text);
    }
    
    result.int_value = total / iterations;
    return result;
}

/* crc32 */

#define CRC32_TEST_DATA_SIZE 128

static char test_crc32_data[CRC32_TEST_DATA_SIZE] = { 0 };

static uint32_t crc32_for_byte(uint32_t r) {
	int j;
    for(j = 0; j < 8; ++j) {
        r = (r & 1? 0: (uint32_t)0xEDB88320L) ^ r >> 1;
    }

    return r ^ (uint32_t)0xFF000000L;
}

static void crc32(const void *data, size_t n_bytes, uint32_t* crc) {
    static uint32_t table[0x100] = { 0 };
    size_t i;

    if(!*table) {
        for(i = 0; i < 0x100; ++i) {
            table[i] = crc32_for_byte((uint32_t)i);
        }
    }

    *crc = 0;
    for(i = 0; i < n_bytes; ++i) {
        *crc = table[(uint8_t)*crc ^ ((uint8_t*)data)[i]] ^ *crc >> 8;
    }
}

static void crc32_fill_test_data(char array[], size_t size) {
    size_t i;

    for(i = 0; i < size; ++i) {
        array[i] = (char)rnd_get_int_range(32, 126);
    }
}

static bench_result_t bench_crc32_hashes() {
    const int iterations = BENCH_CRC32_ITERATIONS;
    
    uint32_t crc32num;
    uint32_t result_hash = 0;
    int i;
    bench_result_t result;
    
    for(i = 0; i < iterations; ++i) {
        crc32_fill_test_data(test_crc32_data, CRC32_TEST_DATA_SIZE);
        crc32(test_crc32_data, CRC32_TEST_DATA_SIZE, &crc32num);
        result_hash ^= crc32num;
    }
    
    result.uint32_value = result_hash;
    return result;
}

/* rle */

#define RLE_TEST_DATA_SIZE 1024

static uint8_t test_rle_data[RLE_TEST_DATA_SIZE] = { 0 };

static void rle_compress(uint8_t* input, size_t input_size, uint8_t* output, size_t* output_size) {
    size_t n, output_marker;
    uint16_t same_bytes_num;
    uint8_t last_byte, current_byte; 

    output_marker = 0;
    same_bytes_num = 1;
    last_byte = input[0];
    for(n = 1; n < input_size; ++n) {
        current_byte = input[n];
        if(current_byte == last_byte) {
            same_bytes_num++;
        } else {
            /* for single bytes, just copy them, for sequences, repeat value twice as a chain marker */
            if(same_bytes_num == 1) {
                output[output_marker++] = last_byte;
            } else {
                output[output_marker++] = last_byte;
                output[output_marker++] = last_byte;
                output[output_marker++] = (uint8_t)(same_bytes_num & 0xFF);
                output[output_marker++] = (uint8_t)(same_bytes_num >> 8);
            }
            
            same_bytes_num = 1;
        }

        last_byte = current_byte;
    }

    *output_size = output_marker;
}

static void rle_fill_test_data(uint8_t data[], size_t size) {
    size_t i, next_chunk_size = 0;
    uint8_t next_chunk_value = 0;

    next_chunk_size = 0;
    for(i = 0; i < size; ++i) {
        if(next_chunk_size == 0) {
            next_chunk_size = (size_t)rnd_get_int_range(1, 64);
            next_chunk_value = (uint8_t)rnd_get_int_range(0, 256);
        }

        data[i] = next_chunk_value;

        next_chunk_size--;
    }
}

static bench_result_t bench_rle_compression() {
    const int iterations = BENCH_RLE_ITERATIONS;
    
    size_t data_size = RLE_TEST_DATA_SIZE;
    size_t compressed_size;
    uint8_t compressed_data[RLE_TEST_DATA_SIZE * 3];
    double compression_factor_sum = 0.0;
    int i;
    bench_result_t result;
    
    for(i = 0; i < iterations; ++i) {
        rle_fill_test_data(test_rle_data, RLE_TEST_DATA_SIZE);
        rnb_strzero((char*)compressed_data, RLE_TEST_DATA_SIZE * 3);
        compressed_size = 0;
        rle_compress((uint8_t*)test_rle_data, data_size, compressed_data, &compressed_size);
        compression_factor_sum += ((double)compressed_size / (double)data_size);
    }
    
    result.double_value = (compression_factor_sum / iterations) * 100.0;
    return result;
}

/* quick sort */

#define BENCH_QSORT_TEST_DATA_SIZE 64

static int test_array[BENCH_QSORT_TEST_DATA_SIZE] = { 0 };

static void qsort_fill_test_data(int array[], size_t size) {
    size_t i;

    for(i = 0; i < size; ++i) {
        array[i] = rnd_get_int_range(0, 100);
    }
}

static uint32_t qsort_array_sum(int array[], size_t size) {
    uint32_t result = 0;

    size_t i;
    for(i = 0; i < size; ++i) {
        result += array[i];
    }

    return result;
}

static void qsort_swap(int array[], size_t i1, size_t i2) {
    int aux = array[i1];
    array[i1] = array[i2];
    array[i2] = aux;
}

static void qsort(int array[], int l, int r) {
    int i = l, j = r;
    int x = array[l + (r - l) / 2]; /* middle element */
    
    do {
        while(array[i] < x) ++i;
        while(array[j] > x) --j;

        if(i <= j) {
            qsort_swap(array, i, j);
            ++i; --j;
        }
    } while(i < j);

    if(l < j) qsort(array, l, j);
    if(r > i) qsort(array, i, r);
}

static bench_result_t bench_quick_sort() {
    const size_t iterations = BENCH_QSORT_ITERATIONS;
    
    size_t i;
    uint32_t sum = 0;
    bench_result_t result;
    
    for(i = 0; i < iterations; ++i) {
        qsort_fill_test_data(test_array, BENCH_QSORT_TEST_DATA_SIZE);
        qsort(test_array, 0, BENCH_QSORT_TEST_DATA_SIZE-1);
        sum += qsort_array_sum(test_array, BENCH_QSORT_TEST_DATA_SIZE);
    }
    
    result.uint32_value = sum;
    return result;
}

/* running benchmarks */
unsigned int rnbench_run(rnbench_progress_fn progress_fn) {
    const double duration = 30.0;
    double start = platform_get_time();
    double now = start;
    int last_percent = -1;
    volatile bench_result_t results[BENCH_TYPE_MAX];
    unsigned int result = 0; /* number of tests performed */
    int i;

    /* seed RNG with a fixed value for deterministic test order */
    rnd_init(BENCH_RND_SEED);

    /* require a progress function; otherwise log and return early */
    if (!progress_fn) {
        platform_log("rnbench_run: progress function not provided, returning 0");
        return 0;
    }

    /* verify results */
    #ifdef DEBUG
    platform_log("Verification:\n");
    for (i = 0; i < BENCH_TYPE_MAX; ++i) {
        bench_result_t res;

        switch (i) {
            case BENCH_TYPE_RAND:
                res = bench_random_numbers();
                platform_log("Random numbers: %f\n", res.double_value);
                break;
            case BENCH_TYPE_WC:
                res = bench_word_count();
                platform_log("Word count: %d\n", res.int_value);
                break;
            case BENCH_TYPE_CRC32:
                res = bench_crc32_hashes();
                platform_log("CRC32 hash: 0x%08X\n", res.uint32_value);
                break;
            case BENCH_TYPE_RLE:
                res = bench_rle_compression();
                platform_log("RLE compression factor: %f%%\n", res.double_value);
                break;
            case BENCH_TYPE_QSORT:
                res = bench_quick_sort();
                platform_log("Quicksort array sum: %u", res.uint32_value);
                break;
        }
    }
    platform_log("\n\n");
    #endif

    progress_fn(0);
    last_percent = 0;

    while (now - start < duration) {
        int choice;
        bench_result_t res;
        int percent;

        /* choose benchmark and remember RNG state after the choice so benches don't affect scheduling RNG */
        choice = rnd_get_int_range(0, BENCH_TYPE_MAX);

        switch (choice) {
            case BENCH_TYPE_RAND:
                res = bench_random_numbers();
                break;
            case BENCH_TYPE_WC:
                res = bench_word_count();
                break;
            case BENCH_TYPE_CRC32:
                res = bench_crc32_hashes();
                break;
            case BENCH_TYPE_RLE:
                res = bench_rle_compression();
                break;
            case BENCH_TYPE_QSORT:
                res = bench_quick_sort();
                break;
        }

        results[choice] = res;
        ++result;

        now = platform_get_time();

        /* progress update only when percentage changes */
        percent = (int)(((now - start) / duration) * 100.0);
        if (percent < 0) percent = 0;
        if (percent > 100) percent = 100;
        if (percent != last_percent) {
            progress_fn(percent);
            last_percent = percent;
        }
    }

    progress_fn(RNBENCH_PROGRESS_FINISHED);

    return result;
}
