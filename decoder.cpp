#include <bits/stdc++.h>
#include <cstdio>
using namespace std;

struct Node {
    int left = 0;
    int right = 0;
    uint8_t c = '\0';
};

Node nodes[513];
// a  b   r  a  c  a  d  a  b   r  a
// 0'110'111'0'100'0'101'0'110'111'0
// 01101110_10001010_1101110
vector<uint8_t> decode(int tree, int start, int remainder, vector<uint8_t> &data) {
    vector<uint8_t> res;
    int v = tree;
    for (int i = 8 * start; i < 8 * data.size() - remainder; ++i) {
        if ((data[i/8] >> (7 - i % 8)) & 1) {
            v = nodes[v].right;
        } else {
            v = nodes[v].left;
        }
        if (nodes[v].left == 0 && nodes[v].right == 0) {
            res.push_back(nodes[v].c);
            v = tree;
        }
    }
    return res;
}

vector<uint8_t> parse_bytes(vector<uint8_t> &data) {
    int root = data[0] << 1;
    root |= data[1] >> 7;
    int n = root;
    int remainder = (data[1] >> 4) & 0b111;
    for (int i = 0; i < n; ++i) {
        nodes[i + 1].left = data[2 + i * 4] << 1;
        nodes[i + 1].left |= (data[2 + i * 4 + 1] >> 7);
        nodes[i + 1].right = (data[2 + i * 4 + 1] & 0b01111111) << 2;
        nodes[i + 1].right |= (data[2 + i * 4 + 2] >> 6);
        nodes[i + 1].c = data[2 + i * 4 + 3];
    }
    return decode(root, 2 + 4 * n, remainder, data);
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
void write_bytes_to_file(const string& filename, vector<uint8_t> &data) {
    ofstream file(filename, ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file: " + filename);
    }
    for (uint8_t chunk : data) {
        file.put(chunk);
    }
    file.close();
}

int main() {
    string path;
    cin >> path;

    auto data = read_bytes(path);
    auto parsed = parse_bytes(data);
    write_bytes_to_file(path + ".decompressed", parsed);

    return 0;
}
