#include "NoobSudokuSolver.hpp"

NoobSudokuSolver::NoobSudokuSolver(const char* str) {
    rec = 0;
    reset();
    std::stringstream ss(str);
    uint16_t t;
    for(uint8_t y = 0; y < 9; y++) {
        for(uint8_t x = 0; x < 9; x++) {
            ss>>t;
            if(t) set(x, y, t);
        }
    }
}

void NoobSudokuSolver::set(uint8_t x, uint8_t y, uint8_t v) {
    if(cells[y][x].value) return;
    rec++;
    cells[y][x].value = v--;
    cells[y][x].flags.reset();
    uint8_t d;
    for(uint8_t i = 0; i < 9; i++)
        if(!cells[i][x].value) {
            cells[i][x].flags.reset(v);
            d = decode(cells[i][x].flags.to_ulong());
            if(d) set(x, i, d);
        }
    for(uint8_t i = 0; i < 9; i++)
        if(!cells[y][i].value) {
            cells[y][i].flags.reset(v);
            d = decode(cells[y][i].flags.to_ulong());
            if(d) set(i, y, d);
        }
    uint8_t gx = x / 3;
    uint8_t gy = y / 3;
    for(uint8_t cx = gx * 3; cx < gx * 3 + 3; cx++)
        for(uint8_t cy = gy * 3; cy < gy * 3 + 3; cy++)
            if(!cells[cy][cx].value) {
                cells[cy][cx].flags.reset(v);
                d = decode(cells[cy][cx].flags.to_ulong());
                if(d) set(cx, cy, d);
            }

    cols[x][v].reset();
    rows[y][v].reset();
    grids[gy][gx][v].reset();
    for(uint8_t i = 0; i < 9; i++) {
        cols[x][i].reset(y);
        d = decode(cols[x][i].to_ulong());
        if(d) set(x, d - 1, i + 1);

        rows[y][i].reset(x);
        d = decode(rows[y][i].to_ulong());
        if(d) set(d - 1, y, i + 1);

        grids[gy][gx][i].reset((y % 3) * 3 + x % 3);
        d = decode(grids[gy][gx][i].to_ulong());
        if(d) set(gx * 3 + (d - 1) % 3, gy * 3 + (d - 1) / 3, i + 1);
    }

    for(uint8_t i = 0; i < 9; i++) {
        if(i != x) {
            cols[i][v].reset(y);
            d = decode(cols[i][v].to_ulong());
            if(d) set(i, d - 1, v + 1);
        }

        if(i != y) {
            rows[i][v].reset(x);
            d = decode(rows[i][v].to_ulong());
            if(d) set(d - 1, i, v + 1);
        }

        if(i / 3 != gy) {
            grids[i / 3][gx][v].reset((i % 3) * 3 + x % 3);
            d = decode(grids[i / 3][gx][v].to_ulong());
            if(d) set(gx * 3 + (d - 1) % 3, (i / 3) * 3 + (d - 1) / 3, v + 1);
        }

        if(i / 3 != gx) {
            grids[gy][i / 3][v].reset((y % 3) * 3 + i % 3);
            d = decode(grids[gy][i / 3][v].to_ulong());
            if(d) set((i / 3) * 3 + (d - 1) % 3, gy * 3 + (d - 1) / 3, v + 1);
        }
    }
}

uint8_t NoobSudokuSolver::decode(unsigned long v) {
    switch(v) {
        case 1<<0: return 1;
        case 1<<1: return 2;
        case 1<<2: return 3;
        case 1<<3: return 4;
        case 1<<4: return 5;
        case 1<<5: return 6;
        case 1<<6: return 7;
        case 1<<7: return 8;
        case 1<<8: return 9;
    }
    return 0;
}

void NoobSudokuSolver::reset() {
    for(uint8_t x = 0; x < 9; x++) {
        for(uint8_t y = 0; y < 9; y++) {
            cells[y][x].flags.set();
            cells[y][x].value = 0;
        }
    }
    for(uint8_t n = 0; n < 9; n++)
        for(uint8_t i = 0; i < 9; i++) {
            rows[n][i].set();
            cols[n][i].set();
            grids[n / 3][n % 3][i].set();
        }
}

void NoobSudokuSolver::print(std::ostream &out) {
    out<<"Recursed "<<rec<<" times!\n";
    for(uint8_t y = 0; y < 9; y++) {
        if(y && !(y % 3)) out<<"----------------------\n";
        for(uint8_t x = 0; x < 9; x++) {
            if(x && !(x % 3)) out<<"| ";
            if(cells[y][x].value) out<<std::hex<<(int)cells[y][x].value;
            else out<<" ";
            out<<" ";
        }
        out<<"\n";
    }
}
