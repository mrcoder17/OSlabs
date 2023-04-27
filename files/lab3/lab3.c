#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 4096

int main(int argc, char *argv[]) {
    // Проверяем, что передан единственный аргумент - PID процесса
    if (argc != 2) {
        printf("Usage: %s <PID>\n", argv[0]);
        return 1;
    }
    // Открываем файл /proc/<PID>/pagemap
    char pagemap_path[256];
    snprintf(pagemap_path, sizeof(pagemap_path), "/proc/%s/pagemap", argv[1]);
    FILE *fp = fopen(pagemap_path, "rb");
    if (fp == NULL) {
        printf("Failed to open %s\n", pagemap_path);
        return 1;
    }
    // Читаем содержимое файла постранично
    unsigned long long page_frame_number;
    unsigned long long page_flags;
    int page_count = 0;
    while (fread(&page_frame_number, sizeof(page_frame_number), 1, fp) == 1) {
        // Получаем номер фрейма страницы
        unsigned long long page_number = page_count * PAGE_SIZE / sizeof(page_frame_number);
        // Получаем флаги страницы
        page_flags = 0;
        if (page_frame_number & (1ull << 63)) {
            page_flags |= (1 << 0); // Present
        }
        if (page_frame_number & (1ull << 62)) {
            page_flags |= (1 << 1); // Soft-dirty
        }
        if (page_frame_number & (1ull << 61)) {
            page_flags |= (1 << 2); // Exclusive
        }
        if (page_frame_number & (1ull << 55)) {
            page_flags |= (1 << 3); // Swap
        }
        if (page_frame_number & (1ull << 51)) {
            page_flags |= (1 << 4); // File
        }
        // Выводим информацию о странице
        printf("Page %d: frame=%llu flags=%llu\n", page_number, page_frame_number & ((1ull << 55) - 1), page_flags);
        page_count++;
    }
    // Закрываем файл
    fclose(fp);
    return 0;
}
