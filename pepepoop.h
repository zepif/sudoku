#include <bits/stdc++.h>
#define ll long long

using namespace std;

//Меняем местами 2 ряда
vector<vector<int>> Swap_rows(vector<vector<int>> S, int n1, int n2) {
    vector<vector<int>> Sudoku = S;
    for (ll i = 0; i < 9; i++)
        swap(Sudoku[n1][i], Sudoku[n2][i]);
    return Sudoku;
}

//Меняем местами 2 столбца
vector<vector<int>> Swap_columns(vector<vector<int>> S, int n1, int n2) {
    vector<vector<int>> Sudoku = S;
    for (ll i = 0; i < 9; i++)
        swap(Sudoku[i][n1], Sudoku[i][n2]);
    return Sudoku;
}

//Меняем местами группу по 3 строки
vector<vector<int>> Swap_3rows(vector<vector<int>> S, int n1, int n2) {
    vector<vector<int>> Sudoku = S;
    Sudoku = Swap_rows(Sudoku, n1 * 3 + 0, n2 * 3 + 0);
    Sudoku = Swap_rows(Sudoku, n1 * 3 + 1, n2 * 3 + 1);
    Sudoku = Swap_rows(Sudoku, n1 * 3 + 2, n2 * 3 + 2);
    return Sudoku;
}

//Меняем местами группу по 3 столбца
vector<vector<int>> Swap_3columns(vector<vector<int>> S, int n1, int n2) {
    vector<vector<int>> Sudoku = S;
    Sudoku = Swap_columns(Sudoku, n1 * 3 + 0, n2 * 3 + 0);
    Sudoku = Swap_columns(Sudoku, n1 * 3 + 1, n2 * 3 + 1);
    Sudoku = Swap_columns(Sudoku, n1 * 3 + 2, n2 * 3 + 2);
    return Sudoku;
}

//Перегиб поля относительно диагонали
vector<vector<int>> Transposition(vector<vector<int>> S) {
    vector<vector<int>> Sudoku = S;
    for (ll i = 0; i < 8; i++)
        for (ll j = 1 + i; j < 9; j++)
            swap(Sudoku[i][j], Sudoku[j][i]);
    return Sudoku;
}

//Превращаем поле по первичному квадрату и маске, наложенной на него
vector<vector<int>> Field(vector<vector<int>> Sudoku, vector<vector<bool>> T) {
    vector<vector<int>> ReturnField;
    ReturnField.resize(9);
    for (int i = 0; i < 9; i++)
        ReturnField[i].resize(9);
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            ReturnField[i][j] = 0;

    for (ll i = 0; i < 9; i++)
        for (ll j = 0; j < 9; j++) {
            if (T[i][j] == 0) {
                ReturnField[i][j] = Sudoku[i][j];
            }
        }
    return ReturnField;
}

//Авто карандаш - дает варианты чисел, которые
//Можно вставить в клетку
//Даешь ему поле и номер клетки в нем( от 0 до 80)
vector<int> GetCandidates(vector<vector<int>> S, int n) {
    set<int> Candidates = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
    for (ll i = 0; i < 9; i++) {
        Candidates.erase(S[n / 9][i]);
    }
    for (ll i = 0; i < 9; i++) {
        Candidates.erase(S[i][n % 9]);
    }
    int sq1 = (n / 9) / 3;
    int sq2 = (n % 9) / 3;
    for (ll i = sq1 * 3; i < (sq1 + 1) * 3; i++)
        for (ll j = sq2 * 3; j < (sq2 + 1) * 3; j++)
            Candidates.erase(S[i][j]);
    vector<int> Cand;
    set<int>::iterator it = Candidates.begin();
    while (it != Candidates.end()) {
        Cand.push_back(*it);
        it++;
    }
    return Cand;
}

pair<vector<vector<int>>, bool> SudokuSolutionDown(vector<vector<int>> Sud, int r) {
    //Находим возможных кандидатов на эту клетку и перебираем. Если находим, что вариантов в оставшейся клетке нет, то
    //возвращаемся к предидущему шагу
    vector<vector<int>> S = Sud;
    if (r > 80) {
        pair<vector<vector<int>>, bool> FullGrid;
        FullGrid.first = S;
        FullGrid.second = 1;
        return FullGrid;
    }
    //Заполняем все клетки, которые заполняются однозначно
    int FilledCells;
    for (int i = 0; i < 81; i++) {
        FilledCells = 0;
        for (int j = 0; j < 81; j++)
            if (S[j / 9][j % 9] == 0) {
                vector<int> Candidates = GetCandidates(S, j);
                if (Candidates.size() == 1) {
                    S[j / 9][j % 9] = Candidates[0];
                    FilledCells++;
                }
            }
        if (FilledCells == 0) {
            break;
        }
    }
    if (S[r / 9][r % 9] != 0) {
        return SudokuSolutionDown(S, r + 1);
    }
    vector<int> Candidates = GetCandidates(S, r);
    if (Candidates.size() == 0) {
        pair<vector<vector<int>>, bool> FailedGrid;
        FailedGrid.first = S;
        FailedGrid.second = 0;
        return FailedGrid;
    } else {
        int p = 0;
        while (p != Candidates.size()) {
            S[r / 9][r % 9] = Candidates[p];
            pair<vector<vector<int>>, bool> TryGrid;
            TryGrid = SudokuSolutionDown(S, r + 1);
            if (TryGrid.second == 1) {
                return TryGrid;
            } else {
                p++;
            }
        }
        pair<vector<vector<int>>, bool> WrongGrid;
        WrongGrid.first = S;
        WrongGrid.second = 0;
        return WrongGrid;
    }
}

pair<vector<vector<int>>, bool> SudokuSolutionTop(vector<vector<int>> Sud, int r) {
    //Находим возможных кандидатов на эту клетку и перебираем. Если находим, что вариантов в оставшейся клетке нет, то
    //возвращаемся к предидущему шагу
    vector<vector<int>> S = Sud;
    if (r > 80) {
        pair<vector<vector<int>>, bool> FullGrid;
        FullGrid.first = S;
        FullGrid.second = 1;
        return FullGrid;
    }
    //Заполняем все клетки, которые заполняются однозначно
    int FilledCells;
    for (int i = 0; i < 81; i++) {
        FilledCells = 0;
        for (int j = 0; j < 81; j++)
            if (S[j / 9][j % 9] == 0) {
                vector<int> Candidates = GetCandidates(S, j);
                if (Candidates.size() == 1) {
                    S[j / 9][j % 9] = Candidates[0];
                    FilledCells++;
                }
            }
        if (FilledCells == 0) {
            break;
        }
    }
    if (S[r / 9][r % 9] != 0) {
        return SudokuSolutionTop(S, r + 1);
    }
    vector<int> Candidates = GetCandidates(S, r);
    if (Candidates.size() == 0) {
        pair<vector<vector<int>>, bool> FailedGrid;
        FailedGrid.first = S;
        FailedGrid.second = 0;
        return FailedGrid;
    } else {
        int p = Candidates.size() - 1;
        while (p != -1) {
            S[r / 9][r % 9] = Candidates[p];
            pair<vector<vector<int>>, bool> TryGrid;
            TryGrid = SudokuSolutionTop(S, r + 1);
            if (TryGrid.second == 1) {
                return TryGrid;
            } else {
                p--;
            }
        }
        pair<vector<vector<int>>, bool> WrongGrid;
        WrongGrid.first = S;
        WrongGrid.second = 0;
        return WrongGrid;
    }
}

//Решаем судоку, перебирая варианты снизу
vector<vector<int>> SolutionDown(vector<vector<int>> Puzzle) {
    pair<vector<vector<int>>, bool> Ans;
    Ans = SudokuSolutionDown(Puzzle, 0);
    return Ans.first;
}

//Решаем судоку, перебирая варианты сверху
vector<vector<int>> SolutionTop(vector<vector<int>> Puzzle) {
    pair<vector<vector<int>>, bool> Ans;
    Ans = SudokuSolutionTop(Puzzle, 0);
    return Ans.first;
}

//Проверка на то, имеет ли данное судоку единственное решение
bool IsSolvable(vector<vector<int>> Sudoku) {
    //Провера, решаемо ли судоку единственным способом
    return (SolutionDown(Sudoku) == SolutionTop(Sudoku));
    // return 0;
}

//Генерация поля
//Получаем пару - готовое поля и поле с выбитыми клетками
pair<vector<vector<int>>, vector<vector<int>>> Generate_Field() {
    pair<vector<vector<int>>, vector<vector<int>>> S;
    vector<vector<int>> Sudoku;
    Sudoku.resize(9);
    for (ll i = 0; i < 9; i++)
        Sudoku[i].resize(9);
    //Заполняем первичную табличку
    Sudoku[0] = { 9, 7, 6, 4, 8, 1, 3, 2, 5 };
    Sudoku[1] = { 1, 4, 3, 2, 5, 9, 7, 8, 6 };
    Sudoku[2] = { 5, 2, 8, 3, 7, 6, 1, 9, 4 };
    Sudoku[3] = { 6, 9, 4, 5, 1, 8, 2, 3, 7 };
    Sudoku[4] = { 8, 1, 2, 7, 3, 4, 2, 6, 9 };
    Sudoku[5] = { 7, 3, 5, 9, 6, 2, 4, 1, 8 };
    Sudoku[6] = { 4, 6, 7, 8, 2, 3, 9, 5, 1 };
    Sudoku[7] = { 2, 5, 1, 6, 9, 7, 3, 4, 5 };
    Sudoku[8] = { 9, 1, 2, 3, 4, 5, 6, 7, 8 };
    srand(time(0));
    int r1, r2, r3;
    //Перемешиваем нашу таблицу для генерации решенного судоку
    for (ll i = 0; i < 100; i++) {
        r1 = rand() % 3;
        r2 = rand() % 3;
        r3 = rand() % 3;
        Sudoku = Swap_rows(Sudoku, 3 * r1 + r2, 3 * r1 + r3);
        r1 = rand() % 3;
        r2 = rand() % 3;
        r3 = rand() % 3;
        Sudoku = Swap_columns(Sudoku, 3 * r1 + r2, 3 * r1 + r3);
        r1 = rand() % 3;
        r2 = rand() % 3;
        Sudoku = Swap_3rows(Sudoku, r1, r2);
        r1 = rand() % 3;
        r2 = rand() % 3;
        Sudoku = Swap_3columns(Sudoku, r1, r2);
        r1 = rand() % 2;
        if (r1) {
            Sudoku = Transposition(Sudoku);
        }
    }

    S.first = Sudoku;
    S.second = Sudoku;
    //Генерация задачки
    vector<vector<bool>> K, T;
    K.resize(9);
    T.resize(9);
    for (ll i = 0; i < 9; i++) {
        K[i].resize(9);
        T[i].resize(9);
    }
    for (ll i = 0; i < 9; i++)
        for (ll j = 0; j < 9; j++) {
            K[i][j] = 0;
            T[i][j] = 0;
        }
    int it = 0;
    int i, j;
    while (it < 81) {
        i = rand() % 9;
        j = rand() % 9;
        if (K[i][j] == 0) {
            it++;
            K[i][j] = 1;
            T[i][j] = 1;
            if (it < 40) {
                T[i][j] = 0;
            } else if (IsSolvable(Field(Sudoku, T)) == 0) {
                T[i][j] = 0;
            }
        }
    }
    S.second = Field(Sudoku, T);
    return S;
}

//Проверка поля на правильность по 2 полям
bool FieldRevision(vector<vector<int>> TestField, vector<vector<int>> RightField) {
    return (TestField == RightField);
}

//Сумма ряда
int RowSumm(vector<vector<int>> TestField, int n) {
    int Summ;
    for (ll i = 0; i < 9; i++) {
        Summ += TestField[n][i];
    }
    return Summ;
}

//Сумма колонны
int ColumnSumm(vector<vector<int>> TestField, int n) {
    int Summ;
    for (ll i = 0; i < 9; i++) {
        Summ += TestField[i][n];
    }
    return Summ;
}

//Сумма квадрата
int SquareSumm(vector<vector<int>> TestField, int sq1, int sq2) {
    int Summ;
    for (ll i = sq1 * 3; i < (sq1 + 1) * 3; i++)
        for (ll j = sq2 * 3; j < (sq2 + 1) * 3; j++)
            Summ += TestField[i][j];
    return Summ;
}

//Тупая проверка на правильность по 1 полю
bool FieldRevision(vector<vector<int>> TestField) {
    bool res = 1;
    for (ll i = 0; i < 9; i++) {
        if (RowSumm(TestField, i) != 45) {
            res = 0;
        }
    }
    for (ll i = 0; i < 9; i++) {
        if (ColumnSumm(TestField, i) != 45) {
            res = 0;
        }
    }
    for (ll i = 0; i < 3; i++)
        for (ll j = 0; j < 3; j++) {
            if (SquareSumm(TestField, i, j) != 45) {
                res = 0;
            }
        }
    return res;
}

//Находим список ошибок - дает массив номеров клеток
vector<int> FindMistakes(vector<vector<int>> TestField, vector<vector<int>> RightField) {
    vector<int> Mistakes;
    for (ll n = 0; n < 81; n++) {
        if (TestField[n / 9][n % 9] != RightField[n / 9][n % 9]) {
            Mistakes.push_back(n);
        }
    }
    return Mistakes;
}

//Генерация поля по данной сложности
//Получаем пару - готовое поля и поле с выбитыми клетками
pair<vector<vector<int>>, vector<vector<int>>> Generate_Field_Difficulty(int Difficulty) {
    // 0 - эксперт
    // 1 - сложная
    // 2 - средняя
    // 3 - легкая

    pair<vector<vector<int>>, vector<vector<int>>> S = Generate_Field();
    //Добавление элементов - понижение сложности
    int it = 0;
    int i, j;
    while (it < Difficulty * 5) {
        i = rand() % 9;
        j = rand() % 9;
        S.second[i][j] = S.first[i][j];
    }
    return S;
}
