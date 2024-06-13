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
    //! Структура таймера
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
