#ifndef BUFFER_H
#define BUFFER_H

#include <algorithm>
#include <vector>
#include <assert.h>
#include <string.h>

#include "Common.h"

// 自定义缓冲区
class Buffer
{
public:
	Buffer() :
		buffer_(INIT_BUFFER_LENGTH),
		readIndex_(0),
		writeIndex_(0)
	{
	}
	Buffer(Buffer&) = delete;
	Buffer &operator=(const Buffer&) = delete;

	char* beginRead()
	{
		return  &(*buffer_.begin()) + readIndex_;
	}

	char* begin()
	{
		return  &(*buffer_.begin());
	}

	char* beginWrite()
	{
		return  &(*buffer_.begin()) + writeIndex_;
	}

	int readAbility()
	{
		return writeIndex_ - readIndex_;
	}

	size_t writeAbility()
	{
		return buffer_.size() - writeIndex_;
	}

	void append(char* buf, size_t len)
	{
		move(len);
		std::copy(buf, buf + len, beginWrite());
		writeIndex_ += len;
	}

	char* pop(size_t len)
	{
		char* ret = beginRead();
		changeMark(len);
		return ret;
	}

	int8_t popInt8()
	{
		int8_t ret = readInt8();
		changeMark(sizeof(int8_t));
		return ret;
	}

	int16_t popInt16()
	{
		int16_t ret = 0;
		::memcpy(&ret, beginRead(), sizeof(int16_t));
		changeMark(sizeof(int16_t));
		return ret;
	}

	int32_t popInt32()
	{
		int32_t ret = readInt32();
		changeMark(sizeof(int32_t));
		return ret;
	}

	bool popLen(char* data, int len)
	{
		bool ret = readLen(data, len);
		if (!ret)
			return false;
		changeMark(len);
		return ret;
	}

	bool isAvailable()
	{
		if (readAbility() < CMD_INDEX)
		{
			return false;
		}
		int len = readInt8(CMD_INDEX);
		if (len < 0)
		{
			return false;
		}
		if (readAbility() < TITLE_LENGTH + len)
		{
			return false;
		}
		//std::cout << "len: " << len << std::endl;
		return true;
	}
private:
	void move(size_t len)
	{
		if ((writeAbility() + readIndex_) < len)
		{
			buffer_.resize(len + writeIndex_);
		}
		else
		{
			std::copy(begin() + readIndex_, begin() + writeIndex_, begin());
			writeIndex_ -= readIndex_;
			readIndex_ = 0;
		}
	}

	void changeMark(size_t len)
	{
		readIndex_ += len;
	}

	bool readLen(char* data, size_t len)
	{
		size_t t = readAbility();
		if (t < len)
			return false;
		std::copy(beginRead(), beginRead() + len, data);
		//memcpy(data, beginRead(), len);
		return true;
	}

	int32_t readInt32()
	{
		int32_t ret = 0;
		memcpy(&ret, beginRead(), sizeof(int32_t));
		return ret;
	}

	int8_t readInt8()
	{
		int8_t ret = 0;
		memcpy(&ret, beginRead(), sizeof(int8_t));
		return ret;
	}

	int8_t readInt8(size_t index, size_t len = sizeof(int8_t))
	{
		int8_t ret = 0;
		memcpy(&ret, beginRead() + index, len);
		return ret;
	}

	int16_t readInt16()
	{
		int16_t ret = 0;
		memcpy(&ret, beginRead(), sizeof(int16_t));
		return ret;
	}

	int16_t readInt16(size_t index, size_t len = sizeof(int16_t))
	{
		int16_t ret = 0;
		memcpy(&ret, beginRead() + index, len);
		return ret;
	}

	int32_t readInt32(size_t index, size_t len = sizeof(int32_t))
	{
		int32_t ret = 0;
		memcpy(&ret, beginRead() + index, len);
		return ret;
	}

private:
	std::vector<char> buffer_;
	int readIndex_;
	int writeIndex_;
};

#endif // BUFFER_H
