# Универсальные шахматы 
Это шахматное приложение, которое позволяет играть в партии с другим человеком или против бота. Оно работает через библиотеку FLTK (Fast, Light Toolkit), что позволяет пользователям легко взаимодействовать с программным обеспечением через удобный графический интерфейс.

![strategy-competition-pieces-chessboard-checkmate](https://github.com/user-attachments/assets/e9e43acd-4e3e-43ad-95e3-b4a9122f0bca)


## Authors

- [@huz, Репкина Д. А.](https://github.com/Huzmzmzm4444)
- [@petriiqq, Фатеев П.А.]()


## Описание программы


1. Геймплей.
Программное обеспечение позволяет пользователям участвовать в регулируемой шахматной игре на традиционной доске с сеткой 8х8, действуя в соответствии со стандартными шахматными правилами. Пользователи могут просматривать элементы и взаимодействовать с ними, перетаскивая их с помощью указывающих устройств или делая выбор с помощью элементов управления с клавиатуры. 
«Инициатива включает в себя конкуренцию с ботом, использующим технику Minimax и стратегическую оценку для принятия решений».

2. Искусственный интеллект (бот).
У бота есть настройки, которые позволяют моделировать сложные механики игры в шахматы, что позволяет игрокам развивать свои навыки. 
3. Графический интерфейс.
Программа использует графическую библиотеку FLTK для создания визуально привлекательного и удобного интерфейса.
Элементы интерфейса обычно включают в себя дисплеи, значки, панели выбора и элементы управления, повышающие удобство использования и удовольствие от игрового процесса.



## Информация о использованных методах

[Фронтенд (Графический интерфейс и взаимодействие с пользователем)


1. `ChessBoard::showCheckWindow(bool show)`:
   - Показывает или скрывает окно предупреждения о шахе. Сообщает игроку, что его король находится под угрозой.

2. `ChessBoard::loadPieceImages()`:
   - Загружает изображения шахматных фигур из папки `images/` и сохраняет их в соответствующих контейнерах для последующего использования.

3. `ChessBoard::isPossibleMove(int row, int col)`:
   - Оценивает, является ли предложенный ход допустимым, сопоставляя координаты с возможными ходами активной фигуры. Возвращает `true` или `false`.

4. `ChessBoard::drawMoveHistory()`:
   - Отображает историю ходов на экране, включая изображения фигур и текстовое описание каждого хода, располагая их в два столбца.

5. `ChessBoard::getCoordinate(int row, int col)`:
   - Преобразует индексы клетки (строку и столбец) в шахматные координаты (например, (0,0) → "A8"). Возвращает строку с обозначением клетки.

6. `ChessBoard::updateMessage()`:
   - Обновляет текстовое сообщение о текущем состоянии игры. Информирует о ходе белых или черных и о завершении игры.

7. `ChessBoard::draw()`:
   - Основной метод рисования, который отображает шахматную доску, фигуры, возможные ходы, шах/мат и захваченные фигуры.

8. `ChessBoard::handle(int event)`:
   - Обрабатывает взаимодействие с пользователем (например, щелчки мыши). Управляет выбором и перемещением фигур на доске.
     

Бэкэнд (игровая логика и управление состоянием)

1. `ChessGame::isValidMove`:
   - Проверяет, является ли ход допустимым с учетом правил шахмат. Учитывает фигуру, координаты и текущего игрока.

2. `ChessGame::movePiece(int fromRow, int fromCol, int toRow, int toCol)`:
   - Выполняет перемещение фигуры с одной координаты на другую, обновляя состояние доски и, при необходимости, удаляя захваченную фигуру.

3. `ChessGame::isInCheck`:
   - Проверяет, находится ли король текущего игрока под шахом.

4. `ChessGame::isInCheckmate(char king)`:
   - Проверяет, находится ли король под шахом и может ли сделать легальный ход, чтобы избежать захвата.

5. `ChessGame::calculatePossibleMoves()`:
   - Вычисляет все возможные ходы для выбранной фигуры.

6. `ChessGame::findKingPosition(char king, int &row, int &col)`:
   - Находит позицию короля на доске и возвращает его координаты.

7. `ChessGame::moveHistory`:
   - Структура данных, хранящая историю выполненных ходов, включая информацию о предыдущих маневрах и их результатах.

Общая функциональность

- Визуальные элементы: Отображение шахматной доски, фигур, возможных ходов и истории ходов с помощью графической библиотеки FLTK.
- Логика игры: Управление состояниями игры, обработка ходов, управление шахом и матом, а также ведение истории ходов.

Этот набор методов и функций поддерживает комплексный игровой процесс, обеспечивая как взаимодействие с пользователем, так и реализацию правил шахмат.

Чуть подробнее о методах и функциях: 

Фронтенд (Графический интерфейс и взаимодействие с пользователем)

![c24e8a12-36dc-4f47-b18d-0de747b000f7](https://github.com/user-attachments/assets/51f3d5e1-cc03-41d4-a195-5e65e27b8f75)



1. `ChessBoard::showCheckWindow(bool show)`:
   - Описание: Метод отображает или скрывает окно предупреждения о шахе.
   - Параметры: 
     - `bool show`: Булевый флаг, указывающий, следует ли показать (true) или скрыть (false) окно.
   - Действия:
     - Если `show` равно true, создается новое модальное окно `checkWindow`, в котором отображается изображение (например, кошка) и текст "Ваш король под шахом!".
     - Если `checkWindow` уже существует и `show` равно false, окно скрывается.

2. `ChessBoard::loadPieceImages()`:
   - Описание: Метод загружает изображения шахматных фигур (большие и маленькие) из указанного каталога.
   - Действия:
     - Загружает PNG-изображения для каждой фигуры (пешка, ладья, конь, слон, ферзь, король) как для белых, так и для черных.
     - Использует контейнеры `pieceImages` и `smallPieceImages` для хранения загруженных изображений, которые могут быть использованы при отрисовке на доске.

3. `ChessBoard::isPossibleMove(int row, int col)`:
   - Описание: Проверяет, есть ли возможность переместить фигуру на указанную клетку.
   - Параметры: 
     - `int row`: Индекс строки (0-7).
     - `int col`: Индекс столбца (0-7).
   - Действия:
     - Проверяет, есть ли совпадение координат с возможными ходами активной фигуры, возвращая `true` или `false`.

4. `ChessBoard::drawMoveHistory()`:
   - Описание: Отображает историю ходов на экране.
   - Действия:
     - Определяет координаты для отображения и отрисовывает строки с движениями фигур на экране.
     - Каждому движению соответствует изображение фигуры и текстовое описание перемещения (например, "A2 → A3").

5. `ChessBoard::getCoordinate(int row, int col)`:
   - Описание: Преобразует индексы клетки (строку и столбец) в шахматные обозначения (например, "A8").
   - Параметры:
     - `int row`: Индекс строки (0-7).
     - `int col`: Индекс столбца (0-7).
   - Действия:
     - Генерирует и возвращает строку, представляющую координаты в шахматной нотации.

6. `ChessBoard::updateMessage()`:
   - Описание: Обновляет текст сообщения о текущем состоянии игры.
   - Действия:
     - Устанавливает текст в текстовом виджете, информируя игрока о том, чей теперь ход, и о завершении игры.

7. `ChessBoard::draw()`:
   - Описание: Основной метод для отрисовки шахматной доски и фигур.
   - Действия:
     - Отрисовывает клетки доски, фигуры на них, подсвечивает возможные ходы и отображает информацию о шахе/мате.

8. `ChessBoard::handle(int event)`:
   - Описание: Обрабатывает события взаимодействия пользователя с доской.
   - Параметры: 
     - `int event`: Тип события, которое произошло (например, щелчок мышью).
   - Действия:
     - В зависимости от типа события (например, нажатие кнопки мыши), выполняет действия по выбору фигур и перемещению, обновляя состояние игры и доски. Также позволяет игрокам анализировать свои действия и делать выводы о стратегии игры.


Бэкенд (Логика игры и управление состоянием)

1. `ChessGame::isValidMove(int fromRow, int fromCol, int toRow, int toCol, char player)`:
   - Описание: Проверяет корректность хода в соответствии с шахматными правилами.
   - Параметры:
     - `int fromRow, fromCol`: Начальная позиция фигуры.
     - `int toRow, toCol`: Конечная позиция фигуры.
     - `char player`: Символ, обозначающий игрока ('W' для белых, 'B' для черных).
   - Действия:
     - Проверяет, соответствует ли перемещение правилам игры, включая возможные захваты и ограничения.

2. `ChessGame::movePiece(int fromRow, int fromCol, int toRow, int toCol)`:
   - Описание: Выполняет перемещение фигуры на доске.
   - Параметры: 
     - `int fromRow, fromCol`: Исходные координаты.
     - `int toRow, toCol`: Целевые координаты.
   - Действия:
     - Обновляет состояние доски, перемещает фигуру в новую позицию и убирает фигуру соперника, если это возможно.

3. `ChessGame::isInCheck(char king)`:
   - Описание: Проверяет, находится ли король текущего игрока под шахом.
   - Параметры:
     - `char king`: Символ, обозначающий короля ('K' для белого и 'k' для черного).
   - Действия:
     - Анализирует угрозы к королю и возвращает `true` если он под шахом, иначе `false`.

4. `ChessGame::isInCheckmate(char king)`:
   - Описание: Проверяет, под шахом ли король и есть ли у него легальные ходы.
   - Возвращает: `true`, если король под шахом и не может сделать легальный ход; иначе `false`.

5. `ChessGame::calculatePossibleMoves(int row, int col, char player, std::vector>& possibleMoves)`:
   - Описание: Рассчитывает все возможные ходы для заданной фигуры.
   - Параметры:
     - `int row, col`: Текущие координаты фигуры.
     - `char player`: Символ текущего игрока.
     - `std::vector>& possibleMoves`: Ссылка на вектор для сохранения всех возможных ходов.
   - Действия:
     - Анализирует тип фигуры и добавляет возможные ходы в вектор `possibleMoves`.

6. `ChessGame::findKingPosition(char king, int &row, int &col)`:
   - Описание: Находит координаты короля на доске.
   - Параметры: 
     - `char king`: Символ, обозначающий короля.
     - `int &row, &col`: Ссылки для вывода координат короля.
   - Действия:
     - Ищет позицию короля и обновляет значения `row` и `col` с его координатами.

7. `ChessGame::moveHistory`:
   - Описание: Структура данных, хранящая историю совершенных ходов, что позволяет игрокам анализировать свои действия и делать выводы о стратегии игры.



## С какими проблемами мы столкнулись во время разработки:

1. Создание умного бота без нейросети.

Описание проблемы.
Реализация шахматного бота, который принимает проницательные решения и хитрые игры, является сложной задачей, особенно без использования передовых методов, таких как нейронные сети. Основные задачи заключались в учете многочисленных переменных, влияющих на тактический подход. Оцените игровое поле, чтобы разработать стратегию, используя логические рассуждения для оптимального игрового процесса без прямого состязательного прогнозирования или произвольного выбора.


Решение проблемы:
Мы разработали алгоритм, основанный на классических методах шахматного анализа. Вот несколько ключевых моментов решения:

- Алгоритм Минимакса: Мы применили алгоритм минимакса, который позволяет анализировать возможные ходы на несколько шагов вперед. Бот оценивает текущую позицию, разрабатывает варианты своих и соперника ходов, выбирая наиболее выгодный.
  
- Оценочная функция: Мы создали оценочную функцию, которая присваивает численные значения различным позициям на доске в зависимости от статуса фигур, контроля центра, безопасности короля и других критически важных факторов. Это позволило боту оценивать и сравнивать позиции.

- Поиск с отсечением (Alpha-Beta Pruning): Чтобы улучшить производительность алгоритма, мы реализовали поиск с отсечением. Это позволило сократить количество вариантов, которые бот анализировал, уменьшая вычислительные затраты и время реакций.

- Настройка уровней сложности: В зависимости от силы противника вы включили возможность настраивать уровень сложности бота. Это позволило адаптировать алгоритм в соответствии с опытом и навыками игрока.

2. Сложности графической реализации с изображения съеденных фигур

Описание проблемы:
При разработке графической части интерфейса возникли сложности с отображением съеденных фигур. Нужно было не только разработать визуальные элементы, но и правильно управлять состоянием игры, чтобы игроки могли видеть, какие фигуры были сняты с доски. Это требовало полного понимания архитектуры графической модели и логики.

Решение проблемы:
Для эффективного отображения съеденных фигур мы предприняли следующее:

- Создание контейнера для съеденных фигур: мы создали специальный контейнер, который хранит информацию о всех съеденных фигурах, включая их тип и цвет. Это позволило управлять состоянием фигур во время игры.

- Отрисовка на отдельной области: мы выделили отдельную область на интерфейсе для отображения съеденных фигур. В этом разделе каждую раз, когда фигура покидала доску, бот обновлял целевой контейнер, а графический интерфейс производил торможение фигур с помощью соответствующих изображений.

- Динамическое обновление изображения: Для отображения съеденных фигур вы обеспечили динамическую отрисовку, т.е. после каждого хода вызывался метод, который обновлял состояние области съеденных фигур, добавляя новые изображения.

- Улучшение визуального представления: мы использовали различные визуальные элементы для разных типов фигур, чтобы сделать соблюдение шахматной эстетики максимально привлекательным и интуитивно понятным для игроков.





## Потенциальные применения программы


1. Обучение:
   - Программа может быть использована как учебное пособие для новичков, которые хотят научиться основам шахмат и понять стратегическую и тактическую игру.
   - Пользователи могут тренироваться на различных уровнях сложности против бота, что позволяет им постепенно увеличивать свои навыки.

2. Развлечение:
   - Это отличная игра для людей, которые любят шахматы и хотят развлечься в одиночку или с друзьями.
   - Программа предлагает возможность играть с ботом, что делает её доступной в любое время и в любом месте, даже если у игрока нет партнера по игре.

3. Анализ партий:
   - Интересующиеся игроки могут использовать программу для анализа своих партий, обдумывая стратегии и ошибки, что может улучшить их игровую практику.
   - Бот предоставляет возможность увидеть, как принимаются решения на разных уровнях игры, что может быть полезно для самосовершенствования.

## Интерес для пользователей


- Интерактивный опыт: Графический интерфейс делает игру вовлекающей и предоставляет пользователям приятный и удобный опыт.
- Индивидуальная настройка: Пользователи могут выбирать уровень сложности бота, что позволяет им адаптировать игру под свои предпочтения.
- Обучение и развитие навыков: Программа предоставляет отличные возможности для саморазвития и повышения уровня игры.
- Интересные особенности: Включение различных шахматных механизмов, таких как шах и мат, рокировка и захват фигур, делает игру максимально приближенной к реальной.



## Заключение


Наш шахматный движок представляет собой мощный инструмент как для развлекательных, так и образовательных целей. Благодаря простому интерфейсу, возможности игры против бота и адаптивному уровню сложности, он может привлечь внимание как новичков, так и опытных игроков, желающих улучшить свои навыки в шахматах. Код и реализованные методы значительно повышают интерактивность и доступность шахматной игры. Он делает процесс более понятным и визуально привлекательным, что важно не только для опытных игроков, но и для новичков. Программа вместе с реализованными функциями создает полноценное шахматное приложение, которое может быть использовано для игр, обучения и анализа. Оно обеспечивает пользователю интерактивный и вовлекающий опыт благодаря простоте и функциональности интерфейса.

![82a13cd1-6d6a-4be8-a2c1-4af0c9924d77](https://github.com/user-attachments/assets/6b41ac2b-261f-4e49-83cf-485e45578325)


## Тестирование проекта на фокус-группе.

![OEIKCJ0](https://github.com/user-attachments/assets/bbb43c70-729d-40d8-a1b3-048e2964da19)

Для дополнительного тестирования нашей программы мы привлекли нескольких сотрудников компании L, занимающихся финансовыми операциями в аграрной сфере. Их хобби - шахматы и сложные квесты. Пара A, являющаяся основной фокус группой, которой была представлена готова программа в два подхода тестировала разные функции данного приложения.
#### Мотивация участников фокус-группы A
- Развитие навыков игры
- Конкуренция
- Удобство компактного формата приложения
- Желание испробовать новый формат любимой игры

#### Предварительное мнение (ожидание)
###### Опрос:
- Что вас привлекает в игре в шахматы?
-Постоянное развитие в плане стратегии и методах анализа
- Насколько часто вы используете программы на ПК для игры?
-Довольно редко
- Что вы ожидаете от использования нашей программы?
-Удобный интерфейс и достаточно сложного бота
- С какой целью вы планируете использовать приложение?
-Для весёлого соревновательного процесса друг с другом и развитие навыков игры

#### Сложности, с которыми столкнулась фокус-группа
Так как пользователи из группы A не имели достаточного опыта работы со средами разработки и графическими библиотеками, для них оказалось проблематичным вывести приложение на экран.
###### Решение проблемы:
Для решения данной проблемы был привлечен сотрудник компании L, имеющий достаточный опыт в разработке приложений.
### Итоги тестирования
Пользователям понравился механизм и уровень развития бота, а также возможность настраивать сложность противника (хоть и внутри кода). Режим игры "против друга" удовлетворил потребность соперников в конкуренции и совместом развитии методов анализа. Особо участники фокус-группы отметили удобную функцию выбора фигуры, в которую превращается пешка после достижения крайнего поля. В результате было проведено более четырёх партий участниками группы A. Тестирование можно считать удачным. (участники оценили изображение котёнка-шаха)
![5422835681299919134](https://github.com/user-attachments/assets/8da25d4e-53e5-490c-a399-fb5ab3282767)


![471ba8e8-9419-49ac-91c9-4b2e90a7dec6](https://github.com/user-attachments/assets/3d2ca465-705a-43fa-b550-573e74d44384)
![d2a8c98e-6fa0-47ed-84b0-9c23e84e9afe](https://github.com/user-attachments/assets/25d9111d-78b5-42c4-a289-bcb579dac730)



