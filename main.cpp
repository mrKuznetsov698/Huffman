#include <bits/stdc++.h>
using namespace std;

tuple<uint8_t, uint8_t, uint8_t> compress(uint16_t a, uint16_t b) {
    return {a >> 1, ((a & 1) << 7) | (b >> 2), (b & 0b11) << 6};
}

pair<uint16_t, uint16_t> decompress(uint8_t a, uint8_t b, uint8_t c) {
    return {(a << 1) | (b >> 7), ((b & 0b01111111) << 2) | (c >> 6)};
}

int main() {
    for (uint16_t i = 0; i < 512; ++i) {
        for (uint16_t j = 0; j < 512; ++j) {
            auto [a, b, c] = compress(i, j);
            auto [x, y] = decompress(a, b, c);
            if (x != i || y != j) {
                cout << i << ' ' << j << '\n';
                cout << x << ' ' << y << '\n';
                return 0;
            }
        }
    }
    return 0;
}
