#ifndef MEMORY_H
#define MEMORY_H

#include <Arduino.h>
#include <Wire.h>

class IMemory
{
  private:

  public:
    IMemory()
    {

    }
    void begin()
    {

    }
    virtual float getCurent(byte counterID)
    {
      return counterID * 105.2;
    }
};

class Memory : public IMemory
{
  private:
    int _address;
    int _pageSize;
  public:
    Memory(int addr) : IMemory()
    {
      _address = addr;

    }

    void write(unsigned int dataAddress, byte data )
    {
      Wire.beginTransmission(_address);
      Wire.send((int)(dataAddress >> 8));   // MSB
      Wire.send((int)(dataAddress & 0xFF)); // LSB
      Wire.send(data);
      Wire.endTransmission();

      delay(5);
    }

    void write(unsigned int dataAddress, unsigned int dataLength, char* data)
    {
      // Uses Page Write for 24LC256
      // Allows for 64 byte page boundary
      // Splits string into max 16 byte writes
      unsigned char i = 0, counter = 0;
      unsigned int  address;
      unsigned int  page_space;
      unsigned int  page = 0;
      unsigned int  num_writes;
      unsigned char first_write_size;
      unsigned char last_write_size;
      unsigned char write_size;

      // Calculate space available in first page
      page_space = int(((dataAddress / _pageSize) + 1) * _pageSize) - dataAddress;

      // Calculate first write size
      if (page_space > 16) {
        first_write_size = page_space - ((page_space / 16) * 16);
        if (first_write_size == 0) first_write_size = 16;
      }
      else
        first_write_size = page_space;

      // calculate size of last write
      if (dataLength > first_write_size)
        last_write_size = (data_len - first_write_size) % 16;

      // Calculate how many writes we need
      if (dataLength > first_write_size)
        num_writes = ((data_len - first_write_size) / 16) + 2;
      else
        num_writes = 1;

      i = 0;
      address = dataAddress;
      for (page = 0; page < num_writes; page++)
      {
        if (page == 0) write_size = first_write_size;
        else if (page == (num_writes - 1)) write_size = last_write_size;
        else write_size = 16;

        Wire.beginTransmission(deviceaddress);
        Wire.write((int)((address) >> 8));   // MSB
        Wire.write((int)((address) & 0xFF)); // LSB
        counter = 0;
        do {
          Wire.write((byte) data[i]);
          i++;
          counter++;
        } while ((data[i]) && (counter < write_size));
        Wire.endTransmission();
        address += write_size; // Increment address for next write

        delay(6);  // needs 5ms for page write
      }
    }

    byte read(unsigned int dataAddress )
    {
      byte rdata = 0xFF;

      Wire.beginTransmission(_address);
      Wire.send((int)(dataAddress >> 8));   // MSB
      Wire.send((int)(dataAddress & 0xFF)); // LSB
      Wire.endTransmission();

      Wire.requestFrom(_address, 1);

      if (Wire.available()) rdata = Wire.receive();

      return rdata;
    }
};

#endif//MEMORY_H
