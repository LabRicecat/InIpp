# InIpp
Ini++ (Or InIpp) is just normal InI, but with changes and additions

# Syntax
Ini++ is very much the same as Ini, but with some changes:

## Types
Every value of a key has a type. <br>
Known types are:
 - Null
 - String
 - Int
 - Float
 - Vector
 - List
 - Dictionary
As defined in `IniTypes`.
<br>
These types will be checked for the user. <br>

### Element
As defined in the `IniElement` class, it's a storage class for every known type.

### List
A list begins and ends with `[]`. <br>
A possible list is:
```
[10,"hello",(12,56,89),[sublist,12]]
```
A list can hold any type, also sublist work. <br>
`IniList` is defined as `std::vector<IniElement>`.

### Dictionary
A dictionary begins and ends with `{}` <br>
A possible dictionary is:
```
{key:value,41:"string",K:[list]}
```
A key has always to be a string or a number. The value can be any `IniType` <br>
`IniDictionary` is defined as `std::map<std::string,IniElement>`

### Vector
A vector begins and ends with `()` <br>
It has three values: `x`,`y` and `z`. Everyone is an int. <br>
A possible vector would be:
```
(14,685432,1279)
```
Floats are not yet supported. <br>
`IniVector` is defined as `struct IniVector { ... };`

## How to use
An example:
```c++
#include <iostream>
#include <fstream>
#include "Inipp.hpp"

int main() {

    IniFile file = IniFile::from_file("test.inipp");
    if(!file) {
        std::cout << file.error_msg() << "\n"; // An error occured!
        return 1;
    }
    IniVector vector = file.get("vector","section_a").to_vector(); // file.get() returns an IniElement

    std::cout << vector.x << " | " << vector.y << " | " << vector.z << "\n"; // Prints the vector

    vector.x += 10; // modifies the vector

    file.set("vector",IniElement(IniType::Vector,vector.to_string()),"section_a"); // apply changes

    vector = file.get("vector","section_a").to_vector(); // gets the new vector

    IniList list = file.get("list","section_b").to_list(); // if these function fail, they'll return an empty instance
    IniDictionary dictionary = file.get("dictionary","section_b").to_dictionary();

    dictionary["key"] = list.front(); // sets key to the first element in the list
    list.pop_back(); // deletes the last element

    // writes everything back
    file.set("list",IniElement(IniType::List,IniHelper::to_string(list)),"section_b");
    file.set("list",IniElement(IniType::List,IniHelper::to_string(dictionary)),"section_b");

    // write back to file
    file.to_file("test.inipp");
}
```
