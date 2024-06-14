#include "chess.cpp"
#include <iostream>
#include <string>

using namespace std;

int main()
{
  string s;
  bool newgame = true;

  cout << "CHESS" << endl;
  cout << "by Латипова Ляйля и Борисенко Анастасия, БИБ231" << endl;
  cout << "Нажмите любую кнопку и enter, чтобы начать" << endl;
  cin >> s;

  while (newgame) {
    ChessGame game;
    game.start();
    cout << "Хотите сыграть ещё раз? (y - да, n - нет) ";
    cin >> s;
    if (s != "y") {
      newgame = false;
    }
  }

  return 0;
}
