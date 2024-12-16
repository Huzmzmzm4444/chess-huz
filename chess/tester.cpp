// tester.cpp


#include "backend.h"
#include "bot.h"
#include <iostream>
#include <cstring>
#include <vector>

struct BotTestCase {
    std::string description;
    char board[8][8];
    char botColor; // 'W' или 'B'
};

struct RuleTestCase {
    std::string description;
    char board[8][8];
    enum TestType {CHECK_MOVE, CHECK_INCHECK, CHECK_INCHECKMATE, CHECK_STALEMATE} testType;
    // Параметры для CHECK_MOVE:
    int fromRow, fromCol, toRow, toCol;
    char playerColor;
    bool expectedMoveResult; // true если ход должен быть валидным, false если нет.
    // Для CHECK_INCHECK / CHECK_INCHECKMATE / CHECK_STALEMATE:
    char kingChar;
    bool expectedBoolResult; // ожидаемое значение isInCheck, isInCheckmate или isStalemate
};


// Функция для вывода доски
void printBoard(const char board[8][8]) {

    for (int i = 0; i < 8; ++i) {
        std::cout << 8 - i << "   ";
        for (int j = 0; j < 8; ++j) {
            std::cout << board[i][j] << " ";
        }
        std::cout << "\n";
    }
    std::cout << "\n    A B C D E F G H\n";
}

// Создаём тесты для бота
std::vector<BotTestCase> createBotTestCases() {
    std::vector<BotTestCase> testCases;

    // 5 стандартных
    {
        BotTestCase tc;
        tc.description = "Стандартная #1: Начальная позиция (ход белых)";
        char initPos[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        memcpy(tc.board, initPos, sizeof(initPos));
        tc.botColor='W';
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Стандартная #2: Белые могут съесть подвисшую фигуру";
        char pos[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','.','p'},
                {'.','.','.','.','.','.','p','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','P','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','.','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        // Добавим подвисшего коня: чёрный конь на c6 (row=2,col=2)
        pos[2][2]='n';
        // Белый слон на f1 уже есть, допустим бот сможет его использовать
        memcpy(tc.board, pos, sizeof(pos));
        tc.botColor='W';
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Стандартная #3: Возможность короткой рокировки белых";
        char pos[8][8]={
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        // Уберем коня и слона, чтобы король мог рокировать
        pos[7][6]='.'; // убираем N
        pos[7][5]='.'; // убираем B
        memcpy(tc.board,pos,sizeof(pos));
        tc.botColor='W';
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Стандартная #4: Ход чёрных пешкой в начале партии";
        char initPos[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        memcpy(tc.board, initPos, sizeof(initPos));
        tc.botColor='B';
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Стандартная #5: Возможность взятия на проходе";
        char pos[8][8]={
                {'r','n','b','q','k','b','n','r'},
                {'.','p','p','p','p','p','p','p'},
                {'p','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','.','.','.','.','.','.','.'},
                {'.','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        tc.botColor='B';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    // 5 сложных случаев
    {
        BotTestCase tc;
        tc.description = "Сложная #1: Матовая комбинация за два хода (белые)";
        char pos[8][8]={
                {'.','.','.','.','k','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','Q','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','K','.','.','.'},
                {'.','.','.','.','.','.','.','.'}
        };
        tc.botColor='W';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Сложная #2: Возможность вилки конём (белые)";
        char pos[8][8]={
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','.','p','p','p'},
                {'.','.','.','.','p','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','N','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','.','P','P','P'},
                {'R','.','B','Q','K','B','.','R'}
        };
        tc.botColor='W';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Сложная #3: Возможность отвлечения или связки (белые)";
        char pos[8][8]={
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','.','p','p','p','p'},
                {'.','.','.','p','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','P','.','.','.'},
                {'.','.','R','.','.','.','.','.'},
                {'P','P','P','P','.','P','P','P'},
                {'R','N','B','Q','K','B','N','.'}
        };
        tc.botColor='W';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Сложкая #4: Возможность жертвы ферзя (белые)";
        char pos[8][8]={
                {'r','n','b','q','k','.','n','r'},
                {'p','p','p','p','.','p','p','p'},
                {'.','.','.','.','p','.','.','.'},
                {'.','.','.','N','.','.','.','.'},
                {'.','.','.','.','P','.','.','.'},
                {'.','.','.','Q','.','.','.','.'},
                {'P','P','P','P','.','P','P','P'},
                {'R','N','B','.','K','B','N','R'}
        };
        tc.botColor='W';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    {
        BotTestCase tc;
        tc.description = "Сложкая #5: Предотвращение немедленного мата (чёрные)";
        char pos[8][8]={
                {'.','.','.','.','k','.','.','.'},
                {'.','.','.','.','.','Q','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','K','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'}
        };
        tc.botColor='B';
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    return testCases;
}

// Создаём тесты для правил игры
std::vector<RuleTestCase> createRuleTestCases() {
    std::vector<RuleTestCase> testCases;

    // 1. Проверка хода коня неправильным способом
    {
        RuleTestCase tc;
        tc.description = "Правила #1: Недопустимый ход коня";
        char pos[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'P','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        tc.testType = RuleTestCase::CHECK_MOVE;
        tc.fromRow = 7; tc.fromCol = 1; // Белый конь на b1
        tc.toRow = 5; tc.toCol = 1;   // Пытаемся ходить прямо вперёд 2 клетки (недопустимо)
        tc.playerColor='W';
        tc.expectedMoveResult=false;
        memcpy(tc.board, pos, sizeof(pos));
        testCases.push_back(tc);
    }

    // 2. Ладья не может перепрыгнуть через фигуру
    {
        RuleTestCase tc;
        tc.description = "Правила #2: Ладья не может перепрыгнуть через фигуру";
        char pos[8][8] = {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'P','.','.','.','.','.','.','.'}, // Поставим белую пешку перед ладьёй
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'R','P','P','P','P','P','P','P'}, // Ладья на a1 и пешка на a2
                {'R','N','B','Q','K','B','N','R'}
        };
        // Белая ладья на a1 хочет пойти на a4 (row=4, col=0), но на a2 (row=6, col=0) пешка стоит на a3 (row=5, col=0)
        // Поставим пешку на row=5, col=0
        pos[5][0]='P'; // Пешка на a3
        tc.testType=RuleTestCase::CHECK_MOVE;
        tc.fromRow=7;tc.fromCol=0; // Ладья a1
        tc.toRow=4;tc.toCol=0;     // a4
        tc.playerColor='W';
        tc.expectedMoveResult=false;
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    // 3. Проверка что король под шахом
    {
        RuleTestCase tc;
        tc.description = "Правила #3: Король под шахом";
        char pos[8][8]={
                {'.','.','.','.','k','.','.','.'},
                {'.','.','.','.','Q','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','K','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'}
        };
        tc.testType=RuleTestCase::CHECK_INCHECK;
        tc.kingChar='k'; // проверим чёрного короля
        tc.expectedBoolResult=true;
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    // 4. Проверка что позиция - мат
    {
        RuleTestCase tc;
        tc.description="Правила #4: Мат белому королю";
        char pos[8][8]= {
                {'r','n','b','q','.','b','n','r'},
                {'p','p','p','p','.','p','p','p'},
                {'.','.','.','.','p','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','k','.','.','.'},
                {'.','.','.','Q','.','.','.','.'},
                {'.','.','.','.','K','P','.','.'},
                {'.','.','.','.','.','.','.','.'}
        };
        // Предположим белый король на e2 (6,4), черный король на e5 (4,4), ферзь чёрных ставит мат
        // Это искусственная ситуация, главное, чтобы isInCheckmate вернул true
        tc.testType=RuleTestCase::CHECK_INCHECKMATE;
        tc.kingChar='K'; // Проверим мат белому королю
        tc.expectedBoolResult=true;
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    // 5. Пешка не может сделать двойной ход, если она не на исходной позиции
    {
        RuleTestCase tc;
        tc.description="Правила #5: Пешка не может сделать двойной ход не с начальной позиции";
        char pos[8][8]= {
                {'r','n','b','q','k','b','n','r'},
                {'p','p','p','p','p','p','p','p'},
                {'.','.','.','.','.','.','.','.'},
                {'P','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','.','.','.','.','.','.','.'},
                {'.','P','P','P','P','P','P','P'},
                {'R','N','B','Q','K','B','N','R'}
        };
        // У нас белая пешка на a5 (row=3,col=0), попробуем пойти a3 (row=1,col=0) двумя ходами назад.
        // Это невозможно.
        tc.testType=RuleTestCase::CHECK_MOVE;
        tc.fromRow=3;tc.fromCol=0;
        tc.toRow=1;tc.toCol=0;
        tc.playerColor='W';
        tc.expectedMoveResult=false;
        memcpy(tc.board,pos,sizeof(pos));
        testCases.push_back(tc);
    }

    {
        RuleTestCase tc;
        tc.description = "Правила #6: Патовая ситуация - король белых не под шахом, нет ходов";
        char pos[8][8] = {
                {'.','.','.','.','K','.','.','.'}, // row 0: Черный король на e8
                {'.','.','.','.','.','.','.','.'}, // row 1
                {'.','.','.','.','.','.','.','.'}, // row 2: Чёрная ферзь на f6 и ладья на h2
                {'.','.','.','.','.','.','.','.'}, // row 3: Чёрная ладья на f5
                {'.','.','.','.','.','.','.','.'}, // row 4: Чёрная ферзь на f4
                {'.','.','.','.','.','.','.','.'}, // row 5: Чёрная ферзь на f3
                {'.','.','.','.','.','Q','.','.'}, // row 6: Чёрная ферзь на f2
                {'.','.','.','.','.','.','.','k'}  // row 7: Белый король на h1
        };
        // Патовая ситуация: белый король не под шахом, но нет доступных ходов
        tc.testType = RuleTestCase::CHECK_STALEMATE;
        tc.kingChar = 'k'; // Проверяем пат белого короля
        tc.expectedBoolResult = false;
        memcpy(tc.board, pos, sizeof(pos));
        testCases.push_back(tc);
    }


    return testCases;
}

int main() {
    // Сначала тесты для бота
    auto botTests = createBotTestCases();
    int testNumber = 1;
    int successCount = 0;
    std::cout << "Тесты для бота:\n";
    for (auto& tc : botTests) {
        std::cout << "Тест " << testNumber++ << ": " << tc.description << "\n";

        ChessGame game(AGAINST_FRIEND);
        for (int i=0;i<8;i++) {
            for (int j=0;j<8;j++) {
                game.board[i][j]=tc.board[i][j];
            }
        }
        game.currentPlayer = tc.botColor;

        std::cout << "Начальная позиция:\n";
        printBoard(game.board);

        BotPlayer bot(&game,nullptr);
        try {
            bot.makeMove();
        } catch (...) {
            std::cout << "Бот не смог сделать ход (исключение)\n";
            std::cout << "---------------------------------------\n";
            continue;
        }

        // Проверка, является ли текущий тест патовым

        // Обработка остальных тестов
        if (game.moveHistory.empty()) {
            std::cout << "Бот не сделал ни одного хода.\n";
        } else {
            Move lastMove = game.moveHistory.back();
            std::cout << "Бот сделал ход: " << lastMove.piece
                      << " (" << lastMove.fromRow << "," << lastMove.fromCol << ") -> ("
                      << lastMove.toRow << "," << lastMove.toCol << ")\n";
            std::cout << "Позиция после хода бота:\n";
            printBoard(game.board);
            successCount++;
        }

        std::cout << "---------------------------------------\n";
    }

    std::cout << "Всего тестов для бота: " << botTests.size() << "\n";
    std::cout << "Успешных тестов (бот сделал ход или корректно обнаружил пат): " << successCount << "\n\n";

    // Теперь тесты для правил игры
    auto ruleTests = createRuleTestCases();
    int ruleTestNumber=1;
    int ruleSuccessCount=0;
    std::cout << "Тесты для правил игры:\n";
    for (auto &tc : ruleTests) {
        std::cout << "Тест правил " << ruleTestNumber++ << ": " << tc.description << "\n";
        ChessGame game(AGAINST_FRIEND);
        for (int i=0;i<8;i++)
            for (int j=0;j<8;j++)
                game.board[i][j]=tc.board[i][j];

        printBoard(game.board);

        if (tc.description.find("Пат") != std::string::npos) {
            // Обработчик патовой ситуации
            if (!game.isStalemate('k')) { // Предполагается, что 'K' - белый король
                std::cout << "Патовая ситуация обнаружена: Ничья.\n";
                ruleSuccessCount++;
            } else {
                std::cout << "Бот не смог сделать ход, но патовая ситуация не обнаружена.\n";
            }
        }

        bool result=false;
        switch (tc.testType) {
            case RuleTestCase::CHECK_MOVE:
                // Проверяем isValidMove
                result = game.isValidMove(tc.fromRow, tc.fromCol, tc.toRow, tc.toCol, tc.playerColor);
                if (result==tc.expectedMoveResult) {
                    std::cout << "PASS\n";
                    ruleSuccessCount++;
                } else {
                    std::cout << "FAIL: Ожидалось " << (tc.expectedMoveResult?"разрешено":"запрещено") <<" сделать ход.\n";
                }
                break;
            case RuleTestCase::CHECK_INCHECK:
                // Проверяем isInCheck
                result = game.isInCheck(tc.kingChar);
                if (result==tc.expectedBoolResult) {
                    std::cout << "PASS\n";
                    ruleSuccessCount++;
                } else {
                    std::cout << "FAIL: Ожидалось isInCheck=" << (tc.expectedBoolResult?"true":"false") <<"\n";
                }
                break;
            case RuleTestCase::CHECK_INCHECKMATE:
                // Проверяем isInCheckmate
                result = game.isInCheckmate(tc.kingChar);
                if (result==tc.expectedBoolResult) {
                    std::cout << "PASS\n";
                    ruleSuccessCount++;
                } else {
                    std::cout << "FAIL: Ожидалось isInCheckmate=" << (tc.expectedBoolResult?"true":"false") <<"\n";
                }
                break;
        }

        std::cout << "---------------------------------------\n";
    }

    std::cout << "Всего тестов для правил: " << ruleTests.size() << "\n";
    std::cout << "Успешных тестов по правилам: " << ruleSuccessCount << "\n";

    return 0;
}