#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include"pepepoop.h" //функции Леши
using namespace std;
using namespace sf;

RenderWindow window(VideoMode(400, 300), "Sudoku!");

/*int gamov_field[9][9] = {
    {4,0,0,5,1,9,0,0,0},
    {0,9,0,4,7,2,8,0,0},
    {2,0,0,8,6,3,0,0,0},
    {5,0,0,2,4,6,9,3,1},
    {9,0,6,1,0,5,2,0,7},
    {1,0,0,9,0,7,0,0,0},
    {0,0,4,0,5,1,6,0,0},
    {7,5,9,6,2,0,0,0,0},
    {0,0,0,0,9,0,0,0,5}}; // поле, которое должно будет генерироваться*/

int gamov_field[9][9], correct_field[81];
bool wrong_fields[81] = {}, start[81] = {};

void generate() {
    //pair<vector<vector<int>>,vector<vector<int>>> F = Generate_Field();
    pair<vector<vector<int>>,vector<vector<int>>> F =  Generate_Field_Difficulty(0);
    //0 - эксперт
    //1 - сложная
    //2 - средняя
    //3 - легкая

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            gamov_field[i][j] = F.second[i][j];
            start[j * 9 + i] = (F.second[i][j] > 0);
            correct_field[j * 9 + i] = F.first[i][j];
        }
    }
}

int field[81] = {};
int length, width;
int step;
int errors = 0;

struct point {
    int x, y;
};

void draw_line(int x1, int y1, int x2, int y2, int w, int c1, int c2, int c3) {
    int l = length;// w = width;
    int x = x1;
    int y = y1 - w / 2;
    if (x1 == x2) {
        x = x1 - w / 2;
        y = y1;
        swap(l, w);
    }
    RectangleShape line(Vector2f(l, w));
    line.setPosition(x, y);
    line.setFillColor(Color(c1, c2, c3));
    window.draw(line);
}

vector <point> points, dig_points;
vector <int> num(81, 0);

vector <point> get_field() {
    int x = window.getSize().x, y = window.getSize().y;
    int h, w;
    if (y <= x) {
        h = int(float(y) / 20);
        w = (x - y + 2 * h) / 2;
    }
    else {
        w = int(float(x) / 20);
        h = (y - x + 2 * w) / 2;
    }
    vector <point> rez;
    point p;
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            p.x = w + i * 2 * step + step;
            p.y = h + j * 2 * step + step;
            rez.push_back(p);
        }
    }
    return rez;
}

int count_radius() {
    return length / 2 - 3;
}

vector <point> get_dig() {
    int r = count_radius();
    //cout << 1;
    vector <point> rez(10);
    int llll = window.getSize().y;
    rez[0] = {points[8].x - r, points[8].y + (llll - points[8].y - length / 2) / 4 * 3};
    for (int i = 2; i < 10; i++) {
        rez[i - 1] = {rez[i - 2].x + 2 * r + 6, rez[0].y};
    }
    return rez;
}

void draw_field(int chosen) {
    step = length / 2;
    length = int(float(min(window.getSize().x, window.getSize().y)) * 0.7 / 9);
    width = int(float(min(window.getSize().x, window.getSize().y)) / 200);
    // ----------------------------------------------------------------
    points = get_field();
    int count = 0;
    for (auto i : points) {
        if (wrong_fields[count]) {
            CircleShape square(step * 3 / 2, 4);
		    square.setPosition(i.x, i.y - step * 4 / 2 - width);
		    square.setFillColor(Color(196, 100, 100)); // устанавливаем цвет квадрата - красный
            square.rotate(45);
		    window.draw(square); // отрисовка квадрата  
        } else {
            if (!start[count] && field[count]) {
                CircleShape square(step * 3 / 2, 4);
		        square.setPosition(i.x, i.y - step * 4 / 2 - width);
		        square.setFillColor(Color(100, 164, 100)); // устанавливаем цвет квадрата - красный
                square.rotate(45);
		        window.draw(square); // отрисовка квадрата 
            }
        }
        if (field[count] == field[chosen] && chosen != -1 && field[chosen]) {
            CircleShape square(step * 3 / 2, 4);
		    square.setPosition(i.x, i.y - step * 4 / 2 - width);
		    square.setFillColor(Color(255, 255, 0)); // устанавливаем цвет квадрата - красный
            square.rotate(45);
		    window.draw(square); // отрисовка квадрата 
        }
        if (count == chosen) {
            CircleShape square(step * 3 / 2, 4);
            square.setPosition(points[chosen].x, points[chosen].y - step * 4 / 2);
            square.setFillColor(Color(151, 151, 177)); // устанавливаем цвет квадрата - голубой
            square.rotate(45);
            window.draw(square); // отрисовка квадрата 
        }
        draw_line(i.x - step, i.y - step, i.x + step, i.y - step, width, 0, 0, 0);
        draw_line(i.x - step, i.y - step, i.x - step, i.y + step, width, 0, 0, 0);
        draw_line(i.x - step, i.y + step, i.x + step, i.y + step, width, 0, 0, 0);
        draw_line(i.x + step, i.y - step, i.x + step, i.y + step, width, 0, 0, 0);
        //cout << count << " " << i.x << " " << i.y << endl;
        if (field[count]) {
            Font font;
            font.loadFromFile("arial.ttf");
            Text text;
            text.setFont(font);
            stringstream ss;
            ss << (field[count]);
            string s=ss.str();
            text.setString(s);
            text.setCharacterSize(2*step - 6);
            text.setColor(Color::Black);
            //text.setPosition(i.x + (step - 3) / 2, i.y - (step - 3) / 4);
            text.setPosition(i.x - step / 2, i.y - step);
            window.draw(text);
        }
        count++;
    }
    draw_line(points[chosen].x - step, points[chosen].y - step, points[chosen].x + step, points[chosen].y - step, width*15/10, 0, 128, 0);
    draw_line(points[chosen].x - step, points[chosen].y - step, points[chosen].x - step, points[chosen].y + step, width*15/10, 0, 128, 0);
    draw_line(points[chosen].x - step, points[chosen].y + step, points[chosen].x + step, points[chosen].y + step, width*15/10, 0, 128, 0);
    draw_line(points[chosen].x + step, points[chosen].y - step, points[chosen].x + step, points[chosen].y + step, width*15/10, 0, 128, 0); 
    for (int i = 0; i < 3; i++) {
        for (int j = 3 * 9 * i; j < 3 * 9 * i + 9; j++) {
            draw_line(points[j].x - step, points[j].y - step, points[j].x - step, points[j].y + step, width*20/10, 0, 0, 0);
        }
    }
    for (int i = 72; i < 81; i++)
        draw_line(points[i].x + step, points[i].y - step, points[i].x + step, points[i].y + step, width*20/10, 0, 0, 0);

    for (int i = 0; i < 9; i++) {
        for (int j = 9 * i; j < 9 * i + 9; j += 3) {
            draw_line(points[j].x - step, points[j].y - step, points[j].x + step, points[j].y - step, width*20/10, 0, 0, 0);
        }
    }
    for (int i = 8; i < 81; i += 9) {
        draw_line(points[i].x - step, points[i].y + step, points[i].x + step, points[i].y + step, width*20/10, 0, 0, 0);
    }
    // ----------------------------------------------------------------
    int r = count_radius();
    //cout << 1;
    dig_points = get_dig();
    //cout << 1;
    for (int i = 1; i <= 9; i++) {
        CircleShape circle(r);
        circle.setFillColor(Color(230, 0, 0));
        circle.setOutlineColor(Color(230,0,230));
        circle.move(dig_points[i - 1].x, dig_points[i - 1].y);
        Font font;
        font.loadFromFile("arial.ttf");
        Text text;
        text.setFont(font);
        stringstream ss;
        ss << (i);
        string s=ss.str();
        text.setString(s);
        text.setCharacterSize(2*r);
        //text.setColor(Color::Blue);
        text.setPosition(dig_points[i - 1].x + r / 2, dig_points[i - 1].y - r / 4);
        window.draw(circle);
        window.draw(text);
    }
    Font font;
    font.loadFromFile("arial.ttf");
    Text text;
    text.setFont(font);
    text.setString(L"Количество ошибок:");
    text.setCharacterSize(2*r);
    text.setColor(Color::Red);
    text.setPosition((points[8].x + points[80].x) / 2, (dig_points[0].y + points[80].y) / 2);
    stringstream ss;
    ss << (errors);
    string s=ss.str();
}

int dist(int x, int y, int xx, int yy) {
    return float(sqrt((x - xx) * (x - xx) + (y - yy) * (y - yy)));
}

void translate() {
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (gamov_field[i][j]) 
                field[j * 9 + i] = gamov_field[i][j];
        }
    }
}

int main()
{
    generate();
    translate();
    int square = -1;
    while (window.isOpen()) {
        window.clear(Color::White);
        draw_field(square);
        Event event;
        while (window.pollEvent(event)) {
            // Window
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::Resized) {
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                window.setView(View(visibleArea));
            }

            // Game
            if (event.type == Event::MouseButtonPressed) {
                int x = event.mouseButton.x, y = event.mouseButton.y;
                if (x >= points[0].x - step && y >= points[0].y - step && x <= points[80].x + step && y <= points[80].y + step) {
                    double min_dist = 1e9;
                    int new_square;
                    for (int i = 0; i < 81; i++) {
                        if (dist(points[i].x, points[i].y, x, y) <= min_dist) {
                            min_dist = dist(points[i].x, points[i].y, x, y);
                            new_square = i;
                        }
                    }
                    //if (field[new_square] == 0)
                    square = new_square;
                } else {
                   if (square != -1) {
                        int digit = -1;
                        int r = count_radius();
                        for (int i = 0; i < 9; i++) {
                            if ((x - dig_points[i].x - r) * (x - dig_points[i].x - r) + (y - dig_points[i].y - r) * (y - dig_points[i].y - r) <= r * r) {
                                digit = i + 1;
                                //break;
                            }
                        }
                        //cout << count_radius() << endl;
                        if (digit != -1 && !start[square]) {
                            field[square] = digit;
                            if (correct_field[square] != digit)
                                wrong_fields[square] = 1, errors++;
                        }
                        //square = -1;
                        cout << "Количество ошибок : " << errors << "\n";
                    }
                }
            } 
        }
        window.display();
    }
    return 0;
}
