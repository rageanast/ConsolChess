#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <thread>
#include <map>
#include <cctype>
#include <cmath>

using namespace std;
using namespace std::chrono;

class ChessGameTest; // Предварительное объявление класса тестов

/**
 * @brief Структура игры
 * 
 */
class ChessGame {
public:
    //! Инициализация доски
    ChessGame() {
        initializeBoard();
        whiteTimeLeft = initialTime;
        blackTimeLeft = initialTime;
        currentPlayer = 'W';
    }
    /**
     * @brief Информация к игре
     * 
     */
    void start() {
        while (true) {
            auto startTime = steady_clock::now();
            printBoard();
            printTime();
            cout << "Current player: " << (currentPlayer == 'W' ? "White" : "Black") << endl;
            cout << "Enter move (e.g., e2e4), 'save' to save the game, or 'exit' to quit: ";
            string input;
            cin >> input;

            if (input == "save") {
                saveGame();
                continue;
            }

            if (input == "exit") {
                break;
            }

            auto endTime = steady_clock::now();
            updateClock(startTime, endTime);

            if (currentPlayer == 'W' && whiteTimeLeft <= seconds(0)) {
                cout << "Black wins on time!" << endl;
                break;
            }

            if (currentPlayer == 'B' && blackTimeLeft <= seconds(0)) {
                cout << "White wins on time!" << endl;
                break;
            }
            /**
             * @brief Ход игрока
             * @param currentPlayer Проверяет цвет игрока 
             * @return Комментарий 
             * 
             */
            if (isValidMove(input)) {
                makeMove(input);
                switchPlayer();
                if (isCheckmate()) {
                    cout << (currentPlayer == 'W' ? "Black" : "White") << " wins by checkmate!" << endl;
                    break;
                }
                if (isStalemate()) {
                    cout << "The game is a draw by stalemate!" << endl;
                    break;
                }
            } else {
                cout << "Invalid move. Try again." << endl;
            }
        }
    }

    // Геттеры для тестов
    char getCurrentPlayer() const { return currentPlayer; }
    std::vector<std::vector<char>> getBoard() const { return board; }
    std::chrono::seconds getWhiteTimeLeft() const { return whiteTimeLeft; }
    std::chrono::seconds getBlackTimeLeft() const { return blackTimeLeft; }

//private:
    vector<vector<char>> board;
    map<char, string> pieceNames;
    seconds initialTime = minutes(5);
    seconds whiteTimeLeft;
    seconds blackTimeLeft;
    char currentPlayer;
    vector<string> moveLog;
    /**
     * @brief Вид доски
     */
    void initializeBoard() {
        board = {
            {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
            {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
            {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
            {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
        };

        pieceNames = {
            {'r', "Rook"}, {'n', "Knight"}, {'b', "Bishop"},
            {'q', "Queen"}, {'k', "King"}, {'p', "Pawn"},
            {'R', "Rook"}, {'N', "Knight"}, {'B', "Bishop"},
            {'Q', "Queen"}, {'K', "King"}, {'P', "Pawn"}
        };
    }
    /**
     * @brief Рисовка доски
     */
    void printBoard() {
        cout << "  a b c d e f g h" << endl;
        for (int i = 0; i < 8; i++) {
            cout << 8 - i << " ";
            for (int j = 0; j < 8; j++) {
                cout << board[i][j] << " ";
            }
            cout << 8 - i << endl;
        }
        cout << "  a b c d e f g h" << endl;
    }
    /**
     * @brief Рисовка времени
     * 
     */
    void printTime() {
        cout << "White time left: " << whiteTimeLeft.count() << " seconds" << endl;
        cout << "Black time left: " << blackTimeLeft.count() << " seconds" << endl;
    }
    /**
     * @brief Обновление таймера после хода 
     * 
     */
    void updateClock(const steady_clock::time_point& start, const steady_clock::time_point& end) {
        auto duration = duration_cast<seconds>(end - start);
        if (currentPlayer == 'W') {
            whiteTimeLeft -= duration;
        } else {
            blackTimeLeft -= duration;
        }
    }

    /**
     * @brief Определение неправильного ввода 
     * 
     */
    bool isValidMove(const string& move) {
        if (move.length() != 4) {
            cout << "Invalid input length." << endl;
            return false;
        }
        char fromFile = move[0], toFile = move[2];
        char fromRank = move[1], toRank = move[3];

        if (fromFile < 'a' || fromFile > 'h' || toFile < 'a' || toFile > 'h') {
            cout << "Invalid file." << endl;
            return false;
        }
        if (fromRank < '1' || fromRank > '8' || toRank < '1' || toRank > '8') {
            cout << "Invalid rank." << endl;
            return false;
        }

        int fromX = 8 - (fromRank - '0');
        int fromY = fromFile - 'a';
        int toX = 8 - (toRank - '0');
        int toY = toFile - 'a';

        char piece = board[fromX][fromY];
        if (currentPlayer == 'W' && piece >= 'a' && piece <= 'z') {
            cout << "It's white's turn, but selected piece is black." << endl;
            return false;
        }
        if (currentPlayer == 'B' && piece >= 'A' && piece <= 'Z') {
            cout << "It's black's turn, but selected piece is white." << endl;
            return false;
        }

        if (!isValidPieceMove(piece, fromX, fromY, toX, toY)) {
            cout << "Invalid piece move." << endl;
            return false;
        }
        
        if (isKingInCheckAfterMove(fromX, fromY, toX, toY)) {
            cout << "Move puts or leaves king in check." << endl;
            return false;
        }

        return true;
    }

    /**
     * @brief Ввод хода фигуры 
     * 
     */
    bool isValidPieceMove(char piece, int fromX, int fromY, int toX, int toY) {
        switch (toupper(piece)) {
            case 'P':
                return isValidPawnMove(piece, fromX, fromY, toX, toY);
            case 'K':
                return isValidKingMove(piece, fromX, fromY, toX, toY);
            case 'R':
                return isValidRookMove(fromX, fromY, toX, toY);
            case 'N':
                return isValidKnightMove(fromX, fromY, toX, toY);
            case 'B':
                return isValidBishopMove(fromX, fromY, toX, toY);
            case 'Q':
                return isValidQueenMove(fromX, fromY, toX, toY);
            default:
                return false;
        }
    }

    /**
     * @brief Ход пешки
     * 
     */
    bool isValidPawnMove(char piece, int fromX, int fromY, int toX, int toY) {
        int direction = (piece == 'P') ? -1 : 1;
        int startRow = (piece == 'P') ? 6 : 1;

        if (fromY == toY) {
            if (board[toX][toY] == ' ' && fromX + direction == toX) {
                return true;
            }
            if (fromX == startRow && fromX + 2 * direction == toX && board[fromX + direction][toY] == ' ' && board[toX][toY] == ' ') {
                return true;
            }
        } else if (abs(fromY - toY) == 1 && fromX + direction == toX && board[toX][toY] != ' ') {
            return true;
        }

        return false;
    }

    /**
     * @brief Ход Короля
     * 
     */
    bool isValidKingMove(char piece, int fromX, int fromY, int toX, int toY) {
        if (abs(fromX - toX) <= 1 && abs(fromY - toY) <= 1) {
            return true;
        }
        return false;
    }

    /**
     * @brief Ход ладьи
     * 
     */
    bool isValidRookMove(int fromX, int fromY, int toX, int toY) {
        if (fromX != toX && fromY != toY) {
            return false;
        }
        if (fromX == toX) {
            int step = (fromY < toY) ? 1 : -1;
            for (int y = fromY + step; y != toY; y += step) {
                if (board[fromX][y] != ' ') {
                    return false;
                }
            }
        } else {
            int step = (fromX < toX) ? 1 : -1;
            for (int x = fromX + step; x != toX; x += step) {
                if (board[x][fromY] != ' ') {
                    return false;
                }
            }
        }
        return true;
    }
    
    /**
     * @brief Ход Коня
     * 
     */
    bool isValidKnightMove(int fromX, int fromY, int toX, int toY) {
        int dx = abs(fromX - toX);
        int dy = abs(fromY - toY);
        return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
    }

    /**
     * @brief Ход Слона
     * 
     */
    bool isValidBishopMove(int fromX, int fromY, int toX, int toY) {
        if (abs(fromX - toX) != abs(fromY - toY)) {
            return false;
        }
        
        int stepX = (fromX < toX) ? 1 : -1;
        int stepY = (fromY < toY) ? 1 : -1;
        
        for (int x = fromX + stepX, y = fromY + stepY; x != toX; x += stepX, y += stepY) {
            if (board[x][y] != ' ') {
                return false;
            }
        }
        
        char destinationPiece = board[toX][toY];
        if (destinationPiece != ' ' && ((isupper(destinationPiece) && isupper(board[fromX][fromY])) || (islower(destinationPiece) && islower(board[fromX][fromY])))) {
            return false;
        }

        return true;
    }

    /**
     * @brief Ход Королевы
     * 
     */
    bool isValidQueenMove(int fromX, int fromY, int toX, int toY) {
        return isValidRookMove(fromX, fromY, toX, toY) || isValidBishopMove(fromX, fromY, toX, toY);
    }

    /**
     * @brief Проверка на Шах после хода Игрока
     * 
     */
    bool isKingInCheckAfterMove(int fromX, int fromY, int toX, int toY) {
        char capturedPiece = board[toX][toY];
        board[toX][toY] = board[fromX][fromY];
        board[fromX][fromY] = ' ';
        
        bool inCheck = isKingInCheck(currentPlayer);

        board[fromX][fromY] = board[toX][toY];
        board[toX][toY] = capturedPiece;

        return inCheck;
    }

    /**
     * @brief Проверка на Шаг до хода Игрока
     * 
     */
    bool isKingInCheck(char player) {
        int kingX, kingY;
        findKing(player, kingX, kingY);

        char opponent = (player == 'W') ? 'B' : 'W';
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                char piece = board[x][y];
                if ((opponent == 'W' && isupper(piece)) || (opponent == 'B' && islower(piece))) {
                    if (isValidPieceMove(piece, x, y, kingX, kingY)) {
                        return true;
                    }
                }
            }
        }
        return false;
    }

    /**
     * @brief Определение координат Короля указанного игрока на шахматной доске
     * 
     */
    void findKing(char player, int& kingX, int& kingY) {
        char king = (player == 'W') ? 'K' : 'k';
        for (int x = 0; x < 8; ++x) {
            for (int y = 0; y < 8; ++y) {
                if (board[x][y] == king) {
                    kingX = x;
                    kingY = y;
                    return;
                }
            }
        }
    }

    /**
     * @brief Высчитывание координат для шага
     * 
     */
    void makeMove(const string& move) {
        int fromX = 8 - (move[1] - '0');
        int fromY = move[0] - 'a';
        int toX = 8 - (move[3] - '0');
        int toY = move[2] - 'a';

        board[toX][toY] = board[fromX][fromY];
        board[fromX][fromY] = ' ';
        moveLog.push_back(move);

        if ((toX == 0 && board[toX][toY] == 'P') || (toX == 7 && board[toX][toY] == 'p')) {
            promotePawn(toX, toY);
        }
    }

    /**
     * @brief Выбор фигуры для Пешки после достижения первой линии Противника
     * 
     */
    void promotePawn(int x, int y) {
        char newPiece;
        while (true) {
            cout << "Promote pawn to (Q/R/B/N): ";
            cin >> newPiece;
            newPiece = toupper(newPiece);
            if (newPiece == 'Q' || newPiece == 'R' || newPiece == 'B' || newPiece == 'N') {
                if (islower(board[x][y])) {
                    newPiece = tolower(newPiece);
                }
                board[x][y] = newPiece;
                break;
            } else {
                cout << "Invalid choice. Try again." << endl;
            }
        }
    }

    /**
     * @brief Смена игрока
     * 
     */
    void switchPlayer() {
        currentPlayer = (currentPlayer == 'W') ? 'B' : 'W';
    }

    /**
     * @brief Сохранение игры
     * 
     */
    void saveGame() {
        ofstream outFile("chess_game_log.txt");
        for (const string& move : moveLog) {
            outFile << move << endl;
        }
        outFile.close();
        cout << "Game saved to chess_game_log.txt" << endl;
    }

    /**
     * @brief Невозможность хода Короля
     * 
     */
    bool isCheckmate() {
        return false;
    }

    /**
     * @brief Невозможность хода Игрока (Король не под угрозой)
     * 
     */
    bool isStalemate() {
        return false;
    }

    friend class ChessGameTest; // Объявление класса тестов как друга
};
