#include <stdio.h>
#include <stdlib.h>

int read_numbers(const char *filename, int *array, int max_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Could not open the file %s\n", filename);
        return -1;
    }
    int count = 0;
    int number;
    while (fscanf(file, "%d", &number) == 1 && count < max_size) {
        array[count] = number;
        count++;
    }
    fclose(file);
    if(count < 100 || count > 1000) {
        printf("The file %s must contain between 100 and 1000 numbers\n", filename);
        exit(1);
    }
    else return count;
}

int write_numbers(const char *filename, int *array, int size) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Could not open the file %s\n", filename);
        return -1;
    }
    for (int i = 0; i < size; i++) {
        fprintf(file, "%d\n", array[i]);
    }
    fclose(file);
    return 0;
}

void sort(int *array, int size) {
    __asm__ (
            "movq %rcx, %rdi;" // rdi = array - адрес начала массива (rdi используется для хранения адреса назначения)
            "movq %rdx, %rcx;" // rcx = size - количество элементов в массиве, для количества повторений цикла
            "loop1:;"
            "movq %rcx, %rdx;" // rdx = rcx - счетчик для внутреннего цикла (rdx используется для хранения дополнительных данных)
            "movq %rdi, %rsi;" // rsi = rdi - указатель на текущий элемент массива (rsi используется для хранения адреса источника)
            "loop2:;"
            "movl (%rsi), %eax;" // eax = *rsi - значение текущего элемента массива (eax используется для временного хранения данных)
            "cmpl 4(%rsi), %eax;" // compare eax and *(rsi + 4) - сравниваем текущий и следующий элементы массива по 4 байта
            "jle next;" // jump to next if eax <= *(rsi + 4) - переходим к следующей итерации, если текущий элемент не больше следующего
            "xchg 4(%rsi), %eax;" // swap eax and *(rsi + 4) - меняем местами текущий и следующий элементы массива по 4 байта
            "movl %eax, (%rsi);" // *rsi = eax - сохраняем новое значение текущего элемента массива
            "next:;"
            "addq $4, %rsi;" // rsi += 4 - переходим к следующему элементу массива
            "decq %rdx;" // rdx-- - уменьшаем счетчик для внутреннего цикла
            "cmpq $1, %rdx;" // compare rdx and 1 - сравниваем счетчик с единицей
            "jne loop2;" // jump to loop2 if rdx != 1 - повторяем внутренний цикл, если счетчик не равен единице
            "decq %rcx;" // rcx-- - уменьшаем счетчик для внешнего цикла
            "cmpq $1, %rcx;" // compare rcx and 1 - сравниваем счетчик с единицей
            "jne loop1;" // jump to loop1 if rcx != 1 - повторяем внешний цикл, если счетчик не равен единице
            );
}

void copy_last_n(int *array, int *answer, int size) {
    int *start = array + size - 10;
    __asm__ (
            "movl $10, %%ecx\n" // Устанавливаем счетчик цикла в 10
            "rep movsl\n" // Повторяем команду movsl ecx раз
            : // Нет выходных операндов
            : "S" (start), "D" (answer) // Входные операнды: указатели на исходный и новый массивы
            : "%ecx"
            );
}


int main(void) {
    int array[1000];
    const char *input_file = "input.txt";
    const char *output_file = "output.txt";

    int size = read_numbers(input_file, array, 1000);
    if (size == -1) {
        printf("An error occurred while reading numbers from a file %s\n", input_file);
        return 1;
    }

    sort(array, size);

    int answer[10];
    copy_last_n(array, answer, size);

    if (write_numbers(output_file, answer, 10) == -1) {
        printf("An error occurred when writing numbers to a file %s\n", output_file);
        return 1;
    }

    printf("The program has successfully completed its work.\n");
    return 0;
}
