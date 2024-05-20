#pragma once

#include "stdint.h";
# include <iostream>
#include <bitset>
class FixedPoint
{
public:
	int16_t number;
	FixedPoint(const FixedPoint& other) {
		this->number = other.number;
	}
	FixedPoint(float number) {
		this->number = (int16_t) (number * 32768);
	}
	FixedPoint(double number) {
		this->number = (int16_t)(number * 32768);
	}

	void printBinary() {
		std::cout << std::bitset<16>(this->number) << std::endl;
	}
	float getNumber() {
		return (float) (number / 32768.);
	}
	FixedPoint(int16_t number) {
		this->number = number;
	}
	FixedPoint operator+(const FixedPoint& other) {
		int16_t num = this->number + other.number;
		return FixedPoint{ num };
	}
	FixedPoint operator-(const FixedPoint& other) {
		int16_t num = this->number - other.number;
		return FixedPoint{ num };
	}
	FixedPoint operator*(const FixedPoint& other) {
		int16_t num = this->number * other.number;
		return FixedPoint{ num };
	}
	static int32_t mul(const FixedPoint& left, const FixedPoint& right) {
		return (int32_t)left.number * (int32_t)right.number;
	}
	friend std::ostream& operator<<(std::ostream& stream, FixedPoint& fixedPoint)
	{
		stream << fixedPoint.getNumber();
		return stream;
	}
	friend std::istream& operator>>(std::istream& stream, FixedPoint& fixedPoint)
	{
		float tmp;
		stream >> tmp;
		fixedPoint.number = (int16_t)(tmp * 32768);
		return stream;
	}
private:
	

};


