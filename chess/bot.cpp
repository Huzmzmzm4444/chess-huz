// bot.cpp

#include "bot.h"
#include "backend.h"
#include "frontend.h"
#include <FL/Fl.H>
#include <FL/fl_ask.H>
#include <algorithm>
#include <cstdlib>
#include <iostream>

// Вспомогательная функция для добавления ходов в определённых направлениях (например для слона, ладьи, ферзя)
void BotPlayer::addMovesInDirections(const GameState& state, char piece, int fromRow, int fromCol, char playerColor,
                                 const int directions[][2], int directionCount, std::vector<Move>& validMoves) {
    for (int i = 0; i < directionCount; ++i) {
        int toRow = fromRow;
        int toCol = fromCol;
        while (true) {
            toRow += directions[i][0];
            toCol += directions[i][1];
            if (toRow < 0 || toRow >= SIZE || toCol < 0 || toCol >= SIZE) break;
            char targetPiece = state.board[toRow][toCol];
            if (targetPiece == '.') {
                validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
            } else {
                if ((piece >= 'A' && piece <= 'Z' && targetPiece >= 'a' && targetPiece <= 'z') ||
                    (piece >= 'a' && piece <= 'z' && targetPiece >= 'A' && targetPiece <= 'Z')) {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                }
                break;
            }
        }
    }
}

BotPlayer::BotPlayer(ChessGame* game, ChessBoard* board)
        : chessGame(game), chessBoard(board) {
    maxDepth = 3;
}

void BotPlayer::makeMove() {
    if (chessBoard) {
        // В GUI режиме - задержка и асинхронный ход
        chessBoard->isPlayerTurn = false;
        chessBoard->updateMessage();

        double delay = 1.0 + static_cast<double>(rand()) / RAND_MAX * 2.0;
        Fl::add_timeout(delay, botMoveCallback, this);
    } else {
        // В тестовом режиме - ход сразу
        performBotMove();
    }
}

void BotPlayer::performBotMove() {
    GameState currentState;
    copyGameState(*chessGame, currentState);

    BotMove bestMove = minimax(currentState, maxDepth, -1000000, 1000000, true);

    if (bestMove.move.fromRow == -1) {
        // Нет доступных ходов
        return;
    }

    chessGame->movePiece(bestMove.move.fromRow, bestMove.move.fromCol, bestMove.move.toRow, bestMove.move.toCol);

    // Добавляем ход в историю
    chessGame->moveHistory.push_back({chessGame->board[bestMove.move.toRow][bestMove.move.toCol],
                                      bestMove.move.fromRow, bestMove.move.fromCol,
                                      bestMove.move.toRow, bestMove.move.toCol, 'B'});
}

void BotPlayer::botMoveCallback(void* data) {
    BotPlayer* bot = static_cast<BotPlayer*>(data);

    if (bot->chessBoard && bot->chessBoard->gameOver) {
        delete bot;
        return;
    }

    bot->performBotMove();

    if (bot->chessBoard) {
        Fl::redraw();
        if (bot->chessGame->isInCheck('K')) {
            if (bot->chessGame->isInCheckmate('K')) {
                bot->chessBoard->gameOver = true;
                fl_message("Чёрные победили! Мат белому королю.");
                bot->chessBoard->updateMessage();
                bot->chessBoard->showCheckWindow(false);
                delete bot;
                return;
            } else {
                bot->chessBoard->showCheckWindow(true);
            }
        } else {
            bot->chessBoard->showCheckWindow(false);
        }

        bot->chessBoard->isPlayerTurn = true;
        bot->chessBoard->currentPlayer = 'W';
        bot->chessBoard->updateMessage();

        delete bot;
    }
}

BotPlayer::BotMove BotPlayer::minimax(const GameState& state, int depth, int alpha, int beta, bool isMaximizingPlayer) {
    if (depth == 0 || isGameOver(state)) {
        int score = evaluateBoard(state);
        return {{-1, -1, -1, -1, ' '}, score};
    }

    char playerColor = isMaximizingPlayer ? 'B' : 'W';
    std::vector<Move> possibleMoves = generateAllPossibleMoves(state, playerColor);

    if (possibleMoves.empty()) {
        int score = evaluateBoard(state);
        return {{-1, -1, -1, -1, ' '}, score};
    }

    BotMove bestMove;
    if (isMaximizingPlayer) {
        bestMove.score = -1000000;
        for (const auto& move : possibleMoves) {
            GameState newState = state;
            makeMoveOnBoard(newState, move);
            if (isInCheck(newState, 'k')) {
                continue;
            }
            BotMove currentMove = minimax(newState, depth - 1, alpha, beta, false);
            if (currentMove.score > bestMove.score) {
                bestMove.move = move;
                bestMove.score = currentMove.score;
            }
            alpha = std::max(alpha, bestMove.score);
            if (beta <= alpha) break;
        }
    } else {
        bestMove.score = 1000000;
        for (const auto& move : possibleMoves) {
            GameState newState = state;
            makeMoveOnBoard(newState, move);
            if (isInCheck(newState, 'K')) {
                continue;
            }
            BotMove currentMove = minimax(newState, depth - 1, alpha, beta, true);
            if (currentMove.score < bestMove.score) {
                bestMove.move = move;
                bestMove.score = currentMove.score;
            }
            beta = std::min(beta, bestMove.score);
            if (beta <= alpha) break;
        }
    }
    return bestMove;
}

void BotPlayer::makeMoveOnBoard(GameState& state, const Move& move) {
    char piece = state.board[move.fromRow][move.fromCol];
    char targetPiece = state.board[move.toRow][move.toCol];

    if (targetPiece != '.') {
        if (targetPiece >= 'A' && targetPiece <= 'Z') {
            state.whiteCapturedPieces.push_back(targetPiece);
        } else {
            state.blackCapturedPieces.push_back(targetPiece);
        }
    }

    state.board[move.toRow][move.toCol] = piece;
    state.board[move.fromRow][move.fromCol] = '.';

    // Превращение пешки
    if (tolower(piece) == 'p') {
        if (piece == 'P' && move.toRow == 0) {
            state.board[move.toRow][move.toCol] = 'Q';
        } else if (piece == 'p' && move.toRow == 7) {
            state.board[move.toRow][move.toCol] = 'q';
        }
    }
}

int BotPlayer::evaluateBoard(const GameState& state) {
    int score = 0;
    int pieceValues[256] = {0};
    pieceValues['P'] = -100;
    pieceValues['N'] = -320;
    pieceValues['B'] = -330;
    pieceValues['R'] = -500;
    pieceValues['Q'] = -900;
    pieceValues['K'] = -20000;
    pieceValues['p'] = 100;
    pieceValues['n'] = 320;
    pieceValues['b'] = 330;
    pieceValues['r'] = 500;
    pieceValues['q'] = 900;
    pieceValues['k'] = 20000;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            char piece = state.board[i][j];
            if (piece != '.') {
                score += pieceValues[(unsigned char)piece];
            }
        }
    }

    score += evaluateTactics(state, 'B');
    score -= evaluateTactics(state, 'W');

    return score;
}

int BotPlayer::evaluateTactics(const GameState& state, char playerColor) {
    int score = 0;
    for (int i = 2; i <= 5; ++i) {
        for (int j = 2; j <= 5; ++j) {
            char piece = state.board[i][j];
            if (piece != '.') {
                if ((playerColor == 'W' && piece >= 'A' && piece <= 'Z') ||
                    (playerColor == 'B' && piece >= 'a' && piece <= 'z')) {
                    score += 10;
                }
            }
        }
    }
    return score;
}

std::vector<Move> BotPlayer::generateAllPossibleMoves(const GameState& state, char playerColor) {
    std::vector<Move> possibleMoves;

    for (int fromRow = 0; fromRow < SIZE; ++fromRow) {
        for (int fromCol = 0; fromCol < SIZE; ++fromCol) {
            char piece = state.board[fromRow][fromCol];
            if (piece == '.') continue;

            if ((playerColor == 'W' && piece >= 'A' && piece <= 'Z') ||
                (playerColor == 'B' && piece >= 'a' && piece <= 'z')) {
                std::vector<Move> pieceMoves = getValidMovesForPiece(state, fromRow, fromCol, playerColor);
                possibleMoves.insert(possibleMoves.end(), pieceMoves.begin(), pieceMoves.end());
            }
        }
    }

    return possibleMoves;
}

std::vector<Move> BotPlayer::getValidMovesForPiece(const GameState& state, int fromRow, int fromCol, char playerColor) {
    std::vector<Move> validMoves;
    char piece = state.board[fromRow][fromCol];

    auto isOpponentPiece = [&](char pc, char tp) {
        if ((pc >= 'A' && pc <= 'Z') && (tp >= 'a' && tp <= 'z')) return true;
        if ((pc >= 'a' && pc <= 'z') && (tp >= 'A' && tp <= 'Z')) return true;
        return false;
    };

    if (tolower(piece) == 'p') {
        // Пешка
        int direction = (piece == 'P') ? -1 : 1;
        int startRow = (piece == 'P') ? 6 : 1;

        int toRow = fromRow + direction;
        if (toRow >= 0 && toRow < SIZE) {
            if (state.board[toRow][fromCol] == '.') {
                validMoves.push_back({piece, fromRow, fromCol, toRow, fromCol, playerColor});
                if (fromRow == startRow) {
                    int toRow2 = toRow + direction;
                    if (toRow2 >= 0 && toRow2 < SIZE && state.board[toRow2][fromCol] == '.') {
                        validMoves.push_back({piece, fromRow, fromCol, toRow2, fromCol, playerColor});
                    }
                }
            }
            for (int dc : {-1, 1}) {
                int toCol = fromCol + dc;
                if (toCol >= 0 && toCol < SIZE) {
                    char targetPiece = state.board[toRow][toCol];
                    if (targetPiece != '.' && isOpponentPiece(piece, targetPiece)) {
                        validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                    }
                }
            }
        }
    } else if (tolower(piece) == 'n') {
        // Конь
        int knightMoves[8][2] = {
                {-2, -1}, {-2, 1},
                {-1, -2}, {-1, 2},
                {1, -2},  {1, 2},
                {2, -1},  {2, 1}
        };
        for (auto& m : knightMoves) {
            int toRow = fromRow + m[0];
            int toCol = fromCol + m[1];
            if (toRow >= 0 && toRow < SIZE && toCol >= 0 && toCol < SIZE) {
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.' || isOpponentPiece(piece, targetPiece)) {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                }
            }
        }
    } else if (tolower(piece) == 'b') {
        // Слон
        int bishopDirections[4][2] = {
                {-1, -1}, {-1, 1},
                {1, -1},  {1, 1}
        };
        addMovesInDirections(state, piece, fromRow, fromCol, playerColor, bishopDirections, 4, validMoves);
    } else if (tolower(piece) == 'r') {
        // Ладья
        int rookDirections[4][2] = {
                {-1, 0}, {1, 0},
                {0, -1}, {0, 1}
        };
        addMovesInDirections(state, piece, fromRow, fromCol, playerColor, rookDirections, 4, validMoves);
    } else if (tolower(piece) == 'q') {
        // Ферзь
        int queenDirections[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1},           {0, 1},
                {1, -1},  {1, 0},  {1, 1}
        };
        addMovesInDirections(state, piece, fromRow, fromCol, playerColor, queenDirections, 8, validMoves);
    } else if (tolower(piece) == 'k') {
        // Король
        int kingMoves[8][2] = {
                {-1, -1}, {-1, 0}, {-1, 1},
                {0, -1},          {0, 1},
                {1, -1},  {1, 0},  {1, 1}
        };
        for (auto& m : kingMoves) {
            int toRow = fromRow + m[0];
            int toCol = fromCol + m[1];
            if (toRow >= 0 && toRow < SIZE && toCol >= 0 && toCol < SIZE) {
                char targetPiece = state.board[toRow][toCol];
                if (targetPiece == '.' || isOpponentPiece(piece, targetPiece)) {
                    validMoves.push_back({piece, fromRow, fromCol, toRow, toCol, playerColor});
                }
            }
        }
    }

    return validMoves;
}

bool BotPlayer::isOpponentPiece(char piece, char targetPiece) {
    if ((piece >= 'A' && piece <= 'Z') && (targetPiece >= 'a' && targetPiece <= 'z')) return true;
    if ((piece >= 'a' && piece <= 'z') && (targetPiece >= 'A' && targetPiece <= 'Z')) return true;
    return false;
}

bool BotPlayer::isInCheck(const GameState& state, char kingChar) {
    int kingRow = -1, kingCol = -1;

    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state.board[i][j] == kingChar) {
                kingRow = i;
                kingCol = j;
                break;
            }
        }
        if (kingRow != -1) break;
    }

    if (kingRow == -1) return true; // Король отсутствует

    char opponentColor = (kingChar == 'K') ? 'B' : 'W';

    std::vector<Move> opponentMoves = generateAllPossibleMoves(state, opponentColor);
    for (const auto& mv : opponentMoves) {
        if (mv.toRow == kingRow && mv.toCol == kingCol) {
            return true;
        }
    }

    return false;
}

bool BotPlayer::isGameOver(const GameState& state) {
    bool whiteKingPresent = false;
    bool blackKingPresent = false;
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (state.board[i][j] == 'K') whiteKingPresent = true;
            if (state.board[i][j] == 'k') blackKingPresent = true;
        }
    }
    return !(whiteKingPresent && blackKingPresent);
}

void BotPlayer::copyGameState(const ChessGame& game, GameState& state) {
    for (int i = 0; i < SIZE; ++i) {
        std::copy(game.board[i], game.board[i] + SIZE, state.board[i]);
    }
    state.whiteCapturedPieces = game.whiteCapturedPieces;
    state.blackCapturedPieces = game.blackCapturedPieces;
}