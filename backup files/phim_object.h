#pragma once

#ifndef PHIM_OBJECT_H
#define PHIM_OBJECT_H


namespace objects {

    /* Default object */
    struct Phim_Object {
        // unsigned char data[bytesCount]; // Represents bytes information be aware that is backwards meaning -> data[0] is the biggest bit
        /**/
        ~Phim_Object();
        /**/
        Phim_Object();
        /**/
        virtual unsigned char* getData() = 0;
        /**/
        virtual int getDataLen() = 0;
        /* Set every byte into 0 */
        void clean();
        /* Copy number of bytes into data field from bigger field
        *   @param _cpy Data to copy
        *   @param _s Data to copy size
        *   @param _inPos Inicial position in data field
        */
        void copyBytes(unsigned char* _cpy, int _s, int _inPos);
        /* Return data byte from given position || Position must be >= 0 and < bytes amount
        *   @param _ Position of byte
        */
        unsigned char& operator[](int _);
        /* Return bit from data field || Position must be >= 0 and < bytes amount * 8 
        *   @param _ Bit position 
        */
        bool getBit(int _);
        /* Set a byte in given pos into a given value 
        *   @param _p Byte position || Position must be >= 0 and < bytes amount
        *   @param _v Value to set
        */
        bool setByte(int, unsigned char);
        /* Set a bit in given pos into a given value
        *   @param _p Bit position || Position must be >= 0 and < bytes amount * 8 
        *   @param _v Value to set
        */
        bool setBit(int, bool);
        /* Get string version of bits in data field */
        void bitsRepresentation();
        /* Return true if data field is represent a odd binary number 
        *   Is odd if last bit is 1
        */
        bool isOdd();
        /* Return true if all data field are 0 */
        bool isZero(); 

        bool operator<(Phim_Object*);
        bool operator>(Phim_Object*);
        bool operator<=(Phim_Object*);
        bool operator>=(Phim_Object*);

        bool operator==(Phim_Object*);
        bool operator!=(Phim_Object*); 

        virtual Phim_Object* operator&(Phim_Object*) = 0;
        virtual Phim_Object* operator|(Phim_Object*) = 0;
        virtual Phim_Object* operator^(Phim_Object*) = 0;
        virtual Phim_Object* operator~() = 0;
        void operator&=(Phim_Object*);
        void operator|=(Phim_Object*);
        void operator^=(Phim_Object*);

        virtual Phim_Object* operator+(Phim_Object*) = 0;
        virtual Phim_Object* operator-(Phim_Object*) = 0;
        void operator+=(Phim_Object*);
        void operator-=(Phim_Object*);

        virtual Phim_Object* operator*(Phim_Object*) = 0;
        virtual Phim_Object* operator/(Phim_Object*) = 0;
        virtual Phim_Object* operator%(Phim_Object*) = 0;
        void operator*=(Phim_Object*);
        void operator/=(Phim_Object*);
        void operator%=(Phim_Object*);

        virtual Phim_Object* operator<<(int) = 0;
        virtual Phim_Object* operator>>(int) = 0;
        void operator<<=(int);
        void operator>>=(int);

    };

}

#endif