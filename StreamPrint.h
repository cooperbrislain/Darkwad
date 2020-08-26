#ifndef DARKWAD_STREAMPRINT_H
#define DARKWAD_STREAMPRINT_H

template<class T> inline Print &operator <<(Print &stream, T arg) { stream.print(arg); return stream; }

#endif //DARKWAD_STREAMPRINT_H
