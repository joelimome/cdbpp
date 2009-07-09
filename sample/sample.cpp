#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include <cdbpp.h>

#define N   100000

// Convert an integer to its string representation.
std::string int2str(int i)
{
    std::stringstream ss;
    ss << std::setfill('0') << std::setw(6) << i;
    return ss.str();
}

int main(int argc, char *argv[])
{
    // Open a file in binary mode.
    std::ofstream ofs("sample.cdb", std::ios_base::binary);
    if (ofs.fail()) {
        std::cerr << "ERROR: Failed to open a database file." << std::endl;
        return 1;
    }

    try {
        // Create an instance of CDB++ writer.
        cdbpp::builder dbw(ofs);

        // Insert key/value pairs to the CDB++ writer.
        for (int i = 0;i < N;++i) {
            std::string key = int2str(i);
            dbw.put(key.c_str(), key.length(), &i, sizeof(i));
        }

        // Destructing the CDB++ writer flushes the database to the stream.

    } catch (const cdbpp::builder_exception& e) {
        // Abort if something went wrong...
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    // Close the database file.
    ofs.close();


    // Then, open the database file for reading (with binary mode).
    std::ifstream ifs("sample.cdb", std::ios_base::binary);
    if (ifs.fail()) {
        std::cerr << "ERROR: Failed to open a database file." << std::endl;
        return 1;
    }

    try {
        // Open the database from the input stream.
        cdbpp::cdbpp dbr(ifs);
        if (!dbr.is_open()) {
            std::cerr << "ERROR: Failed to read a database file." << std::endl;
            return 1;
        }

        // Issue queries to the database.
        for (int i = 0;i < N;++i) {
            size_t vsize;
            std::string key = int2str(i);
            const int *value = (const int*)dbr.get(key.c_str(), key.length(), &vsize);
            if (value == NULL) {
                std::cerr << "ERROR: The key is not found." << std::endl;
                return 1;
            }

            if (vsize != sizeof(int) || *value != i) {
                std::cerr << "ERROR: The key value is wrong." << std::endl;
                return 1;
            }
        }

    } catch (const cdbpp::cdbpp_exception& e) {
        // Abort if something went wrong...
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}