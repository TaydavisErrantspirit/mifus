#ifndef SERIAL_PROTOCOL_H
#define SERIAL_PROTOCOL_H

#include <Arduino.h>
#include <SoftwareSerial.h>


#define PROTOCOL_VERSION    0x013A
#define MIN_PACKET_SIZE     0

#define START_MARKER        "<oct>"
#define END_MARKER          "<13a>"

class MySerial
{
public:
	virtual void begin(int rate) = 0;
	virtual bool available() = 0;
	virtual int read() = 0;
	virtual void write(byte byte) = 0;
	virtual void print(String data) = 0;
};
class MyHardwareSerial :public MySerial
{
private:

public:
	MyHardwareSerial()
	{

	}
	virtual void begin(int rate)
	{
		Serial.begin(rate);
	}
	virtual bool available()
	{
		return Serial.available();
	}
	virtual int read()
	{
		return Serial.read();
	}
	virtual void write(byte byte)
	{
		Serial.write(byte);
	}
	virtual void print(String data)
	{
		Serial.print(data);
	}
};

class MySoftwareSerial :public MySerial
{
private:
	SoftwareSerial serial;
public:
	MySoftwareSerial(byte rx, byte tx):serial(rx, tx)
	{
		
	}
	virtual void begin(int rate)
	{
		serial.begin(rate);
	}
	virtual bool available()
	{
		return serial.available();
	}
	virtual int read()
	{
		return serial.read();
	}
	virtual void write(byte byte)
	{
		serial.write(byte);
	}
	virtual void print(String data)
	{
		serial.print(data);
	}
};

class SerialProtocol
{
private:
	MySerial* _serial;
    
	String _startMarker;         
    String _stopMarker;          
    String _dataString;      
	int _startMarkerStatus; 
	int _stopMarkerStatus; 
	int _dataLength;   
	boolean _packetAvailable;

  public:
    SerialProtocol(MySerial* serial)
    {
		_serial = serial;
	}
    void begin(int rate)
    {
      _startMarker = START_MARKER; 
	  _stopMarker = END_MARKER; 
	  _dataString.reserve(64); 
	  ResetAll();   
	  _serial->begin(rate);
    }

    bool isDataAviable()
    {
      return _packetAvailable;
    }
    String getData()
    {
      return _dataString;
    }

    void ResetAll()
    {
      _dataString = ""; 
      Reset();    
	}

    void Reset()
    {
      _startMarkerStatus = 0; 
      _stopMarkerStatus = 0;  
      _dataLength = 0;   
      _packetAvailable = false; 
    }

    void Send(String data)
    {
      _serial->print(_startMarker);    
      _serial->write(data.length());   
      _serial->print(data);   
      _serial->print(_stopMarker);   
    }

    void Read()
    {
      while (_serial->available() && !_packetAvailable)    
	  {
        int bufferChar = _serial->read();                  
        if (_startMarkerStatus < _startMarker.length())      
		{
          if (_startMarker[_startMarkerStatus] == bufferChar) 
		  {
            _startMarkerStatus++;                                
		  }
          else
          {
            ResetAll();
		  }
        }
        else
        {
          if (_dataLength <= 0)                                
		  {
            _dataLength = bufferChar;                          
		  }
          else                                                    
		  {
            if (_dataLength > _dataString.length())           
			{
              _dataString += (char)bufferChar;                  
			}
            else                                             
			{
              if (_stopMarkerStatus < _stopMarker.length())  
              {
                if (_stopMarker[_stopMarkerStatus] == bufferChar) 
				{
                  _stopMarkerStatus++;                              
                  if (_stopMarkerStatus == _stopMarker.length())
                  {
                    Reset();
                    _packetAvailable = true;                       
				  }
                }
                else
                {
                  ResetAll();                                      
                }
              }
              //
            }
          }
        }
      }
    }

    String getProtocolVersion()
    {
      return String(PROTOCOL_VERSION, HEX);
    }
};

#endif
