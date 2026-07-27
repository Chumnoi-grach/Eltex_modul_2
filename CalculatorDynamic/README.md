Сборка проекта:
    make directories - создать необходимые директории
    make - чтобы собрать проект
    make run - сборка и запуск проекта
    make run-test - только запуск проекта (для тетирования с удалением )

Как работает программа:
    Работает цикл: он постоянно проверяет доступные библиотеки в директории lib и записывает несколько func_name как ключевые 
    для дальнейшего выбора действия в меню по слову, я также решил добавить отображение того, какие библиотеки
    смогли дать функцию.


====================================
andrey@Ubuntu-Andrey:~/Programming/Элтекс/Модуль 2/Eltex_modul_2/CalculatorDynamic$ make run
gcc -Wall -Wextra -Werror -Wno-unused-value -Wno-unused-parameter -fPIC -o bin/calculator src/main.c -ldl
./bin/calculator
Файлы библиотеки:
        libost.so
        func_name: ost
        libsumm.so
        func_name: summ
        libpower.so
        func_name: power
        libsub.so
        func_name: sub
        libdivide.so
        func_name: divide
        libmul.so
        func_name: mul

Выберите действие:
1. Сложение (summ)
2. Вычитание (sub)
3. Умножение (mul)
4. Деление (divide)
5. Возведение в степень (power)
6. Остаток от деления (ost)
0. Выход
summ
Введите первое число: 3
Введите второе число: 4
Результат: 7.000000
===========================================

Файлы библиотеки:
        libost.so
        func_name: ost
        libsumm.so
        func_name: summ
        libpower.so
        func_name: power
        libsub.so
        func_name: sub
        libdivide.so
        func_name: divide
        libmul.so
        func_name: mul

Выберите действие:
1. Сложение (summ)
2. Вычитание (sub)
3. Умножение (mul)
4. Деление (divide)
5. Возведение в степень (power)
6. Остаток от деления (ost)
0. Выход
mul
Введите первое число: 3
Введите второе число: 4
Результат: 12.000000

===========================================

Вот тут я удалил библиотеку libsumm.so и вот программа не может найти библиотеку, а то есть и функцию,
в связи с чем ключевое слово summ не проходит валидацию.

===========================================

Файлы библиотеки:
        libost.so
        func_name: ost
        libpower.so
        func_name: power
        libsub.so
        func_name: sub
        libdivide.so
        func_name: divide
        libmul.so
        func_name: mul

Выберите действие:
1. Сложение (summ)
2. Вычитание (sub)
3. Умножение (mul)
4. Деление (divide)
5. Возведение в степень (power)
6. Остаток от деления (ost)
0. Выход
summ


===========================================

Здесь мы снова скомпилировали библиотеку libsumm.so и вот что вышло:
ключевое слово из этой библиотеки вернулось в список функций.

===========================================

Файлы библиотеки:
        libost.so
        func_name: ost
        libsumm.so
        func_name: summ
        libpower.so
        func_name: power
        libsub.so
        func_name: sub
        libdivide.so
        func_name: divide
        libmul.so
        func_name: mul

Выберите действие:
1. Сложение (summ)
2. Вычитание (sub)
3. Умножение (mul)
4. Деление (divide)
5. Возведение в степень (power)
6. Остаток от деления (ost)
0. Выход
summ 
Введите первое число: 4
Введите второе число: 5
Результат: 9.000000


===========================================

То есть программа может обрабатывать имеющиеся библиотеки постоянно обновляя список доступных функций