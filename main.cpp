#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
    // Проверить правильно ли введен запрос пользователя на работу программы
    if (argc != 3) {
        printf("Ошибка: неверное количество аргументов!\n");
        printf("Используйте: %s входной_файл выходной_файл\n", argv[0]);
        return 1;
    }
    
    FILE* input_file = fopen(argv[1], "rb");
    if (input_file == NULL) {
        printf("Ошибка: невозможно открыть входной файл '%s'\n", argv[1]);
        return 1;
    }
    
    FILE* output_file = fopen(argv[2], "wb");
    if (output_file == NULL) {
        printf("Ошибка: невозможно создать выходной файл '%s'\n", argv[2]);
        fclose(input_file);
        return 1;
    }
    
    // Структура для хранения информации о строке
    struct LineData {
        int line_number;
        int char_count;
        long file_position;
    };
    
    LineData* lines = NULL;
    int total_lines = 0;
    char buffer[1000];
    long current_pos = 0;
    int line_num = 0;
    
    printf("Чтение файла\n");
    
    // Собираем информацию о строках
    while (fgets(buffer, 1000, input_file) != NULL) {
        int chars = strlen(buffer);
        
        lines = (LineData*)realloc(lines, (total_lines + 1) * sizeof(LineData));
        
        // Сохраняем текущую позицию как начало строки
        lines[total_lines].line_number = line_num;
        lines[total_lines].char_count = chars;
        lines[total_lines].file_position = current_pos; // позиция начала строки
        
        total_lines++;
        line_num++;
        
        // Получаем позицию для следующей строки
        current_pos = ftell(input_file);
    }
    
    printf("Прочитано строк: %d\n", total_lines);
    
    // Сортировка по убыванию длины
    printf("Сортировка строк по длине...\n");
    
    for (int i = 0; i < total_lines - 1; i++) {
        for (int j = 0; j < total_lines - i - 1; j++) {
            if (lines[j].char_count < lines[j + 1].char_count) {
                LineData temp = lines[j];
                lines[j] = lines[j + 1];
                lines[j + 1] = temp;
            }
        }
    }
    
    printf("Запись в выходной файл\n");
    
    // Читаем и записываем строки в отсортированном порядке
    for (int i = 0; i < total_lines; i++) {
        // Переходим к началу строки
        fseek(input_file, lines[i].file_position, SEEK_SET);
        
        if (fgets(buffer, 1000, input_file) != NULL) {
            fputs(buffer, output_file);
        }
    }
    
    fclose(input_file);
    fclose(output_file);
    
    free(lines);
    
    printf("Готово! Отсортированные строки записаны в '%s'\n", argv[2]);
    
    return 0;
}
