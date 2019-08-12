#include "nat.hpp"

int ctr = 0;
int cpy = 0;
int cpy_assign = 0;
int mv = 0;
int mv_assign = 0;
int dtr = 0;

nat::nat()
: cnt{++ctr} {}

nat::nat(int c0)
: cnt{c0} {
    ++ctr;
}
nat::nat(int c0, int c1)
: cnt{c0 + c1} {
    ++ctr;
}
nat::nat(nat const& other) noexcept
: cnt{other.cnt} {
    ++cpy;
}
nat& nat::operator=(nat const& other) noexcept {
    cnt = other.cnt;
    ++cpy_assign;
    return *this;
}
nat::nat(nat&& other) noexcept
: cnt{other.cnt} {
    ++mv;
}
nat& nat::operator=(nat&& other) noexcept {
    cnt = other.cnt;
    ++mv_assign;
    return *this;
}
nat::~nat() {
    ++dtr;
    cnt = -1;
}

nat_no_move::nat_no_move()
: cnt{++ctr} {}
nat_no_move::nat_no_move(nat_no_move const& other) noexcept
: cnt{other.cnt} {
    ++cpy;
}
nat_no_move& nat_no_move::operator=(nat_no_move const& other) noexcept {
    cnt = other.cnt;
    ++cpy_assign;
    return *this;
}
nat_no_move::~nat_no_move() {
    ++dtr;
    cnt = -1;
}

void reset_static_nat_counter() noexcept {
    ctr = 0;
    cpy = 0;
    cpy_assign = 0;
    mv = 0;
    mv_assign = 0;
    dtr = 0;
}
