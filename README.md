UPD: смотри конец чтобы увидеть список оптимизаций

# ВОЙНЫ ЖУКОВ

Buggle Royale всегда в моде. На поле периодически появляются жуки. Все они пытаются съесть друг друга. Когда два жука сближаются достаточно близко, молодой (с большим ID) ест старого.
Каждый кадр жук выбирает себе жертву: ближайшего жука, которого он может съесть.

Танк едет на сафари по заранее сформированному маршруту и стреляет по жукам.

Симуляция заканчивается после 5000 кадров. Esc - выход из игры на любом этапе.

## Сборка:

Всё должно собраться через стандартный Build Solution. Можно использовать Debug и Release, но только x64!
Запуск либо по F5 из студии, либо сам .exe НО **с рабочей директорией установленной в каталог, где лежит BugWars.vcxproj**. Иначе игра не найдёт данные.

## Задание:

Код игры состоит из следующих частей:
* **Фреймворк**. Статическая библиотека. Проект GameBase
* **Игровой код**. Статическая библиотека. Проект BugWars
* **Исполняемый код**. Исполняемый файл. Проект BugWarsGame << Запускать по умолчанию
* **Тесты**. Юнит тесты. Проект BugWarsTest
	
Редактируйте только **игровой код**!
Во фреймворке намеренно оставлен баг, но вы можете его исправить в игровом коде.

В коде игры есть множество проблем. Все их нужно будет исправить, но сделать это нужно в определённом порядке: make it work, make it right, make it fast.

**Make it work**:

* Игра крашит в режиме отладки (Debug)
* Присутствуют утечки памяти. Количество выделенной памяти логируется на выходе. Без утечек оно должно быть равно 0.
	
**Make it right**:

* Жуки и пули ведут себя странно. Вы поймёте, о чём я.
* Танк стреляет всё время в одном направлении. Сделайте так, чтобы он попадал в жуков, стреляя с линейной поправкой на скорость цели. Это не гарантирует 100% точность, но это не страшно.
* Для автоматической проверки функции стрельбы танка, в проекте есть юнит тесты. Поначалу они не будут проходить, но это не помешает вам запустить игру.
	
**Make it fast**:

* Оптимизируйте игру. Чем быстрее она будет работать, тем лучше. Для удобства тестирования, игру можно запускать с параметром -lock60fps. 

	
Код написан без инкапсулирования - хорошие техники ООП не являются предметом данного упражнения.  
В нём намеренно допущены ошибки и неточности - исправляйте по мере обнаружения.  
Кое-где есть пара комментариев на английском - это скорее декорация. Считайте документацией сам код.  
Следовать стилю не обязательно, но приветствуется.  
К проекту подключён PIX, и в паре мест расставлены маркеры для примера. Можете использовать PIX для профилирования.  
Обратите внимание на функции логирования: вы можете их использовать. Печать идёт в консоль и в Debug Output студии. Класс Point умеет печатать себя в логе. Кликните дважды на сообщение, чтобы перейти к строке в коде. Но помните, что объёмное логгирование замедлит игру.
Игровые объекты знают свой тип - пример его проверки есть в коде.
Скорости указаны в пикселях/с.  
Время обычно считается кадрами.  
Для анимации в кадрах используется фиксированное время равное dt=~16 миллисекундам. Оно не зависит от того, сколько времени прошло между кадрами на самом деле. При запуске с -lock60fps, должна получиться плавная анимация (при условии, что игра не тормозит). Бенчмарк будет проводиться без этого параметра.  
Не удаляйте жуков, находящихся вне кадра.
Внимание: могут присутствовать ошибки, которых я не планировал. Если возникают сомнения - пишите в ЛС на Дискорде. Но не в групповой чат, чтобы не спойлерить другим студентам.  
  
**Good luck, have fun**.


# Лог оптимизаций

1. Рендеряться все объекты, давайте обновлять `visible` по мере пропадания жуков из экрана
2. Пока делал это - нашёл "баг" во фреймворке с рандомным заполнением памяти. А я-то думал почему половина объектов `disabled`
3. ~~Давайте сохранять цель жука и не применять алгоритм каждый раз~~ не сразу понял что это алгоритмически неверно
4. Дальше хотим чистить список объектов. ~~Но из-за того что мы теперь храним указатель на жука внутри другого жука, придётся извратиться~~
5. Чтобы залатать финальную утечку памяти надо просто не забывать в деструкторе `Game` отчищать все объекты
6. Кстати, зачем сравнивать расстояния если можно сравнивать их квадрат?
7. ~~Давайте хранить жуков отдельно от всех объектов чтобы не делать дайнамик каст~~ новые тесты это сломали =(
8. Починим наведение танка
9. Почему-то если переделать ranged-based for на обычный то перестаёт падать в дебаге...
10. Боттлнек - поиск ближайшего жука. Напишем алгоритм из лекции
11. А потом переделаем его так чтобы он корректно обыскивал всю карту
12. После добавления сетки всплыл ещё минибаг фреймворка: самый старый жук начинает совпадать с новым жуком (так как он не двигается), и при нормализации вектора направления всё падает
13. Теперь чтобы проходили тесты нужно добавить переход жуков из клетки в клетку сразу после перемещения во время апдейта (это странный тест, я бы поспорил)
14. Последняя оптимизация - сделать так чтобы сетка не пересоздавалась 
15. Оказывается, моя реализация увеличения радиуса неправильная, переделываем
16. Добавил оптимизацию по которой не проверяем клетки которые уже целиком проверили
17. Осознал что можно чистить клетки сетки на лету при удалении жука
18. А ещё что удалять старые объекты можно одновременно с удалением
19. Ну и на последок реорганизовал функции поиска так чтобы было меньше всяких вычислений
20. Параллельно спотыкался об ошибках в тестах

Итог: локально симуляция справляется за 6с +- чуть-чуть (иногда меньше 6с)

Конец