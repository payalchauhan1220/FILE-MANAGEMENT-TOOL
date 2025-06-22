#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <chrono>

using namespace std;
using namespace std::chrono;

mutex output_mutex;

// ----------- Compression Functions --------------

string rle_compress(const vector<char>& data) {
    string output;
    for (size_t i = 0; i < data.size();) {
        char current = data[i];
        size_t count = 1;
        while (i + count < data.size() && data[i + count] == current && count < 255) {
            count++;
        }
        output += current;
        output += static_cast<char>(count);
        i += count;
    }
    return output;
}

vector<char> rle_decompress(const string& data) {
    vector<char> output;
    for (size_t i = 0; i < data.size(); i += 2) {
        char ch = data[i];
        unsigned char count = static_cast<unsigned char>(data[i + 1]);
        output.insert(output.end(), count, ch);
    }
    return output;
}

// ------------- Multithreading Logic ----------------

void compress_chunk(const vector<char>& chunk, string& result) {
    string compressed = rle_compress(chunk);
    lock_guard<mutex> lock(output_mutex);
    result += compressed;
}

void decompress_chunk(const string& chunk, vector<char>& result) {
    vector<char> decompressed = rle_decompress(chunk);
    lock_guard<mutex> lock(output_mutex);
    result.insert(result.end(), decompressed.begin(), decompressed.end());
}

vector<vector<char>> split_data(const vector<char>& data, int num_chunks) {
    vector<vector<char>> chunks;
    size_t chunk_size = data.size() / num_chunks;

    for (int i = 0; i < num_chunks; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_chunks - 1) ? data.size() : start + chunk_size;
        chunks.emplace_back(data.begin() + start, data.begin() + end);
    }
    return chunks;
}

vector<string> split_compressed(const string& data, int num_chunks) {
    vector<string> chunks;
    size_t chunk_size = (data.size() / 2) / num_chunks * 2;

    for (int i = 0; i < num_chunks; ++i) {
        size_t start = i * chunk_size;
        size_t end = (i == num_chunks - 1) ? data.size() : start + chunk_size;
        if ((end - start) % 2 != 0) ++end;
        chunks.push_back(data.substr(start, end - start));
    }
    return chunks;
}

// ------------- File IO -------------------

vector<char> read_file(const string& filename) {
    ifstream in(filename, ios::binary);
    return vector<char>((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
}

void write_file(const string& filename, const vector<char>& data) {
    ofstream out(filename, ios::binary);
    out.write(data.data(), data.size());
}

void write_file(const string& filename, const string& data) {
    ofstream out(filename, ios::binary);
    out.write(data.data(), data.size());
}

// ------------- Main Interface -------------------

int main() {
    string input_file, output_file;
    int mode, threads;

    cout << "🛠️  Multithreaded File Compressor/Decompressor (RLE)\n";
    cout << "1. Compress\n2. Decompress\nEnter choice: ";
    cin >> mode;

    cout << "Enter input file path: ";
    cin >> input_file;

    cout << "Enter output file path: ";
    cin >> output_file;

    cout << "Enter number of threads: ";
    cin >> threads;

    auto start_time = high_resolution_clock::now();

    if (mode == 1) {
        vector<char> data = read_file(input_file);
        vector<vector<char>> chunks = split_data(data, threads);

        string final_result;
        vector<thread> workers;
        for (int i = 0; i < threads; ++i) {
            workers.emplace_back(compress_chunk, cref(chunks[i]), ref(final_result));
        }
        for (auto& t : workers) t.join();

        write_file(output_file, final_result);
        cout << "✅ Compression completed.\n";
    } else if (mode == 2) {
        ifstream in(input_file, ios::binary);
        string data((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
        vector<string> chunks = split_compressed(data, threads);

        vector<char> final_result;
        vector<thread> workers;
        for (int i = 0; i < threads; ++i) {
            workers.emplace_back(decompress_chunk, cref(chunks[i]), ref(final_result));
        }
        for (auto& t : workers) t.join();

        write_file(output_file, final_result);
        cout << "✅ Decompression completed.\n";
    } else {
        cout << "❌ Invalid mode.\n";
        return 1;
    }

    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);

    cout << "⏱️  Time taken: " << duration.count() << " ms\n";

    return 0;
}
