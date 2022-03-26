#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <stdexcept>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>


class ISendable {

protected:
    std::vector<char> content;

public:
    virtual const std::vector<char>& data() { return content; };
    virtual ~ISendable() {};

    std::vector<std::vector<char>> split(int size) const {

        std::vector<std::vector<char>> outp;
        int number_of_pieces = ceil(content.size() / double(size));

        for (int i = 0; i < number_of_pieces; ++i) {

            std::vector<char> obj;
            std::copy(content.begin() + i * size, content.begin() + min((i + 1) * size, content.size()), std::back_inserter(obj));
            outp.push_back(obj);

        }
        return outp;

    }

    void append(char* buf, int buflen) {
        content.insert(content.end(), buf, &buf[buflen - 1]);
    }

};


class FileSendable : public ISendable {

private:
    std::string filename;

public:
    FileSendable(std::string filename_) : filename(filename_) {

        std::ifstream ifs(filename);
        std::string str((std::istreambuf_iterator<char>(ifs)),
            (std::istreambuf_iterator<char>()));

        std::copy(str.begin(), str.end(), std::back_inserter(content));

    };

    void save() {

        std::ofstream file;
        file.open(filename);
        for (int i = 0; i < content.size(); ++i) {
            file << content[i];
        }
        file.close();

    }

};


class StringSendable : ISendable {

private:
    std::string str;

public:
    StringSendable(std::string str) {

        std::copy(str.begin(), str.end(), std::back_inserter(content));

    };

};