# Сборка и запуск
```bash
mkdir build && cd build
cmake ..
make -j
./timer
```

# Задание
Разработка асинхронного таймера на C++

Описание задания:
Требуется разработать простое асинхронное приложение на языке C++, которое будет выполнять следующую задачу: 
выводить сообщение в консоль каждую секунду.

Требования к заданию:
1. Приложение должно использовать асинхронное программирование, например,
   с помощью библиотеки Boost.Asio или C++20 асинхронных операций (std::async, std::future и т.д.).
2. Таймер должен запускаться один раз при старте приложения, а затем выводить сообщение в консоль каждую секунду.
3. Приложение должно быть спроектировано таким образом, чтобы выполнять таймер асинхронно, не блокируя основной поток программы.
4. При необходимости, приложение должно предоставить возможность пользователю остановить таймер и завершить программу.

Дополнительные задания (не обязательные):
1. Реализовать возможность изменения интервала времени между выводом сообщений в консоль.
2. Добавить обработку сигналов или команды для остановки таймера и завершения программы.