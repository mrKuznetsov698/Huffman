#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

#pragma pack(push, 1) // exactly 3 bytes
struct Node {
    uint8_t left = 0;
    uint8_t right = 0;
    char c = '\0';
};
#pragma pack(pop)

Node nodes[256];
// a  b   r  a  c  a  d  a  b   r  a
// 0'110'111'0'100'0'101'0'110'111'0
// 01101110_10001010_1101110
string decode(int tree, int start, int remainder, vector<uint8_t> &data) {
    string res;
    int v = tree;
    for (int i = 8 * start; i < 8 * data.size() - remainder; ++i) {
        if ((data[i/8] >> (7 - i % 8)) & 1) {
            v = nodes[v].right;
        } else {
            v = nodes[v].left;
        }
        if (nodes[v].left == 0 && nodes[v].right == 0) {
            res += nodes[v].c;
            v = tree;
        }
    }
    return res;
}

string parse_bytes(vector<uint8_t> &data) {
    int root = data[0] << 1;
    root |= data[1] >> 7;
    int n = root;
    int remainder = (data[1] >> 4) & 0b111;
    for (int i = 1; i <= n; ++i) {
        auto v = (uint8_t*)&nodes[i];
        v[0] = data[3 * i - 1];
        v[1] = data[3 * i + 0];
        v[2] = data[3 * i + 1];
    }
    return decode(root, 2 + 3 * n, remainder, data);
}

// written by ChatGPT
vector<uint8_t> read_bytes(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file) {
        throw runtime_error("Cannot open file: " + filename);
    }

    // Get file size
    file.seekg(0, ios::end);
    streamsize size = file.tellg();
    file.seekg(0, ios::beg);

    // Read all bytes
    vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw runtime_error("Failed to read file: " + filename);
    }

    return buffer;
}
void write_string_to_file(const std::string& filename, const std::string& content) {
    std::ofstream file(filename);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }

    file << content;
    file.close();
}

int main() {
    string path;
    cin >> path;

    auto data = read_bytes(path);
    write_string_to_file(path + ".decompressed", parse_bytes(data));

    return 0;
}
