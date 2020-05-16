# gui_1h2020-post

Проект представляет собой mp3-плеер, реализованный на Qt в рамках курса "Разработка прикладного программного обеспечения с графическим интерфейсом".

## Основные возможности

Добавление в плейлист mp3 с помощью окна выбора файла в системе.<br/>
Добавление в плейлист mp3 с помощью механизма "drag and drop".<br/>
Нельзя добавить больше одного экземпляра того же файла в плейлист.<br/>
Удаление из плейлиста выделенного трека.<br/>
Очистка плейлиста.<br/>
Вывод второго окна с мета информацией о треке.<br/>
Показ/скрытие списка воспроизведения.<br/>
Регулирование уровни громкости плеера.<br/>
Стандартное управление плеером: пауза, воспроизведение, переход к следующему/предыдущему треку.<br/>
Переход на нужную позицию трека с помощью движения слайдера.<br/>
Отображение прошедшей длительности трека.<br/>

## Дополнительный функционал
Гибкий интерфейс, реагирующий на изменение размеров окна приложения.<br/>
Реализована модель данных, основанная на QAbstractListModel, которая содержит в себе плейлист со всеми треками в нем.<br/>
Модель данных отображает стандартный QTableView.<br/>
Добавлены 10 движущихся с плавной анимацией столбиков на QGraphicsScene, эмитирующих отображение частотных свойств проигрываемого трека.<br/>

## Результат
![](https://github.com/esiole/gui_1h2020-post/raw/master/examples/1.png)
![](https://github.com/esiole/gui_1h2020-post/raw/master/examples/2.png)
![](https://github.com/esiole/gui_1h2020-post/raw/master/examples/3.png)
![](https://github.com/esiole/gui_1h2020-post/raw/master/examples/4.png)
![](https://github.com/esiole/gui_1h2020-post/raw/master/examples/5.png)
