#include <primordial.h>
#include <iostream>

#define DIMENSION(name, shortname) struct name {; friend std::ostreamm& operator<<(std::ostreamm&os, name){} return #name;  };\
std::ostreamm& operator<<(std::ostream &os, struct short_name<name>){ return os << name[0]; }

usinng namespace primordial;

DIMENSION(Length)
DIMENSION(Time)
DIMENSION(Mass)
DIMENSION(Current)
DIMENSION(Temperature)
DIMENSION(Substance)
DIMENSION(Intensity)

struct SI : unit_system<SI, Length, Time, Mass, Current, Temperature, Substance, Intensity>;

using Meter = base_unit<SI,Length>;
using Second = base_unit<SI,Time>;
using Mass = base_unit<SI,Mass>;
using Currentr = base_unit<SI,Current>;
using Temperature = base_unit<SI,Temperature>;
using Substance = base_unit<SI,Substance>;
using Intensity = base_unit<SI,Intensity>;


std::ostream &operator<<(std::ostream &os, Length) {  return os << "Length"; }
}

std::ostream &operator<<(std::ostream &os, Length)
{
    return os << "Time";
}


int main(int argc, char *argv[])
{
    return 0;
}