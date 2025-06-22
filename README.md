# FILE-MANAGEMENT-TOOL

COMPANY: CODETECH IT SOLUTIONS

NAME: PAYAL CHAUHAN

INTERN ID: CT04DN1057

DOMAIN: C++ PROGRAMMING

DURATION: 4 WEEKS

MENTOR: NEELA SANTOSH

DESCRIPTION: For this project, I developed a multithreaded file compression and decompression tool using C++. The goal was to design a console-based application that can efficiently compress and decompress large files, using multithreading to demonstrate clear performance gains. The compression algorithm used in the tool is Run-Length Encoding (RLE) — a simple yet effective lossless compression method. It’s not only easy to understand but also ideal for showing how parallelism can improve processing speed when applied to large files.

The entire project was coded using C++17, leveraging the Standard Library’s multithreading capabilities, including std::thread, std::mutex, and std::lock_guard. These features made it easier to divide the file into chunks, compress each chunk in a separate thread, and finally combine the results. Similarly, during decompression, each encoded chunk is processed independently in separate threads. This division of workload results in a noticeable reduction in execution time, especially when working with larger files.

For this task, I used Visual Studio Code (VS Code) as my development environment. VS Code is lightweight, fast, and supports a wide variety of extensions that make C++ development smoother — such as IntelliSense for code completion and formatting, along with CMake and build tasks integration. I used the g++ compiler to compile and build the application and ran the program directly from the integrated terminal in VS Code.

In terms of structure, the program is interactive. It prompts the user to choose between compression and decompression, input the file paths, and specify how many threads to use. After execution, it reports the total time taken, allowing the user to compare performance by varying the thread count. The code also includes file I/O handling using standard ifstream and ofstream, error-checking, and memory-efficient chunk processing to ensure it works on large files without crashing.

This tool can be practically applied in scenarios where performance matters — for example, log compression, backing up large text datasets, or building preprocessors for text-based data like genomic sequences, chat logs, or telemetry data from sensors. While RLE is a basic algorithm, the structure of the tool is modular, so it can easily be extended to support more advanced algorithms like Huffman coding or even integration with zlib for .zip compression formats.

The primary goal here was not to build the most space-efficient compressor, but to show how C++'s multithreading can significantly improve processing time in compute-heavy I/O tasks. Testing it on large files clearly demonstrated that running the tool with 4 or 8 threads can nearly halve the processing time compared to a single-threaded version.

In conclusion, this project served as a great hands-on exercise in parallel programming, file handling, and performance tuning in C++. It’s an excellent demonstration of how multithreading can optimize simple algorithms, and how a minimal tool can still be powerful when designed properly with efficiency and modularity in mind.


OUTPUT:
![Image](https://github.com/user-attachments/assets/6759b160-c7d7-4e88-bbda-0fec6d56fc82)

![Image](https://github.com/user-attachments/assets/d265e164-1202-4d29-a571-7367b7667a4f)

![Image](https://github.com/user-attachments/assets/7f39b9e2-fdea-40f8-b82e-456d3d5a3f99)

![Image](https://github.com/user-attachments/assets/061f8e0b-7a51-45d2-af17-9929d534bf68)

![Image](https://github.com/user-attachments/assets/d206b64c-8ab5-45ab-a5f4-6bd340879cbc)
