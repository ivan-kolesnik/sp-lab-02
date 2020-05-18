# Labwork #2
### Задание
##### Часть 1
Написать программу, реализующую произвольный доступ к записям в файле
с помощью указателя файла (file pointer).  

Структура записи:
* номер записи
* время создания записи (в формате FILETYME)
* текстовая строка заданной длины (80 символов)
* счетчик, показывающий, сколько раз запись изменялась  

Запись может быть пустая (инициализирована нулями). В заголовке файла необходимо
хранить количество непустых записей в файле и размер файла. Общее
количество записей в файле задается из командной строки. Пользователь должен иметь
возможность удалять и модифицировать существующие записи, обращаясь к ним по
номеру. Интерфейс с пользователем реализуется на усмотрение студента.

##### Часть 2
Написать программу, реализующую функцию файлового менеджера. Программа должна
выдавать приглашение на ввод команды.  

Поддерживаемые команды:
* Сменить директорию
* Распечатать директорию
* Скопировать файл
* Создать директорию
* Удалить файл (пустую директорию)
* Вывести подробную информацию о файле

### Выполнение
##### Часть 1
![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_1.png)

![File Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_1_result.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_2.png)

![File Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_2_result.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_3.png)

![File Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_1/demo_3_result.png)

##### Часть 2
![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_1.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_2.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_3.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_4.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_5.png)

![Console Screenshot](https://raw.githubusercontent.com/ivan-kolesnik/sp-lab-02/assets/part_2/demo_6.png)
