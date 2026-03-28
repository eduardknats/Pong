#include <ncurses.h>
#include <stdio.h>

#define W 80
#define H 25

void print_field(int ball_x, int ball_y, int player1_y, int player2_y, int score1, int score2);
int update_paddle(int current_y, char move, char up_key, char down_key);
int update_ball_x(int ball_x, int ball_y, int d_x, int player1_y, int player2_y);
int update_dir_x(int ball_x, int ball_y, int d_x, int player1_y, int player2_y);
int update_dir_y_smart(int ball_x, int ball_y, int d_x, int d_y, int player1_y, int player2_y);

int main(void) {
    int ball_x = W / 2, ball_y = H / 2;
    int d_x = 1, d_y = 0;
    int player1_y = H / 2, player2_y = H / 2;
    int score1 = 0, score2 = 0;
    int speed = 100;

    initscr();
    noecho();
    cbreak();
    curs_set(0);

    while (score1 < 21 && score2 < 21) {
        timeout(speed);
        print_field(ball_x, ball_y, player1_y, player2_y, score1, score2);

        int move = getch();
        if (move == 'q') break;

        player1_y = update_paddle(player1_y, move, 'a', 'z');
        player2_y = update_paddle(player2_y, move, 'k', 'm');

        int next_dx = update_dir_x(ball_x, ball_y, d_x, player1_y, player2_y);
        int next_dy = update_dir_y_smart(ball_x, ball_y, d_x, d_y, player1_y, player2_y);
        int next_x = update_ball_x(ball_x, ball_y, d_x, player1_y, player2_y);
        int next_y = ball_y + next_dy;

        if (next_x < 0) {
            score2++;
            ball_x = W / 2;
            ball_y = H / 2;
            d_x = 1;
            d_y = 0;
        } else if (next_x >= W) {
            score1++;
            ball_x = W / 2;
            ball_y = H / 2;
            d_x = -1;
            d_y = 0;
        } else {
            ball_x = next_x;
            ball_y = next_y;
            d_x = next_dx;
            d_y = next_dy;
        }

        refresh();
    }

    printw("Game Over! Score %d:%d\n", score1, score2);
    timeout(-1);
    getch();
    endwin();
    return 0;
}

int update_dir_y_smart(int bx, int by, int dx, int dy, int p1y, int p2y) {
    if (bx + dx == 2 && by >= p1y - 1 && by <= p1y + 1) {
        if (by == p1y) return 0;
        return (by < p1y) ? -1 : 1;
    }
    if (bx + dx == W - 3 && by >= p2y - 1 && by <= p2y + 1) {
        if (by == p2y) return 0;
        return (by < p2y) ? -1 : 1;
    }
    if (by + dy < 1 || by + dy > H - 2) return -dy;
    return dy;
}

int update_dir_x(int bx, int by, int dx, int p1y, int p2y) {
    if ((bx + dx == 2 && by >= p1y - 1 && by <= p1y + 1) ||
        (bx + dx == W - 3 && by >= p2y - 1 && by <= p2y + 1))
        return -dx;
    return dx;
}

int update_ball_x(int bx, int by, int dx, int p1y, int p2y) {
    if ((bx + dx == 2 && by >= p1y - 1 && by <= p1y + 1) ||
        (bx + dx == W - 3 && by >= p2y - 1 && by <= p2y + 1))
        return bx;
    return bx + dx;
}

int update_paddle(int y, char move, char up, char down) {
    if (move == up && y > 2) return y - 1;
    if (move == down && y < H - 3) return y + 1;
    return y;
}

void print_field(int bx, int by, int p1y, int p2y, int score1, int score2) {
    clear();
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            if (i == 0 || i == H - 1 || j == 0 || j == W - 1)
                addch('|');
            else if (j == 2 && i >= p1y - 1 && i <= p1y + 1)
                addch('[');
            else if (j == W - 3 && i >= p2y - 1 && i <= p2y + 1)
                addch(']');
            else if (i == by && j == bx)
                addch('o');
            else
                addch(' ');
        }
        addch('\n');
    }
    printw("P1 (A/Z): %d | P2 (K/M): %d | Q: Quit\n", score1, score2);
}