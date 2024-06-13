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
