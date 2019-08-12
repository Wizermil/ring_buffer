#pragma once

extern int ctr;
extern int cpy;
extern int cpy_assign;
extern int mv;
extern int mv_assign;
extern int dtr;

struct nat {
    int cnt;
    nat();
    explicit nat(int c0);
    explicit nat(int c0, int c1);
    nat(nat const& other) noexcept;
    nat& operator=(nat const& other) noexcept;
    nat(nat&& other) noexcept;
    nat& operator=(nat&& other) noexcept;
    ~nat();
};

struct nat_no_move {
    int cnt;
    nat_no_move();
    nat_no_move(nat_no_move const& other) noexcept;
    nat_no_move& operator=(nat_no_move const& other) noexcept;
    ~nat_no_move();
};

void reset_static_nat_counter() noexcept;
