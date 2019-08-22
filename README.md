# AttackLogger

Задание выполнял в ОС Ubuntu 18.04.3 LTS.
В архиве есть скомпилированный бинарник в папке build/bin/
Собирается командой:

    $ make

Сделал 2 варианта сервера с std::map и std:unordered_map чтобы сравнить скорость работы.
Написал 2 теста, которые запускаются поочереди для первого и для второго сервера.
Каждый тест запускает 2 потока, в одном происходит внесение информации об атаке,
во втором периодическое считывание Топ100 из списка.
Для удобства вывожу только первые 5 из Топ100.
 
Ключевыми в логе являются строчки:

    server: std::map           logger:   execution time: 2502 ms
    server: std::map           attacker: execution time: 4053 ms, attacks: 999752 of 1000000
    server: std::unordered_map attacker: execution time: 3563 ms, attacks: 999778 of 1000000
    server: std::unordered_map logger:   execution time: 3741 ms
 
Видно, что в первом случае так как при внесении информации происходит сортировка поток attacker затянулся по времени, но зато поток logger сработал быстрее чем во втором случае.
Во втором случае сортировка происходит при получении Топ100 и по этому времени logger требуется побольше.