#include <string.h>
#include <sys/stat.h>
#include "SD.h"



bool SDLinux::begin(uint8_t csPin/* = SD_CHIP_SELECT_PIN*/) {
    _begin = true;
    std::system(("mkdir --parents " + _rootPath).c_str());
    return true;
}


void SDLinux::setRootPath(std::string rootPath) {
    _rootPath = rootPath;
}


SDLinux::~SDLinux() {
}


File SDLinux::open(const char *filename, uint8_t mode) {
    std::string fullPath = _rootPath+ "/" + filename;
    std::system(("touch " + fullPath).c_str());

    return File(fullPath.c_str(), filename, (bool)mode);
}


File::File() {

}


File::File(const char *fullPath, const char *name, bool rw) {
    _full_path = fullPath;
    strcpy(_name, name);
    _read = rw;
}

bool SDLinux::exists(const char *filepath) {
    std::string full_path = _rootPath +  filepath;
    struct stat buffer;
    return (stat(full_path.c_str(), &buffer) == 0);
}

bool SDLinux::remove(const char *filepath) {
    return !std::remove(filepath);
}



void File::close() {
    _closed = true;
}


size_t File::write(const uint8_t *buf, size_t size) {
    if (_closed) {
        return 0;
    }
    if (_read) {
        return 0;
    }
    // open file
    std::fstream _file;
    _file.open(_full_path);
    _file.seekg(_position);
    for (uint16_t i = 0; i < size; i++) {
        _file << *buf++;
    }
    _position += size;
    _file.close();
    return size;
}

size_t File::write(const char *buf, size_t size) {
    if (_closed) {
        return 0;
    }
    if (_read) {
        return 0;
    }
    // open file
    std::fstream _file;
    _file.open(_full_path);
    _file.seekg(_position);
    for (uint16_t i = 0; i < size; i++) {
        _file << *buf++;
    }
    _position += size;
    _file.close();
    return size;
}

void File::flush() {

}

int File::read(void *buf, uint16_t nbyte) {
    if(!_read){
        return 0;
    }
    std::fstream _file;
    _file.open(_full_path);
    _file.seekg(_position);
    _file.read((char *) buf, nbyte);
    if (_file.rdstate() == std::ios_base::eofbit) {
        long read_bytes = _file.gcount();
        _position += read_bytes;
        _file.close();
        return read_bytes;
    } else if (_file.rdstate() == std::ios_base::failbit) {
        return 0;
    } else if (_file.rdstate() == std::ios_base::badbit) {
        return 0;
    }
    long read_bytes = _file.gcount();
    _position += read_bytes;
    _file.close();
    return read_bytes;
}

bool File::seek(uint32_t pos) {
    _position = pos;
    return true;
}

int File::read() {
    if(!_read){
        return 0;
    }
    std::fstream _file;
    _file.open(_full_path);
    _file.seekg(_position,std::fstream::beg);
    int c = _file.get();
    if (_file.rdstate() == std::ios_base::eofbit) {
        long read_bytes = _file.gcount();
        _position += read_bytes;
        _file.close();
        return c;
    } else if (_file.rdstate() == std::ios_base::failbit) {
        return 0;
    } else if (_file.rdstate() == std::ios_base::badbit) {
        return 0;
    }
    long read_bytes = _file.gcount();
    _position += read_bytes;
    _file.close();
    return c;
}











































